#include <array>
#include <cmath>
// Função para multiplicar matrizes 4x4
std::array<float, 16> multiplyMatrices(const std::array<float, 16>& m1, const std::array<float, 16>& m2) {
    std::array<float, 16> result = {0};
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += m1[k * 4 + row] * m2[col * 4 + k];
            }
            result[col * 4 + row] = sum;
        }
    }
    return result;
}

void cross(float *a, float *b, float *res) {
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {
    float l = std::sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
    if (l > 0.0f) {
        a[0] = a[0]/l;
        a[1] = a[1]/l;
        a[2] = a[2]/l;
    }
}
