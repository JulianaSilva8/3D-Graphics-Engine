#include "../../../include/generator/graphical_primitives/Box.hpp"
#include <iostream>
#include "../../../include/utils/utils.hpp"

Box::Box(float length, int divisions)
    : length(length), divisions(divisions) {}

IndexedVertices Box::generate()
{
    IndexedVertices indexedVertices;

    if (divisions <= 0)
        return indexedVertices;

    float half = length / 2.0f;
    float step = length / divisions;

    // face da frente
    unsigned int base = indexedVertices.vertices.size();
    for (int i = 0; i <= divisions; i++)
    {
        for (int j = 0; j <= divisions; j++)
        {
            float x = -half + j * step;
            float y = -half + i * step;
            float u = (float)j / divisions;
            float v = (float)i / divisions;

            Vertex vertex;
            vertex.position = {x, y, half};
            vertex.normal = {0.0f, 0.0f, 1.0f};
            vertex.texCoord = {u, v};
            indexedVertices.vertices.push_back(vertex);

            if (i > 0 && j > 0)
            {
                unsigned int p1 = base + i * (divisions + 1) + (j - 1);
                unsigned int p2 = base + (i - 1) * (divisions + 1) + (j - 1);
                unsigned int p3 = base + (i - 1) * (divisions + 1) + j;
                unsigned int p4 = base + i * (divisions + 1) + j;

                indexedVertices.indices.push_back(p2);
                indexedVertices.indices.push_back(p3);
                indexedVertices.indices.push_back(p4);

                indexedVertices.indices.push_back(p2);
                indexedVertices.indices.push_back(p4);
                indexedVertices.indices.push_back(p1);
            }
        }
    }

    // face de tras
    base = indexedVertices.vertices.size();

    for (int i = 0; i <= divisions; i++)
    {
        for (int j = 0; j <= divisions; j++)
        {
            float x = -half + j * step;
            float y = -half + i * step;
            float u = (float)j / divisions;
            float v = (float)i / divisions;

            Vertex vertex;
            vertex.position = {x, y, -half};
            vertex.normal = {0.0f, 0.0f, -1.0f};
            vertex.texCoord = {u, v};
            indexedVertices.vertices.push_back(vertex);

            if (i > 0 && j > 0)
            {
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

    // face de cima
    base = indexedVertices.vertices.size();

    for (int i = 0; i <= divisions; i++)
    {
        for (int j = 0; j <= divisions; j++)
        {
            float x = -half + j * step;
            float z = -half + i * step;
            float u = (float)j / divisions;
            float v = (float)i / divisions;

            Vertex vertex;
            vertex.position = {x, half, z};
            vertex.normal = {0.0f, 1.0f, 0.0f};
            vertex.texCoord = {u, v};
            indexedVertices.vertices.push_back(vertex);

            if (i > 0 && j > 0)
            {
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
    // face de baixo
    base = indexedVertices.vertices.size();

    for (int i = 0; i <= divisions; i++)
    {
        for (int j = 0; j <= divisions; j++)
        {
            float x = -half + j * step;
            float z = -half + i * step;
            float u = (float)j / divisions;
            float v = (float)i / divisions;

            Vertex vertex;
            vertex.position = {x, -half, z};
            vertex.normal = {0.0f, -1.0f, 0.0f};
            vertex.texCoord = {u, v};
            indexedVertices.vertices.push_back(vertex);

            if (i > 0 && j > 0)
            {
                unsigned int p1 = base + i * (divisions + 1) + (j - 1);
                unsigned int p2 = base + (i - 1) * (divisions + 1) + (j - 1);
                unsigned int p3 = base + (i - 1) * (divisions + 1) + j;
                unsigned int p4 = base + i * (divisions + 1) + j;

                indexedVertices.indices.push_back(p2);
                indexedVertices.indices.push_back(p3);
                indexedVertices.indices.push_back(p4);

                indexedVertices.indices.push_back(p2);
                indexedVertices.indices.push_back(p4);
                indexedVertices.indices.push_back(p1);
            }
        }
    }

    // face direita
    base = indexedVertices.vertices.size();

    for (int i = 0; i <= divisions; i++)
    {
        for (int j = 0; j <= divisions; j++)
        {
            float y = -half + j * step;
            float z = -half + i * step;
            float u = (float)i / divisions;
            float v = (float)j / divisions;

            Vertex vertex;
            vertex.position = {half, y, z};
            vertex.normal = {1.0f, 0.0f, 0.0f};
            vertex.texCoord = {u, v};
            indexedVertices.vertices.push_back(vertex);

            if (i > 0 && j > 0)
            {
                unsigned int p1 = base + i * (divisions + 1) + (j - 1);
                unsigned int p2 = base + (i - 1) * (divisions + 1) + (j - 1);
                unsigned int p3 = base + (i - 1) * (divisions + 1) + j;
                unsigned int p4 = base + i * (divisions + 1) + j;

                indexedVertices.indices.push_back(p2);
                indexedVertices.indices.push_back(p3);
                indexedVertices.indices.push_back(p4);

                indexedVertices.indices.push_back(p2);
                indexedVertices.indices.push_back(p4);
                indexedVertices.indices.push_back(p1);
            }
        }
    }

    // face esquerda
    base = indexedVertices.vertices.size();
    for (int i = 0; i <= divisions; i++)
    {
        for (int j = 0; j <= divisions; j++)
        {
            float y = -half + j * step;
            float z = -half + i * step;
            float u = (float)i / divisions;
            float v = (float)j / divisions;

            Vertex vertex;
            vertex.position = {-half, y, z};
            vertex.normal = {-1.0f, 0.0f, 0.0f};
            vertex.texCoord = {u, v};
            indexedVertices.vertices.push_back(vertex);

            if (i > 0 && j > 0)
            {
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