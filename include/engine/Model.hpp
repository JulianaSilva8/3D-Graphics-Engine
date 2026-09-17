#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include "../utils/utils.hpp"
#include <GL/glew.h>

class Model {
public:
    std::string modelName;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    GLuint vboID = 0;
    GLuint iboID = 0;
    GLuint textureID = 0;

    unsigned int numVertices = 0;
    unsigned int numIndices = 0;

    Material material;
    std::string textureFile;
    bool hasTexture = false;
    bool textureLoaded = false;

    Model() = default;
    Model(const std::string& name, const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds);

    void setupModel();
    void drawModel() const;
    bool loadTexture();
};

#endif