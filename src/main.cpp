#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#include "USB_RX_task.hpp"
#include "USB_TX_task.hpp"

#include "freertos_cpp_util/Task_static.hpp"
#include "freertos_cpp_util/BSema_static.hpp"
#include "freertos_cpp_util/Mutex_static.hpp"
#include "freertos_cpp_util/object_pool/Object_pool.hpp"

#include <array>
#include <algorithm>
#include <cstdio>
#include <cinttypes>
#include <cstdarg>
#include <mutex>

extern USBD_HandleTypeDef hUsbDeviceHS;
extern UART_HandleTypeDef huart1;

Mutex_static m_uart1_mutex;

template<size_t LEN>
bool uart1_print(const char* fmt, ...)
{
	std::array<uint8_t, LEN> m_buf;

	va_list args;
	va_start (args, fmt);
	int ret = vsnprintf(reinterpret_cast<char*>(m_buf.data()), m_buf.size(), fmt, args);
	va_end(args);

	if(ret < 0)	
	{
		return false;
	}

	size_t num_to_print = 0;
	if(ret > 0)	
	{
		num_to_print = std::min<size_t>(ret, m_buf.size()-1);
	}

  HAL_StatusTypeDef uartret;
  {
    std::lock_guard<Mutex_static> lock(m_uart1_mutex);
	  uartret = HAL_UART_Transmit(&huart1, m_buf.data(), num_to_print, -1);
  }


	return uartret == HAL_OK;
}

#if 0
class foo
{
public:
  foo()
  {
    m_v1 = 0; 
    m_v2 = 0;
  }
  foo(int x)
  {
   m_v1 = x; 
   m_v2 = 2;
  }
  ~foo()
  {
    uart1_print<64>("called ~foo on 0x%" PRIXPTR "\r\n", this);
  }
  int m_v1;
  int m_v2;
};

class Pool_test_task : public Task_static<1024>
{
public:

  void work() override
  {
    for(;;)
    {
		HAL_UART_Transmit(&huart1, (uint8_t*)"test\r\n", 6, 100);
		vTaskDelay(500);

		foo* a = m_pool.allocate();
		foo* b = m_pool.try_allocate_for_ticks(3, 4);
		foo* c = m_pool.try_allocate_for(std::chrono::milliseconds(5), 5);

		if(a)
		{
			uart1_print<64>("a is ok\r\n");

			Object_pool_node<foo>* n_ptr = Object_pool_node<foo>::get_this_from_val_ptr(a);

			uart1_print<64>("\ta                     is 0x%" PRIXPTR "\r\n", a);
			uart1_print<64>("\t&m_pool               is 0x%" PRIXPTR "\r\n", &m_pool);
			uart1_print<64>("\tn_ptr                 is 0x%" PRIXPTR "\r\n", n_ptr);
			uart1_print<64>("\tn_ptr->get_val_ptr()  is 0x%" PRIXPTR "\r\n", n_ptr->get_val_ptr());
			uart1_print<64>("\tn_ptr->get_pool_ptr() is 0x%" PRIXPTR "\r\n", n_ptr->get_pool_ptr());

      uart1_print<64>("a.v1 is %d\r\n", a->m_v1);
      uart1_print<64>("a.v2 is %d\r\n", a->m_v2);
		}
		if(b)
		{
			uart1_print<64>("b is ok\r\n");

			Object_pool_node<foo>* n_ptr = Object_pool_node<foo>::get_this_from_val_ptr(b);

			uart1_print<64>("\tb                     is 0x%" PRIXPTR "\r\n", b);
			uart1_print<64>("\t&m_pool               is 0x%" PRIXPTR "\r\n", &m_pool);
			uart1_print<64>("\tn_ptr                 is 0x%" PRIXPTR "\r\n", n_ptr);
			uart1_print<64>("\tn_ptr->get_val_ptr()  is 0x%" PRIXPTR "\r\n", n_ptr->get_val_ptr());
			uart1_print<64>("\tn_ptr->get_pool_ptr() is 0x%" PRIXPTR "\r\n", n_ptr->get_pool_ptr());
			
      uart1_print<64>("b.v1 is %d\r\n", b->m_v1);
      uart1_print<64>("b.v2 is %d\r\n", b->m_v2);
    }
    if(c)
    {
      uart1_print<64>("c.v1 is %d\r\n", c->m_v1);
      uart1_print<64>("c.v2 is %d\r\n", c->m_v2);
		}


		Object_pool<foo, 16>::free(a);
		Object_pool<foo, 16>::free(b);
		Object_pool<foo, 16>::free(c);
		//m_pool.deallocate(a);


    }
  }

protected:
  Object_pool<foo, 16> m_pool;
};

Pool_test_task pool_test_task;
#endif
USB_RX_task usb_rx_task;
USB_TX_task usb_tx_task;

extern "C"
{
  int8_t CDC_Init_HS(void);
  int8_t CDC_DeInit_HS(void);
  int8_t CDC_Control_HS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
  int8_t CDC_Receive_HS(uint8_t* pbuf, uint32_t *Len);

  USBD_CDC_ItfTypeDef USBD_Interface_fops_HS =
  {
    CDC_Init_HS,
    CDC_DeInit_HS,
    CDC_Control_HS,
    CDC_Receive_HS
  };

  int8_t CDC_Init_HS(void)
  {
    usb_rx_task.handle_init_callback();
    usb_tx_task.handle_init_callback();
    // usb_rx_task.launch("usb_rx", 3);
    // usb_tx_task.launch("usb_tx", 2);
    return (USBD_OK);
  }

  int8_t CDC_DeInit_HS(void)
  {
    /* USER CODE BEGIN 9 */
    return (USBD_OK);
    /* USER CODE END 9 */
  }

  int8_t CDC_Receive_HS(uint8_t* Buf, uint32_t *Len)
  {
    return usb_rx_task.handle_rx_callback(Buf, *Len);
  }

  int8_t CDC_Control_HS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
  {
    return (USBD_OK);
  }
}

int main(void)
{
  SCB_EnableICache();

  //SCB_EnableDCache();

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_FDCAN1_Init();
  MX_CRC_Init();
  MX_HASH_Init();
  MX_RTC_Init();
  MX_RNG_Init();

  //pool_test_task.launch("pool_test", 1);
  usb_rx_task.launch("usb_rx", 3);
  usb_tx_task.launch("usb_tx", 2);
  vTaskStartScheduler();
  
  for(;;)
  {

  }
}