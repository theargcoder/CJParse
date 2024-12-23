#include "cjparse_print.h"

cjparse_json_print::cjparse_json_print (cjparse::cjparse_json_value &JSON)
{
    print_json_from_memory (JSON);
}

void
cjparse_json_print::print_json_from_memory (cjparse::cjparse_json_value &JSON)
{
    std::visit (
        [this] (auto &json_val) {
            using T = std::decay_t<decltype (json_val)>;
            if constexpr (std::is_same_v<T, cjparse::json_object>)
                {
                    n_of_iteration++;
                    std::string n_of_tabs;
                    for (int i = 0; i < n_of_iteration; i++)
                        n_of_tabs = n_of_tabs + std::string ("    ");
                    std::cout << "{\n";
                    std::size_t size = json_val.size ();
                    int counter = 0;
                    for (auto &[name, value] : json_val)
                        {
                            std::cout << n_of_tabs << " \"" << name << "\": ";
                            print_json_from_memory (value);
                            if (counter != size - 1)
                                std::cout << ",\n";
                            else
                                std::cout << '\n';
                            counter++;
                        }
                    n_of_iteration--;
                    n_of_tabs = "";
                    for (int i = 0; i < n_of_iteration; i++)
                        n_of_tabs = n_of_tabs + std::string ("    ");
                    std::cout << n_of_tabs << " }";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_array>)
                {
                    n_of_iteration++;
                    std::string n_of_tabs;
                    for (int i = 0; i < n_of_iteration; i++)
                        n_of_tabs = n_of_tabs + std::string ("    ");
                    std::size_t size = json_val.size ();
                    int counter = 0;
                    std::cout << "[\n";
                    for (auto &value : json_val)
                        {
                            std::cout << n_of_tabs;
                            print_json_from_memory (value);
                            if (counter != size - 1)
                                std::cout << ",\n";
                            else
                                std::cout << '\n';
                            counter++;
                        }
                    n_of_iteration--;
                    n_of_tabs = "";
                    for (int i = 0; i < n_of_iteration; i++)
                        n_of_tabs = n_of_tabs + std::string ("    ");
                    std::cout << n_of_tabs << " ]";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_null>)
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
        JSON.value);
}

void
cjparse_json_print::print_json_number (cjparse::json_number &num)
{
    std::visit ([] (auto &val) { std::cout << val; }, num);
}
