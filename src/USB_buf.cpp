#include "USB_buf.hpp"

USB_rx_pool_type rx_buf_pool __attribute__ (( section(".ram_d1_s0_area") ));

USB_tx_pool_type tx_buf_pool __attribute__ (( section(".ram_d1_s0_area") ));
