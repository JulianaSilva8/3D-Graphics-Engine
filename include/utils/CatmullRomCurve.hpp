#ifndef CATMULLROMCURVE_HPP
#define CATMULLROMCURVE_HPP

#include "utils.hpp"
#include <vector>
#include <cmath>

class CatmullRomCurve
{
private:
    std::vector<Point> controlPoints;
    int pointCount;

    void multMatrixVector(float m[4][4], float *v, float *res);
    void getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3,
                            Point &pos, Point &deriv);

public:
    CatmullRomCurve(std::vector<Point> &points);
    void getGlobalCatmullRomPoint(float gt, Point &pos, Point &deriv);
    int getPointCount() { return pointCount; }
    std::vector<Point> &getControlPoints() { return controlPoints; }
};

#endif
