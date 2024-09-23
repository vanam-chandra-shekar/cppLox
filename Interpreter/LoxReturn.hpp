#pragma once

#include <any>


class Return
{
public:
    const std::any value;
    
    Return(std::any _value)
     : value(std::move(_value)){}
};