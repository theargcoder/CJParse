#include "cjparse.cpp"
#include "cjparse_json_generate.cpp"
#include <string>

std::string json_2 = R"({
        "Image": {
            "Width":  800,
            "Height": 600,
            "Title":  "View from 15th Floor",
            "Thumbnail": {
                "Url":    "http://www.example.com/image/481989943",
                "Height": 125,
                "Width":  100
            },
            "Animated" : false,
            "IDs": [116, 943, 234, 38793]
          },
        "Image": {
            "Width":  800,
            "Height": 600,
            "Title":  "View from 15th Floor",
            "Thumbnail": {
                "Url":    "http://www.example.com/image/481989943",
                "Height": 125,
                "Width":  100
            },
            "Animated" : false,
            "IDs": [116, 943, 234, 38793]
          }
      })";

std::string json_1 = R"([
        {
           "precision": "zip",
           "Latitude":  37.7668,
           "Longitude": -122.3959,
           "Address":   "",
           "City":      "SAN FRANCISCO",
           "State":     "CA",
           "Zip":       "94107",
           "Country":   "US"
        }, 53,
        {
           "precision": "zip",
           "Latitude":  37.371991,
           "Longitude": -122.026020,
           "Address":   "",
           "City":      "SUNNYVALE",
           "State":     "CA",
           "Zip":       "94085",
           "Country":   "MX"
        },
        90,
  {"people": 93}
      ])";

int
main ()
{
    // Create the parser with the given JSON string (assuming cjparse class has
    // a constructor that takes a string)
    cjparse parser (json_2);

    // Print the JSON structure
    std::cout << "Parsed JSON: \n";
    cjparse_json_generator JSON_gen
        = cjparse_json_generator (parser.JSON, true);
    std::cout << JSON_gen.JSON_string << "\n";
    return 0;
};
