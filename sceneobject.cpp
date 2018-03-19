#include "sceneobject.h"
#include "vertex.h"
#include <QVector3D>


SceneObject::SceneObject()
{
    mVertices = nullptr;
}


SceneObject::~SceneObject()
{

}


//Finds and stores the vertex with the highest value
void SceneObject::findMaxVertex()
{
    if(mVertices)
    {
        QVector3D max = mVertices[0].getPosition();

        for(int i = 1; i < mNumberOfVertices; i++)
        {
            if((mVertices[i].getPosition().x() + mVertices[i].getPosition().y() + mVertices[i].getPosition().z()) > (max.x() + max.y() + max.z()))
            {
                max = mVertices[i].getPosition();
            }
        }

        mMaxVertex = max;
    }
}


//Finds and stores the vertex with the smallest value
void SceneObject::findMinVertex()
{
    if(mVertices)
    {
        QVector3D min = mVertices[0].getPosition();

        for(int i = 1; i < mNumberOfVertices; i++)
        {
            if((mVertices[i].getPosition().x() + mVertices[i].getPosition().y() + mVertices[i].getPosition().z()) < (min.x() + min.y() + min.z()))
            {
                min = mVertices[i].getPosition();
            }
        }

        mMinVertex = min;
    }
}
