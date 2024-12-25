# **CJParse**

**CJParse** is a lightweight and portable C++ library for parsing and generating JSON, adhering to the [JSON Standard (*RFC 7159*)](https://www.rfc-editor.org/rfc/rfc7159).  

---

## **Features**

### **Parsing JSON**
- Converts any JSON file into a `CJParse` struct, storing the original contents exactly as they appear in memory.
- **Compact and portable**: The only dependency is the C++ Standard Library.
- **Internal Classes**:
  - A dedicated parser for handling JSON strings.
  - A generator for creating JSON strings with customization options.
  
### **Future Enhancements**
- Map live memory into a JSON file (**coming soon**).
- Error handling: Trace parsing errors down to the exact line and character (**planned**).

---

## **Classes Overview**

### `cjparse_json_parser`
- **Purpose**: Parses input JSON strings into memory.
- **Design Details**:
  - **Objects**: Stored in an unordered map (`std::unordered_map`). Original order is **not preserved** for efficiency; access by key is prioritized.
  - **Arrays**: Stored in a vector (`std::vector`) with their **original order preserved**.
  - **JSON Values**: Mapped to their C++ equivalents:
    - **JSON String** → `std::string`
    - **JSON Number** → `int` or `double` (based on type)
    - **JSON Bool** → `bool`
    - **JSON Null** → `std::any` (*subject to change*)

---

### `cjparse_json_generator`
- **Purpose**: Generates JSON strings from parsed or newly created JSON objects.
- **Key Features**:
  - **Customizable Output**:
    1. **Compact Mode** (`pretty = false`): Produces a minimal JSON string.
    2. **Pretty Mode** (`pretty = true`): Outputs a human-readable JSON string.
  - Future plans include additional customization options such as:
    - Wider whitespace.
    - Specific patterns for newlines or compact formatting.
    - Mixed formatting (e.g., compact arrays with pretty objects).

---

## **Important Notes**

- **Limitations** *(as of Dec 25th, 2024)*:
  - Decimal places are not parsed correctly (**fix pending**).
  - Parsing exponential numbers may cause errors or exceptions (**investigating**).
- **Error Handling** *(planned)*:
  - No current support for exceptions or error messages pinpointing parsing issues. These features will be added in upcoming updates.  

---

CJParse aims to provide robust JSON parsing and generation capabilities while maintaining simplicity and portability. Stay tuned for updates!  
