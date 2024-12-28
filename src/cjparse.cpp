#include "cjparse.h"
#include "cjparse_json_parser.cpp"

cjparse::cjparse (std::string &str) { cjparse_json_parser (str, JSON); }

cjparse::cjparse (std::stringstream &fake_str)
{
    // to be determied
}

cjparse::json_value
cjparse::return_the_value (std::string &name_to_return_value)
{
    json_value value;
    std::optional<json_value> when_valid_return
        = return_the_value_internal (name_to_return_value);
    if (when_valid_return.has_value ())
        return when_valid_return;
    else
        return value;
}

cjparse::json_value
cjparse::return_the_value_in_tree (std::string &name_to_return_value)
{
    json_value value;
    std::optional<json_value> when_valid_return
        = return_the_value_in_tree_internal (name_to_return_value);
    if (when_valid_return.has_value ())
        return when_valid_return;
    else
        return value;
}

template <class T>
bool
cjparse::check_if_type (std::string &name_to_check_if_type)
{
    bool bool_to_return;
    std::optional<bool> when_valid_return
        = check_if_type_internal<T> (name_to_check_if_type);
    if (when_valid_return.has_value ())
        return when_valid_return.value ();
    else
        return false;
}

template <class T>
bool
cjparse::check_if_type_in_tree (std::string &name_to_check_if_type)
{
    bool bool_to_return;
    std::optional<bool> when_valid_return
        = check_if_type_in_tree_internal<T> (name_to_check_if_type);
    if (when_valid_return.has_value ())
        return when_valid_return.value ();
    else
        return false;
}

template <class T>
bool
cjparse::check_if_type_inside_object (std::string &name_of_object_container,
                                      std::string &name_to_check_if_type)
{
    bool bool_to_return;
    std::optional<bool> when_valid_return
        = check_if_type_inside_object_internal<T> (name_of_object_container,
                                                   name_to_check_if_type);
    if (when_valid_return.has_value ())
        return when_valid_return.value ();
    else
        return false;
}

std::optional<cjparse::json_value>
cjparse::return_the_value_internal (std::string &name)
{
    if (!std::holds_alternative<json_object> (JSON.value))
        {
            return std::nullopt;
            throw std::runtime_error ("JSON is not an object.");
        }

    const auto &json_obj = std::get<json_object> (JSON.value);
    auto it = json_obj.find (name);

    if (it != json_obj.end ())
        {
            return it->second.value;
        }

    // if key is not found we retun nullopt
    return std::nullopt;
    // throw std::runtime_error ("Key not found: " + name);
}

void
cjparse::return_the_value_in_tree_helper (
    std::optional<json_value> &value_to_alter, std::string &name,
    json_value &value_in)
{
    std::cout << "We are here..." << '\n';
    if (std::holds_alternative<json_object> (value_in))
        {
            auto &json_obj = std::get<json_object> (value_in);
            auto it = json_obj.begin ();

            while (it
                   != json_obj.end ()) // json is object and key 'name' exists
                {
                    if (it->first == name)
                        {
                            value_to_alter = it->second.value;
                            break;
                        }
                    else if (std::holds_alternative<json_object> (
                                 it->second.value))
                        {
                            return_the_value_in_tree_helper (
                                value_to_alter, name, it->second.value);
                        }
                    it++;
                }
        }
    else if (std::holds_alternative<json_array> (value_in))
        {
            auto &json_obj = std::get<json_array> (value_in);
            auto it = json_obj.begin ();

            while (it != json_obj.end ())
                {
                    if (std::holds_alternative<json_object> (it->value))
                        {
                            return_the_value_in_tree_helper (value_to_alter,
                                                             name, it->value);
                        }
                    else if (std::holds_alternative<json_array> (it->value))
                        {
                            return_the_value_in_tree_helper (value_to_alter,
                                                             name, it->value);
                        }
                    it++;
                }
        }
}

std::optional<cjparse::json_value>
cjparse::return_the_value_in_tree_internal (std::string &name_to_return_value)
{
    std::optional<json_value> value_to_return;

    json_value value;

    std::visit ([&value] (auto &value_in) { value = value_in; }, JSON.value);

    return_the_value_in_tree_helper (value_to_return, name_to_return_value,
                                     value);

    return value_to_return;
}

template <class T>
std::optional<bool>
cjparse::check_if_type_internal (std::string &name)
{
    // JSON not an object so no key value pairs
    if (!std::holds_alternative<json_object> (JSON.value))
        return std::nullopt;
    //  throw std::runtime_error ("JSON is not an object");

    const auto &json_obj = std::get<json_object> (JSON.value);
    auto it = json_obj.find (name);

    if (it != json_obj.end ()) // json is object and key 'name' exists
        {
            // the value of key "name" is same type as template
            if (std::holds_alternative<T> (it->second.value))
                return true;
            else // the value of key "name" is not same sas template
                return false;
        }
    else
        {
            return std::nullopt; // json is object but key "name" doesnt exist.
        }
}

template <class T>
void
cjparse::check_if_type_in_tree_helper (std::optional<bool> &bool_to_alter,
                                       std::string &name, json_value &value_in)
{
    if (std::holds_alternative<json_object> (value_in))
        {
            auto &json_obj = std::get<json_object> (value_in);
            auto it = json_obj.begin ();

            while (it
                   != json_obj.end ()) // json is object and key 'name' exists
                {
                    if (it->first == name)
                        {
                            if (std::holds_alternative<T> (it->second.value))
                                bool_to_alter = true;
                            else
                                bool_to_alter = false;
                            break;
                        }
                    else if (std::holds_alternative<json_object> (
                                 it->second.value))
                        {
                            check_if_type_in_tree_helper<T> (
                                bool_to_alter, name, it->second.value);
                        }
                    it++;
                }
        }
    else if (std::holds_alternative<json_array> (value_in))
        {
            auto &json_obj = std::get<json_array> (value_in);
            auto it = json_obj.begin ();

            while (it != json_obj.end ())
                {
                    if (std::holds_alternative<json_object> (it->value))
                        {
                            check_if_type_in_tree_helper<T> (bool_to_alter,
                                                             name, it->value);
                        }
                    else if (std::holds_alternative<json_array> (it->value))
                        {
                            check_if_type_in_tree_helper<T> (bool_to_alter,
                                                             name, it->value);
                        }
                    it++;
                }
        }
}

template <class T>
std::optional<bool>
cjparse::check_if_type_in_tree_internal (std::string &name)
{
    std::optional<bool> bool_to_return;

    json_value value;

    std::visit ([&value] (auto &value_in) { value = value_in; }, JSON.value);

    check_if_type_in_tree_helper<T> (bool_to_return, name, value);

    return bool_to_return;
}

template <class T>
std::optional<bool>
cjparse::check_if_type_inside_object_internal (
    std::string &name_of_object_container, std::string &name_to_check_if_type)
{
    std::optional<bool> bool_to_return;
    std::optional<bool> bool_to_find_container;

    json_value value;

    std::visit ([&value] (auto &value_in) { value = value_in; }, JSON.value);

    check_if_type_in_tree_helper<json_object> (
        bool_to_find_container, name_of_object_container, value);

    if (bool_to_find_container != std::nullopt)
        {
            value = return_the_value_in_tree (name_of_object_container);
            check_if_type_in_tree_helper<T> (bool_to_return,
                                             name_to_check_if_type, value);
        }

    return bool_to_return;
}
