#ifndef GROUP_HPP
#define GROUP_HPP

#include <vector>
#include <string>
#include <array>
#include "../utils/utils.hpp"
#include "Model.hpp"
#include "Transformation.hpp"

class Group {
public:
    // models
    std::vector<Model> models;
    std::vector<std::unique_ptr<Transformation>> transformations; // apontador porque não se podem instanciar classes abstratas
    std::vector<Group> subgroups = {};

    Group();
    std::array<float, 16> geTransformationtMatrix(float time) const;
};

#endif