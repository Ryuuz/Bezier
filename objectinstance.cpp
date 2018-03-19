#include "objectinstance.h"
#include "sceneobject.h"
#include "shader.h"
#include "camera.h"
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>
#include <QQuaternion>


ObjectInstance::ObjectInstance(SceneObject *a, Shader *s) : mModel(a), mShader(s)
{
    mModelMatrix = new QMatrix4x4;
    mModelMatrix->setToIdentity();
}


ObjectInstance::~ObjectInstance()
{
    delete mModelMatrix;
    mModel = nullptr;
    mShader = nullptr;
}


//Calculates the MVP matrix and draws the object using the given 'mDrawType'
void ObjectInstance::draw(GLuint m, Camera *camera)
{
    if(mModel)
    {
        initializeOpenGLFunctions();

        glUseProgram(mShader->getProgram());
        glBindVertexArray(mModel->getVao());

        QMatrix4x4 mvp = (*camera->getPerspectiveMatrix())*(*camera->getViewMatrix())*(*getModelMatrix());
        glUniformMatrix4fv(m, 1, GL_FALSE, mvp.constData());
        glDrawArrays(mModel->mDrawType, 0, mModel->getVertexNumber());
    }
}


//Performs all transformations on the model matrix and returns it
QMatrix4x4* ObjectInstance::getModelMatrix()
{
    return mModelMatrix;
}
