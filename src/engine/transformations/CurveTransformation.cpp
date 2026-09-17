#include "../../../include/engine/Transformation.hpp"
#include "../../../include/utils/utils.hpp"

CurveTransformation::CurveTransformation(CatmullRomCurve c, float time, bool a):
    curve(std::move(c)), time(time), align(a),
        matrix{ 1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f} {}

std::array<float, 16> CurveTransformation::getMatrix(float t) {
    // Get the position and derivative in the curve for current t value
    Point pos, deriv;
    float progress = t / time;
    curve.getGlobalCatmullRomPoint(progress, pos, deriv);

    std::array<float, 16> transMatrix = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        pos.x, pos.y, pos.z, 1
    };

    if (!align) {
        return transMatrix;
    }
    
	// 3. Build the rotation matrix to align the teapot with the curve
		// 3.1. Compute the axis
		float x[3] = {deriv.x, deriv.y, deriv.z};
		normalize(x);
		// z = xi*y(i-i) e y = zxi
		float z[3];
		float y[3] = {0,1,0};
		cross(x, y, z);
		normalize(z);
		cross(z, x, y);
		normalize(y);

    // final transformation matrix combining the rotation and the translation
    std::array<float, 16> res = {
        x[0], x[1], x[2], 0,
        y[0], y[1], y[2], 0,
        z[0], z[1], z[2], 0,
        pos.x, pos.y, pos.z, 1
    };

    return res;
}

void CurveTransformation::initializeCurveVBO() {
    std::vector<Point> vertices;

    for (float gt = 0; gt < 1; gt += 0.01) {
        Point pos, deriv;
        curve.getGlobalCatmullRomPoint(gt, pos, deriv);
        vertices.push_back(pos);
    }

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    numVertices = vertices.size();
}

void CurveTransformation::drawCurveVBO() const {
    if (vboID == 0 || numVertices == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glVertexPointer(3, GL_FLOAT, sizeof(Point), nullptr);
    glDrawArrays(GL_LINE_STRIP, 0, numVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}