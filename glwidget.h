#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QOpenGLWidget>

#include "model/robject.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    float speed = 0.2f;
    RObject *world = nullptr;

public:
    explicit GLWidget(QWidget *parent);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // GLWIDGET_H
