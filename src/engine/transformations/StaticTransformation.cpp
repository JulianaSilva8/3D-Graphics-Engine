#include "../../../include/engine/Transformation.hpp"

// Transformação estática
StaticTransformation::StaticTransformation() {
    matrix = {1.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 1.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 1.0f}; }

void StaticTransformation::translate(float x, float y, float z) {
    // Matriz de Translação em Column-Major (x, y, z ficam no final)
    std::array<float, 16> tMatrix = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1};
    matrix = multiplyMatrices(matrix, tMatrix);
}

void StaticTransformation::scale(float x, float y, float z) {
    std::array<float, 16> sMatrix = {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1};
    matrix = multiplyMatrices(matrix, sMatrix);
}

void StaticTransformation::rotate(float angle, float x, float y, float z) {
    float rad = angle * (M_PI / 180.0f);
    float c = std::cos(rad);
    float s = std::sin(rad);
    float t = 1.0f - c;

    // Normalizar o eixo de rotação
    float length = std::sqrt(x * x + y * y + z * z);
    if (length > 0.0f){
        x /= length;
        y /= length;
        z /= length;
    }

    // Matriz de Rotação em Column-Major (transposta em relação à Row-Major)
    std::array<float, 16> rMatrix = {
        t*x*x + c,   t*x*y + z*s, t*x*z - y*s, 0,
        t*x*y - z*s, t*y*y + c,   t*y*z + x*s, 0,
        t*x*z + y*s, t*y*z - x*s, t*z*z + c,   0,
        0,           0,           0,           1
    };
    
    matrix = multiplyMatrices(matrix, rMatrix);
}


std::array<float, 16> StaticTransformation::getMatrix(float time) {
    return matrix;
}
