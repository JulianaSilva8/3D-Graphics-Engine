#include "../../../include/generator/graphical_primitives/Torus.hpp"
#include <math.h>
#include "../../../include/utils/utils.hpp"

Torus::Torus(float r, float r_ring, int st, int sl)
    : radius(r), ring_radius(r_ring), stacks(st), slices(sl) {}

IndexedVertices Torus::generate() {
    IndexedVertices indexedVertices;

    if (stacks <= 0 || slices <= 0) return indexedVertices;

    float beta_delta = 2 * M_PI / stacks;
    float alpha_delta = 2 * M_PI / slices;

    for(int i = 0; i < slices; i++) {
        float alpha = i*alpha_delta;
        for(int j = 0; j < stacks; j++) {
            float beta = j*beta_delta;

            float x = (-radius + ring_radius*cos(beta)) * cos(alpha);
            float y = ring_radius * sin(beta);
            float z = (-radius + ring_radius*cos(beta)) * sin(alpha);
            
            // Normal do torus
            float nx = cos(beta) * cos(alpha);
            float ny = sin(beta);
            float nz = cos(beta) * sin(alpha);
            
            Vertex vertex;
            vertex.position = {x, y, z};
            vertex.normal = {nx, ny, nz};
            vertex.texCoord = {(float)i / slices, (float)j / stacks};
            
            indexedVertices.vertices.push_back(vertex);
        }
    }

    for(int i = 0; i < slices; i++) {
        for(int j = 0; j < stacks; j++) {
            int current = i*stacks + j;
            int next_stack = (j+1)%stacks;
            int next_slice = ((i+1)%slices)*stacks + j;
            int next_slice_next_stack = ((i+1)%slices) * stacks + next_stack;

            indexedVertices.indices.push_back(current);
            indexedVertices.indices.push_back(next_slice);
            indexedVertices.indices.push_back(next_stack + i*stacks);

            indexedVertices.indices.push_back(next_stack + i*stacks);
            indexedVertices.indices.push_back(next_slice);
            indexedVertices.indices.push_back(next_slice_next_stack);
        }
    }

    return indexedVertices;
}