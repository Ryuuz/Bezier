#include "beziercurve.h"
#include "vertex.h"


BezierCurve::BezierCurve() : mStep(0.01f), mIndex(0)
{
    mNumberOfVertices = (1.f/mStep) + 1;
    mVertices = new Vertex[mNumberOfVertices];
}


BezierCurve::~BezierCurve()
{
    clearBuffers();
}


void BezierCurve::addPoint(const QVector3D &p)
{
    mPoints.push_back(p);
}


void BezierCurve::clearPoints()
{
    mPoints.clear();
}


void BezierCurve::setPoints(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4)
{
    clearPoints();
    mPoints.push_back(p1);
    mPoints.push_back(p2);
    mPoints.push_back(p3);
    mPoints.push_back(p4);
}


void BezierCurve::setPoints(const std::vector<QVector3D> &points)
{
    mPoints = points;
}


//Creates a bezier curve from the stored points (only up to 4 points are considered)
void BezierCurve::makeBezier()
{
    if(mPoints.size() > 1)
    {
        mDegree = (mPoints.size() >= 4) ? 3 : static_cast<int>(mPoints.size() - 1);

        for(float t = 0.f; t <= 1.f; t += mStep)
        {
            deCasteljau(mPoints, mDegree, t);
        }

        init();
    }
}


void BezierCurve::init()
{
    initializeOpenGLFunctions();

    clearBuffers();

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

    mDrawType = GL_POINTS;

    delete[] mVertices;
}


void BezierCurve::deCasteljau(std::vector<QVector3D> &points, int degree, float t)
{
    std::vector<QVector3D> temp;

    for(int i = 0; i < degree; i++)
    {
        //Divides the lines between the points at t
        temp.push_back(points.at(i+1)*(1-t) + points.at(i)*t);
    }

    //If this was the last line and we've found the final point, save it for drawing
    if(degree == 1)
    {
        mVertices[mIndex].setPosition(temp.at(0));
        mVertices[mIndex].setNormal(1.f, 1.f, 1.f);
        mVertices[mIndex].setUV(0.f, 0.f);

        mIndex++;
    }
    //Else continue dividing
    else
    {
        deCasteljau(temp, degree-1, t);
    }
}


void BezierCurve::clearBuffers()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}
