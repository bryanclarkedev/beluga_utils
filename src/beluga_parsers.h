#pragma once
#include <vector>
#include <string>
#include <map>
#include <list>
#include "beluga_debug.h"
#include "beluga_constants.h"
#include "beluga_string.h"
#include <Arduino.h>

namespace beluga_utils
{
        bool get_device(std::string this_str,  std::string & return_val);
        bool get_payload(std::string this_str, std::string & return_val);
        bool get_topic(std::string this_str, std::string & return_val);

        bool deserialise(std::string this_str,  std::map<std::string, std::string> & return_val, std::string * topic_str = nullptr);

        bool deserialise_target_topic(std::string this_str, std::string target_topic, std::map<std::string, std::string> & subtopic_map);

        bool serialise(std::map<std::string, std::string> message_map, std::string topic_str, std::string & return_val);

        bool prepend_topic_to_payload(std::string this_topic, std::string this_payload, std::string & return_val);
        bool parse_primary_mqtt_rx(std::list<std::string> primary_rx_list, std::vector< std::map<std::string, std::string> > & rx_subtopic_map_vec, std::string target_topic = beluga_utils::default_topic, bool clear_primary_rx_list = true);

        #if 0
        void debug_print(std::string s, bool newline = true)
        {
            if(newline)
            {
                Serial.println(s.c_str());
            }else{
                Serial.print(s.c_str());
            }
        }
        #endif
        std::string combine_for_primary_tx(std::string target_device, std::string topic, std::string payload);
        std::vector<std::string> split_from_primary_rx(std::string rx_str);
        //std::string _master_topic_prefix = "";
        //bool _use_master_topic_prefix = false;
        /*
        We pass multiple messages <device id><field delim><key><field delim><value><field delim><message delim><device id><field delim>...
        We may have multiple messages for the same device id, or the same device id and key
        */
        const std::string _message_delimiter = "|";
        const std::string _field_delimiter = ":";
}