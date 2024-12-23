#include "cjparse.cpp"
#include "cjparse_print.cpp"
#include <string>

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
    cjparse_json_print (parser.JSON);
    std::cout << "\n";
    return 0;
};
