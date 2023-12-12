#ifndef COLOR_H
#define COLOR_H

#include <ostream>
#include <string>
#include <iostream>

namespace Color
{
    enum Code
    {
        FG_RED = 31,
        FG_GREEN = 32,
        FG_BLUE = 34,
        FG_DEFAULT = 39,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_BLUE = 44,
        BG_DEFAULT = 49
    };
    class Mod
    {
        Code code;

    public:
        Mod(Code pCode) : code(pCode) {}
        friend std::ostream &
        operator<<(std::ostream &os, const Mod &mod)
        {
            return os << "\033[" << mod.code << "m";
        }
        static std::string getString(Code colorCode, const std::string &input)
        {
            return "\033[" + std::to_string(colorCode) + "m" + input + "\033[" + std::to_string(Color::FG_DEFAULT) + "m";
        }
    };
};

#endif
