#include "../../../include/engine/Transformation.hpp"

RotationTimeTransformation::RotationTimeTransformation(float time, float x, float y, float z): 
    time(time), x(x), y(y), z(z),
        matrix{ 1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f } {}

// Transformação de rotação dependente do tempo
std::array<float, 16> RotationTimeTransformation::getMatrix(float elapsed) {
    // Calculate rotation angle based on progress
    float progress = elapsed / time;
    float angle = progress * 360.0f;
    
    float rad = angle*(M_PI / 180.0f);
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
    std::array<float, 16> res = {
        t*x*x + c,   t*x*y + z*s, t*x*z - y*s, 0,
        t*x*y - z*s, t*y*y + c,   t*y*z + x*s, 0,
        t*x*z + y*s, t*y*z - x*s, t*z*z + c,   0,
        0,           0,           0,           1
    };

    return res;
}
