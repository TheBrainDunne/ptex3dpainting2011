/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <Ptexture.h>

#include "glwidget.h"
#include "geometry.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    setAutoFillBackground(false);

    this->canvas = new QImage(width(),height(),QImage::Format_ARGB32);
    this->canvas->fill(QColor(0,0,0,0).value());
    painter = new QPainter(this);

    createGradients();

}


GLWidget::~GLWidget()
{
    _3d_Model->deleteTexture();
    ptexWriter->release();
    delete canvas;
    delete painter;
}

void GLWidget::createGradients()
{
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient.setCenter(0.45, 0.50);
    gradient.setFocalPoint(0.40, 0.45);
    gradient.setColorAt(0.0, QColor(105, 146, 182));
    gradient.setColorAt(0.4, QColor(81, 113, 150));
    gradient.setColorAt(0.8, QColor(16, 56, 121));

    brush.setCoordinateMode(QGradient::ObjectBoundingMode);
    brush.setCenter(0.45, 0.50);
    brush.setFocalPoint(0.40, 0.45);
    brush.setColorAt(0.0, QColor(105, 146, 182));
    brush.setColorAt(0.4, QColor(81, 113, 150));
    brush.setColorAt(0.8, QColor(16, 56, 121));
}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}


void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}


void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::initializeGL()
{
    qglClearColor(qtPurple.light());

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    _3d_Model = new Geometry();
    _3d_Model->setupTexture();


}


void GLWidget::paintEvent(QPaintEvent *)
{

    setupViewport(width(), height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    qglColor(Qt::green);

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    //drawBackground(painter);

    _3d_Model->draw();
    glDisable(GL_TEXTURE_2D);



    painter->begin(this);
    painter->drawImage(0,0, *(this->canvas));
    painter->end();
}

void GLWidget::bakePaint()
{

}

void GLWidget::drawLineTo(const QPoint &endPos)
 {
    painter->begin(this->canvas);
    int myPenWidth = 10;
    painter->setPen(QPen(brush, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(lastPos, endPos);
    painter->end();
     int rad = (myPenWidth / 2) + 2;
     update(QRect(lastPos, endPos).normalized()
                                      .adjusted(-rad, -rad, +rad, +rad));

 }

void GLWidget::drawBackground(QPainter *painter)
{
    painter->begin(this);
    painter->setPen(Qt::NoPen);
    painter->setBrush(gradient);
    painter->drawRect(rect());
    painter->end();
}

void GLWidget::resizeGL(int w, int h)
{
    resizeImage();
    setupViewport(w, h);
    update();
}

void GLWidget::resizeImage()
{
    QSize newSize = this->size();
    if (this->canvas->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(QColor(0,0,0,0).value());
    QPainter painter(&newImage);
    QPointF newPos((newSize.width() - this->canvas->size().width())/2,
    (newSize.height() - this->canvas->size().height())/2);
    painter.drawImage(newPos,*(this->canvas));
    *this->canvas = newImage;
}

void GLWidget::setupViewport(int width, int height)
{
    glViewport(0, 0 , width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, +1, -1, +1, 4.0, 15.0);

    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        //setXRotation(xRot + 8 * dy);
        //setYRotation(yRot + 8 * dx);
        drawLineTo(event->pos());

    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{

}
