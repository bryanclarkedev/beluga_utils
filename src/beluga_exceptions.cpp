#include "beluga_exceptions.h"


namespace beluga_utils
{
/*! \brief Exception function that prints the file and line number
\author Bryan Clarke 
\date 19/10/2024
\details This defines a throw_exception global function.
\return Doesn't, throws an exception.
It takes a string as an argument, and adds in the file and line number of the function where the exception is thrown.
Use this in place of regular exceptions if you want to speed debugging.
Based on example code from:
https://stackoverflow.com/questions/348833/how-to-know-the-exact-line-of-code-where-an-exception-has-been-caused/348862#348862
Usage:
#include "beluga_exceptions.h"
beluga_utils::throw_exception("This is my message, which will be printed in addition to the file and line number info");
DO NOT USE THE CLASS! The global function wraps it up for you.
*/
    void throw_exception(std::string s)
    {
        throw_line(s);
    }

}
