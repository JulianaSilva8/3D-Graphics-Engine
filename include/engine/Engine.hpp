#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "Scene.hpp"

class Engine {
    Scene scene;
public:
        Engine() : scene{} {};

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    void loadScene(const char* xmlPath);
    void run(int argc, char** argv);

    void resize(int w, int h);
    void render();

    void keyboardInput(unsigned char key, int x, int y);
};

#endif