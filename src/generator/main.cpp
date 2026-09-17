#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../../include/utils/utils.hpp"
#include "../../include/generator/graphical_primitives/Primitive.hpp"
#include "../../include/generator/graphical_primitives/Plane.hpp"
#include "../../include/generator/graphical_primitives/Box.hpp"
#include "../../include/generator/graphical_primitives/Sphere.hpp"
#include "../../include/generator/graphical_primitives/Cone.hpp"
#include "../../include/generator/graphical_primitives/Cylinder.hpp"
#include "../../include/generator/graphical_primitives/Torus.hpp"
#include "../../include/generator/patches.hpp"

// Função para guardar os pontos no ficheiro .3d
void saveToFile(std::string filename, const IndexedVertices& data) {
    std::ofstream file(filename); // Cria e abre o ficheiro para escrita
    if (file.is_open()) {
        file << data.vertices.size() << "\n"; // Escreve o número de vértices
        for (const auto& v : data.vertices) {
            file << v.position.x << " " << v.position.y << " " << v.position.z << " "
                 << v.normal.x << " " << v.normal.y << " " << v.normal.z << " "
                 << v.texCoord.u << " " << v.texCoord.v << "\n";
        }

        file << data.indices.size() << "\n"; // Escreve o número de índices
        for (size_t i = 0; i < data.indices.size(); i += 3) {
            file << data.indices[i] << " " << data.indices[i + 1] << " " << data.indices[i + 2] << "\n";
        }
        file.close();
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: generator <shape> <params> <file.3d>" << std::endl;
        return 1;
    }

    std::string shape = argv[1];
    IndexedVertices data;

    if (shape == "plane") {
        float length = std::stof(argv[2]);
        int divisions = std::stoi(argv[3]);
        Plane plane(length, divisions);
        data = plane.generate();
    } else if (shape == "box") {
        float length = std::stof(argv[2]);
        int divisions = std::stoi(argv[3]);

        Box box(length, divisions);
        data = box.generate();
    } else if (shape == "sphere") {
        float radius = std::stof(argv[2]);
        int stacks = std::stoi(argv[3]);
        int slices = std::stoi(argv[4]);
        Sphere sphere(radius, stacks, slices);
        data = sphere.generate();
    } else if (shape == "cone") {
        float radius = std::stof(argv[2]);
        float height = std::stof(argv[3]);
        int slices = std::stoi(argv[4]);
        int stacks = std::stoi(argv[5]);
        
        Cone cone(radius, height, slices, stacks);
        data = cone.generate();
    } else if (shape == "cylinder") {
        float radius = std::stof(argv[2]);
        float height = std::stof(argv[3]);
        int slices = std::stoi(argv[4]);
        
        Cylinder cylinder(radius, height, slices);
        data = cylinder.generate();
    } else if (shape == "torus") {
        float radius = std::stof(argv[2]);
        float ring_radius = std::stof(argv[3]);
        int stacks = std::stoi(argv[4]);
        int slices = std::stoi(argv[5]);
        
        Torus torus(radius, ring_radius, stacks, slices);
        data = torus.generate();
    } else if (shape == "patch") {
        if (argc != 5) {
            std::cout << "Usage: generator patch <patch_file> <tessellation_level> <output_file.3d>" << std::endl;
            return 1;
        }

        char* bezier_patch = argv[2];
        int tessellation_lvl = std::stoi(argv[3]);
        data = generate_patch(bezier_patch, tessellation_lvl);

        if (data.vertices.empty() || data.indices.empty()) {
            return 1;
        }
    } else {
        std::cout << "Unknown shape: " << shape << std::endl;
        return 1;
    }

    saveToFile(argv[argc - 1], data);
    return 0;
}