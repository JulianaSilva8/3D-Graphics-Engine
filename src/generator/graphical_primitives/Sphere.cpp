#include "../../../include/generator/graphical_primitives/Sphere.hpp"
#include"../../../include/utils/utils.hpp"
#include <math.h>

Sphere::Sphere(float r, int st, int sl)
    : radius(r), stacks(st), slices(sl) {}

IndexedVertices Sphere::generate() {
    IndexedVertices indexedVertices;

    if (stacks <= 0 || slices <= 0) return indexedVertices;

    float polar_delta = M_PI / stacks;
    float azimuth_delta = 2 * M_PI / slices;

    // Vértice do topo
    Vertex top;
    top.position = {0.0f, radius, 0.0f};
    top.normal = {0.0f, 1.0f, 0.0f};
    top.texCoord = {0.5f, 0.0f};
    indexedVertices.vertices.push_back(top);

    for (int i = 1; i < stacks; i++) {
        float polar = i * polar_delta;
        float y = radius * cos(polar);
        float radius_circle = radius * sin(polar);

        for (int j = 0; j <= slices; j++) {
            float azimuth = (M_PI / 2) + j * azimuth_delta;
            float x = radius_circle * cos(azimuth);
            float z = radius_circle * sin(azimuth);

            Vertex vertex;
            vertex.position = {x, y, z};
            vertex.normal = {x / radius, y / radius, z / radius};

            vertex.texCoord = {
                1.0f - ((float)j / slices),
                (float)i / stacks
            };

            indexedVertices.vertices.push_back(vertex);
        }
    }



    int columns = slices + 1;

    int S_idx = indexedVertices.vertices.size();

    // Vértice do fundo
    Vertex bottom;
    bottom.position = {0.0f, -radius, 0.0f};
    bottom.normal = {0.0f, -1.0f, 0.0f};
    bottom.texCoord = {0.5f, 1.0f};
    indexedVertices.vertices.push_back(bottom);

    // triangulos do topo
    for (int j = 0; j < slices; j++) {
        unsigned int topIndex = indexedVertices.vertices.size();

        Vertex topCopy;
        topCopy.position = {0.0f, radius, 0.0f};
        topCopy.normal = {0.0f, 1.0f, 0.0f};

        float u = 1.0f - (((float)j + 0.5f) / slices);
        topCopy.texCoord = {u, 0.0f};

        indexedVertices.vertices.push_back(topCopy);

        indexedVertices.indices.push_back(1 + j);
        indexedVertices.indices.push_back(topIndex);
        indexedVertices.indices.push_back(1 + j + 1);
    }

    // triangulos do meio
    for (int i = 1; i < stacks - 1; i++) {
        for (int j = 0; j < slices; j++) {
            // vértices do quadrado
            int supleft = 1 + (i - 1) * columns + j;
            int supright = 1 + (i - 1) * columns + j + 1;
            int infleft = 1 + i * columns + j;
            int infright = 1 + i * columns + j + 1;

            // triângulo 1
            indexedVertices.indices.push_back(infleft);
            indexedVertices.indices.push_back(supleft);

            indexedVertices.indices.push_back(supright);

            // triângulo 2
            indexedVertices.indices.push_back(infleft);
            indexedVertices.indices.push_back(supright);

            indexedVertices.indices.push_back(infright);
        }
    }

    // triangulos do fundo
    int lastStackStart = 1 + (stacks - 2) * columns;

    for (int j = 0; j < slices; j++) {
        indexedVertices.indices.push_back(S_idx);
        indexedVertices.indices.push_back(lastStackStart + j);
        indexedVertices.indices.push_back(lastStackStart + j + 1);
    }

    return indexedVertices;
}

