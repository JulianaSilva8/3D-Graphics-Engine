#include "../../include/engine/Group.hpp"
#include <cmath>

Group::Group() {
}

std::array<float, 16> Group::geTransformationtMatrix(float time) const {
    std::array<float, 16> res = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    for (const auto& t : transformations) {
        std::array<float, 16> matrix = t->getMatrix(time);
        res = multiplyMatrices(res, matrix);
    }

    return res;
}