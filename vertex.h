#ifndef VERTEX_H
#define VERTEX_H

#include <QtGui/qopengl.h>
#include <QVector3D>
#include <QVector2D>

class Vertex
{
    public:
        Vertex();
        Vertex(QVector3D pos, QVector3D norm, QVector2D uv);

        void setPosition(const QVector3D &pos);
        void setPosition(GLfloat x, GLfloat y, GLfloat z);
        void setNormal(const QVector3D &norm);
        void setNormal(GLfloat x, GLfloat y, GLfloat z);
        void setUV(const QVector2D &uv);
        void setUV(GLfloat u, GLfloat v);

        const QVector3D& getPosition() const {return mPosition;}
        const QVector3D& getNormal() const {return mNormal;}
        const QVector2D& getUV() const {return mUV;}

    private:
        QVector3D mPosition;
        QVector3D mNormal;
        QVector2D mUV;
};

#endif // VERTEX_H
