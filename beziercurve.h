#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "sceneobject.h"
#include <vector>
#include <QVector3D>

class BezierCurve : public SceneObject
{
    public:
        BezierCurve();
        ~BezierCurve();

        void addPoint(const QVector3D &p);
        void clearPoints();
        void setPoints(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4);
        void setPoints(const std::vector<QVector3D> &points);
        void makeBezier();

    private:
        void init() override;
        void deCasteljau(std::vector<QVector3D> &points, int degree, float t);
        void clearBuffers();

        std::vector<QVector3D> mPoints;
        int mDegree;
        float mStep;
        int mIndex;
};

#endif // BEZIERCURVE_H
