#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Primitive.hpp"

class Cylinder : public Primitive
{
    float radius;
    float height;
    int slices;

public:
    Cylinder(float r, float h, int s);
    IndexedVertices generate() override;
};

#endif