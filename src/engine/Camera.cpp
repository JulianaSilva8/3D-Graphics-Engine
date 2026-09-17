#include "../../include/engine/Camera.hpp"

Camera::Camera() 
    : Pos({0.0f, 0.0f, 5.0f}), LookAt({0.0f, 0.0f, 0.0f}), Up({0.0f, 1.0f, 0.0f}), fov(45.0f), nearPlane(0.1f), farPlane(100.0f) {}

Camera::Camera(const Point& pos, const Point& lookAt, const Point& up, float fov, float nearPlane, float farPlane)
    : Pos(pos), LookAt(lookAt), Up(up), fov(fov), nearPlane(nearPlane), farPlane(farPlane) {}