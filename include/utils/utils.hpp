#ifndef UTILS_HPP
#define UTILS_HPP

#include <array>
#include <vector>

typedef struct Point {
    float x, y, z;

    Point(float x_val = 0.0f, float y_val = 0.0f, float z_val = 0.0f)
    : x(x_val), y(y_val), z(z_val) {}
} Point;

typedef struct Point2D {
    float u, v;

    Point2D(float u_val = 0.0f, float v_val = 0.0f)
    : u(u_val), v(v_val) {}
} Point2D;

typedef struct Vertex {
    Point position;
    Point normal;
    Point2D texCoord;

        Vertex(Point p = {}, Point n = {}, Point2D t = {})
        : position(p), normal(n), texCoord(t) {}
} Vertex;

typedef struct Material {
    float diffuse[4]  = {200.0f/255, 200.0f/255, 200.0f/255, 1.0f};
    float ambient[4]  = {50.0f/255, 50.0f/255, 50.0f/255, 1.0f};
    float specular[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float emissive[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float shininess = 0.0f;
} Material;

typedef struct IndexedVertices {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
} IndexedVertices;

std::array<float, 16> multiplyMatrices(const std::array<float, 16>& m1, const std::array<float, 16>& m2);

void cross(float *a, float *b, float *res);

void normalize(float *a);


#endif