#include "../../include/engine/Model.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Model::Model(const std::string &name, const std::vector<Vertex> &verts, const std::vector<unsigned int> &inds)
    : modelName(name), vertices(verts), indices(inds), numVertices(verts.size()), numIndices(inds.size()) {}

void Model::setupModel() {
    if (vertices.empty())
        return;

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * vertices.size(),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (!indices.empty()) {
        glGenBuffers(1, &iboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned int) * indices.size(),
                     indices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    if (hasTexture && !textureFile.empty()) {
        textureLoaded = loadTexture();
    }
}

bool Model::loadTexture() {
    int width, height, channels;
    unsigned char* data = stbi_load(
        textureFile.c_str(),
        &width,
        &height,
        &channels,
        STBI_rgb_alpha
    );

    if (!data) {
        std::fprintf(stderr, "Erro ao carregar textura: %s\n", textureFile.c_str());
        textureID = 0;
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return true;
}

void Model::drawModel() const {
    if (vboID == 0 || numVertices == 0)
        return;

    glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, material.emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

    if (hasTexture && textureLoaded && textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    if (iboID != 0 && numIndices > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    if (hasTexture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}