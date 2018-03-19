#include "axis.h"
#include "vertex.h"
#include <QVector3D>
#include <QVector2D>


Axis::Axis()
{
    mNumberOfVertices = 6;

    //The vertices that will be used to draw the object
    mVertices = new Vertex[mNumberOfVertices]
    {   //Position                     //Color                   //UV
        {QVector3D{-500.f, 0.f, 0.f},  QVector3D{1.f, 0.f, 0.f}, QVector2D{0.f, 0.f}},
        {QVector3D{500.f, 0.f, 0.f},   QVector3D{1.f, 0.f, 0.f}, QVector2D{0.f, 0.f}},
        {QVector3D{0.f, -500.f, 0.f},  QVector3D{0.f, 1.f, 0.f}, QVector2D{0.f, 0.f}},
        {QVector3D{0.f, 500.f, 0.f},   QVector3D{0.f, 1.f, 0.f}, QVector2D{0.f, 0.f}},
        {QVector3D{0.f, 0.f, -500.f},  QVector3D{0.f, 0.f, 1.f}, QVector2D{0.f, 0.f}},
        {QVector3D{0.f, 0.f, 500.f},   QVector3D{0.f, 0.f, 1.f}, QVector2D{0.f, 0.f}}
    };

    findMaxVertex();
    findMinVertex();

    init();
}


Axis::~Axis()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}


//Initiate everything needed for rendering
void Axis::init()
{
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Sets up and fills the VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mNumberOfVertices*sizeof(Vertex), mVertices, GL_STATIC_DRAW);


    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6*sizeof(GLfloat)));

    mDrawType = GL_LINES;

    delete[] mVertices;
}
