# libjson

## About 
This is a modern C++, free, tiny, simple and one-header JSON library. You are welcome to use it freely in commercial projects.
The tests are designed for Linux but can be adapted for any platform with gtest/gmock support and a modern compiler available.

## Examples
Here are some examples. Their source files can be found in the 'examples' directory.

### JSON from string
```c++
#include <iostream>
#include "json.h"

int main()
{
    auto obj = json::parse("{\"key1\": 12, \"key2\": 13}");
    std::cout << obj << std::endl;
    return 0;
}

/* STDOUT:
{
  "key1": 12,
  "key2": 13
}
*/
```

### JSON as map
```c++
#include <iostream>
#include "json.h"

int main()
{
    json::Object obj;
    obj["key1"] = 1;
    obj["key2"] = 2.0;
    obj["key3"] = {1, 2, 3, 4, 5};
    obj["key3"][5] = "abc";
    obj["key4"]["key5"] = "jest ok!";
    obj["key4"]["key5"] = 3;
    obj["key4"]["key7"]["key9"] = {4, 7};
    obj["key4"]["key7"]["key9"][2] = 9;
    obj["key19"] = false;
    std::cout << obj << std::endl;
    return 0;
}

/* STDOUT:
{
  "key1": 1,
  "key19": false,
  "key2": 2,
  "key3": [1, 2, 3, 4, 5, "abc"],
  "key4": {
    "key5": 3,
    "key7": {
      "key9": [4, 7, 9]
    }
  }
}
*/
```
### JSON as tree

```c++
#include <iostream>
#include "json.h"

int main()
{
    json::Object obj{
        {"key1"_, 1},
        {"key2"_, 2.0},
        {"key3"_, {1, 2, 3, 4, 5, "abc"}},
        {"key4"_, {
            {"key5"_, "jest ok!"},
            {"key6"_, 3},
            {"key7"_, {
                {"key9"_, {4, 7, 9}}
            }}
        }},
        {"key10"_, false}
    };
    std::cout << obj << std::endl;
    return 0;
}

/* STDOUT:
{
  "key1": 1,
  "key10": false,
  "key2": 2,
  "key3": [1, 2, 3, 4, 5, "abc"],
  "key4": {
    "key5": "jest ok!",
    "key6": 3,
    "key7": {
      "key9": [4, 7, 9]
    }
  }
}
*/
```
