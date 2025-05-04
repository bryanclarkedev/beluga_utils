#pragma once
#include <string>
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

    const std::string config_rx_topics_list_key = "rx_topics";
    const std::string config_tx_topics_list_key = "tx_topics";
    const std::string topic_list_delimiter = ",";

    
}