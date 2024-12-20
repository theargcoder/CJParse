#include "cjparse.h"

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
    std::size_t start_of_number
        = str.find_first_not_of ({ 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b }, 0);
    std::size_t end_of_number = str.find_first_of (
        { ',', '\"', '{', '}', '[', ']', 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
        0);
    if (std::binary_search (str.begin (), str.end () - end_of_number, '.')
        || std::binary_search (str.begin (), str.end () - end_of_number, 'e')
        || std::binary_search (str.begin (), str.end () - end_of_number, 'E'))
        {
            return std::stod (str.substr (
                start_of_number, end_of_number - start_of_number - 1));
        }
    else
        {
            return std::stoi (str.substr (
                start_of_number, end_of_number - start_of_number - 1));
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

    bool keep_looping = true;
    int state = 0;

    std::size_t st_of_val, en_of_val, next_en_of_val;

    while (keep_looping)
        {
            switch (state)
                {
                case 0: // initial check
                    {
                        st_of_val
                            = str.find_first_of ({ ',', '[' }, st_of_object);
                        en_of_val
                            = str.find_first_of ({ ',', ']' }, st_of_val + 1);
                        state = 1;
                        if (en_of_val == std::string::npos)
                            state = 6;
                        if (en_of_val == std::string::npos)
                            en_of_val = str.find_first_of ('}', st_of_val);
                    }
                case 1:
                    {
                        if (str[st_of_val] == '[' && str[en_of_val] == '[')
                            state = 2;
                        if (str[st_of_val] == '[' && str[en_of_val] == ',')
                            state = 3;
                        if (str[st_of_val] == ',' && str[en_of_val] == ',')
                            state = 4;
                        if (str[st_of_val] == ',' && str[en_of_val] == '[')
                            state = 5;
                    }
                case 2: // First Iteration
                    {
                        parse_internal_array (str, st_of_object, en_of_object,
                                              temp_value);
                        array.push_back (temp_value);
                        state = 0;
                    }
                case 3:
                    {
                        parse_internal_array (str, st_of_val, en_of_val,
                                              temp_value);
                        array.push_back (temp_value);
                        state = 0;
                    }
                case 4:
                    {
                        parse_internal_array (str, st_of_val, en_of_val,
                                              temp_value);
                        array.push_back (temp_value);
                        state = 0;
                    }
                case 5: // special case (obj of objs inside obj of objs)
                    {
                        next_en_of_val = cjparse_json_parser::
                            return_the_matching_bracket (str, en_of_val, ']');
                        parse_internal_array (str, en_of_val, next_en_of_val,
                                              temp_value);
                        array.push_back (temp_value);
                        state = 0;
                    }
                case 6:
                    {
                        parse_internal_array (str, st_of_val, en_of_val,
                                              temp_value);
                        array.push_back (temp_value);
                        keep_looping = false;
                    }
                }
        }

    array.push_back (temp_value);
    value = cjparse::cjparse_json_value (array);
}

void
cjparse_json_parser::cjparse_parse_object (std::string &str,
                                           cjparse::cjparse_json_value &value)
{
    cjparse::json_object object;
    cjparse::cjparse_json_value temp_value;

    std::size_t st_of_object = 0;
    std::size_t en_of_object = str.length () - 1;

    bool keep_looping = true;
    int state = 0;

    std::size_t st_of_val, en_of_val, next_en_of_val;

    while (keep_looping)
        {
            switch (state)
                {
                case 0: // initial check
                    {
                        st_of_val
                            = str.find_first_of ({ ',', '{' }, st_of_object);
                        en_of_val
                            = str.find_first_of ({ ',', '{' }, st_of_val + 1);
                        state = 1;
                        if (en_of_val == std::string::npos)
                            state = 6;
                        if (en_of_val == std::string::npos)
                            en_of_val = str.find_first_of ('}', st_of_val);
                    }
                case 1:
                    {
                        if (str[st_of_val] == '{' && str[en_of_val] == '{')
                            state = 2;
                        if (str[st_of_val] == '{' && str[en_of_val] == ',')
                            state = 3;
                        if (str[st_of_val] == ',' && str[en_of_val] == ',')
                            state = 4;
                        if (str[st_of_val] == ',' && str[en_of_val] == '{')
                            state = 5;
                    }
                case 2: // First Iteration
                    {
                        std::string temp_str;
                        parse_internal_object (str, st_of_object, en_of_object,
                                               temp_str, temp_value);
                        object[temp_str] = temp_value;
                        state = 0;
                    }
                case 3:
                    {
                        std::string temp_str;
                        parse_internal_object (str, st_of_val, en_of_val,
                                               temp_str, temp_value);
                        object[temp_str] = temp_value;
                        state = 0;
                    }
                case 4:
                    {
                        std::string temp_str;
                        parse_internal_object (str, st_of_val, en_of_val,
                                               temp_str, temp_value);
                        object[temp_str] = temp_value;
                        state = 0;
                    }
                case 5: // special case (obj of objs inside obj of objs)
                    {
                        std::string temp_str;
                        next_en_of_val = cjparse_json_parser::
                            return_the_matching_bracket (str, en_of_val, '{');
                        parse_internal_object (str, en_of_val, next_en_of_val,
                                               temp_str, temp_value);
                        object[temp_str] = temp_value;
                        state = 0;
                    }
                case 6:
                    {
                        std::string temp_str;
                        parse_internal_object (str, st_of_val, en_of_val,
                                               temp_str, temp_value);
                        object[temp_str] = temp_value;
                        keep_looping = false;
                    }
                }
        }

    value = cjparse::cjparse_json_value (object);
}

cjparse::cjparse (std::string &str)
{
    cjparse_json_parser::cjparse_parse_object (str, JSON);
}

void
cjparse::remove_json_whitespace_between_delimeters (std::string &str,
                                                    std::size_t lower_bound,
                                                    std::size_t upper_bound)
{
    std::string::iterator lower_bound_it = str.begin () + lower_bound;
    std::string::iterator upper_bound_it = str.begin () + upper_bound;
    if (!(upper_bound_it > str.end ()))
        {
            for (std::string::iterator k = upper_bound_it; k >= lower_bound_it;
                 k--)
                {
                    if (std::isspace (*k))
                        {
                            str.erase (k);
                        }
                }
        }
};

void
cjparse::remove_json_whitespace_outside_delimeters (std::string &str,
                                                    std::size_t lower_bound,
                                                    std::size_t upper_bound)
{
    std::string::iterator low_bound_it = str.begin () + lower_bound;
    int difference_between_upper_bound_and_end
        = str.length () - upper_bound + 1;

    for (std::string::iterator k = low_bound_it; k >= str.begin (); k--)
        {
            if (std::isspace (*k))
                {
                    str.erase (k);
                }
        }
    for (std::string::iterator k
         = str.end () - difference_between_upper_bound_and_end;
         k < str.end ();)
        {
            if (std::isspace (*k))
                {
                    str.erase (k);
                }
            else
                {
                    k++;
                }
        }
};
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

void
cjparse_json_parser::check_if_next_is_comma (std::string &str,
                                             std::size_t &position,
                                             char pattern)
{
    while (position < str.length () && str[position + 1] == ',')
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
    std::size_t pos_of_nearest_pattern_match
        = str.find_first_of (pattern_match, pos_of_bracket_to_match);

    for (std::size_t i = pos_of_nearest_pattern_match + 1;
         i < pos_of_nearest_pattern_match; i++)
        {
            if (str[i] == pattern)
                numb_of_pattern_between++;
            if (str[i] == pattern_match)
                numb_of_pattern_match_between++;
        }

    if (numb_of_pattern_between == 0)
        return pos_of_nearest_pattern_match;
    if (numb_of_pattern_between == 0 && numb_of_pattern_match_between != 0)
        {
        } // BIG FAT ERROR , HORRIBLE JSON

    while (numb_of_pattern_between != numb_of_pattern_match_between)
        {
            pos_of_nearest_pattern_match = str.find_first_of (
                pattern_match, pos_of_nearest_pattern_match + 1);

            if (pos_of_nearest_pattern_match == std::string::npos)
                break;

            for (std::size_t i = pos_of_nearest_pattern_match + 1;
                 i < pos_of_nearest_pattern_match; i++)
                {
                    if (str[i] == pattern)
                        numb_of_pattern_between++;
                    if (str[i] == pattern_match)
                        numb_of_pattern_match_between++;
                }
        }

    if (numb_of_pattern_between == numb_of_pattern_match_between)
        return pos_of_nearest_pattern_match;

    return std::string::npos;
}

void
cjparse_json_parser::parse_internal_object (std::string &str,
                                            std::size_t st_of_object,
                                            std::size_t en_of_object,
                                            std::string &obj_name,
                                            cjparse::cjparse_json_value &value)
{
    std::size_t st_quote_of_name = str.find ('\"', st_of_object);
    check_if_prev_is_backlash (str, st_quote_of_name, '\"');

    std::size_t en_quote_of_name = str.find ('\"', st_quote_of_name + 1);
    check_if_prev_is_backlash (str, en_quote_of_name, '\"');
    std::size_t first_double_point_after_name
        = str.find (':', en_quote_of_name);
    check_if_prev_is_backlash (str, first_double_point_after_name, ':');

    obj_name = str.substr (st_quote_of_name + 1,
                           en_quote_of_name - st_quote_of_name - 1);

    std::cout << "obj_name: " << obj_name << '\n';

    std::size_t st_of_value
        = str.find_first_not_of ({ 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b },
                                 first_double_point_after_name + 1);
    std::string str_value
        = str.substr (st_of_value, en_of_object - st_of_value);

    std::cout << "str_value: " << str_value << '\n';

    unsigned char state_n = check_what_is_the_value (str_value);

    switch (state_n)
        {
        case 1: // object
            {
                cjparse_parse_object (str_value, value);
            }
        case 2: // array
            {
                cjparse_parse_array (str_value, value);
            }
        case 3: // string
            {
                value = cjparse::cjparse_json_value (
                    cjparse_parse_value_string (str_value));
            }
        case 4: // number
            {
                value = cjparse::cjparse_json_value (
                    cjparse_parse_value_number (str_value));
            }
        case 5: // bool
            {
                value = cjparse::cjparse_json_value (
                    cjparse_parse_value_bool (str_value));
            }
        case 6: // null
            {
                value = cjparse::cjparse_json_value (
                    cjparse_parse_value_null (str_value));
            }
        }
}

void
cjparse_json_parser::parse_internal_array (std::string &str,
                                           std::size_t st_of_object,
                                           std::size_t en_of_object,
                                           cjparse::cjparse_json_value &value)
{
    std::size_t st_of_value = str.find_first_not_of (
        { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b }, st_of_object + 1);
    std::string str_value
        = str.substr (st_of_value, en_of_object - st_of_value);

    std::cout << "str_value: " << str_value << '\n';

    unsigned char state_n = check_what_is_the_value (str_value);

    switch (state_n)
        {
        case 1: // object
            {
                cjparse_parse_object (str_value, value);
            }
        case 2: // array
            {
                cjparse_parse_array (str_value, value);
            }
        case 3: // string
            {
                value = cjparse::cjparse_json_value (
                    cjparse_parse_value_string (str_value));
            }
        case 4: // number
            {
                value = cjparse::cjparse_json_value (
                    cjparse_parse_value_number (str_value));
            }
        case 5: // bool
            {
                value = cjparse::cjparse_json_value (
                    cjparse_parse_value_bool (str_value));
            }
        case 6: // null
            {
                value = cjparse::cjparse_json_value (
                    cjparse_parse_value_null (str_value));
            }
        }
}
