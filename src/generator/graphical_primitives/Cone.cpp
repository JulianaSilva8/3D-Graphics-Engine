#include "../../../include/generator/graphical_primitives/Cone.hpp"
#include <cmath>
#include "../../../include/utils/utils.hpp"

Cone::Cone(float radius, float height, int slices, int stacks) 
    : radius(radius), height(height), slices(slices), stacks(stacks) {}

IndexedVertices Cone::generate() {
    IndexedVertices indexedVertices;

    if (slices <= 0 || stacks <= 0) return indexedVertices;

    float alpha = (2.0f * M_PI) / slices;
    float stack_height = height / stacks;

    float normalY = radius / sqrtf(radius * radius + height * height);
    float normalXZ = height / sqrtf(radius * radius + height * height);

    // Lateral do cone
    for (int i = 0; i <= stacks; i++) {
        float t = (float)i / stacks;

        float y = t * height;
        float r = radius * (1.0f - t);

        for (int j = 0; j <= slices; j++) {
            float u = (float)j / slices;
            float angle = j * alpha;

            float x = r * sinf(angle);
            float z = r * cosf(angle);

            Vertex vertex;
            vertex.position = {x, y, z};
            vertex.normal = {
                normalXZ * sinf(angle),
                normalY,
                normalXZ * cosf(angle)
            };
            vertex.texCoord = {u, 1.0f - t};

            indexedVertices.vertices.push_back(vertex);
        }
    }

    int columns = slices + 1;

    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            int current = i * columns + j;
            int next = current + 1;
            int upper = (i + 1) * columns + j;
            int upperNext = upper + 1;

            indexedVertices.indices.push_back(current);
            indexedVertices.indices.push_back(next);
            indexedVertices.indices.push_back(upper);

            indexedVertices.indices.push_back(next);
            indexedVertices.indices.push_back(upperNext);
            indexedVertices.indices.push_back(upper);
        }
    }

    // Base separada, com UV circular
    unsigned int baseCenterIndex = indexedVertices.vertices.size();

    Vertex baseCenter;
    baseCenter.position = {0.0f, 0.0f, 0.0f};
    baseCenter.normal = {0.0f, -1.0f, 0.0f};
    baseCenter.texCoord = {0.5f, 0.5f};
    indexedVertices.vertices.push_back(baseCenter);

    unsigned int baseStartIndex = indexedVertices.vertices.size();

    for (int j = 0; j <= slices; j++) {
        float angle = j * alpha;

        float x = radius * sinf(angle);
        float z = radius * cosf(angle);

        Vertex vertex;
        vertex.position = {x, 0.0f, z};
        vertex.normal = {0.0f, -1.0f, 0.0f};
        vertex.texCoord = {
            0.5f + 0.5f * sinf(angle),
            0.5f - 0.5f * cosf(angle)
        };

        indexedVertices.vertices.push_back(vertex);
    }

    for (int j = 0; j < slices; j++) {
        indexedVertices.indices.push_back(baseCenterIndex);
        indexedVertices.indices.push_back(baseStartIndex + j + 1);
        indexedVertices.indices.push_back(baseStartIndex + j);
    }

    return indexedVertices;
}