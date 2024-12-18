#include "cjparse.h"

bool
cjparse_json_checkers::cjparse_check_if_object (std::string &str)
{
    if (str.length () == 0)
        {
            // ERROR 0;
        }
    std::size_t first_curly_pos = str.find ('{', 0);
    std::size_t first_square_pos = str.find ('[', 0);
    std::size_t first_quote_pos = str.find ('\"', 0);

    if ((first_curly_pos < first_square_pos)
        && (first_curly_pos < first_quote_pos))
        {
            return true;
        }
    else
        {
            return false;
        }
};

bool
cjparse_json_checkers::cjparse_check_if_array (std::string &str)
{
    if (str.length () == 0)
        {
            // ERROR 0;
        }
    std::size_t first_curly_pos = str.find ('{', 0);
    std::size_t first_square_pos = str.find ('[', 0);
    std::size_t first_quote_pos = str.find ('\"', 0);

    if ((first_square_pos < first_curly_pos)
        && (first_square_pos < first_quote_pos))
        {
            return true;
        }
    else
        {
            return false;
        }
};

bool
cjparse_json_checkers::cjparse_check_if_value_string (std::string &str)
{
    if (str.length () == 0)
        {
            // ERROR 0
        }
    std::size_t first_curly_pos = str.find ('{', 0);
    std::size_t first_square_pos = str.find ('[', 0);
    std::size_t first_quote_pos = str.find ('\"', 0);

    if ((first_quote_pos < first_curly_pos)
        && (first_quote_pos < first_square_pos))
        {
            return true;
        }
    else
        {
            return false;
        }
};

bool
cjparse_json_checkers::cjparse_check_if_value_number (std::string &str)
{
    std::string str_cpy = str;

    char whitespace[6] = { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b };
    for (char chr : whitespace)
        {
            str_cpy.erase (remove (str_cpy.begin (), str_cpy.end (), chr),
                           str_cpy.end ());
        }
    bool is_number = true;
    for (char i : str_cpy)
        {
            if (!std::isdigit (i))
                {
                    if (!(i != 'e' || i == 'E' || i == '-' || i == '.'
                          || i == '+'))
                        {
                            is_number = false;
                        }
                }
        }
    return is_number;
};

bool
cjparse_json_checkers::cjparse_check_if_value_bool (std::string &str)
{
    std::string str_cpy = str;

    char whitespace[6] = { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b };
    for (char chr : whitespace)
        {
            str_cpy.erase (remove (str_cpy.begin (), str_cpy.end (), chr),
                           str_cpy.end ());
        }
    if (str_cpy == "true" || str_cpy == "false")
        {
            return true;
        }
    else
        {
            return false;
        }
};

bool
cjparse_json_checkers::cjparse_check_if_value_null (std::string &str)
{
    std::string str_cpy = str;

    char whitespace[6] = { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b };
    for (char chr : whitespace)
        {
            str_cpy.erase (remove (str_cpy.begin (), str_cpy.end (), chr),
                           str_cpy.end ());
        }
    if (str_cpy == "null")
        {
            return true;
        }
    else
        {
            return false;
        }
};

std::string
cjparse_json_parser::cjparse_parse_value_string (std::string &str)
{
    std::size_t st_of_string = str.find ('\"', 0);
    std::size_t en_of_string = str.rfind ('\"', str.length () - 1);

    cjparse::remove_json_whitespace_outside_delimeters (str, st_of_string,
                                                        en_of_string);
    return str.substr (1, str.length () - 3);
}

std::variant<int, long int, long long int, double, long double>
cjparse_json_parser::cjparse_parse_value_number (std::string &str)
{
    char whitespace[6] = { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b };
    for (char chr : whitespace)
        {
            str.erase (remove (str.begin (), str.end (), chr), str.end ());
        }
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
    char whitespace[6] = { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b };
    for (char chr : whitespace)
        {
            str.erase (remove (str.begin (), str.end (), chr), str.end ());
        }
    if (str == "true")
        {
            return true;
        }
    else
        {
            return false;
        }
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
    cjparse::json_array array;
    cjparse::cjparse_json_value temp_value;
    std::size_t start = 0;
    std::size_t end = str.find (',', start + 1);

    if (end != std::string::npos)
        {
            bool keepgoing_00 = true;
            while (keepgoing_00 && (end != std::string::npos))
                {
                    if (str[end - 1] == '\\')
                        {
                            end = str.find (',', end + 1);
                            keepgoing_00 = true;
                        }
                    else
                        {
                            keepgoing_00 = false;
                        }
                }
        }

    if (end != std::string::npos)
        {
            bool done_the_first_one = false;
            if (!done_the_first_one)
                {
                    std::size_t st_of_first_object = 0;
                    std::size_t en_of_first_object
                        = str.find (',', st_of_first_object + 1);

                    bool keepgoing_0 = true;
                    while (keepgoing_0)
                        {
                            if (str[en_of_first_object - 1] == '\\')
                                {
                                    en_of_first_object = str.find (
                                        ',', en_of_first_object + 1);
                                    keepgoing_0 = true;
                                }
                            else
                                {
                                    keepgoing_0 = false;
                                }
                        }

                    std::size_t st_quote_of_name
                        = str.find ('\"', st_of_first_object);
                    cjparse::remove_json_whitespace_between_delimeters (
                        str, 0, st_quote_of_name);
                    std::size_t en_quote_of_name
                        = str.find ('\"', st_quote_of_name + 1);
                    bool keepgoing_1 = true;
                    while (keepgoing_1)
                        {
                            if (str[en_quote_of_name - 1] == '\\')
                                {
                                    st_quote_of_name = str.find (
                                        '\"', en_quote_of_name + 1);
                                    keepgoing_1 = true;
                                }
                            else
                                {
                                    keepgoing_1 = false;
                                }
                        }
                    std::size_t first_double_point_after_name
                        = str.find (':', en_quote_of_name);

                    cjparse::remove_json_whitespace_between_delimeters (
                        str, en_quote_of_name, first_double_point_after_name);

                    std::size_t st_first_object_name
                        = str.find ("{\"", st_of_first_object + 1);
                    std::size_t en_first_object_name
                        = str.find ("\":", st_first_object_name + 1);

                    std::string obj_name = str.substr (
                        st_first_object_name + 1,
                        en_first_object_name - st_first_object_name - 1);

                    std::string str_value = str.substr (
                        en_first_object_name + 2,
                        en_of_first_object - en_first_object_name - 2);

                    bool str_value_is_object
                        = cjparse_json_checkers::cjparse_check_if_object (
                            str_value);
                    bool str_value_is_array
                        = cjparse_json_checkers::cjparse_check_if_array (
                            str_value);

                    bool str_value_is_string = cjparse_json_checkers::
                        cjparse_check_if_value_string (str_value);
                    bool str_value_is_number = cjparse_json_checkers::
                        cjparse_check_if_value_number (str_value);
                    bool str_value_is_bool
                        = cjparse_json_checkers::cjparse_check_if_value_bool (
                            str_value);
                    bool str_value_is_null
                        = cjparse_json_checkers::cjparse_check_if_value_null (
                            str_value);

                    if (str_value_is_object)
                        {
                            cjparse_parse_object (str_value, temp_value);
                        }
                    if (str_value_is_array)
                        {
                            cjparse_parse_array (str_value, temp_value);
                        }
                    if (str_value_is_array)
                        {
                            temp_value = cjparse::cjparse_json_value (
                                cjparse_parse_value_string (str_value));
                        }
                    if (str_value_is_array)
                        {
                            temp_value = cjparse::cjparse_json_value (
                                cjparse_parse_value_number (str_value));
                        }
                    if (str_value_is_array)
                        {
                            temp_value = cjparse::cjparse_json_value (
                                cjparse_parse_value_bool (str_value));
                        }
                    if (str_value_is_array)
                        {
                            temp_value = cjparse::cjparse_json_value (
                                cjparse_parse_value_null (str_value));
                        }
                    array.push_back (temp_value);
                    done_the_first_one = true;
                }
            else
                {
                    std::size_t st_of_first_object = 0;
                    std::size_t en_of_first_object
                        = str.find (',', st_of_first_object + 1);

                    bool keepgoing_0 = true;
                    while (keepgoing_0)
                        {
                            if (str[en_of_first_object - 1] == '\\')
                                {
                                    en_of_first_object = str.find (
                                        ',', en_of_first_object + 1);
                                    keepgoing_0 = true;
                                }
                            else
                                {
                                    keepgoing_0 = false;
                                }
                        }
                    std::size_t curr_st_of_nth_object = en_of_first_object;
                    std::size_t curr_end_of_nth_object
                        = str.find (',', en_of_first_object + 1);

                    bool keepgoing_1 = true;
                    while (keepgoing_1)
                        {
                            if (str[en_of_first_object - 1] == '\\'
                                && curr_end_of_nth_object != std::string::npos)
                                {
                                    en_of_first_object = str.find (
                                        ',', en_of_first_object + 1);
                                    keepgoing_1 = true;
                                }
                            else
                                {
                                    keepgoing_1 = false;
                                }
                        }
                    bool still_obj_to_parse
                        = curr_end_of_nth_object != std::string::npos;
                    if (still_obj_to_parse)
                        {

                            std::size_t st_of_curr_object
                                = curr_st_of_nth_object;
                            std::size_t en_of_curr_object
                                = curr_end_of_nth_object;

                            bool keeplooping = true;
                            while (keeplooping)
                                {
                                    std::size_t st_quote_of_name
                                        = str.find ('\"', st_of_first_object);
                                    cjparse::
                                        remove_json_whitespace_between_delimeters (
                                            str, 0, st_quote_of_name);
                                    std::size_t en_quote_of_name = str.find (
                                        '\"', st_quote_of_name + 1);
                                    bool keepgoing_1 = true;
                                    while (keepgoing_1)
                                        {
                                            if (str[en_quote_of_name - 1]
                                                    == '\\'
                                                && en_quote_of_name
                                                       != std::string::npos)
                                                {
                                                    st_quote_of_name
                                                        = str.find (
                                                            '\"',
                                                            en_quote_of_name
                                                                + 1);
                                                    keepgoing_1 = true;
                                                }
                                            else
                                                {
                                                    keepgoing_1 = false;
                                                }
                                        }
                                    std::size_t first_double_point_after_name
                                        = str.find (':', en_quote_of_name);

                                    cjparse::
                                        remove_json_whitespace_between_delimeters (
                                            str, en_quote_of_name,
                                            first_double_point_after_name);

                                    std::size_t st_first_object_name
                                        = str.find ("{\"",
                                                    st_of_first_object + 1);
                                    std::size_t en_first_object_name
                                        = str.find ("\":",
                                                    st_first_object_name + 1);

                                    std::string obj_name = str.substr (
                                        st_first_object_name + 1,
                                        en_first_object_name
                                            - st_first_object_name - 1);

                                    std::string str_value = str.substr (
                                        en_first_object_name + 2,
                                        en_of_curr_object
                                            - en_first_object_name - 2);

                                    bool str_value_is_object
                                        = cjparse_json_checkers::
                                            cjparse_check_if_object (
                                                str_value);
                                    bool str_value_is_array
                                        = cjparse_json_checkers::
                                            cjparse_check_if_array (str_value);

                                    bool str_value_is_string
                                        = cjparse_json_checkers::
                                            cjparse_check_if_value_string (
                                                str_value);
                                    bool str_value_is_number
                                        = cjparse_json_checkers::
                                            cjparse_check_if_value_number (
                                                str_value);
                                    bool str_value_is_bool
                                        = cjparse_json_checkers::
                                            cjparse_check_if_value_bool (
                                                str_value);
                                    bool str_value_is_null
                                        = cjparse_json_checkers::
                                            cjparse_check_if_value_null (
                                                str_value);

                                    if (str_value_is_object)
                                        {
                                            cjparse_parse_object (str_value,
                                                                  temp_value);
                                        }
                                    if (str_value_is_array)
                                        {
                                            cjparse_parse_array (str_value,
                                                                 temp_value);
                                        }
                                    if (str_value_is_array)
                                        {
                                            temp_value
                                                = cjparse::cjparse_json_value (
                                                    cjparse_parse_value_string (
                                                        str_value));
                                        }
                                    if (str_value_is_array)
                                        {
                                            temp_value
                                                = cjparse::cjparse_json_value (
                                                    cjparse_parse_value_number (
                                                        str_value));
                                        }
                                    if (str_value_is_array)
                                        {
                                            temp_value
                                                = cjparse::cjparse_json_value (
                                                    cjparse_parse_value_bool (
                                                        str_value));
                                        }
                                    if (str_value_is_array)
                                        {
                                            temp_value
                                                = cjparse::cjparse_json_value (
                                                    cjparse_parse_value_null (
                                                        str_value));
                                        }
                                    array.push_back (temp_value);
                                    curr_st_of_nth_object
                                        = curr_end_of_nth_object;
                                    std::size_t curr_end_of_nth_object
                                        = str.find (',', curr_end_of_nth_object
                                                             + 1);

                                    bool keepgoing_2 = true;
                                    while (keepgoing_2)
                                        {
                                            if (str[en_of_first_object - 1]
                                                == '\\')
                                                {
                                                    en_of_first_object
                                                        = str.find (
                                                            ',',
                                                            en_of_first_object
                                                                + 1);
                                                    keepgoing_2 = true;
                                                }
                                            else
                                                {
                                                    keepgoing_2 = false;
                                                }
                                        }
                                    keeplooping = curr_end_of_nth_object
                                                  != std::string::npos;
                                }
                            std::size_t st_of_object = en_of_first_object;
                            std::size_t en_of_object
                                = str.find ('}', st_of_object + 1);

                            std::size_t st_quote_of_name
                                = str.find ('\"', st_of_object);
                            cjparse::
                                remove_json_whitespace_between_delimeters (
                                    str, 0, st_quote_of_name);
                            std::size_t en_quote_of_name
                                = str.find ('\"', st_quote_of_name + 1);
                            bool keepgoing = true;
                            while (keepgoing)
                                {
                                    if (str[en_quote_of_name - 1] == '\\')
                                        {
                                            st_quote_of_name = str.find (
                                                '\"', en_quote_of_name + 1);
                                            keepgoing = true;
                                        }
                                    else
                                        {
                                            keepgoing = false;
                                        }
                                }
                            std::size_t first_double_point_after_name
                                = str.find (':', en_quote_of_name);

                            cjparse::
                                remove_json_whitespace_between_delimeters (
                                    str, en_quote_of_name,
                                    first_double_point_after_name);

                            std::size_t st_first_object_name
                                = str.find ("{\"", st_of_object + 1);
                            std::size_t en_first_object_name
                                = str.find ("\":", st_first_object_name + 1);

                            std::string obj_name
                                = str.substr (st_first_object_name + 1,
                                              en_first_object_name
                                                  - st_first_object_name - 1);

                            std::string str_value = str.substr (
                                en_first_object_name + 2,
                                en_of_object - en_first_object_name - 2);

                            bool str_value_is_object = cjparse_json_checkers::
                                cjparse_check_if_object (str_value);
                            bool str_value_is_array = cjparse_json_checkers::
                                cjparse_check_if_array (str_value);

                            bool str_value_is_string = cjparse_json_checkers::
                                cjparse_check_if_value_string (str_value);
                            bool str_value_is_number = cjparse_json_checkers::
                                cjparse_check_if_value_number (str_value);
                            bool str_value_is_bool = cjparse_json_checkers::
                                cjparse_check_if_value_bool (str_value);
                            bool str_value_is_null = cjparse_json_checkers::
                                cjparse_check_if_value_null (str_value);

                            if (str_value_is_object)
                                {
                                    cjparse_parse_object (str_value,
                                                          temp_value);
                                }
                            if (str_value_is_array)
                                {
                                    cjparse_parse_array (str_value,
                                                         temp_value);
                                }
                            if (str_value_is_array)
                                {
                                    temp_value = cjparse::cjparse_json_value (
                                        cjparse_parse_value_string (
                                            str_value));
                                }
                            if (str_value_is_array)
                                {
                                    temp_value = cjparse::cjparse_json_value (
                                        cjparse_parse_value_number (
                                            str_value));
                                }
                            if (str_value_is_array)
                                {
                                    temp_value = cjparse::cjparse_json_value (
                                        cjparse_parse_value_bool (str_value));
                                }
                            if (str_value_is_array)
                                {
                                    temp_value = cjparse::cjparse_json_value (
                                        cjparse_parse_value_null (str_value));
                                }
                            array.push_back (temp_value);
                        }
                    else
                        {
                            std::size_t st_of_object = en_of_first_object;
                            std::size_t en_of_object
                                = str.find ('}', st_of_object + 1);

                            std::size_t st_quote_of_name
                                = str.find ('\"', st_of_object);
                            cjparse::
                                remove_json_whitespace_between_delimeters (
                                    str, 0, st_quote_of_name);
                            std::size_t en_quote_of_name
                                = str.find ('\"', st_quote_of_name + 1);
                            bool keepgoing = true;
                            while (keepgoing)
                                {
                                    if (str[en_quote_of_name - 1] == '\\')
                                        {
                                            st_quote_of_name = str.find (
                                                '\"', en_quote_of_name + 1);
                                            keepgoing = true;
                                        }
                                    else
                                        {
                                            keepgoing = false;
                                        }
                                }
                            std::size_t first_double_point_after_name
                                = str.find (':', en_quote_of_name);

                            cjparse::
                                remove_json_whitespace_between_delimeters (
                                    str, en_quote_of_name,
                                    first_double_point_after_name);

                            std::size_t st_first_object_name
                                = str.find ("{\"", st_of_object + 1);
                            std::size_t en_first_object_name
                                = str.find ("\":", st_first_object_name + 1);

                            std::string obj_name
                                = str.substr (st_first_object_name + 1,
                                              en_first_object_name
                                                  - st_first_object_name - 1);

                            std::string str_value = str.substr (
                                en_first_object_name + 2,
                                en_of_object - en_first_object_name - 2);

                            bool str_value_is_object = cjparse_json_checkers::
                                cjparse_check_if_object (str_value);
                            bool str_value_is_array = cjparse_json_checkers::
                                cjparse_check_if_array (str_value);

                            bool str_value_is_string = cjparse_json_checkers::
                                cjparse_check_if_value_string (str_value);
                            bool str_value_is_number = cjparse_json_checkers::
                                cjparse_check_if_value_number (str_value);
                            bool str_value_is_bool = cjparse_json_checkers::
                                cjparse_check_if_value_bool (str_value);
                            bool str_value_is_null = cjparse_json_checkers::
                                cjparse_check_if_value_null (str_value);

                            if (str_value_is_object)
                                {
                                    cjparse_parse_object (str_value,
                                                          temp_value);
                                }
                            if (str_value_is_array)
                                {
                                    cjparse_parse_array (str_value,
                                                         temp_value);
                                }
                            if (str_value_is_array)
                                {
                                    temp_value = cjparse::cjparse_json_value (
                                        cjparse_parse_value_string (
                                            str_value));
                                }
                            if (str_value_is_array)
                                {
                                    temp_value = cjparse::cjparse_json_value (
                                        cjparse_parse_value_number (
                                            str_value));
                                }
                            if (str_value_is_array)
                                {
                                    temp_value = cjparse::cjparse_json_value (
                                        cjparse_parse_value_bool (str_value));
                                }
                            if (str_value_is_array)
                                {
                                    temp_value = cjparse::cjparse_json_value (
                                        cjparse_parse_value_null (str_value));
                                }
                            array.push_back (temp_value);
                        }
                    value = cjparse::cjparse_json_value (array);
                }
        }
    else
        {
            std::size_t st_of_object = 0;
            std::size_t en_of_object = str.find (']', st_of_object + 1);

            std::size_t initial_comma = str.find (',', st_of_object);
            bool keepgoing0 = true;
            while (keepgoing0)
                {
                    if (str[initial_comma - 1] == '\\')
                        {
                            initial_comma = str.find ('\"', initial_comma + 1);
                            keepgoing0 = true;
                        }
                    else
                        {
                            keepgoing0 = false;
                        }
                }

            std::size_t final_comma = str.find (',', initial_comma + 1);
            bool keepgoing1 = true;
            while (keepgoing1)
                {
                    if (str[final_comma - 1] == '\\')
                        {
                            final_comma = str.find ('\"', final_comma + 1);
                            keepgoing1 = true;
                        }
                    else
                        {
                            keepgoing1 = false;
                        }
                }

            std::string str_value = str.substr (
                initial_comma + 1, final_comma - initial_comma - 1);

            bool str_value_is_object
                = cjparse_json_checkers::cjparse_check_if_object (str_value);
            bool str_value_is_array
                = cjparse_json_checkers::cjparse_check_if_array (str_value);

            bool str_value_is_string
                = cjparse_json_checkers::cjparse_check_if_value_string (
                    str_value);
            bool str_value_is_number
                = cjparse_json_checkers::cjparse_check_if_value_number (
                    str_value);
            bool str_value_is_bool
                = cjparse_json_checkers::cjparse_check_if_value_bool (
                    str_value);
            bool str_value_is_null
                = cjparse_json_checkers::cjparse_check_if_value_null (
                    str_value);

            if (str_value_is_object)
                {
                    cjparse_parse_object (str_value, temp_value);
                }
            if (str_value_is_array)
                {
                    cjparse_parse_array (str_value, temp_value);
                }
            if (str_value_is_array)
                {
                    temp_value = cjparse::cjparse_json_value (
                        cjparse_parse_value_string (str_value));
                }
            if (str_value_is_array)
                {
                    temp_value = cjparse::cjparse_json_value (
                        cjparse_parse_value_number (str_value));
                }
            if (str_value_is_array)
                {
                    temp_value = cjparse::cjparse_json_value (
                        cjparse_parse_value_bool (str_value));
                }
            if (str_value_is_array)
                {
                    temp_value = cjparse::cjparse_json_value (
                        cjparse_parse_value_null (str_value));
                }
            array.push_back (temp_value);
            value = cjparse::cjparse_json_value (array);
        }
}

void
cjparse_json_parser::cjparse_parse_object (std::string &str,
                                           cjparse::cjparse_json_value &value)
{
    cjparse::json_object object;
    cjparse::cjparse_json_value temp_value;
    std::size_t start = 0;
    std::size_t end = str.find (',', start + 1);

    if (end != std::string::npos)
        {
            check_if_prev_is_backlash (str, end, ',');
        }

    if (end != std::string::npos)
        {
            bool done_the_first_one = false;
            if (!done_the_first_one)
                {
                    std::size_t st_of_first_object = 0;
                    std::size_t en_of_first_object
                        = str.find (',', st_of_first_object + 1);
                    check_if_prev_is_backlash (str, en_of_first_object, ',');

                    std::string obj_name;

                    parse_internal_object (str, st_of_first_object,
                                           en_of_first_object, obj_name,
                                           temp_value);
                    object[obj_name] = temp_value;
                    done_the_first_one = true;
                }
            else
                {
                    std::size_t st_of_first_object = 0;
                    std::size_t en_of_first_object
                        = str.find (',', st_of_first_object + 1);
                    check_if_prev_is_backlash (str, en_of_first_object, ',');

                    std::size_t curr_st_of_nth_object = en_of_first_object;
                    std::size_t curr_end_of_nth_object
                        = str.find (',', en_of_first_object + 1);
                    check_if_prev_is_backlash (str, curr_end_of_nth_object,
                                               ',');

                    bool still_obj_to_parse
                        = curr_end_of_nth_object != std::string::npos;
                    if (still_obj_to_parse)
                        {

                            std::size_t st_of_curr_object
                                = curr_st_of_nth_object;
                            std::size_t en_of_curr_object
                                = curr_end_of_nth_object;

                            bool keeplooping = true;
                            while (keeplooping)
                                {
                                    std::string obj_name;

                                    parse_internal_object (
                                        str, curr_st_of_nth_object,
                                        curr_end_of_nth_object, obj_name,
                                        temp_value);

                                    object[obj_name] = temp_value;
                                    curr_st_of_nth_object
                                        = curr_end_of_nth_object;
                                    std::size_t curr_end_of_nth_object
                                        = str.find (',', curr_end_of_nth_object
                                                             + 1);
                                    check_if_prev_is_backlash (
                                        str, curr_end_of_nth_object, ',');

                                    keeplooping = curr_end_of_nth_object
                                                  != std::string::npos;
                                }
                            std::size_t st_of_object = en_of_first_object;
                            std::size_t en_of_object
                                = str.find ('}', st_of_object + 1);
                            check_if_prev_is_backlash (str, en_of_object, '}');

                            std::string obj_name;

                            parse_internal_object (str, st_of_object,
                                                   en_of_object, obj_name,
                                                   temp_value);

                            object[obj_name] = temp_value;
                        }
                    else
                        {
                            std::size_t st_of_object = en_of_first_object;
                            std::size_t en_of_object
                                = str.find ('}', st_of_object + 1);
                            check_if_prev_is_backlash (str, en_of_object, '}');
                            std::string obj_name;

                            parse_internal_object (str, st_of_object,
                                                   en_of_object, obj_name,
                                                   temp_value);

                            object[obj_name] = temp_value;
                        }
                    value = cjparse::cjparse_json_value (object);
                }
        }
    else
        {
            std::size_t st_of_object = 0;
            std::size_t en_of_object = str.find ('}', st_of_object + 1);

            std::string obj_name;

            parse_internal_object (str, st_of_object, en_of_object, obj_name,
                                   temp_value);

            object[obj_name] = temp_value;
            value = cjparse::cjparse_json_value (object);
        }
}

cjparse::cjparse (std::string &str)
{
    cjparse_json_parser::cjparse_parse_object (str, JSON);
}

void
cjparse::parse_json_string (std::string &str)
{
    bool yeai = cjparse_json_checkers::cjparse_check_if_object (str);
    // json.cjparse_json_object.
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
cjparse_json_parser::parse_internal_object (std::string &str,
                                            std::size_t st_of_object,
                                            std::size_t en_of_object,
                                            std::string &obj_name,
                                            cjparse::cjparse_json_value &value)
{
    std::size_t st_quote_of_name = str.find ('\"', st_of_object);
    cjparse::remove_json_whitespace_between_delimeters (str, 0,
                                                        st_quote_of_name);
    check_if_prev_is_backlash (str, st_quote_of_name, '\"');

    std::size_t en_quote_of_name = str.find ('\"', st_quote_of_name + 1);
    check_if_prev_is_backlash (str, en_quote_of_name, '\"');
    std::size_t first_double_point_after_name
        = str.find (':', en_quote_of_name);
    check_if_prev_is_backlash (str, first_double_point_after_name, ':');

    cjparse::remove_json_whitespace_between_delimeters (
        str, en_quote_of_name, first_double_point_after_name);

    obj_name = str.substr (st_quote_of_name + 1,
                           en_quote_of_name - st_quote_of_name);

    std::cout << obj_name << '\n';

    std::string str_value
        = str.substr (en_quote_of_name, en_of_object - en_quote_of_name - 1);
    std::cout << str_value << '\n';

    bool str_value_is_object
        = cjparse_json_checkers::cjparse_check_if_object (str_value);
    bool str_value_is_array
        = cjparse_json_checkers::cjparse_check_if_array (str_value);

    bool str_value_is_string
        = cjparse_json_checkers::cjparse_check_if_value_string (str_value);
    bool str_value_is_number
        = cjparse_json_checkers::cjparse_check_if_value_number (str_value);
    bool str_value_is_bool
        = cjparse_json_checkers::cjparse_check_if_value_bool (str_value);
    bool str_value_is_null
        = cjparse_json_checkers::cjparse_check_if_value_null (str_value);

    if (str_value_is_object)
        {
            cjparse_parse_object (str_value, value);
        }
    if (str_value_is_array)
        {
            cjparse_parse_array (str_value, value);
        }
    if (str_value_is_array)
        {
            value = cjparse::cjparse_json_value (
                cjparse_parse_value_string (str_value));
        }
    if (str_value_is_array)
        {
            value = cjparse::cjparse_json_value (
                cjparse_parse_value_number (str_value));
        }
    if (str_value_is_array)
        {
            value = cjparse::cjparse_json_value (
                cjparse_parse_value_bool (str_value));
        }
    if (str_value_is_array)
        {
            value = cjparse::cjparse_json_value (
                cjparse_parse_value_null (str_value));
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
