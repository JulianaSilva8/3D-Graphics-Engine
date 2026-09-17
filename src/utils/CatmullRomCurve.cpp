#include "../../include/utils/CatmullRomCurve.hpp"
#include <cstdio>

CatmullRomCurve::CatmullRomCurve(std::vector<Point>& points) 
    : controlPoints(points), pointCount(points.size()) {
    if (pointCount < 4) {
        printf("Curva Catmull-Rom requere pelo menos 4 pontos de controlo. -  %d\n", pointCount);
    }
}

void CatmullRomCurve::multMatrixVector(float m[4][4], float *v, float *res) {
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j][k];
        }
    }
}

void CatmullRomCurve::getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3,
                                         Point &pos, Point &deriv) {
    // Catmull-Rom matrix
    float m[4][4] = {
        {-0.5f,  1.5f, -1.5f,  0.5f},
        { 1.0f, -2.5f,  2.0f, -0.5f},
        {-0.5f,  0.0f,  0.5f,  0.0f},
        { 0.0f,  1.0f,  0.0f,  0.0f}
    };

    // For each component (x, y, z)
    for (int i = 0; i < 3; ++i) {
        // Get the correct component for each point
        float points[4];
        points[0] = (i == 0) ? p0.x : (i == 1) ? p0.y : p0.z;
        points[1] = (i == 0) ? p1.x : (i == 1) ? p1.y : p1.z;
        points[2] = (i == 0) ? p2.x : (i == 1) ? p2.y : p2.z;
        points[3] = (i == 0) ? p3.x : (i == 1) ? p3.y : p3.z;

        // Compute vector A = M * P
        float a[4];
        multMatrixVector(m, points, a);

        // Compute pos = T * A
        float tVector[4] = {t * t * t, t * t, t, 1.0f};
        float posComponent = a[0] * tVector[0] + a[1] * tVector[1] + 
                             a[2] * tVector[2] + a[3] * tVector[3];

        // Compute deriv = T' * A
        float dVector[4] = {3 * t * t, 2 * t, 1, 0};
        float derivComponent = a[0] * dVector[0] + a[1] * dVector[1] + 
                               a[2] * dVector[2] + a[3] * dVector[3];

        // Assign to result
        if (i == 0) {
            pos.x = posComponent;
            deriv.x = derivComponent;
        } else if (i == 1) {
            pos.y = posComponent;
            deriv.y = derivComponent;
        } else {
            pos.z = posComponent;
            deriv.z = derivComponent;
        }
    }
}

void CatmullRomCurve::getGlobalCatmullRomPoint(float gt, Point &pos, Point &deriv) {
    gt = gt - std::floor(gt);

    float t = gt * pointCount;
    int index = (int)std::floor(t); 
    t = t - index; 

    int indices[4];
    indices[0] = (index + pointCount - 1) % pointCount;
    indices[1] = (indices[0] + 1) % pointCount;
    indices[2] = (indices[1] + 1) % pointCount;
    indices[3] = (indices[2] + 1) % pointCount;

    getCatmullRomPoint(t, controlPoints[indices[0]], controlPoints[indices[1]],
                       controlPoints[indices[2]], controlPoints[indices[3]], pos, deriv);
}
