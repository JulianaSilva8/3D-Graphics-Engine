#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include <vector>
#include "../utils/utils.hpp"

class Camera {
public:
    Point Pos;
    Point LookAt;
    Point Up;
    float fov;
    float nearPlane;
    float farPlane;

    Camera();
    Camera(const Point& pos, const Point& lookAt, const Point& up, float fov, float nearPlane, float farPlane);
};

#endif