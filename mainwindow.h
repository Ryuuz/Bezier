#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

class QSurface;
class QTextEdit;
class RenderWindow;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void init();
        void renderWindowReady();
        void renderWindowError(const QString &msg);

    private:
        void addRenderWindow();
        Ui::MainWindow *ui;

        QWidget *mRenderWindowContainer;
        RenderWindow *mRenderWindow;
        QSurface *mSurface;

        QTextEdit *mOutput;
};

#endif // MAINWINDOW_H
