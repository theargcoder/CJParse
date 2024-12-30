# **CJParse**

**CJParse** is a lightweight and portable C++ library for parsing and generating JSON, adhering to the [JSON Standard (*RFC 7159*)](https://www.rfc-editor.org/rfc/rfc7159).  

---

## **Features**

### **Parsing JSON**
- Converts any JSON string into a `CJParse` struct, containing all the contents of the JSON string.
- **Compact and portable**: The only dependency is the C++ Standard Library.
- **Internal Classes**:
  - A dedicated parser for handling JSON strings.
  - A generator for creating JSON strings with customization options.
  
### **Future Enhancements**
- Map live memory into a JSON file (**coming soon**).
- Error handling: Trace parsing errors down to the exact line and character (**planned**).

---

## **Classes Overview**

### `cjparse`
- **Purpose**: Main constructor of the library, alows user to interact with stored JSON in memory via an API, also defines the JSON datatypes
- **JSON datatypes Details**:
  - **Objects**: Stored in an unordered map (`std::unordered_map`). Original order is **not preserved** for efficiency; access by key is prioritized.
  - **Arrays**: Stored in a vector (`std::vector`) with their **original order preserved**.
  - **JSON Values**: Mapped to their C++ equivalents:
    - **JSON String** → `std::string`
    - **JSON Number** → `int` or `double` (based on type)
    - **JSON Bool** → `bool`
    - **JSON Null** → `std::any` (*subject to change*)
      
- **API** so you the end user can interact with the library
  - **`return_the_value`**: returns the value of the object with inputted name **IN THE FIRST LAYER OF JSON**
    - *input* -> `std::string &`
    - *returns* -> `json_value`
    - *exception* -> returns `std::variant::variant_npos` aka empty `json_value` if obj with name not found.
  - **`return_the_value_in_tree`**: returns the value of the first intance found of object with inputted name **IN FULL JSON TREE**
    - *input* -> `std::string &`
    - *returns* -> `json_value`
    - *exception* -> returns `std::variant::variant_npos` aka empty `json_value` if obj with name not found.
  - **`check_if_type`**: searches for the value of a json_object with inputted name, does so on the first layer of JSON.
    - *input* -> `std::string &`
    - *returns* -> `bool`
      - *true* -> if object with name  `name` exists and havs value of template definition.
      - *false* -> if object with name  `name` exists and DOES NOT value of template definition.
      - *exception* -> the internal function returns `std::nullopt` if obj with name not found, (*will implement a method to check if this happned soon*).
   - **`check_if_type_in_tree`**: searches for the value of a json_object with inputted name IN THE FULL JSON tree.
      - *input* -> `std::string &`
      - *returns* -> `bool`
         -  *true* -> if object with name  `name` exists and havs value of template definition.
         -  *false* -> if object with name  `name` exists and DOES NOT value of template definition.
         - *exception* -> the internal function returns `std::nullopt` if obj with name not found, (*will implement a method to check if this happned soon*).
  - **`check_if_type_inside_object`**: searches for the value of a json_object with inputted name inside of the object container with inputted name IN THE FULL JSON tree.
    - *input* -> `std::string &` and `std::string &`
    - *returns* -> `bool`
      - *true* -> if object with name  `name` inside of object with `name_of_obj` exists and havs value of template definition.
      - *false* -> if object with name  `name` inside of object with `name_of_obj` DOES NOT have value of template definition.
      - *exception* -> the internal function returns `std::nullopt` if object with `name` or object with `name_of_obj` was not found, (*will implement a method to check if this happned soon*).

---
### `cjparse_json_parser`
- **Purpose**: Parses input JSON strings into memory inside cjparse class, this class is called internally by cjparse.
- **NOTE**: End user should never have to interact with this class.
- **Design Details**:
  - **Parsing functions**: These 7 functions can correctly parse nested and non nested JSON **They call each other when nested types are detected and to parse values correctly**.
  - **Checking function**: Provide easy to use methods to verify if the JSON inputted string has x pattern.

---

### `cjparse_json_generator`
- **Purpose**: Generates JSON strings from parsed or newly created JSON objects.
- **Key Features**:
  - **2 overloads**:
    - can input either type `cjparse::json_value` or struct `cjparse::cjparse_json_value` and will produce proper JSON for both with formatting options listed below
  - **Customizable Output**:
    1. **Compact Mode** (`pretty = false`): Produces a minimal JSON string.
    2. **Pretty Mode** (`pretty = true`): Outputs a human-readable JSON string.
  - Future plans include additional customization options such as:
    - Wider whitespace.
    - Specific patterns for newlines or compact formatting.
    - Mixed formatting (e.g., compact arrays with pretty objects).

---

## **Important Notes**

- **API** *(as of Dec 29th, 2024)*:
  - all funtions have been tested and work in the manner explained in the documentation.
- **Limitations** *(as of Dec 26th, 2024)*:
  - There are no known limitations as of the above listed date.
- **Error Handling** *(planned)*:
  - No current support for exceptions or error messages pinpointing parsing issues. These features will be added in upcoming updates.  

---

CJParse aims to provide robust JSON parsing and generation capabilities while maintaining simplicity and portability. Stay tuned for updates!  
