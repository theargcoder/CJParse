#include "cjparse_print.h"

cjparse_json_print::cjparse_json_print (cjparse::cjparse_json_value &JSON)
{
    print_json_from_memory (JSON);
}

void
cjparse_json_print::print_json_from_memory (cjparse::cjparse_json_value &JSON)
{
    std::string n_of_tabs;
    for (int i = 0; i < n_of_iteration; i++)
        n_of_tabs.push_back ('\t');
    std::visit (
        [this, n_of_tabs] (auto &json_val) {
            using T = std::decay_t<decltype (json_val)>;
            if constexpr (std::is_same_v<T, cjparse::json_object>)
                {
                    std::cout << n_of_tabs << "{\n";
                    for (auto &[name, value] : json_val)
                        {
                            std::cout << n_of_tabs << " \"" << name << "\": ";
                            print_json_from_memory (value);
                            std::cout << ",\n";
                        }
                    std::cout << n_of_tabs << " }\n";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_array>)
                {
                    std::cout << n_of_tabs << "[ ";
                    for (auto &value : json_val)
                        {
                            print_json_from_memory (value);
                            std::cout << ", ";
                        }
                    std::cout << n_of_tabs << " ]\n";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_null>)
                {
                    std::cout << "null\n";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_string>)
                {
                    std::cout << "\"" << json_val << "\"\n";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_number>)
                {
                    print_json_number (json_val);
                }
            else if constexpr (std::is_same_v<T, bool>)
                {
                    std::cout << (json_val ? "true\n" : "false\n");
                }
        },
        JSON.value);
    n_of_iteration++;
}

void
cjparse_json_print::print_json_number (cjparse::json_number &num)
{
    std::visit ([] (auto &val) { std::cout << val; }, num);
}
