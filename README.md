About
-------


**CJParse** is a lightweight and efficient C++ library for parsing and generating JSON, adhering to the [JSON Standard (RFC 8259)](https://www.rfc-editor.org/rfc/rfc8259). Designed with performance in mind, it simplifies working with JSON data in C++ applications.


Features
--------
  
  - JSON Parsing
  - JSON Generation
  - Lightweight
  - Portable
  - Standard Library is the only dependency


Getting Started
----------------

To download do:
```bash
git clone https://github.com/theargcoder/CJParse
```
You can straight up include like so:
```cpp
#include "cjparse.cpp"
```
> if you prefer you may create a static library
```bash
g++ -c cjparse.cpp -o cjparse.o
ar rcs libcjparse.a cjparse.o
```
> or a dynamic one
```bash
g++ -shared -fPIC cjparse.cpp -o libcjparse.so
```
> and link in the following fashion
```bash
g++ main.cpp -L. -lcjparse -o main_executable
```
More details can be found in the [getting started](https://github.com/theargcoder/CJParse/wiki/Getting-Started) page of the wiki.


Prerequisites
-------------

Ensure you have a C++ compiler that supports C++11 or later.


Wiki
----

Design details, behavior of functions, and examples all can be found in the [wiki](https://github.com/theargcoder/CJParse/wiki)


Usage
-----

Here’s a simple example demonstrating how to parse a JSON string using CJParse:
```cpp
// Create an instance of the cjparse class, initializing it with a JSON-compliant string.
cjparse parser(json_2);

// Generate a JSON string representation using the cjparse_json_generator.
// The second argument (true) enables pretty-printing for better readability.
cjparse_json_generator JSON_gen(parser.JSON, true);

// Output the formatted JSON string to the console.
std::cout << "Parsed JSON: \n" << JSON_gen.JSON_string << "\n"
```
For more detailed examples and usage guidelines, refer to the [wiki](https://github.com/theargcoder/CJParse/wiki)


Future Enhancements
-------------------

- error handling: Implementation of detailed error tracing to identify parssing errors down to exact line and character.


Contributions
-------------

Pull or commit to new_features branch or create a new branch!!!!


License
-------
This project is licensed under the [GPL-3.0 License](https://github.com/theargcoder/CJParse/blob/main/LICENSE)
