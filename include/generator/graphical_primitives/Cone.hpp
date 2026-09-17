#ifndef CONE_HPP
#define CONE_HPP

#include "Primitive.hpp"

class Cone : public Primitive
{
    float radius;
    float height;
    int slices;
    int stacks;

public:
    Cone(float r, float h, int s, int d);
    IndexedVertices generate() override;
};

#endif