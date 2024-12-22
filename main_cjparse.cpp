#include "cjparse.cpp"
#include <string>

// Helper function to print a json_number
void
printJsonNumber (const cjparse::json_number &num)
{
    std::visit ([] (const auto &val) { std::cout << val; }, num);
}

// Recursive function to print a json_value
void
printJsonValue (const cjparse::cjparse_json_value &jsonValue)
{
    std::visit (
        [] (const auto &arg) {
            using T = std::decay_t<decltype (arg)>;
            if constexpr (std::is_same_v<T, cjparse::json_null>)
                {
                    std::cout << "null";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_string>)
                {
                    std::cout << "\"" << arg << "\"";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_number>)
                {
                    printJsonNumber (arg);
                }
            else if constexpr (std::is_same_v<T, bool>)
                {
                    std::cout << (arg ? "true" : "false");
                }
            else if constexpr (std::is_same_v<T, cjparse::json_object>)
                {
                    std::cout << "\n{ ";
                    for (const auto &[key, value] : arg)
                        {
                            std::cout << " \"" << key << "\": ";
                            printJsonValue (value);
                            std::cout << ", ";
                        }
                    std::cout << " }\n";
                }
            else if constexpr (std::is_same_v<T, cjparse::json_array>)
                {
                    std::cout << "\n[ ";
                    for (const auto &element : arg)
                        {
                            printJsonValue (element);
                            std::cout << ", ";
                        }
                    std::cout << " ]\n";
                }
        },
        jsonValue.value);
}

std::string JSON
    = "{\n"
      "    \"Image\": {\n"
      "        \"Width\": 800,\n"
      "        \"Height\": 600,\n"
      "        \"Title\": \"View from 15th Floor\",\n"
      "        \"Thumbnail\": {\n"
      "            \"Url\": \"http://www.example.com/image/481989943\",\n"
      "            \"Height\": 125,\n"
      "            \"Width\": 100\n"
      "        },\n"
      "        \"Animated\": false,\n"
      "        \"IDs\": [116, 943, 234, 38793]\n"
      "    }\n"
      "}";
int
main ()
{
    // Create the parser with the given JSON string (assuming cjparse class has
    // a constructor that takes a string)
    cjparse parser (JSON);

    // Print the JSON structure
    std::cout << "Parsed JSON: \n";
    printJsonValue (parser.JSON);
    std::cout << "\n";
    return 0;
};
