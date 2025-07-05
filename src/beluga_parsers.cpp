#include "beluga_parsers.h"

namespace beluga_utils
{
    /*
    The string is assumed to be addressed to a device
    <device name><device delimiter><key1><key-val delimiter><val1><topic delimiter><key2><key-val delim><val2>
    e.g. mqtt_client1@led1_setpoint:1|led2_setpoint:0
    If parsing fails, do nothing
    We split on the <device delimiter> and the device name is element 0
    */
   bool get_device(std::string this_str,  std::string & return_val)
   {
       std::vector<std::string> device_and_payload_vec;
       device_and_payload_vec = beluga_utils::split_string(this_str, _field_delimiter);
       if(device_and_payload_vec.size() != 2)
       {
           //Could not parse
           return false;
       }

       return_val = device_and_payload_vec[0];
       return true;
   }

    /*
    The string is assumed to be addressed to a device
    <device name><device delimiter><key1><key-val delimiter><val1><topic delimiter><key2><key-val delim><val2>
    e.g. mqtt_client1@led1_setpoint:1|led2_setpoint:0
    If parsing fails, do nothing
    We split on the <device delimiter> and the payload is element 1
    */
   bool get_payload(std::string this_str,  std::string & return_val)
   {
       std::vector<std::string> device_and_payload_vec;
       device_and_payload_vec = beluga_utils::split_string(this_str, _field_delimiter);
       if(device_and_payload_vec.size() != 2)
       {
           //Could not parse
           return false;
       }

       return_val = device_and_payload_vec[0];
       return true;
   }

    /*
    Given an mqtt string for an arbitrary topic, parse and extract all subtopic key-val pairs and return them in a map
    Note that the map is keyed by SUBTOPIC. The TOPIC is returned in topic_str if possible
    For mqtt topics including both topic and subtopic, you will need to re-prepend the topic to the subtopic
    */
   bool deserialise(std::string this_str,  std::map<std::string, std::string> & return_val, std::string * topic_str)
   {
       std::vector<std::string> topic_and_payload_vec = beluga_utils::split_string(this_str, _field_delimiter);
       if(topic_and_payload_vec.size() != 2)
       {
           //Could not parse
           Serial.print("Got vector size: ");
           Serial.println(topic_and_payload_vec.size());
           return false;
       }

       if(topic_str != nullptr)
       {
           //Return the topic
           *topic_str = topic_and_payload_vec[0];
       }

       std::vector<std::string> subtopics_vec;
       subtopics_vec = beluga_utils::split_string(topic_and_payload_vec[1], _field_delimiter);
       for(auto iter = subtopics_vec.begin(); iter != subtopics_vec.end(); iter++)
       {
           std::vector<std::string> key_val_vec;
           key_val_vec = beluga_utils::split_string(*iter, _field_delimiter);
           if(key_val_vec.size() != 2)
           {
               //Could not parse
               continue;
           }

           std::string key = key_val_vec[0];
           std::string val = key_val_vec[1];
           return_val[key] = val;
       }

       return true;
   }

    /*
    Given an mqtt string for unknown topic
    Process it; if it matches the target topic, parse and extract all subtopic key-val pairs and return them in a map
    If it does not match the target topic do nothing else
    */
   bool deserialise_target_topic(std::string this_str, std::string target_topic,  std::map<std::string, std::string> & subtopic_map)
   {
        std::stringstream _ss;
       _ss.str("");
       _ss << "Deserialising: " << this_str << " with target topic: " << target_topic;
       Serial.println(_ss.str().c_str());
       _ss.str("");

       std::string this_topic;
       bool got_this_topic = get_topic(this_str, this_topic);
       if(! got_this_topic)
       {
           //Could not parse
           return false;
       }

       if(this_topic != target_topic)
       {  
           //"Topic did not match target"
           return false;
       }

   
       return deserialise(this_str, subtopic_map);        
   }

   /*
   Message format: 
   <topic><topic_delimiter><key1><key_val_delimiter><val1><subtopic delimiter><key2><key_val_delimiter><val2> 
   e.g.:
   gpio1@direction:OUT|value:1

   The gist is we might want to send a message to a device <topic> and have it then transmit messages on topic <subtopic>
   */
   bool serialise(std::map<std::string, std::string> message_map, std::string topic_str, std::string & return_val)
   {
        std::stringstream _ss;
       _ss.str("");
       _ss << topic_str << _field_delimiter;
       bool first_subtopic = true;
       for(auto iter = message_map.begin(); iter != message_map.end(); iter++)
       {
           if( first_subtopic)
           {
               first_subtopic = false;
           }else{
               _ss << _field_delimiter;
           }
           _ss << iter->first << _field_delimiter << iter->second;
       }
       if(first_subtopic)
       {
           //No payload to attach
           return false;
       }
       return_val = _ss.str();
       return true;
   }

   bool prepend_topic_to_payload(std::string this_topic, std::string this_payload, std::string & return_val)
   {

       if((this_topic == "") || (this_payload == ""))
       {
           return false;
       }
       std::stringstream _ss;
       _ss.str("");
       _ss << this_topic << _field_delimiter << this_payload;
       return_val = _ss.str();
       _ss.str("");
       return true;
   }


    /*
    Assume the machinery will receive mail targeted at its _rx_mail_topic
    We may get multiple messages to ourselves in one read of primary_rx_list
    Each message may have multiple subtopics
    e.g. a robot car might get told to turn some of its lights on/off, and be given coordinates to drive towards
    These different messages should all be retained

    So we use a vector of maps
    - one item in the vector (one map) == output of parsing one mqtt message-payload targeted at ourselves
    - one map contains all subtopic:message key-val pairs for that payload

    e.g. payloads of messages 1 and 2 each have two subtopics ab and cd: [ {keyA:valA, keyB:valB}, {keyC:valC, keyD:valD}]
    We assume here that both messages are addressed to this machinery. This is not always the case

    Return true if we successfully parsed messages addressed to us
    Return false if we did not.

    */
   bool parse_primary_mqtt_rx(std::list<std::string> primary_rx_list,  std::vector< std::map<std::string, std::string> > & rx_subtopic_map_vec,std::string target_topic, bool clearprimary_rx_list)
   {

       bool got_data = false;
       for(auto rx_iter = primary_rx_list.begin(); rx_iter != primary_rx_list.end(); rx_iter++)
       {
           Serial.print("Parsing ");
           Serial.println(rx_iter->c_str());
           std::map<std::string, std::string> subtopic_data;
           //bool got_parsed_data = beluga_utils::mqtt_target_topic_parser(*rx_iter, target_topic, _device_delimiter, _subtopic_delimiter, _key_val_delimiter, subtopic_data);
           bool got_parsed_data = deserialise_target_topic(*rx_iter, target_topic, subtopic_data);

           if(! got_parsed_data)
           {
               Serial.println("Could not parse");
               continue;
           }

            std::stringstream _ss;
           for(auto print_iter = subtopic_data.begin(); print_iter != subtopic_data.end(); print_iter++)
           {
               _ss.str("");
               _ss << "Parsed message: Topic: " << print_iter->first << " Payload: " << print_iter->second;
               Serial.println(_ss.str().c_str());
               _ss.str("");
           }
           got_data = true;
           //Append this map to rx_subtopic_map_vec
           rx_subtopic_map_vec.push_back(subtopic_data);
       }

       if(clearprimary_rx_list)
       {
           primary_rx_list.clear();
       }

       return got_data;
   }


    std::string combine_for_primary_tx(std::string target_device, std::string topic, std::string payload)
    {
        std::stringstream ss;
        ss << target_device << _field_delimiter << topic << _field_delimiter << payload;
        return ss.str();
    }

    std::vector<std::string> split_from_primary_rx(std::string rx_str)
    {
       std::vector<std::string> return_data = beluga_utils::split_string(rx_str, _field_delimiter);
       return return_data;
    }

}