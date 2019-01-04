#pragma once

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_fdcan.h"

class STM32_fdcan_tx
{
public:

	STM32_fdcan_tx()
	{
		m_is_open = false;
		m_fdcan = nullptr;
	}

	void set_can_instance(FDCAN_GlobalTypeDef* can)
	{
		m_fdcan = can;
	}

	FDCAN_GlobalTypeDef* get_can_instance()
	{
		return m_fdcan;
	}

	FDCAN_HandleTypeDef* get_can_handle()
	{
		return &m_hfdcan;
	}

	bool init();

	bool open();
	bool close();

	bool tx_std(const uint32_t id, const uint8_t dlc, const uint8_t* data);
	bool tx_ext(const uint32_t id, const uint8_t dlc, const uint8_t* data);
	bool tx_std_rtr(const uint32_t id, const uint8_t dlc, const uint8_t* data);
	bool tx_ext_rtr(const uint32_t id, const uint8_t dlc, const uint8_t* data);

	bool tx_fd_std(const uint32_t id, const uint8_t dlc, const uint8_t* data);
	bool tx_fd_ext(const uint32_t id, const uint8_t dlc, const uint8_t* data);
	bool tx_fd_rtr_std(const uint32_t id, const uint8_t dlc, const uint8_t* data);
	bool tx_fd_rtr_ext(const uint32_t id, const uint8_t dlc, const uint8_t* data);

protected:

	bool m_is_open;

	bool send_packet(FDCAN_TxHeaderTypeDef& tx_head, uint8_t* data);

	FDCAN_GlobalTypeDef* m_fdcan;
	FDCAN_HandleTypeDef m_hfdcan;
};
