#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include "../utils/utils.hpp"
#include "../utils/CatmullRomCurve.hpp"
#include <GL/glew.h>
#include <array>
#include <memory>

class Transformation
{
public:
    virtual ~Transformation() = default;
    virtual std::array<float, 16> getMatrix(float time) = 0;
    virtual bool isStatic() const { return true; }
};


// Transformação estática (translate, rotate, scale)
class StaticTransformation : public Transformation {
private:
    std::array<float, 16> matrix;

public:
    StaticTransformation();
    explicit StaticTransformation(const std::array<float, 16> &m) : matrix(m) {}
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    std::array<float, 16> getMatrix(float time) override;

};

// Transformação de curva (translação ao longo da curva)
class CurveTransformation : public Transformation{
private:
    CatmullRomCurve curve;
    float time;
    bool align;
    std::array<float, 16> matrix;
    
    GLuint vboID = 0;
    unsigned int numVertices = 0;
public:
    CurveTransformation(CatmullRomCurve c, float time, bool alignWithCurve);
    std::array<float, 16> getMatrix(float t) override;
    bool isStatic() const override { return false; }
    void initializeCurveVBO();
    void drawCurveVBO() const;
};

// Transformação de rotação dependente do tempo
class RotationTimeTransformation : public Transformation {
private:
    float time;
    float x, y, z;
    std::array<float, 16> matrix;

public:
    RotationTimeTransformation(float time, float x, float y, float z);
    std::array<float, 16> getMatrix(float t) override;
    bool isStatic() const override { return false; }
};

#endif