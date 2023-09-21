/**
 * Copyright (c) 2023 by Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */

#include <iostream>
#include "json.h"

int main()
{
    auto obj = json::parse("{\"key1\": 12, \"key2\": 13}");
    std::cout << obj << std::endl;
    return 0;
}
