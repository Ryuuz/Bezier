#include "camera.h"
#include <QMatrix4x4>
#include <QVector4D>


Camera::Camera()
{
    mViewMatrix = new QMatrix4x4;
    mPerspectiveMatrix = new QMatrix4x4;

    mViewMatrix->setToIdentity();
    mPerspectiveMatrix->setToIdentity();
}


Camera::~Camera()
{
    delete mViewMatrix;
    delete mPerspectiveMatrix;
}


void Camera::setViewMatrix(const QVector3D &pos, const QVector3D &focus)
{
    mViewMatrix->lookAt(pos, focus, QVector3D(0.f, 1.f, 0.f));
}


void Camera::setPerspectiveMatrix(int width, int height, float fieldOfView, float nearPlane, float farPlane)
{
    float aspectRatio = static_cast<float>(width)/static_cast<float>(height ? height : 1);
    mPerspectiveMatrix->perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
}
