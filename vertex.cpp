#include "vertex.h"


Vertex::Vertex()
{

}


Vertex::Vertex(QVector3D pos, QVector3D norm, QVector2D uv) : mPosition(pos), mNormal(norm), mUV(uv)
{

}


//Sets the 3D position the vertex will hold
void Vertex::setPosition(const QVector3D &pos)
{
    mPosition = pos;
}


void Vertex::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    mPosition.setX(x);
    mPosition.setY(y);
    mPosition.setZ(z);
}


//Sets the color the vertex should have
void Vertex::setNormal(const QVector3D &norm)
{
    mNormal = norm;
}


void Vertex::setNormal(GLfloat x, GLfloat y, GLfloat z)
{
    mNormal.setX(x);
    mNormal.setY(y);
    mNormal.setZ(z);
}


void Vertex::setUV(const QVector2D &uv)
{
    mUV = uv;
}


void Vertex::setUV(GLfloat u, GLfloat v)
{
    mUV.setX(u);
    mUV.setY(v);
}
