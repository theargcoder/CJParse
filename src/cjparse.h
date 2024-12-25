#include <any>
#include <array>
#include <exception>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
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
    cjparse (std::fstream &file);

  protected:
    friend class cjparse_json_parser;
    friend class cjparse_json_generator;

  protected:
    struct cjparse_json_value;

  protected:
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

  private:
};

class cjparse_json_parser
{
  public:
    // modify by reference the container due to nesting
    static void cjparse_parse_value (std::string &str,
                                     cjparse::cjparse_json_value &value);

    static void cjparse_parse_array (std::string &str,
                                     cjparse::cjparse_json_value &value);

    static void cjparse_parse_object (std::string &str,
                                      cjparse::cjparse_json_value &value);

    // assign value by returning it
    static std::string cjparse_parse_value_string (std::string &str);

    static std::variant<int, long int, long long int, double, long double>
    cjparse_parse_value_number (std::string &str);

    static bool cjparse_parse_value_bool (std::string &str);

    static std::any cjparse_parse_value_null (std::string &str);

  private:
    static int check_what_is_the_value (std::string &str);

    static void check_if_prev_is_backlash (std::string &str,
                                           std::size_t &position,
                                           char pattern);

    static std::size_t
    return_the_matching_pattern (std::string &str,
                                 std::size_t pos_of_bracket_to_match,
                                 char pattern, char patter_to_match);

    static void find_delimeters_check_if_comma_alter_state (
        std::string &str, std::size_t &initial_delimeter,
        std::size_t &final_delimeter,
        std::size_t &not_white_position_after_final_delimeter, char pattern);
};

#endif // CJPARSE_H
