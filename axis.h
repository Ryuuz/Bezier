#ifndef AXIS_H
#define AXIS_H

#include "sceneobject.h"

class QVector3D;

class Axis : public SceneObject
{
    public:
        Axis();
        ~Axis();

    private:
        void init() override;
};

#endif // AXIS_H
