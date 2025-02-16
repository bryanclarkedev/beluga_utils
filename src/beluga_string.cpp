#include "beluga_string.h"
#include <algorithm> //For string_to_upper
#include <map>
#include "Arduino.h"

namespace beluga_utils{

//String trim code is taken from https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}




//Taken from https://stackoverflow.com/questions/2165921/converting-from-a-stdstring-to-bool
//Assume lowercase, one word (we cast to lowercase just to be sure)
bool string_to_bool(std::string value) 
{
 	bool b;
	std::stringstream ss;
    ss << string_to_lower(value);
	ss >> std::boolalpha >> b; 
	return b;
}

/*
stoi only covers int, we want up to long int
*/
long int string_to_int(std::string value)
{
	long int i;
	std::stringstream ss;
    ss << value;
	ss >> i;
	return i;
}

float string_to_float(std::string value)
{
	float f;
	std::stringstream ss;
    ss << value;
	ss >> f;
	return f;
}


//https://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case
std::string string_to_upper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

//Based on https://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case
std::string string_to_lower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

//https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<std::string> split_string(std::string s, std::string delimiter)
{
    //We add whitespace removal
    std::vector<std::string> split_str;

    //Largely stolen code from link above
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        trim(token);//in-place removal of whitespace, trimming left and right. Assuming there is none in the middle!
        split_str.push_back(token);//My code
        s.erase(0, pos + delimiter.length());
    }
    trim(s);
    split_str.push_back(s);//Store tail end of string
    return split_str;

}

//Snipped from https://cplusplus.com/reference/string/string/find/
bool string_contains_substring(std::string s, std::string substr)
{
    std::size_t found = s.find(substr);
  if (found!=std::string::npos)
  {
    return true;
  }
  return false;
}

//https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool string_is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

    /*
    Pass in a string that is nominalyl formatted x.x.x.x

    */
    bool string_to_ip_address_vec(std::string ip_str, std::vector<uint8_t> & addr_vec)
    {
        std::vector<std::string> ip_elements = beluga_utils::split_string(ip_str, ".");
        if(ip_elements.size() != 4)
        {
            return false;
        }
        for(auto iter = ip_elements.begin(); iter != ip_elements.end(); iter++)
        {
            if(! string_is_number(*iter))
            {
                return false;
            }
        }

        //It is a number!
        addr_vec.clear();
        for(int i = 0; i < 4; i++)
        {
            int this_c = stoi(ip_elements[i]); //Assume it is not a long int so stoi works
            if((this_c > 255) || (this_c < 0))
            {
                return false;
            }
            addr_vec.push_back(this_c);
        }
        return true;
    }

    /*
    Delete all instances of substring (copying)
    */
    std::string delete_substring(std::string s, std::string substr)
    {
        if(substr.size() == 0)
        {
            return s;
        }
        while(string_contains_substring(s, substr))
        {
            auto start_position_to_erase = s.find(substr);
            s.erase(start_position_to_erase, substr.size());
        }
        return s;
    }

    /*
    Deletes a substring (the prefix) from the start of the string
    Returns by reference
    Returns true/false: true if prefix found at start of string; false if not found at start of string
    */
    bool delete_prefix(std::string this_str, std::string prefix, std::string & return_val)
    {

        if(prefix.size() == 0)
        {
            return_val = this_str;
            return false;
        }

        //Assume that the prefix is at the start
        if(this_str.find(prefix) != 0)
        {
            Serial.println("Prefix not at start");
            return_val = this_str;
            return false;
        }

        Serial.println("Found prefix");
        return_val = delete_substring(this_str, prefix);
        return true;
  
    }

}