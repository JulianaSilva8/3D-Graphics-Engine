#include "../../include/engine/Scene.hpp"
#include <GL/glew.h>
#include <GL/glut.h>

Scene::Scene() = default;

void Scene::initializeGroupModels(Group& g) { // criar VBOs em todos os grupos recursivamente
    // Inicializar modelos do grupo atual
    for (Model& model : g.models) {
        model.setupModel();
    }

    // Inicializar modelos dos subgrupos
    for (Group& subgroup : g.subgroups) {
        initializeGroupModels(subgroup);
    }
}

void Scene::initializeGroupCurves(Group& g) { // inicializar os VBOs das linhas das curvas
    for (auto& t : g.transformations) {
        if (auto* c = dynamic_cast<CurveTransformation*>(t.get())) {
            c->initializeCurveVBO();
        }
    }
    for (Group& subgroup : g.subgroups) {
        initializeGroupCurves(subgroup);
    }
}

// Função pública chamada pelo Engine
void Scene::initializeModels() {
    initializeGroupModels(this->group);
    initializeGroupCurves(this->group); 
}

void Scene::drawGroupCurves(const Group& g) const {
    for (const auto& t : g.transformations) {
        if (const auto* c = dynamic_cast<const CurveTransformation*>(t.get())) {
            c->drawCurveVBO();
        }
    }
}

void Scene::drawGroup(const Group& g, float time) {
    // guardar a matriz do nó pai
    glPushMatrix();

    drawGroupCurves(g); // curva do grupo fica por baixo dos modelos do grupo

    // aplicar a matriz de transformações deste grupo à matriz atual do OpenGL
    glMultMatrixf(g.geTransformationtMatrix(time).data());

    for (const Model& model : g.models) {
        model.drawModel();
    }

    // como estamos dentro do glPushMatrix(), estes subgrupos herdam as transformações deste grupo
    for (const Group& subgroup : g.subgroups) {
        drawGroup(subgroup, time);
    }

    // restaurar a matriz para não afetar o resto da cena
    glPopMatrix();
}

void Scene::drawModels(float time) {
    // Iniciar a recursividade a partir do grupo principal
    drawGroup(this->group, time);
}

void Scene::initializeLights()  {
    // cores
    float dark[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4] = {1.0, 1.0, 1.0, 1.0};

    // lightids
    for (size_t i = 0; i < lights.size() && i < 8; ++i) {
        GLenum lightID = GL_LIGHT0 + i;
        lights[i].setLightID(lightID);
        // light colors
        glLightfv(lightID, GL_AMBIENT, dark);
        glLightfv(lightID, GL_DIFFUSE, white);
        glLightfv(lightID, GL_SPECULAR, white);
    }
}

void Scene::drawLights() const {
    for (size_t i = 0; i < lights.size(); ++i) {
        lights[i].draw();
    }
}