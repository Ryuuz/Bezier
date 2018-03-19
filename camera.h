#ifndef CAMERA_H
#define CAMERA_H

#include <QtGui/qopengl.h>
#include <QVector3D>

class QMatrix4x4;

class Camera
{
    public:
        Camera();
        ~Camera();

        void setViewMatrix(const QVector3D &pos, const QVector3D &focus);
        void setPerspectiveMatrix(int width, int height, float fieldOfView = 45.f, float nearPlane = 0.1f, float farPlane = 1000.f);


        QMatrix4x4* getViewMatrix() const {return mViewMatrix;}
        QMatrix4x4* getPerspectiveMatrix() const {return mPerspectiveMatrix;}

    private:
        QMatrix4x4 *mViewMatrix;
        QMatrix4x4 *mPerspectiveMatrix;
};

#endif // CAMERA_H
