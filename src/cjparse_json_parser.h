#include "cjparse.h"

#ifndef CJPARSE_JSON_PARSER
#define CJPARSE_JSON_PARSER

class cjparse_json_parser
{
  public:
    cjparse_json_parser (std::string &json_to_parse,
                         cjparse::cjparse_json_value &JSON_container);

  public:
    // modify by reference the container due to nesting
    void cjparse_parse_value (std::string &str,
                              cjparse::cjparse_json_value &value);

    void cjparse_parse_array (std::string &str,
                              cjparse::cjparse_json_value &value);

    void cjparse_parse_object (std::string &str,
                               cjparse::cjparse_json_value &value);

    // assign value by returning it
    cjparse::json_string cjparse_parse_value_string (std::string &str);

    cjparse::json_number cjparse_parse_value_number (std::string &str);

    bool cjparse_parse_value_bool (std::string &str);

    cjparse::json_null cjparse_parse_value_null (std::string &str);

  private:
    int check_what_is_the_value (std::string &str);

    std::size_t
    return_the_matching_pattern (std::string &str,
                                 std::size_t pos_of_bracket_to_match,
                                 char pattern, char patter_to_match);

    void find_delimeters_check_if_comma_alter_state (
        std::string &str, std::size_t &initial_delimeter,
        std::size_t &final_delimeter,
        std::size_t &not_white_position_after_final_delimeter, char pattern);
};

#endif // CJPARSE_JSON_PARSER
