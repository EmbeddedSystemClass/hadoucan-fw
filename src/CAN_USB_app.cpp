#include "CAN_USB_app.hpp"

#include "uart1_printf.hpp"


CAN_USB_app::CAN_USB_app()
{

}
CAN_USB_app::~CAN_USB_app()
{

}
/*
bool CAN_USB_app::fs_init()
{
	m_qspi.set_handle(&hqspi);

	if(!m_qspi.init())
	{
		uart1_log<64>(LOG_LEVEL::ERROR, "qspi", "m_qspi.init failed");
		return false;
	}

	m_fs.initialize();
	m_fs.set_flash(&m_qspi);

	uint8_t mfg_id = 0;
	uint16_t flash_pn = 0;
	if(m_qspi.get_jdec_id(&mfg_id, &flash_pn))
	{
		uart1_log<128>(LOG_LEVEL::INFO, "qspi", "mfg id %02" PRIX32, uint32_t(mfg_id));
		uart1_log<128>(LOG_LEVEL::INFO, "qspi", "flash pn %04" PRIX32, uint32_t(flash_pn));
	}
	else
	{
		uart1_log<64>(LOG_LEVEL::ERROR, "qspi", "get_jdec_id failed");
	}

	uint64_t unique_id = 0;
	if(m_qspi.get_unique_id(&unique_id))
	{
		// uart1_log<128>(LOG_LEVEL::INFO, "qspi", "flash sn %016" PRIX64, unique_id);
		//aparently PRIX64 is broken
		uart1_log<128>(LOG_LEVEL::INFO, "qspi", "flash sn %08" PRIX32 "%08" PRIX32, Byte_util::get_upper_half(unique_id), Byte_util::get_lower_half(unique_id));
	}
	else
	{
		uart1_log<64>(LOG_LEVEL::ERROR, "qspi", "get_unique_id failed");
	}

	uart1_log<64>(LOG_LEVEL::INFO, "qspi", "Mounting flash fs");
	int mount_ret = m_fs.mount();
	if(mount_ret != SPIFFS_OK)
	{
		return false;
	}

	return true;
}
*/
bool CAN_USB_app::load_config()
{
	tinyxml2::XMLDocument config_doc;
	// if(!load_xml_file("config.xml", &config_doc))
	// {
	// 	uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Opening config.xml failed");
	// 	return false;
	// }

	CAN_Config config;
	if(!parse_config(config_doc, &config))
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Parsing config.xml failed");
		return false;
	}

	return true;
}

bool CAN_USB_app::load_bitrate_table()
{
	tinyxml2::XMLDocument table_doc;
	// if(!load_xml_file("table.xml", &table_doc))
	// {
	// 	uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Opening table.xml failed");
	// }

		Bitrate_Table_Set table_set;
	if(!parse_bitrate_tables(table_doc, &table_set))
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Parsing table.xml failed");
		return false;
	}

	return true;
}

bool CAN_USB_app::get_bool_text(const tinyxml2::XMLElement* root, const char* child, bool* const out_val)
{
	if(root == nullptr)
	{
		return false;
	}

	const tinyxml2::XMLElement* node = root->FirstChildElement(child);
	if(node == nullptr)
	{
		return false;
	}
	if(node->QueryBoolText(out_val) != tinyxml2::XML_SUCCESS)
	{
		return false;
	}

	return true;
}

bool CAN_USB_app::get_int_text(const tinyxml2::XMLElement* root, const char* child, int* const out_val)
{
	if(root == nullptr)
	{
		return false;
	}

	const tinyxml2::XMLElement* node = root->FirstChildElement(child);
	if(node == nullptr)
	{
		return false;
	}
	if(node->QueryIntText(out_val) != tinyxml2::XML_SUCCESS)
	{
		return false;
	}

	return true;
}

bool CAN_USB_app::get_hex_text(const tinyxml2::XMLElement* root, const char* child, unsigned* const out_val)
{
	if(root == nullptr)
	{
		return false;
	}

	const tinyxml2::XMLElement* node = root->FirstChildElement(child);
	if(node == nullptr)
	{
		return false;
	}
	
	const char* str = node->GetText();

	if(str == nullptr)
	{
		return false;
	}

	if(sscanf(str, "%x", out_val) != 1)
	{
		return false;
	}

	return true;
}

bool get_str_text(const tinyxml2::XMLElement* root, const char* child, char const * * const out_val)
{
	if(root == nullptr)
	{
		return false;
	}

	const tinyxml2::XMLElement* node = root->FirstChildElement(child);
	if(node == nullptr)
	{
		return false;
	}
	
	const char* str = node->GetText();
	if(str == nullptr)
	{
		return false;
	}

	*out_val = str;

	return true;
}

bool CAN_USB_app::parse_config(const tinyxml2::XMLDocument& config_doc, CAN_Config* const out_config)
{
	const tinyxml2::XMLElement* config_root = config_doc.FirstChildElement("config");
	if(config_root == nullptr)
	{
		return false;
	}

	{
		bool autopoll = false;
		if(!get_bool_text(config_root, "autopoll", &autopoll))
		{
			return false;
		}
	}

	{
		bool listen_only = false;
		if(!get_bool_text(config_root, "listen_only", &listen_only))
		{
			return false;
		}
	}

	{
		const tinyxml2::XMLElement* timestamp_element = config_root->FirstChildElement("timestamp");
		if(timestamp_element == nullptr)
		{
			return false;
		}

		bool timestamp_enable = false;
		if(!get_bool_text(timestamp_element, "enable", &timestamp_enable))
		{
			return false;
		}
		int timestamp_prescaler = 0;
		if(!get_int_text(config_root, "prescaler", &timestamp_prescaler))
		{
			return false;
		}
		int timestamp_period = 0;
		if(!get_int_text(config_root, "period", &timestamp_period))
		{
			return false;
		}
	}

	{
		int clock = 0;
		if(!get_int_text(config_root, "clock", &clock))
		{
			return false;
		}
	}

	{
		const tinyxml2::XMLElement* bitrate_element = config_root->FirstChildElement("bitrate");
		if(bitrate_element == nullptr)
		{
			return false;
		}

		int bitrate_nominal = 0;
		if(!get_int_text(bitrate_element, "nominal", &bitrate_nominal))
		{
			return false;
		}
		int bitrate_data = 0;
		if(!get_int_text(bitrate_element, "data", &bitrate_data))
		{
			return false;
		}
	}

	{
		const tinyxml2::XMLElement* protocol_element = config_root->FirstChildElement("protocol");
		if(protocol_element == nullptr)
		{
			return false;
		}

		bool protocol_ext_id = false;
		if(!get_bool_text(protocol_element, "ext_id", &protocol_ext_id))
		{
			return false;
		}
		bool protocol_fd = false;
		if(!get_bool_text(protocol_element, "fd", &protocol_fd))
		{
			return false;
		}
		bool protocol_brs = false;
		if(!get_bool_text(protocol_element, "brs", &protocol_brs))
		{
			return false;
		}
	}

	{
		const tinyxml2::XMLElement* filter_element = config_root->FirstChildElement("filter");
		if(filter_element == nullptr)
		{
			return false;
		}

		unsigned filter_code = 0;
		if(!get_hex_text(filter_element, "accept_code", &filter_code))
		{
			return false;
		}
		unsigned filter_mask = 0;
		if(!get_hex_text(filter_element, "accept_mask", &filter_mask))
		{
			return false;
		}
	}
	return true;
}

bool CAN_USB_app::parse_bitrate_tables(const tinyxml2::XMLDocument& bitrate_tables_doc, Bitrate_Table_Set* const out_table_set)
{
	const tinyxml2::XMLElement* bitrate_tables_root = bitrate_tables_doc.FirstChildElement("bitrate_tables");
	if(bitrate_tables_root == nullptr)
	{
		return false;
	}

	tinyxml2::XMLElement const * table_element = bitrate_tables_root->FirstChildElement("table");
	if(table_element == nullptr)
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "No bitrate table found");
		return false;
	}

	do
	{
		int clock = 0;
		if(table_element->QueryIntAttribute("clock", &clock) != tinyxml2::XML_SUCCESS)
		{
			return false;
		}

		tinyxml2::XMLElement const * entry_element = table_element->FirstChildElement("entry");
		if(entry_element == nullptr)
		{
			uart1_log<128>(LOG_LEVEL::WARN, "qspi", "Empty bitrate_table for clock %d", clock);
			continue;
		}

		//the table for this clock setting
		Bitrate_Table& bitrate_table = out_table_set->m_tables[clock];

		do
		{
			Bitrate_Table_Entry entry;

			char const* type = nullptr;
			if(entry_element->QueryStringAttribute("type", &type) != tinyxml2::XML_SUCCESS)
			{
				return false;
			}

			entry.rate = 0;
			if(entry_element->QueryIntAttribute("rate", &entry.rate) != tinyxml2::XML_SUCCESS)
			{
				return false;
			}

			entry.pre = 0;
			if(entry_element->QueryIntAttribute("pre", &entry.pre) != tinyxml2::XML_SUCCESS)
			{
				return false;
			}

			entry.tseg1 = 0;
			if(entry_element->QueryIntAttribute("tseg1", &entry.tseg1) != tinyxml2::XML_SUCCESS)
			{
				return false;
			}

			entry.tseg2 = 0;
			if(entry_element->QueryIntAttribute("tseg2", &entry.tseg2) != tinyxml2::XML_SUCCESS)
			{
				return false;
			}

			entry.sjw = 0;
			if(entry_element->QueryIntAttribute("sjw", &entry.sjw) != tinyxml2::XML_SUCCESS)
			{
				return false;
			}

			const std::string type_str(type);
			const std::string nominal_str("nominal");
			const std::string data_str("data");
			if(type_str == nominal_str)
			{
				bitrate_table.m_nominal_table[entry.rate] = entry;
			}
			else if(type_str == data_str)
			{
				bitrate_table.m_data_table[entry.rate] = entry;
			}
			else
			{
				uart1_log<128>(LOG_LEVEL::WARN, "qspi", "Dropping bitrate_table entry of unknown type %s", type);
				continue;
			}

			entry_element = entry_element->NextSiblingElement("entry");

		} while(entry_element != nullptr);

		table_element = table_element->NextSiblingElement("table");

	} while(table_element != nullptr);

	return true;
}

bool CAN_USB_app::write_default_config()
{
	tinyxml2::XMLDocument config_doc;

	{
		tinyxml2::XMLDeclaration* decl = config_doc.NewDeclaration("xml version=\"1.0\" standalone=\"yes\" encoding=\"UTF-8\"");
		config_doc.InsertFirstChild(decl);
	}

	tinyxml2::XMLElement* config_doc_root = config_doc.NewElement("config");
	config_doc.InsertEndChild(config_doc_root);

	//General Config Settings
	tinyxml2::XMLElement* node = config_doc.NewElement("autopoll");
	node->SetText(false);
	config_doc_root->InsertEndChild(node);

	node = config_doc.NewElement("listen_only");
	node->SetText(false);
	config_doc_root->InsertEndChild(node);

	node = config_doc.NewElement("timesync");
	node->SetText("slave");
	config_doc_root->InsertEndChild(node);

	{
		tinyxml2::XMLElement* timestamp = config_doc.NewElement("timestamp");
		config_doc_root->InsertEndChild(timestamp);

		node = config_doc.NewElement("enable");
		node->SetText(true);
		timestamp->InsertEndChild(node);

		node = config_doc.NewElement("prescaler");
		node->SetText(2000);
		timestamp->InsertEndChild(node);

		node = config_doc.NewElement("period");
		node->SetText(50000);
		timestamp->InsertEndChild(node);
	}

	{
		tinyxml2::XMLComment* comment = config_doc.NewComment("clock may only be 24000000 or 60000000");
		config_doc_root->InsertEndChild(comment);

		node = config_doc.NewElement("clock");
		node->SetText(60000000);
		config_doc_root->InsertEndChild(node);
	}

	{
		tinyxml2::XMLElement* bitrate = config_doc.NewElement("bitrate");
		config_doc_root->InsertEndChild(bitrate);

		node = config_doc.NewElement("nominal");
		node->SetText(500000);
		bitrate->InsertEndChild(node);

		node = config_doc.NewElement("data");
		node->SetText(4000000);
		bitrate->InsertEndChild(node);
	}
	
	{
		tinyxml2::XMLElement* protocol = config_doc.NewElement("protocol");
		config_doc_root->InsertEndChild(protocol);

		node = config_doc.NewElement("ext_id");
		node->SetText(true);
		protocol->InsertEndChild(node);

		node = config_doc.NewElement("fd");
		node->SetText(true);
		protocol->InsertEndChild(node);

		node = config_doc.NewElement("brs");
		node->SetText(false);
		protocol->InsertEndChild(node);
	}

	{
		tinyxml2::XMLElement* filter = config_doc.NewElement("filter");
		config_doc_root->InsertEndChild(filter);

		node = config_doc.NewElement("accept_code");
		node->SetText("00000000");
		filter->InsertEndChild(node);

		node = config_doc.NewElement("accept_mask");
		node->SetText("FFFFFFFF");
		filter->InsertEndChild(node);
	}

	{
		tinyxml2::XMLElement* debug = config_doc.NewElement("debug");
		config_doc_root->InsertEndChild(debug);

		tinyxml2::XMLComment* comment = config_doc.NewComment("log_level may be TRACE, DEBUG, INFO, WARN, ERROR, FATAL");
		debug->InsertEndChild(comment);

		node = config_doc.NewElement("log_level");
		node->SetText("DEBUG");
		debug->InsertEndChild(node);

		node = config_doc.NewElement("baud");
		node->SetText(115200U);
		debug->InsertEndChild(node);
	}

	if(!write_xml_file("config.xml", config_doc))
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Writing config.xml failed");
		return false;
	}

	return true;
}
bool CAN_USB_app::write_default_bitrate_table()
{
	tinyxml2::XMLDocument table_doc;

	{
		tinyxml2::XMLDeclaration* decl = table_doc.NewDeclaration("xml version=\"1.0\" standalone=\"yes\" encoding=\"UTF-8\"");
		table_doc.InsertFirstChild(decl);
	}

	tinyxml2::XMLElement* table_doc_root = table_doc.NewElement("bitrate_tables");
	table_doc.InsertEndChild(table_doc_root);

	//Bit rate table
	{
		tinyxml2::XMLElement* table = table_doc.NewElement("table");
		table->SetAttribute("clock", 24000000U);
		table_doc_root->InsertEndChild(table);

		tinyxml2::XMLElement* entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  5000U);
		entry->SetAttribute("pre",   192U);
		entry->SetAttribute("tseg1", 16U);
		entry->SetAttribute("tseg2", 8U);
		entry->SetAttribute("sjw",   2U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  10000U);
		entry->SetAttribute("pre",   120U);
		entry->SetAttribute("tseg1", 16U);
		entry->SetAttribute("tseg2", 3U);
		entry->SetAttribute("sjw",   2U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  20000U);
		entry->SetAttribute("pre",   60U);
		entry->SetAttribute("tseg1", 16U);
		entry->SetAttribute("tseg2", 3U);
		entry->SetAttribute("sjw",   2U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  50000U);
		entry->SetAttribute("pre",   24U);
		entry->SetAttribute("tseg1", 16U);
		entry->SetAttribute("tseg2", 3U);
		entry->SetAttribute("sjw",   2U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  100000U);
		entry->SetAttribute("pre",   12U);
		entry->SetAttribute("tseg1", 16U);
		entry->SetAttribute("tseg2", 3U);
		entry->SetAttribute("sjw",   2U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  125000U);
		entry->SetAttribute("pre",   12U);
		entry->SetAttribute("tseg1", 13U);
		entry->SetAttribute("tseg2", 3U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  250000U);
		entry->SetAttribute("pre",   6U);
		entry->SetAttribute("tseg1", 12U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  500000U);
		entry->SetAttribute("pre",   3U);
		entry->SetAttribute("tseg1", 13U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  800000U);
		entry->SetAttribute("pre",   3U);
		entry->SetAttribute("tseg1", 7U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  1000000U);
		entry->SetAttribute("pre",   3U);
		entry->SetAttribute("tseg1", 5U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "data");
		entry->SetAttribute("rate",  2000000U);
		entry->SetAttribute("pre",   2U);
		entry->SetAttribute("tseg1", 4U);
		entry->SetAttribute("tseg2", 1U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "data");
		entry->SetAttribute("rate",  6000000U);
		entry->SetAttribute("pre",   1U);
		entry->SetAttribute("tseg1", 1U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);
	}

	{
		tinyxml2::XMLElement* table = table_doc.NewElement("table");
		table->SetAttribute("clock", 60000000U);
		table_doc_root->InsertEndChild(table);

		tinyxml2::XMLElement* entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  250000U);
		entry->SetAttribute("pre",   24U);
		entry->SetAttribute("tseg1", 7U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  500000U);
		entry->SetAttribute("pre",   12U);
		entry->SetAttribute("tseg1", 7U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "nominal");
		entry->SetAttribute("rate",  1000000U);
		entry->SetAttribute("pre",   5U);
		entry->SetAttribute("tseg1", 8U);
		entry->SetAttribute("tseg2", 3U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "data");
		entry->SetAttribute("rate",  2000000U);
		entry->SetAttribute("pre",   3U);
		entry->SetAttribute("tseg1", 7U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "data");
		entry->SetAttribute("rate",  4000000U);
		entry->SetAttribute("pre",   3U);
		entry->SetAttribute("tseg1", 3U);
		entry->SetAttribute("tseg2", 1U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "data");
		entry->SetAttribute("rate",  6000000U);
		entry->SetAttribute("pre",   2U);
		entry->SetAttribute("tseg1", 3U);
		entry->SetAttribute("tseg2", 1U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "data");
		entry->SetAttribute("rate",  10000000U);
		entry->SetAttribute("pre",   1U);
		entry->SetAttribute("tseg1", 3U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);

		entry = table_doc.NewElement("entry");
		entry->SetAttribute("type",  "data");
		entry->SetAttribute("rate",  12000000U);
		entry->SetAttribute("pre",   1U);
		entry->SetAttribute("tseg1", 2U);
		entry->SetAttribute("tseg2", 2U);
		entry->SetAttribute("sjw",   1U);
		table->InsertEndChild(entry);
	}

	if(!write_xml_file("table.xml", table_doc))
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Writing table.xml failed");
		return false;
	}

	return true;
}

bool CAN_USB_app::load_xml_file(const char* name, tinyxml2::XMLDocument* const out_xml)
{
	spiffs_file fd = SPIFFS_open(m_fs.get_fs(), name, SPIFFS_RDONLY, 0);
	if(fd < 0)
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Opening %s failed: %" PRId32, name, SPIFFS_errno(m_fs.get_fs()));
		return false;
	}

	spiffs_stat stat;
	if(SPIFFS_fstat(m_fs.get_fs(), fd, &stat) < 0)
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Getting size of %s: %" PRId32, name, SPIFFS_errno(m_fs.get_fs()));
		return false;
	}

	std::vector<char> data;
	data.resize(stat.size);
	if(SPIFFS_read(m_fs.get_fs(), fd, data.data(), data.size()) < 0)
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Reading %s failed: %" PRId32, name, SPIFFS_errno(m_fs.get_fs()));
		return false;
	}

	if(SPIFFS_close(m_fs.get_fs(), fd) < 0)
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Closing %s failed: %" PRId32, name, SPIFFS_errno(m_fs.get_fs()));
		return false;
	}

	tinyxml2::XMLDocument file;
	tinyxml2::XMLError err = file.Parse(data.data(), data.size());
	if(err != tinyxml2::XML_SUCCESS)
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Parsing %s failed: %" PRId32, name, err);
		return false;
	}

	return true;
}

bool CAN_USB_app::write_xml_file(const char* name, const tinyxml2::XMLDocument& xml)
{
	tinyxml2::XMLPrinter xml_printer(nullptr, false, 0);
	xml.Print(&xml_printer);

	const char* doc_str = xml_printer.CStr();
	int doc_str_len = xml_printer.CStrSize() - 1;

	for(size_t i = 0; i < (doc_str_len); i++)
	{
		uart1_printf<16>("%c", doc_str[i]);
	}

	uart1_log<128>(LOG_LEVEL::INFO, "qspi", "Writing %s", name);
	spiffs_file fd = SPIFFS_open(m_fs.get_fs(), name, SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);
	if(fd < 0)
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Opening %s failed: %" PRId32, name, SPIFFS_errno(m_fs.get_fs()));
		return false;
	}

	if(SPIFFS_write(m_fs.get_fs(), fd, const_cast<char*>(doc_str), doc_str_len) < 0)
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Writing %s failed: %" PRId32, name, SPIFFS_errno(m_fs.get_fs()));
		return false;
	}

	if(SPIFFS_close(m_fs.get_fs(), fd) < 0)
	{
		uart1_log<128>(LOG_LEVEL::ERROR, "qspi", "Closing %s failed: %" PRId32, name, SPIFFS_errno(m_fs.get_fs()));
		return false;
	}
	uart1_log<128>(LOG_LEVEL::INFO, "qspi", "Write %s success", name);

	return true;
}