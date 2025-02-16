#pragma once
/*
These are string utility functions.

A lot of them are copied off the internet.

Many are inline functions because they're small and quick.

They're in the Beluga_Utils namespace
*/
//https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>
#include <map>
#include <sstream>

namespace beluga_utils
{
        
    //----in-place functiomns---
    // trim from start (in place)
    static inline void ltrim(std::string &s);

    // trim from end (in place)
    static inline void rtrim(std::string &s);

    // trim from both ends (in place)
    static inline void trim(std::string &s) ;

    //--copying functions---
    // trim from start (copying)
    static inline std::string ltrim_copy(std::string s);

    // trim from end (copying)
    static inline std::string rtrim_copy(std::string s) ;

    // trim from both ends (copying)
    static inline std::string trim_copy(std::string s);

    //---Split on delimiter. I use this a lot in input processing, reading configs, etc.---
    //Taken from https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    std::vector<std::string> split_string(std::string s, std::string delimiter);

    //---Casting functions---
    //Taken from https://stackoverflow.com/questions/2165921/converting-from-a-stdstring-to-bool
    bool string_to_bool(std::string value);

    long int string_to_int(std::string value);

    float string_to_float(std::string value);

    //Taken from https://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case
    std::string string_to_upper(std::string s);

    //Taken from https://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case
    std::string string_to_lower(std::string s);

    //Taken from https://cplusplus.com/reference/string/string/find/
    bool string_contains_substring(std::string s, std::string substr);

    bool string_to_ip_address_vec(std::string ip_str, std::vector<uint8_t> & addr_vec);

    std::string delete_substring(std::string s, std::string substr);
    bool delete_prefix(std::string this_str, std::string prefix, std::string & return_val);


    //Simple DJB hash for strings, used to allow use of strings in switch statements
    //DO NOT use this for anything other than switch statements UNLESS you are fine with hash collisions 
    //https://stackoverflow.com/questions/650162/why-cant-the-switch-statement-be-applied-to-strings
    //Why 33 and 5381? https://stackoverflow.com/questions/10696223/reason-for-the-number-5381-in-the-djb-hash-function
    constexpr inline unsigned int djb_hash_for_switch(const char *s, int off = 0) {                        
        return !s[off] ? 5381 : (djb_hash_for_switch(s, off+1)*33) ^ s[off];                           
    }  

    bool string_is_number(const std::string& s);
}