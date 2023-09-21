/**
 * Copyright (c) 2023 by Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */

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