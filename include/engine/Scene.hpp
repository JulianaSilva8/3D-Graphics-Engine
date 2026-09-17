#ifndef SCENE_HPP
#define SCENE_HPP
#include "Camera.hpp"
#include "Window.hpp"
#include "Group.hpp"
#include <memory>
#include "Light.hpp"

class Scene
{
public:
    Window window;
    Camera camera;
    Group group;
    std::vector<Light> lights;
    bool axis;
    Scene();
    Scene(bool axis, Window window, Camera camera, Group group, std::vector<Light> lights) : axis(axis), window(window), camera(camera), group(std::move(group)), lights(std::move(lights)) {}
    void initializeModels();
    void drawModels(float time);
    void initializeGroupCurves(Group &g);
    void initializeGroupModels(Group &g);
    void initializeLights();

    void drawGroupCurves(const Group &g) const;
    void drawGroup(const Group &g, float time);
    void drawLights() const;
};

#endif