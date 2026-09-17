#include "../../include/engine/Parser.hpp"
#include "../../include/rapidxml/rapidxml.hpp"
#include "../../include/rapidxml/rapidxml_utils.hpp"

#include <fstream>
#include <iostream>

Model readModelFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening model file: " << filename << std::endl;
        return Model();
    }

    size_t numVertices;
    file >> numVertices;

    std::vector<Vertex> vertices;
    vertices.reserve(numVertices);

    for (size_t i = 0; i < numVertices; ++i) {
        float x, y, z, nx, ny, nz, u, v;
        file >> x >> y >> z >> nx >> ny >> nz >> u >> v;

        vertices.push_back(Vertex(
            Point(x, y, z),
            Point(nx, ny, nz),
            Point2D(u, v)
        ));
    }

    size_t numIndices;
    file >> numIndices;

    std::vector<unsigned int> indices;
    indices.reserve(numIndices);

    for (size_t i = 0; i < numIndices; ++i) {
        unsigned int index;
        file >> index;
        indices.push_back(index);
    }

    return Model(filename, vertices, indices);
}

Group parseGroup(rapidxml::xml_node<>* groupNode) {
    Group group;

    rapidxml::xml_node<>* transformNode = groupNode->first_node("transform");
    if (transformNode) {
        // Percorrer os filhos do <transform> (translate, rotate, scale)
        for (rapidxml::xml_node<>* opNode = transformNode->first_node(); opNode; opNode = opNode->next_sibling()) {
            std::string type = opNode->name();
            
            // Transformação dependente do tempo
            if(opNode->first_attribute("time")) {
                float time = std::stof(opNode->first_attribute("time")->value());
                bool align = false;
                if (type == "translate") {
                    if (opNode->first_attribute("align")) {
                        align = std::string(opNode->first_attribute("align")->value()) == "true";
                    }
                    std::vector<Point> controlPoints;
                    for (rapidxml::xml_node<>* pointNode = opNode->first_node("point"); pointNode; pointNode = pointNode->next_sibling("point")) {
                        float x = std::stof(pointNode->first_attribute("x")->value());
                        float y = std::stof(pointNode->first_attribute("y")->value());
                        float z = std::stof(pointNode->first_attribute("z")->value());
                        controlPoints.push_back({x, y, z});
                    }
                    CatmullRomCurve curve(controlPoints);
                    group.transformations.push_back(std::make_unique<CurveTransformation>(curve, time, align));
                } else if (type == "rotate") {
                    float x = std::stof(opNode->first_attribute("x")->value());
                    float y = std::stof(opNode->first_attribute("y")->value());
                    float z = std::stof(opNode->first_attribute("z")->value());
                    group.transformations.push_back(std::make_unique<RotationTimeTransformation>(time, x, y, z));
                }
            }

            // Transformação estática
            else { 
                StaticTransformation* t = nullptr;
                if(group.transformations.size() == 0 || !group.transformations.back()->isStatic()) {
                    group.transformations.push_back(std::make_unique<StaticTransformation>());
                }
                t = static_cast<StaticTransformation*>(group.transformations.back().get());

                if (type == "translate") {
                    float x = std::stof(opNode->first_attribute("x")->value());
                    float y = std::stof(opNode->first_attribute("y")->value());
                    float z = std::stof(opNode->first_attribute("z")->value());
                    t->translate(x, y, z);
                } else if (type == "rotate") {
                    float angle = std::stof(opNode->first_attribute("angle")->value());
                    float x = std::stof(opNode->first_attribute("x")->value());
                    float y = std::stof(opNode->first_attribute("y")->value());
                    float z = std::stof(opNode->first_attribute("z")->value());
                    t->rotate(angle, x, y, z);
                } else if (type == "scale") {
                    float x = std::stof(opNode->first_attribute("x")->value());
                    float y = std::stof(opNode->first_attribute("y")->value());
                    float z = std::stof(opNode->first_attribute("z")->value());
                    t->scale(x, y, z);
                }
            }
        }
    }
    // Parse models
    rapidxml::xml_node<>* modelsNode = groupNode->first_node("models");
    if (modelsNode) { 
        for (rapidxml::xml_node<>* modelNode = modelsNode->first_node("model"); modelNode; modelNode = modelNode->next_sibling("model")) {
            std::string modelFile = modelNode->first_attribute("file")->value();

            Model model = readModelFile(modelFile);
            std::string textureFile;
            if (modelNode->first_node("texture")) {
                textureFile = modelNode->first_node("texture")->first_attribute("file")->value();
                model.textureFile = textureFile;
                model.hasTexture = true;
            }
            Material material;
            if (modelNode->first_node("color")) {
                rapidxml::xml_node<>* colorNode = modelNode->first_node("color");
                if(colorNode->first_node("diffuse")) {
                    rapidxml::xml_node<>* diffuseNode = colorNode->first_node("diffuse");
                    material.diffuse[0] = std::stof(diffuseNode->first_attribute("R")->value()) / 255.0f;
                    material.diffuse[1] = std::stof(diffuseNode->first_attribute("G")->value()) / 255.0f;
                    material.diffuse[2] = std::stof(diffuseNode->first_attribute("B")->value()) / 255.0f;
                    material.diffuse[3] = 1.0f;
                }
                if(colorNode->first_node("ambient")) {
                    rapidxml::xml_node<>* ambientNode = colorNode->first_node("ambient");
                    material.ambient[0] = std::stof(ambientNode->first_attribute("R")->value()) / 255.0f;
                    material.ambient[1] = std::stof(ambientNode->first_attribute("G")->value()) / 255.0f;
                    material.ambient[2] = std::stof(ambientNode->first_attribute("B")->value()) / 255.0f;
                    material.ambient[3] = 1.0f;
                }
                if(colorNode->first_node("specular")) {
                    rapidxml::xml_node<>* specularNode = colorNode->first_node("specular");
                    material.specular[0] = std::stof(specularNode->first_attribute("R")->value()) / 255.0f;
                    material.specular[1] = std::stof(specularNode->first_attribute("G")->value()) / 255.0f;
                    material.specular[2] = std::stof(specularNode->first_attribute("B")->value()) / 255.0f;
                    material.specular[3] = 1.0f;
                }
                if(colorNode->first_node("emissive")) {
                    rapidxml::xml_node<>* emissiveNode = colorNode->first_node("emissive");
                    material.emissive[0] = std::stof(emissiveNode->first_attribute("R")->value()) / 255.0f;
                    material.emissive[1] = std::stof(emissiveNode->first_attribute("G")->value()) / 255.0f;
                    material.emissive[2] = std::stof(emissiveNode->first_attribute("B")->value()) / 255.0f;
                    material.emissive[3] = 1.0f;
                }
                if(colorNode->first_node("shininess")) {
                    rapidxml::xml_node<>* shininessNode = colorNode->first_node("shininess");
                    material.shininess = std::stof(shininessNode->first_attribute("value")->value());
                }
            }
            model.material = material;
            group.models.push_back(model);
        }
    }

    // Parse Subgrupos
    for (rapidxml::xml_node<>* childGroupNode = groupNode->first_node("group"); childGroupNode; childGroupNode = childGroupNode->next_sibling("group")) {
        Group childGroup = parseGroup(childGroupNode);
        group.subgroups.push_back(std::move(childGroup));
    }

    return group;
}

Scene Parser::parse(const std::string& filename) {
    std::cout << "Parsing XML file: " << filename << std::endl;
    // abrir file no modo leitura
    std::ifstream file(filename);

    // ver se o file abriu corretamente
    if(!file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        exit(1);
    }
    printf("File opened successfully!\n");
    // ler o file XML como string
    std::string xmlContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

    file.close();

    // Parse da string XML
    printf("Parsing XML content...\n");
    rapidxml::xml_document<> doc;
    doc.parse<0>(&xmlContent[0]);
    
    // aceder o nó raiz do XML
    rapidxml::xml_node<>* root = doc.first_node("world");

    bool axis = false;
    rapidxml::xml_node<>* axisNode = root->first_node("axis");

    if(axisNode) {
        axis = std::string(axisNode->first_attribute("show")->value()) == "true";
    }

    // Window
    rapidxml::xml_node<>* windowNode = root->first_node("window");

    int width = std::stoi(windowNode->first_attribute("width")->value());
    int height = std::stoi(windowNode->first_attribute("height")->value());
    
    Window window(width, height);

    // Camera
    rapidxml::xml_node<>* cameraNode = root->first_node("camera");

    rapidxml::xml_node<>* positionNode = cameraNode->first_node("position");
    Point position(
        std::stof(positionNode->first_attribute("x")->value()),
        std::stof(positionNode->first_attribute("y")->value()),
        std::stof(positionNode->first_attribute("z")->value())
    );

    rapidxml::xml_node<>* lookAtNode = cameraNode->first_node("lookAt");
    Point lookAt(
        std::stof(lookAtNode->first_attribute("x")->value()),
        std::stof(lookAtNode->first_attribute("y")->value()),
        std::stof(lookAtNode->first_attribute("z")->value())
    );

    rapidxml::xml_node<>* upNode = cameraNode->first_node("up");
    Point up(
        std::stof(upNode->first_attribute("x")->value()),
        std::stof(upNode->first_attribute("y")->value()),
        std::stof(upNode->first_attribute("z")->value())
    );

    rapidxml::xml_node<>* projectionNode = cameraNode->first_node("projection");

    float fov = std::stof(projectionNode->first_attribute("fov")->value());
    float nearPlane = std::stof(projectionNode->first_attribute("near")->value());
    float farPlane = std::stof(projectionNode->first_attribute("far")->value());

    Camera camera(position, lookAt, up, fov, nearPlane, farPlane);
    printf("Camera parsed successfully!\n");
    // lights
//     > <lights>
// <light type="point" posX="0" posY="10" posZ="0" />
// <light type="directional" dirX="1" dirY="1" dirZ="1"/>
// <light type="spotlight" posX="0" posY="10" posZ="0"
// dirX="1" dirY="1" dirZ="1"
// cutoff="45" />
// </lights>
    std::vector<Light> lights;
    if(root->first_node("lights")) {
        for (rapidxml::xml_node<>* lightNode = root->first_node("lights")->first_node("light"); lightNode; lightNode = lightNode->next_sibling("light")) {
            std::string typeStr = lightNode->first_attribute("type")->value();
            Type type;
            if (typeStr == "point") {
                type = Type::Point;
            } else if (typeStr == "directional") {
                type = Type::Directional;
            } else if (typeStr == "spot") {
                type = Type::Spot;
            } else {
                continue;
            }

            Point position, direction;
            float cutoff = 180.0f;

            if (type == Type::Point || type == Type::Spot) {
                position.x = std::stof(lightNode->first_attribute("posx")->value());
                position.y = std::stof(lightNode->first_attribute("posy")->value());
                position.z = std::stof(lightNode->first_attribute("posz")->value());
            }

            if (type == Type::Directional || type == Type::Spot) {
                direction.x = std::stof(lightNode->first_attribute("dirx")->value());
                direction.y = std::stof(lightNode->first_attribute("diry")->value());
                direction.z = std::stof(lightNode->first_attribute("dirz")->value());
            }

            if (type == Type::Spot && lightNode->first_attribute("cutoff")) {
                cutoff = std::stof(lightNode->first_attribute("cutoff")->value());
            }

            Light light(type, position, direction, cutoff);
            lights.push_back(light);
        }
    }
    printf("Lights parsed successfully!\n");

   // groups
    Group group = parseGroup(root->first_node("group"));

    return Scene(axis, window, camera, std::move(group), std::move(lights));
}