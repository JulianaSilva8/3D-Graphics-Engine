#include "../../../include/generator/graphical_primitives/Cylinder.hpp"
#include <cmath>
#include "../../../include/utils/utils.hpp"

Cylinder::Cylinder(float r, float h, int s)
	: radius(r), height(h), slices(s) {}

IndexedVertices Cylinder::generate() {
    IndexedVertices indexedVertices;

    if (slices <= 0) return indexedVertices;

    float delta_azimuth = 2.0f * M_PI / slices;

    // Lateral do cilindro
    unsigned int wall_start_idx = indexedVertices.vertices.size();

    for (int i = 0; i <= slices; i++) {
        float u = (float)i / slices;
        float azimuth = i * delta_azimuth;

        float x = radius * sinf(azimuth);
        float z = radius * cosf(azimuth);

        // O vetor normal da lateral aponta puramente para fora (horizontal)
        float nx = sinf(azimuth);
        float nz = cosf(azimuth);

        // Vértice Inferior da Lateral
        Vertex bottomWall;
        bottomWall.position = {x, 0.0f, z};
        bottomWall.normal = {nx, 0.0f, nz};
        bottomWall.texCoord = {u, 0.0f}; // V = 0.0 (Fundo)
        indexedVertices.vertices.push_back(bottomWall);

        // Vértice Superior da Lateral
        Vertex topWall;
        topWall.position = {x, height, z};
        topWall.normal = {nx, 0.0f, nz};
        topWall.texCoord = {u, 1.0f}; // V = 1.0 (Topo)
        indexedVertices.vertices.push_back(topWall);
    }

    // Índices da Lateral
    for (int i = 0; i < slices; i++) {
        unsigned int current_bottom = wall_start_idx + (2 * i);
        unsigned int current_top = current_bottom + 1;
        unsigned int next_bottom = wall_start_idx + (2 * (i + 1));
        unsigned int next_top = next_bottom + 1;

        // Triângulo 1
        indexedVertices.indices.push_back(current_bottom);
        indexedVertices.indices.push_back(next_bottom);
        indexedVertices.indices.push_back(current_top);

        // Triângulo 2
        indexedVertices.indices.push_back(next_bottom);
        indexedVertices.indices.push_back(next_top);
        indexedVertices.indices.push_back(current_top);
    }

    // base inferior
    unsigned int bottom_center_idx = indexedVertices.vertices.size();
    
    Vertex bottomCenter;
    bottomCenter.position = {0.0f, 0.0f, 0.0f};
    bottomCenter.normal = {0.0f, -1.0f, 0.0f}; // Aponta para baixo
    bottomCenter.texCoord = {0.5f, 0.5f};
    indexedVertices.vertices.push_back(bottomCenter);

    unsigned int bottom_rim_start_idx = indexedVertices.vertices.size();

    for (int i = 0; i <= slices; i++) {
        float azimuth = i * delta_azimuth;
        float x = radius * sinf(azimuth);
        float z = radius * cosf(azimuth);

        Vertex bVertex;
        bVertex.position = {x, 0.0f, z};
        bVertex.normal = {0.0f, -1.0f, 0.0f};
        bVertex.texCoord = {0.5f + 0.5f * sinf(azimuth), 0.5f + 0.5f * cosf(azimuth)};
        indexedVertices.vertices.push_back(bVertex);
    }

    // Índices da Tampa Inferior
    for (int i = 0; i < slices; i++) {
        indexedVertices.indices.push_back(bottom_center_idx);
        indexedVertices.indices.push_back(bottom_rim_start_idx + i + 1);
        indexedVertices.indices.push_back(bottom_rim_start_idx + i);
    }

    // base superior
    unsigned int top_center_idx = indexedVertices.vertices.size();

    Vertex topCenter;
    topCenter.position = {0.0f, height, 0.0f};
    topCenter.normal = {0.0f, 1.0f, 0.0f}; // Aponta para cima
    topCenter.texCoord = {0.5f, 0.5f};
    indexedVertices.vertices.push_back(topCenter);

    unsigned int top_rim_start_idx = indexedVertices.vertices.size();

    for (int i = 0; i <= slices; i++) {
        float azimuth = i * delta_azimuth;
        float x = radius * sinf(azimuth);
        float z = radius * cosf(azimuth);

        Vertex tVertex;
        tVertex.position = {x, height, z};
        tVertex.normal = {0.0f, 1.0f, 0.0f};
        // Mapeamento em disco circular independente da lateral
        tVertex.texCoord = {0.5f + 0.5f * sinf(azimuth), 0.5f + 0.5f * cosf(azimuth)};
        indexedVertices.vertices.push_back(tVertex);
    }

    // Índices da Tampa Superior (Sentido anti-horário olhando de cima)
    for (int i = 0; i < slices; i++) {
        indexedVertices.indices.push_back(top_center_idx);
        indexedVertices.indices.push_back(top_rim_start_idx + i);
        indexedVertices.indices.push_back(top_rim_start_idx + i + 1);
    }

    return indexedVertices;
}