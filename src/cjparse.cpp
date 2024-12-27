#include "cjparse.h"
#include "cjparse_json_parser.cpp"

cjparse::cjparse (std::string &str) { cjparse_json_parser (str, JSON); }

cjparse::cjparse (std::stringstream &fake_str)
{
    // to be determied
}

std::optional<cjparse::json_value>
cjparse::return_the_value (std::string &name)
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

template <class T>
std::optional<bool>
cjparse::check_if_type (std::string &name)
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
