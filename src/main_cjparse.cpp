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
        "Image2": {
            "Width":  200,
            "Height": 900,
            "Title":  "View from 10th Floor",
            "Thumbnail": {
                "Url":    "http://www.example.com/image/92837592",
                "Height": 260,
                "Width": 1000 
            },
            "Animated" : true,
            "IDs": [899, 831, 254, 648953]
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

    // testing return_the_value 1 input
    cjparse::json_value value_to_return = parser.return_the_value ("Image");
    JSON_gen = cjparse_json_generator (value_to_return, true);
    std::cout << "testing return the value in Image"
              << "\n"
              << JSON_gen.JSON_string << '\n';

    // testing return_the_value multiple inputs
    value_to_return
        = parser.return_the_value ({ "Image2", "Thumbnail", "Width" });
    JSON_gen = cjparse_json_generator (value_to_return, true);
    std::cout << "testing return the value in { \"Image2\", \"Thumbnail\", "
                 "\"Width\" }"
              << "\n"
              << JSON_gen.JSON_string << '\n';

    // testing check_if_type single name
    // checkiong if Width has a value of type "json_number"
    bool checking_if_number
        = parser.check_if_type<cjparse::json_object> ("Image");
    if (checking_if_number == true)
        {
            std::cout << "object named: " << "Image"
                      << "  has value type T (tempate)" << '\n';
        }
    else
        {
            std::cout << "object named: " << "Image"
                      << " has value type NOT T (template)" << '\n';
        }
    // testing check_if_type multiple names
    // checkiong if Width has a value of type "json_number"
    checking_if_number = parser.check_if_type<cjparse::json_number> (
        { "Image2", "Thumbnail", "Width" });
    if (checking_if_number == true)
        {
            std::cout << "object named: " << "Width"
                      << "  has value type T (tempate)" << '\n';
        }
    else
        {
            std::cout << "object named: " << "Width"
                      << " has value type NOT T (template)" << '\n';
        }

    return 0;
};
