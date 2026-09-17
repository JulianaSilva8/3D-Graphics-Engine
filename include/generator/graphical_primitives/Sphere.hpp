#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Primitive.hpp"

class Sphere : public Primitive
{
    float radius;
    int stacks;
    int slices;

public:
    Sphere(float r, int st, int sl);
    IndexedVertices generate() override;
};

#endif