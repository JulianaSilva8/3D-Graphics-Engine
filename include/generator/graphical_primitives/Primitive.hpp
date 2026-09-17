#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "../../utils/utils.hpp"

class Primitive
{
public:
    virtual ~Primitive() {}
    virtual IndexedVertices generate() = 0;
};

#endif