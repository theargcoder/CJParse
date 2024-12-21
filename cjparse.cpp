#include "cjparse.h"

cjparse::cjparse (std::string &str)
{
    cjparse_json_parser::cjparse_parse_value (str, JSON);
}

unsigned char
cjparse_json_parser::check_what_is_the_value (std::string &str)
{
    if (str.length () == 0)
        {
            // ERROR 0;
        }
    std::size_t not_white_position
        = str.find_first_not_of ({ 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b }, 0);
    if (str[not_white_position] == '{')
        return 1; // OBJECT
    if (str[not_white_position] == '[')
        return 2; // ARRAY
    if (str[not_white_position] == '\"')
        return 3; // STRING
    if (std::isdigit (str[not_white_position]))
        return 4; // NUMBER
    if (str.substr (not_white_position, 4) == "true"
        || str.substr (not_white_position, 5) == "false")
        return 5; // BOOL
    if (str.substr (not_white_position, 4) == "null")
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
    if (std::binary_search (str.begin (), str.end (), '.')
        || std::binary_search (str.begin (), str.end (), 'e')
        || std::binary_search (str.begin (), str.end (), 'E'))
        {
            return std::stod (str);
        }
    else
        {
            return std::stoi (str);
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
    std::size_t st_of_object = 0;
    std::size_t en_of_object = str.length () - 1;

    array.push_back (cjparse::cjparse_json_value (9));
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

    std::size_t st_of_name = str.find_first_of ('\"', 0);
    std::size_t en_of_name = str.find_first_of ('\"', st_of_name + 1);
    check_if_prev_is_backlash (str, en_of_name, '\"');

    std::string obj_name
        = str.substr (st_of_name + 1, en_of_name - st_of_name - 2);

    std::size_t double_point_after_name
        = str.find_first_of (':', en_of_name + 1);
    if (str[double_point_after_name - 1] == '\\')
        {
        } // HORRIBLE JSON

    std::string str_temp
        = str.substr (double_point_after_name, str.length () - 2);

    unsigned char state = 5;
    bool keep_looping = true;
    unsigned char what_is_the_value = check_what_is_the_value (str_temp);

    if (what_is_the_value == 1)
        state = 0;
    if (what_is_the_value == 2)
        state = 3;
    if (what_is_the_value == 3)
        state = 4;

    std::size_t initial_delimeter, final_delimeter,
        not_white_position_after_final_delimiter;
    while (keep_looping)
        {
            switch (state)
                {
                case 0: // initial computation if object has value of object
                    {
                        initial_delimeter
                            = str.find_first_of ('{', double_point_after_name);
                        final_delimeter = return_the_matching_bracket (
                            str, initial_delimeter, '{');
                        not_white_position_after_final_delimiter
                            = str.find_first_not_of (
                                { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                                final_delimeter + 1);
                        state = 7;
                        if (str[not_white_position_after_final_delimiter]
                            == ',')
                            state = 1;
                    }
                case 1:
                    {
                        std::string str_value
                            = str.substr (initial_delimeter,
                                          final_delimeter - initial_delimeter);
                        cjparse_json_parser::cjparse_parse_object (str_value,
                                                                   temp_value);
                        object[obj_name] = temp_value;
                        state = 2;
                    }
                case 2:
                    {
                        std::string str_cpy = str;
                        str_cpy.erase (st_of_name,
                                       not_white_position_after_final_delimiter
                                           - st_of_name);
                        cjparse_json_parser::cjparse_parse_value (str_cpy,
                                                                  temp_value);
                        object[obj_name] = temp_value;
                        keep_looping = false;
                    }
                case 3:
                    {
                        initial_delimeter
                            = str.find_first_of ('[', double_point_after_name);
                        final_delimeter = return_the_matching_bracket (
                            str, initial_delimeter, '[');
                        not_white_position_after_final_delimiter
                            = str.find_first_not_of (
                                { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                                final_delimeter + 1);
                        state = 7;
                        if (str[not_white_position_after_final_delimiter]
                            == ',')
                            state = 1;
                    }
                case 4:
                    {
                        initial_delimeter = str.find_first_of ('\"', 0);
                        final_delimeter
                            = str.find_first_of ('\"', st_of_name + 1);
                        check_if_prev_is_backlash (str, en_of_name, '\"');

                        final_delimeter = str.find_first_not_of (
                            { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                            final_delimeter + 1);

                        state = 7;
                        if (str[not_white_position_after_final_delimiter]
                            == ',')
                            state = 1;
                    }
                case 5:
                    {
                        initial_delimeter = str.find_first_not_of (
                            { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                            double_point_after_name);
                        final_delimeter
                            = str.find_first_of (',', initial_delimeter);

                        not_white_position_after_final_delimiter
                            = str.find_first_not_of (
                                { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                                final_delimeter);
                        state = 6;
                        if (str[not_white_position_after_final_delimiter]
                            == ',')
                            state = 1;
                    }
                case 6:
                    {
                        std::string str_value = str.substr (
                            initial_delimeter,
                            outter_final_delimiter - initial_delimeter);
                        cjparse_json_parser::cjparse_parse_value (str_value,
                                                                  temp_value);
                        object[obj_name] = temp_value;
                        keep_looping = false;
                    }
                case 7:
                    {
                        std::string str_value
                            = str.substr (initial_delimeter,
                                          final_delimeter - initial_delimeter);
                        cjparse_json_parser::cjparse_parse_value (str_value,
                                                                  temp_value);
                        object[obj_name] = temp_value;
                        keep_looping = false;
                    }
                }
        }

    value = cjparse::cjparse_json_value (object);
}

void
cjparse_json_parser::cjparse_parse_value (std::string &str,
                                          cjparse::cjparse_json_value &value)
{
    unsigned char what_is_the_value = check_what_is_the_value (str);

    switch (what_is_the_value)
        {
        case 1: // OBJECT
            {
                std::cout << str << '\n';
                cjparse_json_parser::cjparse_parse_object (str, value);
            }
        case 2: // ARRAY
            {
                std::cout << str << '\n';
                cjparse_json_parser::cjparse_parse_array (str, value);
            }
        case 3: // STRING
            {
                std::cout << str << '\n';
                value = cjparse_json_parser::cjparse_parse_value_string (str);
            }
        case 4: // NUMBER
            {
                std::cout << str << '\n';
                value = cjparse_json_parser::cjparse_parse_value_number (str);
            }
        case 5: // BOOL
            {
                std::cout << str << '\n';
                value = cjparse_json_parser::cjparse_parse_value_bool (str);
            }
        case 6: // NULL
            {
                std::cout << str << '\n';
                cjparse_json_parser::cjparse_parse_value_null (str);
            }
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
