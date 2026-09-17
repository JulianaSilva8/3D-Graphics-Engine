#include "../../include/engine/Light.hpp"
#include <cmath>

void Light::setLightID(GLenum lightID) {
    this->lightID = lightID;
    glEnable(this->lightID);
}

void Light::draw() const {
    switch (this->type) {
        case Type::Point: {
            GLfloat position[] = { this->position.x,  this->position.y,  this->position.z, 1.0f };
            glLightfv(this->lightID, GL_POSITION, position);
            break;
        }
        case Type::Directional: {
            GLfloat direction[] = {  this->direction.x,  this->direction.y,  this->direction.z, 0.0f };
            glLightfv(this->lightID, GL_POSITION, direction);
            break;
        }
        case Type::Spot: {
            GLfloat position[] = {  this->position.x,  this->position.y,  this->position.z, 1.0f };
            GLfloat direction[] = {  this->direction.x,  this->direction.y,  this->direction.z };
            glLightfv(this->lightID, GL_POSITION, position);
            glLightfv(this->lightID, GL_SPOT_DIRECTION, direction);
            glLightf(this->lightID, GL_SPOT_CUTOFF,  this->cutoff);
            break;
        }
    }
}
