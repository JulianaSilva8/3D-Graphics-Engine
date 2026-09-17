#include "../../include/engine/Engine.hpp"
#include "../../include/engine/Parser.hpp"
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>

void Engine::loadScene(const char *xmlPath)
{
    Parser parser;
    this->scene = parser.parse(xmlPath);
}

void Engine::resize(int w, int h)
{
    if (h == 0)
        h = 1;

    float ratio = w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(scene.camera.fov, ratio, scene.camera.nearPlane, scene.camera.farPlane);

    glMatrixMode(GL_MODELVIEW);
}

void Engine::render()
{
    glEnable(GL_CULL_FACE); // Ativar o culling
    glCullFace(GL_BACK);    // Culling das faces traseiras

    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CCW); // Definir a ordem dos vértices para as faces frontais (counter-clockwise)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(scene.camera.Pos.x, scene.camera.Pos.y, scene.camera.Pos.z,
              scene.camera.LookAt.x, scene.camera.LookAt.y, scene.camera.LookAt.z,
              scene.camera.Up.x, scene.camera.Up.y, scene.camera.Up.z);

    if (scene.axis)
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        
        glBegin(GL_LINES);
        // X axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);

        // Y Axis in Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);

        // Z Axis in Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);

        glEnable(GL_LIGHTING);
    }

    float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    scene.drawLights();
    scene.drawModels(t);

    glutSwapBuffers();
}

static Engine *g_engine = nullptr;

void changeSize(int w, int h)
{
    if (g_engine)
        g_engine->resize(w, h);
}

void renderScene()
{
    if (g_engine)
        g_engine->render();
}

void keyboardCallback(unsigned char key, int x, int y)
{
    if (g_engine)
        g_engine->keyboardInput(key, x, y);
}

void Engine::keyboardInput(unsigned char key, int x, int y)
{
    // vetor diração da câmara
    float dx = scene.camera.LookAt.x - scene.camera.Pos.x;
    float dy = scene.camera.LookAt.y - scene.camera.Pos.y;
    float dz = scene.camera.LookAt.z - scene.camera.Pos.z;
    float len = sqrtf(dx * dx + dy * dy + dz * dz);
    dx /= len;
    dy /= len;
    dz /= len;

    // vetor right da câmara
    float rx = dy * scene.camera.Up.z - dz * scene.camera.Up.y;
    float ry = dz * scene.camera.Up.x - dx * scene.camera.Up.z;
    float rz = dx * scene.camera.Up.y - dy * scene.camera.Up.x;

    const float k = 0.5f;
    const float z = 0.5f;
    const float alpha = 0.05f; // radianos

    switch (key)
    {
    // move a câmara para a frente
    case 'w':
        scene.camera.Pos.x += dx * k;
        scene.camera.Pos.y += dy * k;
        scene.camera.Pos.z += dz * k;
        scene.camera.LookAt.x += dx * k;
        scene.camera.LookAt.y += dy * k;
        scene.camera.LookAt.z += dz * k;
        break;

    // move a câmara para trás
    case 's':
        scene.camera.Pos.x -= dx * k;
        scene.camera.Pos.y -= dy * k;
        scene.camera.Pos.z -= dz * k;
        scene.camera.LookAt.x -= dx * k;
        scene.camera.LookAt.y -= dy * k;
        scene.camera.LookAt.z -= dz * k;
        break;

    // move a câmara para a esquerda
    case 'a':
        scene.camera.Pos.x -= rx * k;
        scene.camera.Pos.z -= rz * k;
        scene.camera.LookAt.x -= rx * k;
        scene.camera.LookAt.z -= rz * k;
        break;

    // move a câmara para a direita
    case 'd':
        scene.camera.Pos.x += rx * k;
        scene.camera.Pos.z += rz * k;
        scene.camera.LookAt.x += rx * k;
        scene.camera.LookAt.z += rz * k;
        break;

    // roda a câmara no sentido anti-horário em torno do eixo Y
    case 'q':
    {
        float ox = scene.camera.Pos.x - scene.camera.LookAt.x;
        float oz = scene.camera.Pos.z - scene.camera.LookAt.z;
        scene.camera.Pos.x = scene.camera.LookAt.x + ox * cosf(alpha) - oz * sinf(alpha);
        scene.camera.Pos.z = scene.camera.LookAt.z + ox * sinf(alpha) + oz * cosf(alpha);
        break;
    }

    // roda a câmara no sentido horário em torno do eixo Y
    case 'e':
    {
        float ox = scene.camera.Pos.x - scene.camera.LookAt.x;
        float oz = scene.camera.Pos.z - scene.camera.LookAt.z;
        scene.camera.Pos.x = scene.camera.LookAt.x + ox * cosf(-alpha) - oz * sinf(-alpha);
        scene.camera.Pos.z = scene.camera.LookAt.z + ox * sinf(-alpha) + oz * cosf(-alpha);
        break;
    }

    // aumenta o zoom
    case 'x':
        scene.camera.Pos.x += dx * z;
        scene.camera.Pos.y += dy * z;
        scene.camera.Pos.z += dz * z;
        break;

    // diminui o zoom
    case 'z':
        scene.camera.Pos.x -= dx * z;
        scene.camera.Pos.y -= dy * z;
        scene.camera.Pos.z -= dz * z;
        break;

    default:
        break;
    }

    glutPostRedisplay();
}

void Engine::run(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(scene.window.width, scene.window.height);
    glutCreateWindow("Engine");

    glewInit();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);        // normais (para iluminação)
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); // coordenadas de textura

    // settings de iluminação
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_NORMALIZE);

    float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

    scene.initializeLights();
    scene.initializeModels();

    // LIGAR o engine global aos callbacks
    g_engine = this;

    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);

    glutKeyboardFunc(keyboardCallback);

    glutMainLoop();
}
