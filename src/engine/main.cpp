#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

#include "../../include/engine/Engine.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: engine <input_file>" << std::endl;
        return 1;
    }
    Engine engine;
    const char* path = argv[1];
    engine.loadScene(path);
    engine.run(argc, argv);

    return 0;
}