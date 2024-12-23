#include "cjparse_print.h"

cjparse_json_print::cjparse_json_print (cjparse::cjparse_json_value &JSON)
{
    print_json_from_memory (JSON);
}

void
cjparse_json_print::print_json_from_memory (cjparse::cjparse_json_value &JSON)
{
    n_of_iteration++;
    std::string n_of_tabs;
    for (int i = 0; i < n_of_iteration; i++)
        n_of_tabs = n_of_tabs + std::string ("   ");
    std::visit (
        [this, n_of_tabs] (auto &json_val) {
            using T = std::decay_t<decltype (json_val)>;
            if constexpr (std::is_same_v<T, cjparse::json_object>)
                {
                    std::cout << "{\n";
                    for (auto &[name, value] : json_val)
                        {
                            std::cout << n_of_tabs << " \"" << name << "\": ";
                            print_json_value (value);
                            std::cout << ",\n";
                        }
                    std::cout << n_of_tabs << " }";
                    n_of_iteration--;
                }
            else if constexpr (std::is_same_v<T, cjparse::json_array>)
                {
                    std::cout << "[\n";
                    for (auto &value : json_val)
                        {
                            std::cout << n_of_tabs;
                            print_json_value (value);
                            std::cout << ",\n";
                        }
                    std::cout << "\n" << n_of_tabs << " ]";
                    n_of_iteration--;
                }
        },
        JSON.value);
}

void
cjparse_json_print::print_json_value (cjparse::json_value &val)
{
    std::visit (
        [this] (auto &json_val) {
            using T = std::decay_t<decltype (json_val)>;
            if constexpr (std::is_same_v<T, cjparse::json_null>)
                {
                    std::cout << "null";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_string>)
                {
                    std::cout << "\"" << json_val << "\"";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_number>)
                {
                    print_json_number (json_val);
                }
            else if constexpr (std::is_same_v<T, bool>)
                {
                    std::cout << (json_val ? "true" : "false");
                }
        },
        val);
}

void
cjparse_json_print::print_json_number (cjparse::json_number &num)
{
    std::visit ([] (const auto &val) { std::cout << val; }, num);
}
