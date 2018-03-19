#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QBasicTimer>
#include <QTime>
#include <vector>

class QOpenGLContext;
class MainWindow;
class Shader;
class Axis;
class SceneObject;
class Camera;
class ObjectInstance;
class BezierCurve;
class GroundPlane;


class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT

    public:
        RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
        ~RenderWindow();
        QOpenGLContext *context() {return mContext;}
        void exposeEvent(QExposeEvent*) override;

    signals:
        void ready();
        void error(const QString &msg);

    private slots:
        void render();

    private:
        void init();
        void startOpenGLDebugger();

        QOpenGLContext *mContext;
        bool mInitialized;

        Shader *mShaderProgram;
        GLuint mMatrixUniform;

        Camera *mCamera;
        Axis *mAxis;
        BezierCurve *mBezier;
        GroundPlane *mGround;
        std::vector<ObjectInstance*> mStaticObjects;

        QBasicTimer mTimer;     //timer that drives the gameloop
        QTime mTimeStart;       //time variable that reads the actual FPS

        MainWindow *mMainWindow;

        class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

        void checkForGLerrors();

    protected:
        void timerEvent(QTimerEvent *) override;
};

#endif // RENDERWINDOW_H
