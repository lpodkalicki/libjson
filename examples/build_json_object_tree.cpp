/**
 * Copyright (c) 2023 by Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */

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