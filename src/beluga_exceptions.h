#pragma once 

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace beluga_utils
{
/*! \brief Exception class; not to be used, use the throw_exception global function.
\author Bryan Clarke 
\date 19/10/2024
\details This class is used to create exceptions that report the file and line number.
Based on example code from:
https://stackoverflow.com/questions/348833/how-to-know-the-exact-line-of-code-where-an-exception-has-been-caused/348862#348862
Usage: Don't use this. Use beluga_utils::throw_exceptions("This message gets reported"). It creates the beluga_utils::exception and calls
the functions for you.
*/
    class exception : public std::runtime_error {
        std::string msg;
        public:
            exception(const std::string &arg, const char *file, int line) :
            std::runtime_error(arg) {
                std::ostringstream oss;
                oss << file << ":" << line << ": " << arg;
                msg = oss.str();
            }
            ~exception() throw() {}
            const char *what() const throw() {
                return msg.c_str();
            }
    };

    #define throw_line(arg) throw beluga_utils::exception(arg, __FILE__, __LINE__);
    void throw_exception(std::string s);
}