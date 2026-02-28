#pragma once
#include <string>
#include <map>
/*!
\brief Constants for calculations, unit conversions, etc.
\author Bryan Clarke
\date 27/10/2024
\details 
*/
namespace beluga_utils
{
    const int MS_TO_S = 1000;
    
    //Reading config files
    const std::string default_topic = "";
    const std::string wildcard_topic = "*";

    const std::string enabled_key = "enabled";

    const std::string config_rx_topics_list_key = "rx_topics";
    const std::string config_tx_topics_list_key = "tx_topics";
    const std::string topic_list_delimiter = ",";

    const std::string subdevice_names_key = "subdevice_names";
    const std::string device_type_key = "device_type";
    const std::string globals_section_key = "globals";
    
    const uint16_t interthread_buffer_size = 4095;
    const size_t ini_reader_max_line_size = 255;

    const std::string default_mqtt_tx_topic = "beluga_tx";
    #ifdef BOARD
        #if BOARD == "ESP32"
        //Some #defines for the FireBeetle ESP32 board
        const std::map<std::string, uint8_t> firebeetle_gpio_map = { {"D0", D0}, {"D1", D1}, {"D2", D2}, {"D3", D3} };
        #endif
    #endif
}