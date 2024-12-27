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
     * searches for the value of object with inputted name and returns true
     * if it is what function name describes (obj, array, number, etc)
     * returns 'std::nullopt' if obj with name not found
     */
    template <class T> std::optional<bool> check_if_type (std::string &name);

  private:
};

#endif // CJPARSE_H
