#include "cjparse.h"

cjparse::cjparse (std::string &str)
{
    cjparse_json_parser::cjparse_parse_value (str, JSON);
}

int
cjparse_json_parser::check_what_is_the_value (std::string &str)
{
    if (str.length () == 0)
        {
            // ERROR 0;
        }
    if (str[0] == '{')
        return 1; // OBJECT
    if (str[0] == '[')
        return 2; // ARRAY
    if (str[0] == '\"')
        return 3; // STRING
    if (std::isdigit (str[0]))
        return 4; // NUMBER
    if (str == "true" || str == "false")
        return 5; // BOOL
    if (str == "null")
        return 6; // NULL
};

std::string
cjparse_json_parser::cjparse_parse_value_string (std::string &str)
{
    std::size_t st_of_string = str.find ('\"', 0);
    std::size_t en_of_string = str.find ('\"', st_of_string + 1);
    check_if_prev_is_backlash (str, en_of_string, '\"');

    return str.substr (st_of_string + 1, en_of_string - st_of_string - 2);
}

std::variant<int, long int, long long int, double, long double>
cjparse_json_parser::cjparse_parse_value_number (std::string &str)
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
    if (std::binary_search (str_number_only.begin (), str_number_only.end (),
                            '.')
        || std::binary_search (str_number_only.begin (),
                               str_number_only.end (), 'e')
        || std::binary_search (str_number_only.begin (),
                               str_number_only.end (), 'E'))
        {
            return std::stod (str_number_only);
        }
    else
        {
            return std::stoi (str_number_only);
        }
}

bool
cjparse_json_parser::cjparse_parse_value_bool (std::string &str)
{
    if (str.substr (0, 4) == "true")
        return true;
    else
        return false;
}

std::any
cjparse_json_parser::cjparse_parse_value_null (std::string &str)
{
    return std::any ();
}

void
cjparse_json_parser::cjparse_parse_array (std::string &str,
                                          cjparse::cjparse_json_value &value)
{
    // TO BE FIXXED SOON
    cjparse::json_array array;
    cjparse::cjparse_json_value temp_value;

    std::size_t outter_initial_delimeter = str.find_first_of ('[', 0);
    std::size_t outter_final_delimiter
        = return_the_matching_bracket (str, outter_initial_delimeter, '[');

    std::string obj_name, str_temp;

    int state = -1;
    bool keep_looping = true;

    std::size_t initial_delimeter, final_delimeter = outter_initial_delimeter,
                                   not_white_position_after_initial_delimeter,
                                   not_white_position_after_final_delimiter;
    while (keep_looping)
        {
            if (state == -1)
                {
                    not_white_position_after_initial_delimeter
                        = str.find_first_not_of (
                            { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                            final_delimeter + 1);

                    not_white_position_after_final_delimiter
                        = str.find_first_of (
                            { ',', '}', ']' },
                            not_white_position_after_initial_delimeter);

                    str_temp = str.substr (
                        not_white_position_after_initial_delimeter,
                        not_white_position_after_final_delimiter
                            - not_white_position_after_initial_delimeter);

                    std::cout << "str_temp " << str_temp << '\n';

                    state = check_what_is_the_value (str_temp);
                    if (state == 4 || state == 5 || state == 6)
                        state = 4;
                }
            if (state == 0)
                {
                    std::string str_value
                        = str.substr (initial_delimeter,
                                      final_delimeter - initial_delimeter);
                    std::cout << "bewteen commas: " << str_value << "\n";
                    cjparse_json_parser::cjparse_parse_value (str_value,
                                                              temp_value);
                    array.push_back (temp_value);
                    state = -1;
                }
            if (state == 1)
                {
                    cjparse_json_parser::
                        find_delimeters_check_if_comma_alter_state (
                            str, not_white_position_after_final_delimiter,
                            initial_delimeter, final_delimeter,
                            not_white_position_after_final_delimiter, state,
                            '{');
                }
            if (state == 2)
                {
                    cjparse_json_parser::
                        find_delimeters_check_if_comma_alter_state (
                            str, not_white_position_after_initial_delimeter,
                            initial_delimeter, final_delimeter,
                            not_white_position_after_final_delimiter, state,
                            '[');
                }
            if (state == 3)
                {
                    cjparse_json_parser::
                        find_delimeters_check_if_comma_alter_state (
                            str, not_white_position_after_initial_delimeter,
                            initial_delimeter, final_delimeter,
                            not_white_position_after_final_delimiter, state,
                            '\"');
                }
            if (state == 4)
                {
                    initial_delimeter
                        = not_white_position_after_initial_delimeter;
                    final_delimeter
                        = str.find_first_of (',', initial_delimeter);
                    state = 5;
                    if (final_delimeter < outter_final_delimiter)
                        state = 0;
                }
            if (state == 5)
                {
                    std::string str_value = str.substr (
                        initial_delimeter,
                        outter_final_delimiter - initial_delimeter);
                    std::cout << "num/bool/null/final: " << str_value << '\n';
                    cjparse_json_parser::cjparse_parse_value (str_value,
                                                              temp_value);
                    array.push_back (temp_value);
                    keep_looping = false;
                }
            if (state == 6)
                {
                    std::string str_value
                        = str.substr (initial_delimeter,
                                      final_delimeter - initial_delimeter + 1);
                    std::cout << "final obj/arr/str: " << str_value << '\n';
                    cjparse_json_parser::cjparse_parse_value (str_value,
                                                              temp_value);
                    array.push_back (temp_value);
                    keep_looping = false;
                }
        }

    value = cjparse::cjparse_json_value (array);
}

void
cjparse_json_parser::cjparse_parse_object (std::string &str,
                                           cjparse::cjparse_json_value &value)
{
    cjparse::json_object object;
    cjparse::cjparse_json_value temp_value;

    std::size_t outter_initial_delimeter = str.find_first_of ('{', 0);
    std::size_t outter_final_delimiter
        = return_the_matching_bracket (str, outter_initial_delimeter, '{');

    std::size_t st_of_name, en_of_name, double_point_after_name,
        not_white_after_double_point;
    std::string obj_name, str_temp;

    int state = -1;
    bool keep_looping = true;

    std::size_t initial_delimeter, final_delimeter = outter_initial_delimeter,
                                   not_white_position_after_final_delimiter;
    while (keep_looping)
        {
            if (state == -1)
                {
                    st_of_name = str.find_first_of ('\"', final_delimeter + 1);
                    en_of_name = str.find_first_of ('\"', st_of_name + 1);
                    check_if_prev_is_backlash (str, en_of_name, '\"');

                    obj_name = str.substr (st_of_name + 1,
                                           en_of_name - st_of_name - 1);
                    std::cout << "obj_name " << obj_name << '\n';

                    double_point_after_name
                        = str.find_first_of (':', en_of_name + 1);
                    if (str[double_point_after_name - 1] == '\\')
                        {
                        } // HORRIBLE JSON

                    not_white_after_double_point = str.find_first_not_of (
                        { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                        double_point_after_name + 1);

                    not_white_position_after_final_delimiter
                        = str.find_first_of ({ ',', '}', ']' },
                                             not_white_after_double_point);

                    str_temp
                        = str.substr (not_white_after_double_point,
                                      not_white_position_after_final_delimiter
                                          - not_white_after_double_point);
                    std::cout << "str_temp " << str_temp << '\n';

                    state = check_what_is_the_value (str_temp);
                    if (state == 4 || state == 5 || state == 6)
                        state = 4;
                }
            if (state == 0)
                {
                    std::string str_value
                        = str.substr (initial_delimeter,
                                      not_white_position_after_final_delimiter
                                          - initial_delimeter);
                    std::cout << "bewteen commas: " << str_value << "\n";
                    cjparse_json_parser::cjparse_parse_value (str_value,
                                                              temp_value);
                    object[obj_name] = temp_value;
                    state = -1;
                }
            if (state == 1)
                {
                    cjparse_json_parser::
                        find_delimeters_check_if_comma_alter_state (
                            str, not_white_after_double_point,
                            initial_delimeter, final_delimeter,
                            not_white_position_after_final_delimiter, state,
                            '{');
                }
            if (state == 2)
                {
                    cjparse_json_parser::
                        find_delimeters_check_if_comma_alter_state (
                            str, not_white_after_double_point,
                            initial_delimeter, final_delimeter,
                            not_white_position_after_final_delimiter, state,
                            '[');
                }
            if (state == 3)
                {
                    cjparse_json_parser::
                        find_delimeters_check_if_comma_alter_state (
                            str, not_white_after_double_point,
                            initial_delimeter, final_delimeter,
                            not_white_position_after_final_delimiter, state,
                            '\"');
                }
            if (state == 4)
                {
                    initial_delimeter = not_white_after_double_point;
                    final_delimeter = not_white_position_after_final_delimiter;
                    state = 5;
                    if (final_delimeter < outter_final_delimiter)
                        state = 0;
                }
            if (state == 5)
                {
                    std::string str_value = str.substr (
                        initial_delimeter,
                        outter_final_delimiter - initial_delimeter);
                    std::cout << "num/bool/null/final: " << str_value << '\n';
                    cjparse_json_parser::cjparse_parse_value (str_value,
                                                              temp_value);
                    object[obj_name] = temp_value;
                    keep_looping = false;
                }
            if (state == 6)
                {
                    std::string str_value
                        = str.substr (initial_delimeter,
                                      final_delimeter - initial_delimeter + 1);
                    std::cout << "final obj/arr/str: " << str_value << '\n';
                    cjparse_json_parser::cjparse_parse_value (str_value,
                                                              temp_value);
                    object[obj_name] = temp_value;
                    keep_looping = false;
                }
        }

    value = cjparse::cjparse_json_value (object);
}

void
cjparse_json_parser::cjparse_parse_value (std::string &str,
                                          cjparse::cjparse_json_value &value)
{
    int what_is_the_value = check_what_is_the_value (str);
    if (what_is_the_value == 1)
        {
            std::cout << "INPUT IS AN OBJECT" << '\n';
            cjparse_json_parser::cjparse_parse_object (str, value);
        }
    if (what_is_the_value == 2)
        {
            std::cout << "INPUT IS A ARRAY" << '\n';
            cjparse_json_parser::cjparse_parse_array (str, value);
        }
    if (what_is_the_value == 3)
        {
            std::cout << "INPUT IS A STRING" << '\n';
            value = cjparse_json_parser::cjparse_parse_value_string (str);
        }
    if (what_is_the_value == 4)
        {
            std::cout << "INPUT IS A NUMBER" << '\n';
            value = cjparse_json_parser::cjparse_parse_value_number (str);
        }
    if (what_is_the_value == 5)
        {
            std::cout << "INPUT IS A BOOL" << '\n';
            value = cjparse_json_parser::cjparse_parse_value_bool (str);
        }
    if (what_is_the_value == 6)
        {
            std::cout << "INPUT IS A NULL" << '\n';
            cjparse_json_parser::cjparse_parse_value_null (str);
        }
}

void
cjparse_json_parser::check_if_prev_is_backlash (std::string &str,
                                                std::size_t &position,
                                                char pattern)
{
    while (position > 0 && str[position - 1] == '\\')
        {
            position = str.find (pattern, position + 1);
            if (position == std::string::npos)
                {
                    break; // Avoid infinite loop if no double-quote is found.
                }
        }
}

std::size_t
cjparse_json_parser::return_the_matching_bracket (
    std::string &str, std::size_t pos_of_bracket_to_match, char pattern)
{
    std::size_t size = str.length ();
    char pattern_match;
    if (pattern == '{')
        pattern_match = '}';
    if (pattern == '[')
        pattern_match = ']';
    if (pattern != '{' && pattern != '[')
        return std::string::npos;

    int numb_of_pattern_between = 0;
    int numb_of_pattern_match_between = 0;
    std::size_t pos_of_nearest_pattern_match = pos_of_bracket_to_match;

    bool keep_looping = true;

    unsigned char state = 0;
    while (keep_looping)
        {
            switch (state)
                {
                case 0:
                    {
                        numb_of_pattern_between = 0;
                        numb_of_pattern_match_between = 0;
                        pos_of_nearest_pattern_match = str.find_first_of (
                            pattern_match, pos_of_nearest_pattern_match + 1);
                        if (pos_of_nearest_pattern_match == std::string::npos)
                            return std::string::npos;

                        for (std::size_t i = pos_of_bracket_to_match;
                             i <= pos_of_nearest_pattern_match; i++)
                            {
                                if (str[i] == pattern)
                                    numb_of_pattern_between++;
                                if (str[i] == pattern_match)
                                    numb_of_pattern_match_between++;
                            }
                        state = 1;
                    }
                case 1:
                    {
                        if (numb_of_pattern_between
                            == numb_of_pattern_match_between)
                            return pos_of_nearest_pattern_match;

                        if (numb_of_pattern_between
                            != numb_of_pattern_match_between)
                            state = 0;

                        if (numb_of_pattern_between == 0
                            && numb_of_pattern_match_between != 0)
                            {
                                keep_looping = false;
                                return std::string::npos;
                            } // BIG FAT ERROR , HORRIBLE JSON
                    }
                }
        }
    return std::string::npos;
}

void
cjparse_json_parser::find_delimeters_check_if_comma_alter_state (
    std::string &str, std::size_t &not_white_after_double_point,
    std::size_t &initial_delimeter, std::size_t &final_delimeter,
    std::size_t &not_white_position_after_final_delimiter, int &state_to_alter,
    char pattern_to_match)
{
    initial_delimeter
        = str.find_first_of (pattern_to_match, not_white_after_double_point);
    if (pattern_to_match != '\"')
        {
            final_delimeter = return_the_matching_bracket (
                str, initial_delimeter, pattern_to_match);
            not_white_position_after_final_delimiter = str.find_first_not_of (
                { str[final_delimeter], 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                final_delimeter);
        }
    else
        {
            final_delimeter
                = str.find_first_of (pattern_to_match, initial_delimeter + 1);
            check_if_prev_is_backlash (str, final_delimeter, pattern_to_match);
            not_white_position_after_final_delimiter = str.find_first_not_of (
                { pattern_to_match, 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                final_delimeter);
        }
    state_to_alter = 6;
    if (not_white_position_after_final_delimiter != std::string::npos)
        {
            if (str[not_white_position_after_final_delimiter] == ',')
                state_to_alter = 0;
        }
}
