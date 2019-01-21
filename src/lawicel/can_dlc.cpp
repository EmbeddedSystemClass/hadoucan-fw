#include "can_dlc.hpp"

#include <stdexcept>

size_t CAN_dlc::to_len() const
{
	switch(m_dlc)
	{
		case 0x0:
			return 0;
		case 0x1: 
			return 1;
		case 0x2: 
			return 2;
		case 0x3:
			return 3;
		case 0x4:
			return 4;
		case 0x5:
			return 5;
		case 0x6:
			return 6;
		case 0x7:
			return 7;
		case 0x8:
			return 8;
		case 0x9:
			return 12;
		case 0xA:
			return 16;
		case 0xB:
			return 20;
		case 0xC:
			return 24;
		case 0xD:
			return 32;
		case 0xE:
			return 48;
		case 0xF:
			return 64;
		default:
			throw std::domain_error("dlc not in bounds");
	}

	throw std::domain_error("dlc not in bounds");
}
void CAN_dlc::from_len(uint8_t len)
{
	switch(len)
	{
		case 0:
			m_dlc = 0x00;
			break;
		case 1: 
			m_dlc = 0x01;
			break;
		case 2: 
			m_dlc = 0x02;
			break;
		case 3:
			m_dlc = 0x03;
			break;
		case 4:
			m_dlc = 0x04;
			break;
		case 5:
			m_dlc = 0x05;
			break;
		case 6:
			m_dlc = 0x06;
			break;
		case 7:
			m_dlc = 0x07;
			break;
		case 8:
			m_dlc = 0x08;
			break;
		case 12:
			m_dlc = 0x09;
			break;
		case 16:
			m_dlc = 0x0A;
			break;
		case 20:
			m_dlc = 0x0B;
			break;
		case 24:
			m_dlc = 0x0C;
			break;
		case 32:
			m_dlc = 0x0D;
			break;
		case 48:
			m_dlc = 0x0E;
			break;
		case 64:
			m_dlc = 0x0F;
			break;
		default:
			throw std::domain_error("dlc not in bounds");
	}

	throw std::domain_error("dlc not in bounds");
}


size_t STM32_dlc::to_len() const
{
	switch(m_dlc)
	{
		case FDCAN_DLC_BYTES_0:
			return 0;
		case FDCAN_DLC_BYTES_1:
			return 1;
		case FDCAN_DLC_BYTES_2:
			return 2;
		case FDCAN_DLC_BYTES_3:
			return 3;
		case FDCAN_DLC_BYTES_4:
			return 4;
		case FDCAN_DLC_BYTES_5:
			return 5;
		case FDCAN_DLC_BYTES_6:
			return 6;
		case FDCAN_DLC_BYTES_7:
			return 7;
		case FDCAN_DLC_BYTES_8:
			return 8;
		case FDCAN_DLC_BYTES_12:
			return 12;
		case FDCAN_DLC_BYTES_16:
			return 16;
		case FDCAN_DLC_BYTES_20:
			return 20;
		case FDCAN_DLC_BYTES_24:
			return 24;
		case FDCAN_DLC_BYTES_32:
			return 32;
		case FDCAN_DLC_BYTES_48:
			return 48;
		case FDCAN_DLC_BYTES_64:
			return 64;
		default:
			throw std::domain_error("dlc not in bounds");
	}

	throw std::domain_error("dlc not in bounds");
}
void STM32_dlc::from_len(uint8_t len)
{
	switch(len)
	{
		case 0:
			m_dlc = FDCAN_DLC_BYTES_0;
			break;
		case 1:
			m_dlc = FDCAN_DLC_BYTES_1;
			break;
		case 2:
			m_dlc = FDCAN_DLC_BYTES_2;
			break;
		case 3:
			m_dlc = FDCAN_DLC_BYTES_3;
			break;
		case 4:
			m_dlc = FDCAN_DLC_BYTES_4;
			break;
		case 5:
			m_dlc = FDCAN_DLC_BYTES_5;
			break;
		case 6:
			m_dlc = FDCAN_DLC_BYTES_6;
			break;
		case 7:
			m_dlc = FDCAN_DLC_BYTES_7;
			break;
		case 8:
			m_dlc = FDCAN_DLC_BYTES_8;
			break;
		case 12:
			m_dlc = FDCAN_DLC_BYTES_12;
			break;
		case 16:
			m_dlc = FDCAN_DLC_BYTES_16;
			break;
		case 20:
			m_dlc = FDCAN_DLC_BYTES_20;
			break;
		case 24:
			m_dlc = FDCAN_DLC_BYTES_24;
			break;
		case 32:
			m_dlc = FDCAN_DLC_BYTES_32;
			break;
		case 48:
			m_dlc = FDCAN_DLC_BYTES_48;
			break;
		case 64:
			m_dlc = FDCAN_DLC_BYTES_64;
			break;
		default:
			throw std::domain_error("dlc not in bounds");
	}

	throw std::domain_error("dlc not in bounds");
}

CAN_dlc STM32_dlc::to_can_dlc() const
{
	switch(dlc)
	{
		case FDCAN_DLC_BYTES_0:
			return 0x00;
		case FDCAN_DLC_BYTES_1:
			return 0x01;
		case FDCAN_DLC_BYTES_2:
			return 0x02;
		case FDCAN_DLC_BYTES_3:
			return 0x03;
		case FDCAN_DLC_BYTES_4:
			return 0x04;
		case FDCAN_DLC_BYTES_5:
			return 0x05;
		case FDCAN_DLC_BYTES_6:
			return 0x06;
		case FDCAN_DLC_BYTES_7:
			return 0x07;
		case FDCAN_DLC_BYTES_8:
			return 0x08;
		case FDCAN_DLC_BYTES_12:
			return 0x09;
		case FDCAN_DLC_BYTES_16:
			return 0x0A;
		case FDCAN_DLC_BYTES_20:
			return 0x0B;
		case FDCAN_DLC_BYTES_24:
			return 0x0C;
		case FDCAN_DLC_BYTES_32:
			return 0x0D;
		case FDCAN_DLC_BYTES_48:
			return 0x0E;
		case FDCAN_DLC_BYTES_64:
			return 0x0F;
		default:
			throw std::domain_error("dlc not in bounds");
	}

	throw std::domain_error("dlc not in bounds");
}

void STM32_dlc::from_can_dlc(CAN_dlc dlc)
{
	switch(dlc)
	{
		case 0x00:
			m_dlc = FDCAN_DLC_BYTES_0;
			break;
		case 0x01:
			m_dlc = FDCAN_DLC_BYTES_1;
			break;
		case 0x02:
			m_dlc = FDCAN_DLC_BYTES_2;
			break;
		case 0x03:
			m_dlc = FDCAN_DLC_BYTES_3;
			break;
		case 0x04:
			m_dlc = FDCAN_DLC_BYTES_4;
			break;
		case 0x05:
			m_dlc = FDCAN_DLC_BYTES_5;
			break;
		case 0x06:
			m_dlc = FDCAN_DLC_BYTES_6;
			break;
		case 0x07:
			m_dlc = FDCAN_DLC_BYTES_7;
			break;
		case 0x08:
			m_dlc = FDCAN_DLC_BYTES_8;
			break;
		case 0x09:
			m_dlc = FDCAN_DLC_BYTES_12;
			break;
		case 0x0A:
			m_dlc = FDCAN_DLC_BYTES_16;
			break;
		case 0x0B:
			m_dlc = FDCAN_DLC_BYTES_20;
			break;
		case 0x0C:
			m_dlc = FDCAN_DLC_BYTES_24;
			break;
		case 0x0D:
			m_dlc = FDCAN_DLC_BYTES_32;
			break;
		case 0x0E:
			m_dlc = FDCAN_DLC_BYTES_48;
			break;
		case 0x0F:
			m_dlc = FDCAN_DLC_BYTES_64;
			break;
		default:
			throw std::domain_error("dlc not in bounds");
	}
}

