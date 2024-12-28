#include <any>
#include <array>
#include <exception>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

// JSON DATA TYPES
// string      ex: "Hello"
// number      ex: 42
// Object      ex  {"name", "pepe", "age", 30}
// Array       ex: [1, 2, 3, 4]
// Bollean     ex: false
// Null        ex: Null
#pragma once
#ifndef CJPARSE_H
#define CJPARSE_H

class cjparse
{
  public:
    cjparse (std::string &str);
    cjparse (std::stringstream &file);

  protected:
    friend class cjparse_json_parser;
    friend class cjparse_json_generator;

  protected:
    struct cjparse_json_value;

  public:
    using json_null = std::any;
    using json_string = std::string;
    using json_number
        = std::variant<int, long int, long long int, double, long double>;

    using json_object = std::unordered_map<std::string, cjparse_json_value>;
    using json_array = std::vector<cjparse_json_value>;

    using json_value = std::variant<json_object, json_array, json_null,
                                    json_string, json_number, bool>;

  protected:
    struct cjparse_json_value
    {
        json_value value;

        cjparse_json_value () : value () {}
        cjparse_json_value (bool a) : value (a) {}
        cjparse_json_value (json_object job) : value (job) {}
        cjparse_json_value (json_array jarr) : value (jarr) {}
        cjparse_json_value (json_null jnull) : value (jnull) {}
        cjparse_json_value (json_string jstr) : value (jstr) {}
        cjparse_json_value (json_number jnum) : value (jnum) {}
    };

  public:
    cjparse_json_value JSON;

  public:
    /*
     * returns the value of the object with inputted name
     * of type "cjparse::json_value"
     * returns 'std::variant::variant_npos' if obj with name not found
     */
    std::optional<json_value> return_the_value (std::string &name);
    /*
     * searches for the value of object with inputted name in the first layer
     * of json tree
     * returns true if obj with name "name" exists and has value of what the
     * function name describes (obj, array, number, etc)
     * returns false if object with name "name" exists and has value NOT of
     * what the function name describes (obj, array, number, etc)
     * 'std::nullopt' if obj with name "name" was not found
     */
    template <class T> std::optional<bool> check_if_type (std::string &name);
    /*
     * searches for the value of object with inputted name int the FULL json
     * tree
     * returns true if it obj with name "name" exists and obj with name
     * "name" has value of what the function name describes (obj, array,
     * number, etc)
     * returns true if it obj with name "name" exists and obj with name
     * "name" has value NOT of what the function name describes (obj, array,
     * number, etc)
     * returns 'std::nullopt' if obj with name not found in the full JSON
     */

    template <class T>
    std::optional<bool> check_if_type_in_tree (std::string &name);

  private:
    template <class T>
    void check_if_type_in_tree_helper (std::optional<bool> &bool_to_alter,
                                       std::string &name, json_value &value);

  private:
};

#endif // CJPARSE_H
