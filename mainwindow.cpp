#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "renderwindow.h"
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QTextEdit>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mOutput = new QTextEdit;
    mOutput->setReadOnly(true);
    init();
}


MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}


void MainWindow::init()
{
    QSurfaceFormat format;

    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSamples(4);
    format.setOption(QSurfaceFormat::DebugContext);
    format.setDepthBufferSize(16);

    mOutput->clear();

    mRenderWindow = new RenderWindow(format, this);
    if (!mRenderWindow->context())
    {
        mOutput->append(tr("Failed to create context"));
        delete mRenderWindow;
        mRenderWindowContainer = new QWidget;
        addRenderWindow();
        return;
    }
    mSurface = mRenderWindow;

    connect(mRenderWindow, &RenderWindow::ready, this, &MainWindow::renderWindowReady);
    connect(mRenderWindow, &RenderWindow::error, this, &MainWindow::renderWindowError);

    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    addRenderWindow();
}


void MainWindow::renderWindowReady()
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    Q_ASSERT(context);
}


void MainWindow::renderWindowError(const QString &msg)
{
    mOutput->append(tr("An error has occurred:\n%1").arg(msg));
}


void MainWindow::addRenderWindow()
{
    ui->renderLayout->addWidget(mRenderWindowContainer);
}
