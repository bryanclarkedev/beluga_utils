#pragma once
//To convert string to hash
//https://stackoverflow.com/questions/8029121/how-to-hash-stdstring
#include <unordered_map>
#include <sstream>
#include <utility> //for decltype and declval

//https://forum.arduino.cc/t/error-cannot-use-typeid-with-fno-rtti/131351
//Also https://arduino.stackexchange.com/questions/3079/how-to-retrieve-the-data-type-of-a-variable
//The second link has a decent piece of code to workaround this, converting type to a string
#if 0
template <typename T>
char * type_name(const T&)
{
    const char* pf = __PRETTY_FUNCTION__;
    char* begin = strstr(pf, "[with T = ") + 10;
    char* end = strrchr(pf, ']');
    *end = 0;
    return begin;
}
#endif

#if 0
template <class T>
std::string type_name(const T&)
{
    std::string s = __PRETTY_FUNCTION__;
    int start = s.indexOf("[with T = ") + 10;
    std::size_t found1 = str.find("[with T = ");
    std::size_t found2 = str.find("]");
    
    return s.substring(start, stop);
}
#endif

namespace beluga_utils
{
    template <typename T>
    size_t get_type_hash(const T& t)
    {
        std::hash<std::string> hasher;
        std::string s = __PRETTY_FUNCTION__;

        //String s = type_name(t);
        //std::stringstream ss;
        //ss << c;
        //std::string s(ss.str());
        return hasher(s);
    }
        
    template <typename T>
    std::string type_name(const T& t, bool retain_namespace = false)
    {

        std::string s = __PRETTY_FUNCTION__;
        /*If we use start_chars == "[with T = "
        and end_chars == "]" 
        then for type T == beluga_core::digital_input we get:
        [with T = beluga_core::digital_input; std::__cxx11::string = std::__cxx11::basic_string<char>]
        */
        std::string start_chars = std::string("[with T = ");
        std::string end_chars = std::string("; std::");
        std::size_t start = s.find(start_chars.c_str()) + start_chars.size() ;
        std::size_t stop = s.find(end_chars.c_str());
        s = s.substr(start, stop - start);
        //For type T == beluga_core::digital_input we get: "beluga_core::digital_input"
        if(retain_namespace)
        {
            return s;
        }
        std::vector<std::string> v = split_string(s, "::");
        return v[v.size()-1]; //Last entry in vector. This should still work even if there is no namespace.

    }

    //WE need a function for shared_ptr else we end up with a trailing >
    //We might be able to merge these. Future problems.            
    template <typename T>
    std::string type_name(std::shared_ptr<T> t, bool retain_namespace = false)
    {
         std::string s = __PRETTY_FUNCTION__;

         std::string start_chars = std::string("[with T = ");
        std::string end_chars = std::string(";");
        std::size_t start = s.find(start_chars.c_str()) + start_chars.size() ;
        std::size_t stop = s.find(end_chars.c_str());
        s = s.substr(start, stop - start);

        //For type T == beluga_core::digital_input we get: "beluga_core::digital_input"
        if(retain_namespace)
        {
            return s;
        }
        std::vector<std::string> v = split_string(s, "::");
        return v[v.size()-1]; //Last entry in vector. This should still work even if there is no namespace.
    }



}