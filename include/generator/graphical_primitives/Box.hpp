#ifndef BOX_HPP
#define BOX_HPP

#include "Primitive.hpp"

class Box : public Primitive
{
    float length;
    int divisions;

public:
    Box(float s, int d);
    IndexedVertices generate() override;
};

#endif