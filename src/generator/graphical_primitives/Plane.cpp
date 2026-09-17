#include "../../../include/generator/graphical_primitives/Plane.hpp"
#include"../../../include/utils/utils.hpp"

Plane::Plane(float length, int divisions) 
    : length(length), divisions(divisions) {}

IndexedVertices Plane::generate() {
    IndexedVertices indexedVertices;

    if (divisions <= 0) return indexedVertices;

    float half = length / 2.0f;
    float step = length / divisions;

    unsigned int base = 0;

    for (int i = 0; i <= divisions; i++) { 
        for (int j = 0; j <= divisions; j++) { 
            float x = -half + j * step;
            float z = -half + i * step;

            // Calcular coordenadas de textura normalizadas de 0 a 1
            float u = (float)j / divisions;
            float v = (float)i / divisions;

            Vertex vertex;
            vertex.position = {x, 0.0f, z};
            vertex.normal = {0.0f, 1.0f, 0.0f}; // Normal aponta para cima
            vertex.texCoord = {u, v};

            indexedVertices.vertices.push_back(vertex);

            if (i > 0 && j > 0) {
                unsigned int p1 = base + i * (divisions + 1) + (j - 1);
                unsigned int p2 = base + (i - 1) * (divisions + 1) + (j - 1);
                unsigned int p3 = base + (i - 1) * (divisions + 1) + j;
                unsigned int p4 = base + i * (divisions + 1) + j;

                indexedVertices.indices.push_back(p2);
                indexedVertices.indices.push_back(p4);
                indexedVertices.indices.push_back(p3);

                indexedVertices.indices.push_back(p2);
                indexedVertices.indices.push_back(p1);
                indexedVertices.indices.push_back(p4);
            }
        }
    }

    return indexedVertices;
}

