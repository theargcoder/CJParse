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

    // testing return_value
    std::string obj_name = "Image";
    cjparse::json_value value_of_obj_name = parser.return_the_value (obj_name);
    JSON_gen = cjparse_json_generator (value_of_obj_name, true);
    std::cout << "we obtained the value of object named: " << obj_name
              << " with value stored in memory: " << '\n'
              << JSON_gen.JSON_string << '\n';

    // testing return_value_in_tree
    obj_name = "Title";
    cjparse::json_value value_in_tree
        = parser.return_the_value_in_tree (obj_name);
    JSON_gen = cjparse_json_generator (value_in_tree, true);
    std::cout << "we obtained the value of object named: " << obj_name
              << " with value stored in memory: " << '\n'
              << JSON_gen.JSON_string << '\n';

    // testing return_the_value_inside_object
    std::string object_container = "Thumbnail";
    obj_name = "Width";
    cjparse::json_value value_in_obj
        = parser.return_the_value_inside_object (object_container, obj_name);
    JSON_gen = cjparse_json_generator (value_in_obj, true);
    std::cout << "we obtained the value of object named: " << obj_name
              << " inside of container named: " << object_container
              << " with value stored in memory: " << '\n'
              << JSON_gen.JSON_string << '\n';

    // testing check_if_type
    obj_name = "Image";
    std::optional<bool> will_be_null
        = parser.check_if_type<cjparse::json_object> (
            obj_name); // will return nullopt
    if (will_be_null != std::nullopt)
        {
            if (will_be_null == true)
                std::cout << "object named: " << obj_name
                          << "  has value type T (tempate)" << '\n';
            else
                std::cout << "object named: " << obj_name
                          << " has value type NOT T (template)" << '\n';
        }
    else
        {
            std::cout << "object named: " << obj_name << " was not found "
                      << '\n';
        }

    // testing check_if_type_in_tree
    obj_name = "Thumbnail";
    std::optional<bool> shouldnt_be_null
        = parser.check_if_type_in_tree<cjparse::json_string> (obj_name);
    if (shouldnt_be_null != std::nullopt)
        {
            if (shouldnt_be_null == true)
                std::cout << "object named: " << obj_name
                          << "  has value type T (tempate)" << '\n';
            else
                std::cout << "object named: " << obj_name
                          << " has value type NOT T (template)" << '\n';
        }
    else
        {
            std::cout << "object named: " << obj_name
                      << " was not found in the tree" << '\n';
        }
    // testing check_if_type_inside_object
    object_container = "Image";
    obj_name = "Animated";
    std::optional<bool> shouldnt_be_null_2
        = parser.check_if_type_inside_object<bool> (object_container,
                                                    obj_name);
    if (shouldnt_be_null_2 != std::nullopt)
        {
            if (shouldnt_be_null_2 == true)
                std::cout << "object named: " << obj_name << '\n'
                          << "inside of object named: " << object_container
                          << "  has value type T (tempate)" << '\n';
            else
                std::cout << "object named: " << obj_name << '\n'
                          << "inside of object named: " << object_container
                          << "  has value type T (tempate)" << '\n';
        }
    else
        {
            std::cout << "object named: " << obj_name
                      << " was not found in the tree" << '\n';
        }

    return 0;
};
