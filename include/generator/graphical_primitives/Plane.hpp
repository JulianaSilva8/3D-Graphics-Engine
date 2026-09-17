#ifndef PLANE_HPP
#define PLANE_HPP

#include "Primitive.hpp"

class Plane : public Primitive
{
    float length;
    int divisions;

public:
    Plane(float s, int d);
    IndexedVertices generate() override;
};

#endif