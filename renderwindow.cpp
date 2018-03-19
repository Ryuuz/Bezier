#include "renderwindow.h"
#include <iostream>
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QDebug>

#include "mainwindow.h"
#include "shader.h"
#include "objectinstance.h"
#include "sceneobject.h"
#include "camera.h"
#include "axis.h"
#include "beziercurve.h"
#include "groundplane.h"


RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext{0}, mInitialized{false}, mMainWindow{mainWindow}
{
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    mContext = new QOpenGLContext(this);
    mContext->setFormat(requestedFormat());
    if (!mContext->create())
    {
        delete mContext;
        mContext = 0;
    }
}


RenderWindow::~RenderWindow()
{
    delete mContext;
    delete mShaderProgram;
    delete mAxis;
}


void RenderWindow::init()
{
    if (!mContext->makeCurrent(this)) {
        emit error(tr("makeCurrent() failed"));
        return;
    }

    if (!mInitialized)
    {
        mInitialized = true;
    }

    initializeOpenGLFunctions();

    startOpenGLDebugger();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    mCamera = new Camera;

    //Set the perspective and view matrices
    mCamera->setPerspectiveMatrix(width(), height());
    mCamera->setViewMatrix(QVector3D(-2.f, 2.f, 2.f), QVector3D(0.f, 0.f, 0.f));

	//Make shader
    mShaderProgram = new Shader("vertexcolorshader.vert", "fragmentcolorshader.frag");
    mMatrixUniform = glGetUniformLocation(mShaderProgram->getProgram(), "matrix");

	//Create objects
    //mAxis = new Axis;

    //mGround = new GroundPlane(QVector3D(1.f, 0.f, 1.f), QVector3D(-1.f, 0.f, -1.f), 4, mShaderProgram);
    //mGround->writeToFile();
    mGround = new GroundPlane(mShaderProgram);
    //mGround->findPoint(QVector3D(-0.1f, 0.f, -0.4f));
    mBezier = mGround->pointToPoint(QVector3D(-1.f, 0.f, -1.f), QVector3D(-0.1f, 0.f, -0.4f));

    glBindVertexArray(0);

    ObjectInstance *obj;

    //Axis
    //obj = new ObjectInstance(mAxis, mShaderProgram);
    //mStaticObjects.push_back(obj);

    obj = new ObjectInstance(mBezier, mShaderProgram);
    mStaticObjects.push_back(obj);

    emit ready();   //tell the mainWindow that init is finished
}


void RenderWindow::render()
{
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Render objects
    for(auto object : mStaticObjects)
    {
        object->draw(mMatrixUniform, mCamera);
        checkForGLerrors();
    }

    mGround->draw(mMatrixUniform, mCamera);

    glBindVertexArray(0);
    mContext->swapBuffers(this);
}


void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        qDebug() << "Can this system run QOpenGLDebugLogger? :" << format.testOption(QSurfaceFormat::DebugContext);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
            {
                qDebug() << "Started OpenGL debug logger!";
            }
        }
    }
}


void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "glGetError returns " << err;
        }
    }
}


//This function is called from Qt when window is exposed / shown
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
    {
        init();
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    if (isExposed())
    {
        mTimer.start(16, this);
        mTimeStart.start();
    }
}


void RenderWindow::timerEvent(QTimerEvent *)
{
    render();
}
