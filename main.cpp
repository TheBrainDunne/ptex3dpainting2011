#include <QtGui/QApplication>
#include "mainwindow.h"
#include <iostream>
#include <qgl.h>

int main(int argc, char *argv[])
{
    QGL::setPreferredPaintEngine(QPaintEngine::OpenGL) ;
    QApplication a(argc, argv);

    if(!QGLFormat::hasOpenGL())
    {
        std::cerr << "This System has no OpenGL Support" << std::endl;
        return 1;
    }
    MainWindow w;
    w.show();

    return a.exec();
}
