#include "cjparse.h"
#include "cjparse_json_parser.cpp"

cjparse::cjparse (std::string &str) { cjparse_json_parser (str, JSON); }

cjparse::cjparse (std::stringstream &fake_str)
{
    // to be determied
}

cjparse::json_value
cjparse::return_full_json ()
{
    json_value value_to_return;

    std::visit ([&value_to_return] (auto value) { value_to_return = value; },
                JSON.value);
    return value_to_return;
}

bool
cjparse::is_container_an_object ()
{
    json_value possible_object = return_full_json ();
    return std::holds_alternative<json_object> (possible_object);
}

bool
cjparse::is_container_an_array ()
{
    json_value possible_object = return_full_json ();
    return std::holds_alternative<json_array> (possible_object);
}

bool
cjparse::is_container_neither_object_or_array ()
{
    bool bool_to_return;
    json_value possible_object = return_full_json ();
    bool is_array = std::holds_alternative<json_array> (possible_object);
    bool is_object = std::holds_alternative<json_object> (possible_object);

    bool_to_return = !is_array && !is_object;
    return bool_to_return;
}

template <typename json_type>
json_type
cjparse::return_json_container ()
{
    json_type type_to_return;
    type_to_return = std::get<json_type> (return_full_json ());
    return type_to_return;
}

cjparse::json_value
cjparse::return_the_value (std::vector<std::string> object_name_vector_in)
{
    bool return_empty = false;
    json_value full_json, value_to_return;
    std::visit ([&full_json] (auto value) { full_json = value; }, JSON.value);

    json_value temp_value = full_json;

    auto it = object_name_vector_in.begin ();

    while (it != object_name_vector_in.end ())
        {
            return_the_value_internal (*it, temp_value, temp_value,
                                       return_empty);
            it++;
        }
    if (!return_empty)
        value_to_return = temp_value;

    return value_to_return;
}

void
cjparse::return_the_value_internal (std::string object_name,
                                    json_value &first_value,
                                    json_value &value_to_return,
                                    bool &return_empty)
{
    json_value temp_value = first_value;

    if (std::holds_alternative<json_object> (temp_value))
        {
            auto value_object = std::get<json_object> (temp_value);
            auto iter = value_object.find (object_name);
            if (iter != value_object.end ())
                {
                    value_to_return = iter->second.value;
                    return;
                }
            else
                {
                    return_empty = true;
                    return;
                }
        }
    if (std::holds_alternative<json_array> (temp_value))
        {
            auto value_array = std::get<json_array> (temp_value);
            auto iter = value_array.begin ();
            while (iter != value_array.end ())
                {
                    return_the_value_internal (object_name, iter->value,
                                               value_to_return, return_empty);
                    iter++;
                }
        }

    value_to_return = temp_value;
}

template <class T>
bool
cjparse::check_if_type (std::vector<std::string> object_name_vector_in)
{
    bool bool_to_return = false;
    json_value full_json, value_to_return;
    std::visit ([&full_json] (auto value) { full_json = value; }, JSON.value);

    json_value temp_value = full_json;

    auto it = object_name_vector_in.begin ();

    while (it != object_name_vector_in.end ())
        {
            check_if_type_internal<T> (*it, temp_value, temp_value,
                                       bool_to_return);
            it++;
        }
    return bool_to_return;
}

template <class T>
void
cjparse::check_if_type_internal (std::string name_to_check_if_type,
                                 json_value &first_value,
                                 json_value &value_to_alter,
                                 bool &bool_to_return)
{

    json_value temp_value = first_value;

    if (std::holds_alternative<json_object> (temp_value))
        {

            auto value_object = std::get<json_object> (temp_value);
            auto iter = value_object.find (name_to_check_if_type);
            if (iter != value_object.end ())
                {
                    value_to_alter = iter->second.value;
                    if (std::holds_alternative<T> (first_value))
                        bool_to_return = true;
                    else
                        bool_to_return = false;

                    return;
                }
            else
                {
                    bool_to_return = false;
                    return;
                }
        }
    if (std::holds_alternative<json_array> (temp_value))
        {
            auto value_array = std::get<json_array> (temp_value);
            auto iter = value_array.begin ();
            while (iter != value_array.end ())
                {
                    check_if_type_internal<T> (name_to_check_if_type,
                                               iter->value, value_to_alter,
                                               bool_to_return);
                    iter++;
                }
        }
}
