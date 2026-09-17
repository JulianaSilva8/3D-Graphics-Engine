#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include "../../include/generator/patches.hpp"
#include "../../include/utils/utils.hpp"

float bernstein(int i, float t) {
    switch (i) {
    case 0:
        return (1 - t) * (1 - t) * (1 - t);
    case 1:
        return 3 * t * (1 - t) * (1 - t);
    case 2:
        return 3 * t * t * (1 - t);
    case 3:
        return t * t * t;
    default:
        return 0.0f;
    }
}

float bernstein_derivative(int i, float t) {
    switch (i) {
    case 0:
        return -3.0f * (1.0f - t) * (1.0f - t);

    case 1:
        return 3.0f * (1.0f - t) * (1.0f - t)
             - 6.0f * t * (1.0f - t);

    case 2:
        return 6.0f * t * (1.0f - t)
             - 3.0f * t * t;

    case 3:
        return 3.0f * t * t;

    default:
        return 0.0f;
    }
}

Point cross_product(const Point& a, const Point& b) {
    return Point(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

Point normalize_point(const Point& p) {
    float length = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);

    if (length == 0.0f) {
        return Point(0.0f, 1.0f, 0.0f);
    }

    return Point(
        p.x / length,
        p.y / length,
        p.z / length
    );
}

Point get_puv(float u, float v, const std::vector<Point> patch_control_pts) {
    Point p(0, 0, 0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            p.x += patch_control_pts[i * 4 + j].x * bernstein(i, u) * bernstein(j, v);
            p.y += patch_control_pts[i * 4 + j].y * bernstein(i, u) * bernstein(j, v);
            p.z += patch_control_pts[i * 4 + j].z * bernstein(i, u) * bernstein(j, v);
        }
    }
    return p;
}

Point get_du(float u, float v, const std::vector<Point>& patch_control_pts) {
    Point du(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float factor = bernstein_derivative(i, u) * bernstein(j, v);

            du.x += patch_control_pts[i * 4 + j].x * factor;
            du.y += patch_control_pts[i * 4 + j].y * factor;
            du.z += patch_control_pts[i * 4 + j].z * factor;
        }
    }

    return du;
}

Point get_dv(float u, float v, const std::vector<Point>& patch_control_pts) {
    Point dv(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float factor = bernstein(i, u) * bernstein_derivative(j, v);

            dv.x += patch_control_pts[i * 4 + j].x * factor;
            dv.y += patch_control_pts[i * 4 + j].y * factor;
            dv.z += patch_control_pts[i * 4 + j].z * factor;
        }
    }

    return dv;
}

std::vector<std::vector<Point>> get_tesselation_grid(int tessellation_lvl, const std::vector<Point>& patch_control_pts) {
    float delta_tess = 1.0f / tessellation_lvl;
    
    std::vector<std::vector<Point>> grid(
        tessellation_lvl + 1,
        std::vector<Point>(tessellation_lvl + 1));

    for (int u = 0; u <= tessellation_lvl; u++) {
        for (int v = 0; v <= tessellation_lvl; v++) {
            grid[u][v] = get_puv(u * delta_tess, v * delta_tess, patch_control_pts);
        }
    }

    return grid;
}

void append_patch_indices(const std::vector<std::vector<Point>>& tess_grid, std::vector<unsigned int>& indices, unsigned int vertex_offset) {
    size_t side = tess_grid.size();

    for (size_t u = 1; u < side; u++) {
        for (size_t v = 1; v < side; v++) {
            unsigned int topLeft = vertex_offset + static_cast<unsigned int>((u - 1) * side + (v - 1));
            unsigned int topRight = vertex_offset + static_cast<unsigned int>((u - 1) * side + v);
            unsigned int bottomLeft = vertex_offset + static_cast<unsigned int>(u * side + (v - 1));
            unsigned int bottomRight = vertex_offset + static_cast<unsigned int>(u * side + v);

            indices.push_back(bottomLeft);
            indices.push_back(topLeft);
            indices.push_back(bottomRight);

            indices.push_back(bottomRight);
            indices.push_back(topLeft);
            indices.push_back(topRight);
        }
    }
}

Point get_normal(float u, float v, const std::vector<Point>& patch_control_pts) {
    Point du = get_du(u, v, patch_control_pts);
    Point dv = get_dv(u, v, patch_control_pts);

    Point normal = cross_product(dv, du);

    return normalize_point(normal);
}

IndexedVertices generate_patch(char *bezier_patch, int tessellation_lvl) {
    std::ifstream file(bezier_patch);
    IndexedVertices indexedVertices;

    if (!file.is_open()) {
        std::cerr << "Erro a abrir ficheiro: " << bezier_patch << std::endl;
        return indexedVertices;
    }

    if (tessellation_lvl <= 0) {
        std::cerr << "Tessellation level invalido.\n";
        return indexedVertices;
    }

    int nr_patches = 0;
    file >> nr_patches;

    std::vector<std::vector<int>> indices(nr_patches, std::vector<int>(16));

    for (int i = 0; i < nr_patches; i++) {
        for (int j = 0; j < 16; j++) {
            file >> indices[i][j];
            if (j < 15)
                file.ignore(1, ',');
        }
    }
    int nr_control_pts = 0;
    file >> nr_control_pts;

    std::vector<Point> control_pts;
    for (int i = 0; i < nr_control_pts; i++) {
        Point p;
        file >> p.x;
        file.ignore(1, ',');
        file >> p.y;
        file.ignore(1, ',');
        file >> p.z;
        control_pts.push_back(p);
    }

    for (int patch = 0; patch < nr_patches; patch++) {
        std::vector<Point> patch_control_pts;
        for (int j = 0; j < 16; j++) {
            patch_control_pts.push_back(control_pts[indices[patch][j]]);
        }
        std::vector<std::vector<Point>> tess_grid = get_tesselation_grid(tessellation_lvl, patch_control_pts);
        unsigned int vertex_offset = static_cast<unsigned int>(indexedVertices.vertices.size());

        for (size_t u = 0; u < tess_grid.size(); u++) {
            for (size_t v = 0; v < tess_grid[u].size(); v++) {
                Vertex vertex;

                float fu = static_cast<float>(u) / tessellation_lvl;
                float fv = static_cast<float>(v) / tessellation_lvl;

                vertex.position = tess_grid[u][v];

                vertex.normal = get_normal(fu, fv, patch_control_pts);

                vertex.texCoord = {fu, fv};

                indexedVertices.vertices.push_back(vertex);
            }
        }

        append_patch_indices(tess_grid, indexedVertices.indices, vertex_offset);
    }

    return indexedVertices;
}
