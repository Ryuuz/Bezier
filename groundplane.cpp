#include "groundplane.h"
#include "vertex.h"
#include "camera.h"
#include "shader.h"
#include "beziercurve.h"
#include "constants.h"
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <cmath>
#include <iostream>
#include <fstream>


//Reads in data for plane
GroundPlane::GroundPlane(Shader *s) : mShader(s)
{
    readFromFile();
    generateBuffers();
}


//Creates plane from user input
GroundPlane::GroundPlane(QVector3D max, QVector3D min, int divisions, Shader *s) : mMax(max), mMin(min), mDivisions(divisions), mShader(s)
{
    mNumberOfVertices = mDivisions*mDivisions;
    mNumberOfIndices = ((mDivisions-1)*(mDivisions-1))*2*3;
    mVertices = new Vertex[mNumberOfVertices];
    mIndices = new GLushort[mNumberOfIndices];

    dividePlane();
    positionPoints();
}


GroundPlane::~GroundPlane()
{
    mShader = nullptr;
    delete[] mVertices;
    delete[] mIndices;
    delete[] mNeighbors;

    glDeleteBuffers(1, &mEBO);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}


void GroundPlane::writeToFile()
{
    std::ofstream out;
    out.open(gsl::assetFilePath + "planePoints.dat");

    out << mNumberOfVertices << std::endl;
    for(int i = 0; i < mNumberOfVertices; i++)
    {
        out << mVertices[i].getPosition().x() << " " << mVertices[i].getPosition().y() << " " << mVertices[i].getPosition().z() << " ";
        out << mVertices[i].getNormal().x() << " " << mVertices[i].getNormal().y() << " " << mVertices[i].getNormal().z() << std::endl;
    }

    out << mNumberOfIndices << std::endl;
    for(int i = 0; i < mNumberOfIndices; i++)
    {
        out << mIndices[i];

        if((i+1)%3)
        {
            out << " ";
        }
        else
        {
            out << std::endl;
        }
    }

    out.close();
}


//True to find the point 'point' in plane
int GroundPlane::findPoint(const QVector3D &point, int start, std::vector<int> *triangles)
{
    bool triangleVisited[mNumberOfIndices/3]{false};
    int current = (start >= 0 && start < (mNumberOfIndices/3)) ? start : 0;
    bool found = false;
    QVector3D bary(-1.f, -1.f, -1.f);

    //Searches until point is found or it starts backtracking
    while(!found && !triangleVisited[current])
    {
        //Get the barycentric coordinates of the point in relation to the triangle 'current'
        bary = barycentricCoordinates(mVertices[mIndices[current*3]].getPosition(), mVertices[mIndices[(current*3)+1]].getPosition(), mVertices[mIndices[(current*3)+2]].getPosition(), point);
        triangleVisited[current] = true;

        if(triangles)
        {
            triangles->push_back(current);
        }

        //If point is outside current triangle
        if(bary.x() < 0.f || bary.y() < 0.f || bary.z() < 0.f)
        {
            float min;
            int index;

            //Find the first point with a neighbor
            if(mNeighbors[current*3] != -1)
            {
                min = bary.x();
                index = 0;
            }
            else if(mNeighbors[(current*3)+1] != -1)
            {
                min = bary.y();
                index = 1;
            }
            else
            {
                min = bary.z();
                index = 2;
            }

            //Check if there's another point with an neighbor where the barycentric coordinate is smaller
            for(int i = 0; i < 3; i++)
            {
                if(bary[i] < min && mNeighbors[(current*3)+i] != -1)
                {
                    min = bary[i];
                    index = i;
                }
            }

            //Next triangle to visit
            current = mNeighbors[(current*3)+index];
        }
        else
        {
            found = true;
        }
    }

    if(found)
    {
        return current;
    }

    return -1;
}


BezierCurve* GroundPlane::pointToPoint(const QVector3D &p1, const QVector3D &p2)
{
    int startTriangle = findPoint(p1);
    BezierCurve *bezier = nullptr;

    if(startTriangle != -1)
    {
        std::vector<int> *triangles = new std::vector<int>;
        findPoint(p2, startTriangle, triangles);

        bezier = new BezierCurve;

        for(auto element : *triangles)
        {
            bezier->addPoint(triangleCenter(element));
        }

        bezier->makeBezier();
    }

    return bezier;
}


//Draw the plane to screen
void GroundPlane::draw(GLuint m, Camera *camera)
{
    initializeOpenGLFunctions();

    glUseProgram(mShader->getProgram());
    glBindVertexArray(mVAO);

    QMatrix4x4 mvp = (*camera->getPerspectiveMatrix())*(*camera->getViewMatrix());
    glUniformMatrix4fv(m, 1, GL_FALSE, mvp.constData());
    glDrawElements(mDrawType, mNumberOfIndices*sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0);
}


//Divide the plane into 'mDivisions' points in each direction
void GroundPlane::dividePlane()
{
    int index = 0;
    float xLength = (QVector2D(mMax.x(), mMin.z()) - QVector2D(mMin.x(), mMin.z())).length();
    float zLength = (QVector2D(mMin.x(), mMax.z()) - QVector2D(mMin.x(), mMin.z())).length();

    //How far between the points
    float xStep = xLength/(mDivisions-1);
    float zStep = zLength/(mDivisions-1);

    for(int i = 0; i < mDivisions; i++)
    {
        for(int j = 0; j < mDivisions; j++)
        {
            mVertices[index++].setPosition(mMin.x() + (xStep*j), 0.f, mMin.z() + (zStep*i));
        }
    }

    assignIndices();
}


//Positions the points on the y-axis
void GroundPlane::positionPoints()
{
    for(int i = 0; i < mNumberOfVertices; i++)
    {
        mVertices[i].setPosition(calculateY(mVertices[i].getPosition()));
        mVertices[i].setNormal(calculateNormal(mVertices[i].getPosition()));
    }

    generateBuffers();
}


//Creates indices to draw triangles
void GroundPlane::assignIndices()
{
    int index = 0;

    for(int i = 0; i < (mDivisions-1); i++)
    {
        for(int j = 0; j < (mDivisions-1); j++)
        {
            mIndices[index++] = j + (i*mDivisions);
            mIndices[index++] = (j + (i*mDivisions)) + mDivisions;
            mIndices[index++] = (j + (i*mDivisions)) + 1;

            mIndices[index++] = (j + (i*mDivisions)) + 1;
            mIndices[index++] = (j + (i*mDivisions)) + mDivisions;
            mIndices[index++] = (j + (i*mDivisions)) + (mDivisions+1);
        }
    }
}


//Calculates the y coordinate of the point
const QVector3D GroundPlane::calculateY(const QVector3D &pos)
{
    float y = 0.f;

    if(pointInPlane(pos))
    {
        y = (3 * std::pow(pos.x(), 2) * std::pow(pos.z(), 2)) - std::pow(pos.x(), 2) - std::pow(pos.z(), 2);//std::sin(pos.x()) + std::cos(pos.z()) + 4*pos.x();
    }

    return QVector3D(pos.x(), y, pos.z());
}


//Finds the normal of the point
const QVector3D GroundPlane::calculateNormal(const QVector3D &point)
{
    float y1 = 0.f;
    float y2 = 0.f;

    if(pointInPlane(point))
    {
        y1 = (6 * point.x() * std::pow(point.z(), 2)) - (2 * point.x());//-std::sin(point.z());
        y2 = (6 * std::pow(point.x(), 2) * point.z()) - (2 * point.z());//std::cos(point.x()) + 4;

        QVector3D norm = QVector3D::crossProduct(QVector3D(0.f, y2, 1.f), QVector3D(1.f, y1, 0.f));
        norm.normalize();

        return norm;
    }

    return QVector3D(0.f, 0.f, 0.f);
}


//Returns true if the given point is in the plane
bool GroundPlane::pointInPlane(const QVector3D &point)
{
    if((point.x() <= mMax.x() && point.x() >= mMin.x()) && (point.z() <= mMax.z() && point.z() >= mMin.z()))
    {
        return true;
    }

    return false;
}


//https://www.mathopenref.com/coordcentroid.html
const QVector3D GroundPlane::triangleCenter(int triangle)
{
    QVector3D center;

    center.setX((mVertices[mIndices[triangle*3]].getPosition().x() + mVertices[mIndices[(triangle*3)+1]].getPosition().x() + mVertices[mIndices[(triangle*3)+2]].getPosition().x())/3);
    center.setY((mVertices[mIndices[triangle*3]].getPosition().y() + mVertices[mIndices[(triangle*3)+1]].getPosition().y() + mVertices[mIndices[(triangle*3)+2]].getPosition().y())/3);
    center.setZ((mVertices[mIndices[triangle*3]].getPosition().z() + mVertices[mIndices[(triangle*3)+1]].getPosition().z() + mVertices[mIndices[(triangle*3)+2]].getPosition().z())/3);

    return center;
}


//Finds the barycentric coordinates to 'point' in relation to the triangle pqr
const QVector3D GroundPlane::barycentricCoordinates(const QVector3D &p, const QVector3D &q, const QVector3D &r, const QVector3D &point)
{
    QVector3D temp;

    QVector3D pq = q-p;
    pq.setY(0.f);
    QVector3D pr = r-p;
    pr.setY(0.f);

    QVector3D norm = QVector3D::crossProduct(pq, pr);
    float area = norm.length();

    QVector3D pointP = p-point;
    pointP.setY(0.f);
    QVector3D pointQ = q-point;
    pointQ.setY(0.f);
    QVector3D pointR = r-point;
    pointR.setY(0.f);

    temp.setX(QVector3D::crossProduct(pointQ, pointR).y()/area);
    temp.setY(QVector3D::crossProduct(pointR, pointP).y()/area);
    temp.setZ(QVector3D::crossProduct(pointP, pointQ).y()/area);

    return temp;
}


void GroundPlane::generateBuffers()
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

    //Element/indices buffer
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumberOfIndices*sizeof(GLushort), mIndices, GL_STATIC_DRAW);

    mDrawType = GL_TRIANGLES;
}


void GroundPlane::readFromFile()
{
    std::ifstream in;
    in.open(gsl::assetFilePath + "planeData.dat");

    GLfloat x, y, z;

    in >> mNumberOfVertices;
    mVertices = new Vertex[mNumberOfVertices];

    for(int i = 0; i < mNumberOfVertices; i++)
    {
        in >> x >> y >> z;
        mVertices[i].setPosition(x, y, z);

        in >> x >> y >> z;
        mVertices[i].setNormal(x, y, z);
    }

    in >> mNumberOfIndices;
    mIndices = new GLushort[mNumberOfIndices];
    mNeighbors = new GLshort[mNumberOfIndices];

    for(int i = 0; i < mNumberOfIndices; i += 3)
    {
        in >> mIndices[i] >> mIndices[i+1] >> mIndices[i+2] >> mNeighbors[i] >> mNeighbors[i+1] >> mNeighbors[i+2];
    }

    in.close();
}
