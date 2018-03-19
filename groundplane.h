#ifndef GROUNDPLANE_H
#define GROUNDPLANE_H

#include <QVector3D>
#include <QOpenGLFunctions_4_1_Core>
#include <vector>

class Vertex;
class Camera;
class Shader;
class BezierCurve;

class GroundPlane : protected QOpenGLFunctions_4_1_Core
{
    public:
        GroundPlane(Shader *s);
        GroundPlane(QVector3D max, QVector3D min, int divisions, Shader *s);
        ~GroundPlane();

        void writeToFile();
        void draw(GLuint m, Camera *camera);
        int findPoint(const QVector3D &point, int start = 0, std::vector<int> *triangles = nullptr);
        BezierCurve* pointToPoint(const QVector3D &p1, const QVector3D &p2);

    private:
        void dividePlane();
        void positionPoints();
        void assignIndices();
        const QVector3D calculateY(const QVector3D &pos);
        const QVector3D calculateNormal(const QVector3D &point);
        bool pointInPlane(const QVector3D &point);
        const QVector3D triangleCenter(int triangle);
        const QVector3D barycentricCoordinates(const QVector3D &p, const QVector3D &q, const QVector3D &r, const QVector3D &point);
        void generateBuffers();
        void readFromFile();

        QVector3D mMax;
        QVector3D mMin;
        int mDivisions;

        Vertex *mVertices;
        GLushort *mIndices;
        GLshort *mNeighbors;

        int mNumberOfVertices;
        int mNumberOfIndices;

        GLuint mVBO;
        GLuint mVAO;
        GLuint mEBO;

        GLenum mDrawType;
        Shader *mShader;
};

#endif // GROUNDPLANE_H
