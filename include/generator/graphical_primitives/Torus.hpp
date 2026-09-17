#ifndef TORUS_HPP
#define TORUS_HPP

#include "Primitive.hpp"

class Torus : public Primitive {
    float radius;
    float ring_radius;
    int stacks;
    int slices;
public:
    Torus(float r, float r_ring, int st, int sl);
    IndexedVertices generate() override;
};

#endif