#include "cjparse.h"
#include <string_view>

#ifndef CJPARSE_JSON_PARSER
#define CJPARSE_JSON_PARSER

class cjparse_json_parser
{
private:
  enum STATE_CHECK_VAL
  {
    VAL_IS_ERROR = 0,
    VAL_IS_OBJECT = 1,
    VAL_IS_ARRAY = 2,
    VAL_IS_STRING = 3,
    VAL_IS_NUMBER = 4,
    VAL_IS_BOOL = 5,
    VAL_IS_NULL = 6
  };
  enum STATE_PARSE_LOOP
  {
    STATE_INITIAL = 0,
    STATE_PARSING_CMPLX = 1,
    STATE_PARSING_VAL = 2,
    STATE_ACTUALLY_PARSE_CMPLX = 3,
    STATE_ACTUALLY_PARSE_VAL = 4,
    STATE_PARSING_NESTED_VAL = 5,
    STATE_PARSING_NESTED_CMPLX = 6,
    STATE_PARSING_NESTED_VAL_NEXT = 7,
    STATE_PARSING_NESTED_CMPLX_NEXT = 8
  };

public:
  cjparse_json_parser (std::string_view json_to_parse,
                       cjparse::cjparse_json_value &JSON_container);

  cjparse_json_parser (std::string_view json_to_parse,
                       cjparse::cjparse_json_value &JSON_container,
                       std::string json_string_pattern_to_keep_raw);

  cjparse_json_parser (
      std::string_view json_to_parse,
      cjparse::cjparse_json_value &JSON_container,
      std::vector<std::string> json_string_pattern_to_keep_raw);

private:
  bool cjparse_invalid_json_input (std::string_view json_to_validate);

private:
  // modify by reference the container due to nesting
  void cjparse_parse_value (std::string_view str,
                            cjparse::cjparse_json_value &value);

  void cjparse_parse_array (std::string_view str,
                            cjparse::cjparse_json_value &value);

  void cjparse_parse_object (std::string_view str,
                             cjparse::cjparse_json_value &value);

  // assign value by returning it
  cjparse::json_string cjparse_parse_value_string (std::string_view str);

  cjparse::json_number cjparse_parse_value_number (std::string_view str);

  bool cjparse_parse_value_bool (std::string_view str);

  cjparse::json_null cjparse_parse_value_null (std::string_view str);

private:
  // internal helper functions
  STATE_CHECK_VAL
  check_what_is_the_value (std::string_view str);

  // might be possible to improve this algo but it works like magic
  std::size_t return_the_matching_pattern (std::string_view str,
                                           std::size_t pos_of_bracket_to_match,
                                           char pattern, char patter_to_match);

  // exists only for redibility inside the while loops
  void find_delimeters_check_if_comma_alter_state (
      std::string_view str, std::size_t &initial_delimeter,
      std::size_t &final_delimeter,
      std::size_t &not_white_position_after_final_delimeter, char pattern);

  std::string decode_unicode (const std::string_view str);

private:
  std::vector<std::string> inside_str_ignore;
};

#endif // CJPARSE_JSON_PARSER
