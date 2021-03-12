# JSON parser

[日本語](/README.ja.md)

## Description

JSON parser. Implementation in C++.

+ It is a data structure class that conforms to the JSON specification, with parse and output (stringify) functions added.
+ Works only with header files.
+ It can be built with C++11. It does not depend on external libraries such as boost.
+ It is designed so that exceptions do not occur when referencing or editing. (Excluding the at () function)
  + For out-of-range reads, the default value is taken, and for writes, an element is created.
+ Input / output is supported only for std :: string (UTF-8).
+ Unlike javascript, numbers are processed separately as real numbers (double) and integers (std :: intmax_t).
+ It implements some specifications of JSON5.
  + You can parse JSON with comments. (Can be disabled as an option)
+ It does not support the parsing process of stream input.


## Requirement

Works in the C++11 environment. Nothing is required for operation.

We have confirmed the operation in the following environment.
+ linux g++
+ windows VisualStudio 2015,2017,2019

## Usage

It can be used by including Json.h.

## example

```c++

#include "Json.h"

try {
    const rlib::Json j = rlib::Json::parse(             // Constructed from a JSON string
        u8R"({
            "n" : -123.456e+2,
            "list":[
                32,
                "ABC"
            ],
            "b": true,
            "c": null
        })");
    double d0 = j["n"].get<double>();                   // get -123.456e+2
    double d1 = j["e"].get<double>();                   // get 0.0 (Since a position that does not exist is specified, the default value can be taken.)
    std::intmax_t n1 = j["n"].get<std::intmax_t>();     // get -12346 (You can take rounded integer values)
    std::string s0 = j["list"][1].get<std::string>();   // get "ABC"
    std::string s1 = j["ary"][9].get<std::string>();    // get empty string (Since a position that does not exist is specified, the default value can be taken.)
    rlib::Json list = j["list"];                        // duplicate (deep copy) from "list"
    list[10]["add"] = 123;                              // Added {"add": 123} to position [10] (positions of array [2-9] are padded with null)
    bool compare = list == j["list"];                   // It is a comparison. get false.
    std::string json = list.stringify();                // get JSON string
    rlib::Json &c = list.at(11);                        // Referenced with at () raises an exception if out of range
} catch (rlib::Json::ParseException& e) {       // perse failure
    std::cerr << e.what() << std::endl;
} catch (std::out_of_range& e) {                // Out-of-range
    std::cerr << e.what() << std::endl;
}
```

## Feature and Limitations

- It also contains test code using BOOST.TEST. Json_test.cpp
- Code indentation is tabs, not spaces. On github, add "?ts=4" to the end of the URL and refer to it on tab 4. I'm sorry.

## Licence

[LICENSE](/LICENSE)
