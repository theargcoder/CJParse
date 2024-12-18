#include "cjparse.cpp"
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
    cjparse parser = cjparse (JSON);
    cjparse::cjparse_json_value JAISON;
    return 0;
}
