#ifndef OBJECTINSTANCE_H
#define OBJECTINSTANCE_H

#include <QOpenGLFunctions_4_1_Core>

class SceneObject;
class QVector3D;
class QMatrix4x4;
class Shader;
class Camera;


class ObjectInstance : protected QOpenGLFunctions_4_1_Core
{
    public:
        ObjectInstance(SceneObject *a = nullptr, Shader *s = nullptr);
        ~ObjectInstance();
        void draw(GLuint m, Camera *camera);

        void setShader(Shader *s) {mShader = s;}

        Shader* getShader() const {return mShader;}
        QMatrix4x4* getModelMatrix();

    protected:
        SceneObject *mModel;
        QMatrix4x4 *mModelMatrix;
        Shader *mShader;
};

#endif // OBJECTINSTANCE_H
