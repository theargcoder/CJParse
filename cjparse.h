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

class cjparse
{
  public:
    cjparse (std::string &str);
    cjparse (std::fstream &file);

  public:
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

  public:
    char whitespace[6] = { 0x20, 0x0c, 0x0a, 0x0d, 0x09, 0x0b };

    json_null null;
    json_string string;
    bool j_bool;

    json_object cjparse_json_object;
    json_array cjparse_json_array;

  public:
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
    friend class cjparse_json_checkers;
    friend class cjparse_json_parser;

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
    static unsigned char check_what_is_the_value (std::string &str);

    static void check_if_prev_is_backlash (std::string &str,
                                           std::size_t &position,
                                           char pattern);

    static std::size_t return_the_matching_bracket (
        std::string &str, std::size_t pos_of_bracket_to_match, char pattern);
};
