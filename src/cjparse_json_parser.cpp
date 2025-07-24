#include "../include/cjparse_json_parser.h"
#include <string_view>

cjparse_json_parser::cjparse_json_parser (
    std::string_view str, cjparse::cjparse_json_value &JSON_container)
{
   cjparse_invalid_json_input (str);

   cjparse_parse_value (str, JSON_container);
}

cjparse_json_parser::cjparse_json_parser (
    std::string_view str, cjparse::cjparse_json_value &JSON_container,
    std::string json_string_pattern_to_keep_raw)
{
   if (cjparse_invalid_json_input (str))
      throw 123; // do some error throwing

   inside_str_ignore.push_back (json_string_pattern_to_keep_raw);
   cjparse_parse_value (str, JSON_container);
}

cjparse_json_parser::cjparse_json_parser (
    std::string_view str, cjparse::cjparse_json_value &JSON_container,
    std::vector<std::string> json_string_pattern_to_keep_raw)
{
   if (cjparse_invalid_json_input (str))
      throw 123; // do some error throwing

   inside_str_ignore = json_string_pattern_to_keep_raw;
   cjparse_parse_value (str, JSON_container);
}

bool
cjparse_json_parser::cjparse_invalid_json_input (
    std::string_view json_to_validate)
{
   const size_t n = json_to_validate.size ();
   if (!n)
      return true; // EMPTY

   std::stack<char> s;
   char cha = 0;
   bool in_str = false;

   size_t i = 0, ln = 0, charl = 0;

   for (; i < n; i++, charl++)
      {
         if (json_to_validate[i] == '\n')
            {
               charl = 0;
               ln++;
               continue;
            }

         if (in_str && json_to_validate[i] == '\\')
            {
               i++;
               charl++;
               continue;
            }

         if (json_to_validate[i] == '{' || json_to_validate[i] == '[')
            {
               s.push (json_to_validate[i]);
            }
         else if (json_to_validate[i] == '"')
            {
               if (!in_str)
                  in_str = true;
               else
                  in_str = false;
            }
         else if ((json_to_validate[i] == '}' || json_to_validate[i] == ']')
                  && !in_str)
            {
               if (s.empty ())
                  {
                     throw cjparse_error (
                         ln, charl,
                         cjparse_error::ERROR_CODE::JSON_SYNTAX_ERROR);
                  }

               cha = s.top ();

               if ((cha == '{' && json_to_validate[i] != '}')
                   || (cha == '[' && json_to_validate[i] != ']'))
                  {
                     throw cjparse_error (
                         ln, charl,
                         cjparse_error::ERROR_CODE::JSON_SYNTAX_ERROR);
                  }

               s.pop ();
            }
      }

   if (!s.empty ())
      throw cjparse_error (ln, charl,
                           cjparse_error::ERROR_CODE::JSON_SYNTAX_ERROR);

   return true;
}

cjparse_json_parser::STATE_CHECK_VAL
cjparse_json_parser::check_what_is_the_value (std::string_view str)
{
   if (str.empty ())
      return VAL_IS_ERROR;
   else if (str[0] == '{')
      return VAL_IS_OBJECT;
   else if (str[0] == '[')
      return VAL_IS_ARRAY;
   else if (str[0] == '\"')
      return VAL_IS_STRING;
   else if (std::isdigit (str[0]) || str[0] == '-')
      return VAL_IS_NUMBER;
   else if (str == "true" || str == "false")
      return VAL_IS_BOOL;
   else if (str == "null")
      return VAL_IS_NULL;
   else
      return VAL_IS_ERROR; // bad parsing or bad JSON
};

std::string
cjparse_json_parser::decode_unicode (const std::string_view raw_str)
{
   std::string result;
   size_t i = 0;

   while (i < raw_str.size ())
      {
         // Check if the current substring matches any raw pattern
         bool is_raw = false;
         if (!inside_str_ignore.empty ()) // Only check if there are patterns
            {
               for (const auto &pattern : inside_str_ignore)
                  {
                     if (raw_str.compare (i, pattern.size (), pattern) == 0)
                        {
                           result += pattern; // Append raw pattern as-is
                           i += pattern.size ();
                           is_raw = true;
                           break; // Stop checking once a match is
                                  // found
                        }
                  }
            }
         if (is_raw)
            continue; // Skip further processing for this substring

         if (raw_str[i] == '\\' && i + 1 < raw_str.size ())
            {
               std::string_view escape_seq = raw_str.substr (
                   i, 2); // Get two-character escape sequence

               // If the escape sequence is in inside_str_ignore, append
               // as-is
               if (std::find (inside_str_ignore.begin (),
                              inside_str_ignore.end (), escape_seq)
                   != inside_str_ignore.end ())
                  {
                     result += escape_seq;
                     i += 2;
                     continue;
                  }

               // Handle common escape sequences
               switch (raw_str[i + 1])
                  {
                  case 'n':
                     result += '\n';
                     i += 2;
                     break;
                  case 't':
                     result += '\t';
                     i += 2;
                     break;
                  case '"':
                     result += '"';
                     i += 2;
                     break;
                  case '\\':
                     result += '\\';
                     i += 2;
                     break;

                  case 'u': // Unicode escape sequence
                     if (i + 5 < raw_str.size ())
                        {
                           std::stringstream ss;
                           ss << std::hex << raw_str.substr (i + 2, 4);
                           int codePoint;
                           ss >> codePoint;

                           if (codePoint >= 0xD800 && codePoint <= 0xDBFF
                               && i + 9 < raw_str.size ()
                               && raw_str[i + 6] == '\\'
                               && raw_str[i + 7] == 'u')
                              {
                                 std::stringstream lowSurrogate;
                                 lowSurrogate << std::hex
                                              << raw_str.substr (i + 8, 4);
                                 int lowCodePoint;
                                 lowSurrogate >> lowCodePoint;

                                 codePoint = ((codePoint - 0xD800) << 10)
                                             + (lowCodePoint - 0xDC00)
                                             + 0x10000;
                                 i += 6;
                              }

                           if (codePoint <= 0x7F)
                              {
                                 result += static_cast<char> (codePoint);
                              }
                           else if (codePoint <= 0x7FF)
                              {
                                 result += static_cast<char> ((codePoint >> 6)
                                                              | 0xC0);
                                 result += static_cast<char> (
                                     (codePoint & 0x3F) | 0x80);
                              }
                           else if (codePoint <= 0xFFFF)
                              {
                                 result += static_cast<char> ((codePoint >> 12)
                                                              | 0xE0);
                                 result += static_cast<char> (
                                     ((codePoint >> 6) & 0x3F) | 0x80);
                                 result += static_cast<char> (
                                     (codePoint & 0x3F) | 0x80);
                              }
                           else
                              {
                                 result += static_cast<char> ((codePoint >> 18)
                                                              | 0xF0);
                                 result += static_cast<char> (
                                     ((codePoint >> 12) & 0x3F) | 0x80);
                                 result += static_cast<char> (
                                     ((codePoint >> 6) & 0x3F) | 0x80);
                                 result += static_cast<char> (
                                     (codePoint & 0x3F) | 0x80);
                              }

                           i += 6;
                        }
                     break;

                  default:
                     // If it's an unknown escape sequence, just add the
                     // backslash as-is
                     result += raw_str[i];
                     i++;
                  }
            }
         else
            {
               result += raw_str[i];
               i++;
            }
      }

   return result;
}

cjparse::json_string
cjparse_json_parser::cjparse_parse_value_string (std::string_view str)
{
   std::size_t st_of_string = str.find ('\"', 0);
   std::size_t en_of_string = return_the_matching_pattern (str, 0, '\"', '\"');

   std::string str_to_ret = decode_unicode (
       str.substr (st_of_string + 1, en_of_string - st_of_string - 1));

   return str_to_ret;
}

cjparse::json_number
cjparse_json_parser::cjparse_parse_value_number (std::string_view str)
{
   std::string str_number_only;
   for (char ch : str)
      {
         if (std::isdigit (ch))
            str_number_only.push_back (ch);
         if (ch == 'e' || ch == 'E')
            str_number_only.push_back (ch);
         if (ch == '.')
            str_number_only.push_back (ch);
         if (ch == '-' || ch == '+')
            str_number_only.push_back (ch);
      }
   // If nothing valid was found, throw an error.
   if (str_number_only.empty ())
      {
         throw std::invalid_argument ("No valid numeric characters found.");
      }
   try
      {
         // If we detect a decimal point or exponent, treat it as a floating
         // point number.
         if (str_number_only.find ('.') != std::string::npos
             || str_number_only.find ('e') != std::string::npos
             || str_number_only.find ('E') != std::string::npos)
            {
               // Convert using std::stod (or std::stold if you need more
               // precision)
               double d = std::stod (str_number_only);
               return d; // json_number will hold a double
            }
         else
            {
               // For integer numbers, use stoll first to capture large
               // values.
               long long int ll = std::stoll (str_number_only);

               // Choose the smallest type that can fit the value.
               if (ll >= std::numeric_limits<int>::min ()
                   && ll <= std::numeric_limits<int>::max ())
                  {
                     return static_cast<int> (ll);
                  }
               else if (ll >= std::numeric_limits<long int>::min ()
                        && ll <= std::numeric_limits<long int>::max ())
                  {
                     return static_cast<long int> (ll);
                  }
               else
                  {
                     return ll; // fits in long long int
                  }
            }
      }
   catch (const std::out_of_range &)
      {
         // If the number is too large for integer types, return max
         // possible
         if (str_number_only.find ('.') != std::string::npos
             || str_number_only.find ('e') != std::string::npos
             || str_number_only.find ('E') != std::string::npos)
            {
               return std::numeric_limits<double>::max (); // Max representable
                                                           // double
            }
         else
            {
               return std::numeric_limits<
                   long long int>::max (); // Max integer value
            }
      }
   catch (const std::invalid_argument &)
      {
         throw std::invalid_argument (
             "Conversion error: invalid argument in number conversion.");
      }
}

bool
cjparse_json_parser::cjparse_parse_value_bool (std::string_view str)
{
   if (str.substr (0, 4) == "true")
      return true;
   else
      return false;
}

cjparse::json_null
cjparse_json_parser::cjparse_parse_value_null (std::string_view str)
{
   return std::any ();
}

void
cjparse_json_parser::cjparse_parse_array (std::string_view str,
                                          cjparse::cjparse_json_value &value)
{
   // TO BE FIXXED SOON
   cjparse::json_array array;
   cjparse::cjparse_json_value temp_value;

   std::size_t outter_initial_delimeter = str.find_first_of ('[', 0);
   std::size_t outter_final_delimiter
       = return_the_matching_pattern (str, outter_initial_delimeter, '[', ']');

   std::string obj_name, str_temp;

   bool nested = false;
   STATE_PARSE_LOOP state = STATE_INITIAL;

   bool keep_looping = true;

   std::size_t curr_outer_initial_delimeter,
       curr_outer_final_delimeter = outter_initial_delimeter,
       not_white_after_curr_outer_initial, not_white_after_curr_outer_final;

   std::string next_delimiter = { ',', '}', ']' };

   while (keep_looping)
      {
         std::string chars_to_skip{ str[curr_outer_final_delimeter], ' ', '\n',
                                    '\r', '\t' };

         if (state == STATE_INITIAL)
            {
               curr_outer_initial_delimeter = str.find_first_not_of (
                   chars_to_skip, curr_outer_final_delimeter);
               if (str[curr_outer_initial_delimeter] == ',')
                  curr_outer_initial_delimeter = str.find_first_not_of (
                      chars_to_skip, curr_outer_initial_delimeter);
               not_white_after_curr_outer_initial = str.find_first_of (
                   chars_to_skip, curr_outer_initial_delimeter + 1);
               str_temp = std::move (
                   str.substr (curr_outer_initial_delimeter,
                               not_white_after_curr_outer_initial
                                   - curr_outer_initial_delimeter));
               STATE_CHECK_VAL temp_state = check_what_is_the_value (str_temp);
               if (temp_state == VAL_IS_ERROR)
                  throw 123; // TODO : make it mean something, bad error
               else if (temp_state == VAL_IS_OBJECT
                        || temp_state == VAL_IS_ARRAY
                        || temp_state == VAL_IS_STRING)
                  state = STATE_PARSING_CMPLX;
               else
                  state = STATE_PARSING_VAL;
            }
         if (state == STATE_PARSING_CMPLX)
            {
               find_delimeters_check_if_comma_alter_state (
                   str, curr_outer_initial_delimeter,
                   curr_outer_final_delimeter,
                   not_white_after_curr_outer_final,
                   str[curr_outer_initial_delimeter]);
               state = STATE_PARSING_NESTED_VAL;
            }
         if (state == STATE_PARSING_VAL)
            {
               curr_outer_final_delimeter = not_white_after_curr_outer_initial;
               not_white_after_curr_outer_final = str.find_first_not_of (
                   chars_to_skip.substr (1), curr_outer_final_delimeter);
               state = STATE_ACTUALLY_PARSE_CMPLX;
            }
         if (state == STATE_ACTUALLY_PARSE_CMPLX)
            {
               state = STATE_PARSING_NESTED_VAL_NEXT;
               if (not_white_after_curr_outer_final != std::string::npos
                   && not_white_after_curr_outer_final
                          <= outter_final_delimiter)
                  state = STATE_ACTUALLY_PARSE_VAL;
               else
                  nested = false;
            }
         if (state == STATE_ACTUALLY_PARSE_VAL)
            {
               nested = str[not_white_after_curr_outer_final] == ',';
               state = STATE_PARSING_NESTED_VAL_NEXT;
            }
         if (state == STATE_PARSING_NESTED_VAL)
            {
               state = STATE_PARSING_NESTED_CMPLX_NEXT;
               if (not_white_after_curr_outer_final != std::string::npos
                   && not_white_after_curr_outer_final
                          <= outter_final_delimiter)
                  state = STATE_PARSING_NESTED_CMPLX;
               else
                  nested = false;
            }
         if (state == STATE_PARSING_NESTED_CMPLX)
            {
               nested = str[not_white_after_curr_outer_final] == ',';
               state = STATE_PARSING_NESTED_CMPLX_NEXT;
            }
         if (state == STATE_PARSING_NESTED_VAL_NEXT)
            {
               std::string_view str_value = str.substr (
                   curr_outer_initial_delimeter,
                   curr_outer_final_delimeter - curr_outer_initial_delimeter);
               cjparse_json_parser::cjparse_parse_value (str_value,
                                                         temp_value);
               array.push_back (temp_value);
               if (nested)
                  state = STATE_INITIAL;
               else
                  keep_looping = false;
            }
         if (state == STATE_PARSING_NESTED_CMPLX_NEXT)
            {
               std::string_view str_value
                   = str.substr (curr_outer_initial_delimeter,
                                 curr_outer_final_delimeter
                                     - curr_outer_initial_delimeter + 1);
               cjparse_json_parser::cjparse_parse_value (str_value,
                                                         temp_value);
               array.push_back (temp_value);
               if (nested)
                  state = STATE_INITIAL;
               else
                  keep_looping = false;
            }
      }

   value = cjparse::cjparse_json_value (array);
}

void
cjparse_json_parser::cjparse_parse_object (std::string_view str,
                                           cjparse::cjparse_json_value &value)
{
   cjparse::json_object object;
   cjparse::cjparse_json_value temp_value;

   std::size_t outter_initial_delimeter = str.find_first_of ('{', 0);
   std::size_t outter_final_delimiter
       = return_the_matching_pattern (str, outter_initial_delimeter, '{', '}');

   std::size_t st_of_name, en_of_name, double_point_after_name,
       not_white_after_double_point;
   std::string obj_name, str_temp;

   bool nested = false;
   STATE_PARSE_LOOP state = STATE_INITIAL;

   bool keep_looping = true;

   std::size_t curr_outer_initial_delimeter,
       curr_outer_final_delimeter = outter_initial_delimeter,
       not_white_after_curr_outer_initial, not_white_after_curr_outer_final;

   std::string next_delimiter = { ',', '}', ']' };

   while (keep_looping)
      {
         std::string chars_to_skip{ str[curr_outer_final_delimeter], ' ', '\n',
                                    '\r', '\t' };

         if (state == STATE_INITIAL)
            {
               st_of_name
                   = str.find_first_of ('\"', curr_outer_final_delimeter + 1);
               en_of_name
                   = return_the_matching_pattern (str, st_of_name, '\"', '\"');
               obj_name = std::move (
                   str.substr (st_of_name + 1, en_of_name - st_of_name - 1));
               double_point_after_name
                   = str.find_first_of (':', en_of_name + 1);
               if (str[double_point_after_name - 1] == '\\')
                  {
                  } // HORRIBLE JSON

               curr_outer_initial_delimeter = str.find_first_not_of (
                   chars_to_skip.substr (1), double_point_after_name + 1);
               not_white_after_curr_outer_initial = str.find_first_of (
                   next_delimiter, curr_outer_initial_delimeter);
               str_temp = str.substr (curr_outer_initial_delimeter,
                                      not_white_after_curr_outer_initial
                                          - curr_outer_initial_delimeter);

               STATE_CHECK_VAL temp_state = check_what_is_the_value (str_temp);
               if (temp_state == VAL_IS_ERROR)
                  throw 123; // TODO : make it mean something, bad error
               else if (temp_state == VAL_IS_OBJECT
                        || temp_state == VAL_IS_ARRAY
                        || temp_state == VAL_IS_STRING)
                  state = STATE_PARSING_CMPLX;
               else
                  state = STATE_PARSING_VAL;
            }
         else if (state == STATE_PARSING_CMPLX)
            {
               find_delimeters_check_if_comma_alter_state (
                   str, curr_outer_initial_delimeter,
                   curr_outer_final_delimeter,
                   not_white_after_curr_outer_final,
                   str[curr_outer_initial_delimeter]);
               state = STATE_PARSING_NESTED_VAL;
            }
         else if (state == STATE_PARSING_VAL)
            {
               curr_outer_final_delimeter = not_white_after_curr_outer_initial;
               not_white_after_curr_outer_final = str.find_first_not_of (
                   chars_to_skip.substr (1), curr_outer_final_delimeter);
               state = STATE_ACTUALLY_PARSE_CMPLX;
            }
         else if (state == STATE_ACTUALLY_PARSE_CMPLX)
            {
               state = STATE_PARSING_NESTED_VAL_NEXT;
               if (not_white_after_curr_outer_final != std::string::npos
                   && not_white_after_curr_outer_final
                          <= outter_final_delimiter)
                  state = STATE_ACTUALLY_PARSE_VAL;
               else
                  nested = false;
            }
         else if (state == STATE_ACTUALLY_PARSE_VAL)
            {
               nested = str[not_white_after_curr_outer_final] == ',';
               state = STATE_PARSING_NESTED_VAL_NEXT;
            }
         else if (state == STATE_PARSING_NESTED_VAL)
            {
               state = STATE_PARSING_NESTED_CMPLX_NEXT;
               if (not_white_after_curr_outer_final != std::string::npos
                   && not_white_after_curr_outer_final
                          <= outter_final_delimiter)
                  state = STATE_PARSING_NESTED_CMPLX;
               else
                  nested = false;
            }
         else if (state == STATE_PARSING_NESTED_CMPLX)
            {
               nested = str[not_white_after_curr_outer_final] == ',';
               state = STATE_PARSING_NESTED_CMPLX_NEXT;
            }
         else if (state == STATE_PARSING_NESTED_VAL_NEXT)
            {
               std::string_view str_value = str.substr (
                   curr_outer_initial_delimeter,
                   curr_outer_final_delimeter - curr_outer_initial_delimeter);
               cjparse_json_parser::cjparse_parse_value (str_value,
                                                         temp_value);
               auto repeted = object.find (obj_name);
               bool name_already_exists = repeted != object.end ();
               if (name_already_exists)
                  obj_name = obj_name + std::to_string (rand () % 1000);

               object[obj_name] = temp_value;
               if (nested)
                  state = STATE_INITIAL;
               else
                  keep_looping = false;
            }
         else if (state == STATE_PARSING_NESTED_CMPLX_NEXT)
            {
               std::string_view str_value
                   = str.substr (curr_outer_initial_delimeter,
                                 curr_outer_final_delimeter
                                     - curr_outer_initial_delimeter + 1);
               cjparse_json_parser::cjparse_parse_value (str_value,
                                                         temp_value);
               auto repeted = object.find (obj_name);
               bool name_already_exists = repeted != object.end ();
               if (name_already_exists)
                  obj_name
                      = obj_name
                        + std::to_string (
                            rand () % 1000); // will have to change this to
                                             // make it predictable.....
                                             //
                                             // TODO : MAYBE WITH ENUMS???
                                             // BUT IDK

               object[obj_name] = std::move (temp_value);
               if (nested)
                  state = STATE_INITIAL;
               else
                  keep_looping = false;
            }
         else
            throw 123; // TODO : make it mean something (maybe custom exception
                       // class????)
      }

   value = cjparse::cjparse_json_value (object);
}

void
cjparse_json_parser::cjparse_parse_value (std::string_view str,
                                          cjparse::cjparse_json_value &value)
{
   STATE_CHECK_VAL what_is_the_value = check_what_is_the_value (str);
   if (what_is_the_value == VAL_IS_ERROR)
      throw 123; // TODO !! MAKE IT MEAN something
   else if (what_is_the_value == VAL_IS_OBJECT)
      cjparse_json_parser::cjparse_parse_object (str, value);
   else if (what_is_the_value == VAL_IS_ARRAY)
      cjparse_json_parser::cjparse_parse_array (str, value);
   else if (what_is_the_value == VAL_IS_STRING)
      value = cjparse_json_parser::cjparse_parse_value_string (str);
   else if (what_is_the_value == VAL_IS_NUMBER)
      value = cjparse_json_parser::cjparse_parse_value_number (str);
   else if (what_is_the_value == VAL_IS_BOOL)
      value = cjparse_json_parser::cjparse_parse_value_bool (str);
   else if (what_is_the_value == VAL_IS_NULL)
      value = cjparse_json_parser::cjparse_parse_value_null (str);
}

std::size_t
cjparse_json_parser::return_the_matching_pattern (
    std::string_view str, std::size_t pos_of_bracket_to_match, char pattern,
    char pattern_to_match)
{
   const std::size_t n = str.size ();
   if (pos_of_bracket_to_match >= n || str[pos_of_bracket_to_match] != pattern)
      return std::string::npos;

   bool in_str = false;
   int depth = 0;

   for (size_t i = pos_of_bracket_to_match; i < n; ++i)
      {
         // to match '\"'
         if (str[i] == '"')
            {
               size_t backslash_count = 0;
               for (ssize_t j = static_cast<ssize_t> (i) - 1;
                    j >= 0 && str[j] == '\\'; --j)
                  {
                     backslash_count++;
                  }

               if (backslash_count % 2 == 0) // Comilla NO escapada
                  in_str = !in_str;
            }

         if (pattern == '"' && i > pos_of_bracket_to_match)
            {
               size_t backslash_count = 0;
               for (ssize_t j = static_cast<ssize_t> (i) - 1;
                    j >= 0 && str[j] == '\\'; --j)
                  {
                     backslash_count++;
                  }

               if (str[i] == '"' && backslash_count % 2 == 0)
                  {
                     return i; // encontramos la comilla no escapada
                  }

               continue;
            }

         // to match '[' and '('
         if (in_str)
            continue;

         if (str[i] == pattern)
            {
               depth++;
            }
         else if (str[i] == pattern_to_match)
            {
               depth--;
               if (depth == 0)
                  return i;
            }
      }

   return std::string::npos;
}

void
cjparse_json_parser::find_delimeters_check_if_comma_alter_state (
    std::string_view str, std::size_t &initial_delimeter,
    std::size_t &final_delimeter,
    std::size_t &not_white_position_after_final_delimiter, char pattern)
{
   char pattern_to_match;
   if (pattern == '{')
      pattern_to_match = '}';
   else if (pattern == '[')
      pattern_to_match = ']';
   else if (pattern == '\"')
      pattern_to_match = pattern;
   else
      return;

   final_delimeter = return_the_matching_pattern (str, initial_delimeter,
                                                  pattern, pattern_to_match);

   std::string chars_to_skip{ pattern_to_match, ' ', '\n', '\r', '\t' };

   not_white_position_after_final_delimiter
       = str.find_first_not_of (chars_to_skip, final_delimeter);
}
