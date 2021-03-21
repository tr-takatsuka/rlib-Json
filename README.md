# JSON parser

[日本語](/README.ja.md)

## Description

JSON parser. Implementation in C++.

+ It is a data structure class that conforms to the JSON specification, with parse and output (stringify) functions added.
+ Works with one header file.
+ It can be built with C++11. It does not depend on external libraries such as boost.
+ It implements some specifications of JSON5. (Can be disabled as an option)
  + You can parse JSON with comments. 
  + Allows Trailing commas (sometimes called "final commas").
+ Implements JSON Pointer.
+ It is designed so that exceptions do not occur when referencing or editing. (Excluding the at() function)
  + For out-of-range reads, the default value is taken, and for writes, an element is created.
+ Unlike javascript, numbers are divided into floating point numbers (double) and integers (std :: intmax_t).
+ Input / output is supported only for std::string (UTF-8).
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
    using Json = rlib::Json;
    const Json j = Json::parse(u8R"(                    // JSON 文字列から構築
        {                       // allows comments (JSON5)
            "n" : -123.456e+2,
            "list":[
                32,
                "ABC",          // allows Trailing comma (JSON5)
            ],
            "b": true,
            "c": null
        })");
    double d0 = j["n"].get<double>();                   // get -123.456e+2
    double da = j.at("n").get<double>();                // This is the description referenced by at(). (An exception will be thrown if it is out of range)
    double d1 = j["e"].get<double>();                   // get 0.0 (Since a position that does not exist is specified, the default value can be taken.)
    std::intmax_t n1 = j["n"].get<std::intmax_t>();     // get -12346 (You can take rounded integer values)
    std::string s0 = j["list"][1].get<std::string>();   // get "ABC"
    std::string sa = j.at(Json::Pointer("/list/1")).get<std::string>(); // It is a description specified by JSON Pointer.
    std::string s1 = j["ary"][9].get<std::string>();    // get empty string (Since a position that does not exist is specified, the default value can be taken.)
    Json list = j["list"];                              // duplicate (deep copy) from "list"
    list[10]["add"] = 123;                              // Added {"add": 123} to position [10] (positions of array [2-9] are padded with null)
    bool compare = list == j["list"];                   // It is a comparison. get false.
    std::string json = list.stringify();                // get JSON string
    list[10].erase("add");                              // Removed associative array element ({"add": 123}) at position [10]
    list.erase(9);                                      // Removed element (null) at position [9]
    Json& c = list.at(10);                              // Referenced with at() raises an exception if out of range
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

