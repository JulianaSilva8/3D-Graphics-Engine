#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <string>
#include <vector>
#include "../utils/utils.hpp"
#include <GL/glew.h>

enum class Type {
    Point, 
    Directional,
    Spot
};

class Light {
public:
    Type type;
    Point position; // point e spot lights
    Point direction; // luzes direcionais e spot
    GLenum lightID; // setted up no inicio na scene
    float cutoff = 180.0f;

    Light(Type type, Point position = Point(), Point direction = Point(), float cutoff = 180.0f)
        : type(type), position(position), direction(direction), cutoff(cutoff), lightID(0) {}
    void setLightID(GLenum lightID);
    void draw() const;
};

#endif