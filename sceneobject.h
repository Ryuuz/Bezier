#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector3D>

class Vertex;

class SceneObject : protected QOpenGLFunctions_4_1_Core
{
    public:
        SceneObject();
        ~SceneObject();

        Vertex* getVertices() const {return mVertices;}
        int getVertexNumber() const {return mNumberOfVertices;}
        GLuint getVao() const {return mVAO;}
        QVector3D getMaxVertex() const {return mMaxVertex;}
        QVector3D getMinVertex() const {return mMinVertex;}

        GLenum mDrawType;

    protected:
        void findMaxVertex();
        void findMinVertex();
        virtual void init() = 0;

        Vertex *mVertices;
        int mNumberOfVertices;
        GLuint mVBO;
        GLuint mVAO;
        QVector3D mMaxVertex;
        QVector3D mMinVertex;
};

#endif // SCENEOBJECT_H
