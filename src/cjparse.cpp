#include "cjparse.h"
#include "cjparse_json_parser.cpp"

cjparse::cjparse (std::string &str) { cjparse_json_parser (str, JSON); }

cjparse::cjparse (std::stringstream &fake_str)
{
    // to be determied
}
