#include "glwidget.h"
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setAutoFillBackground(false);
}

void GLWidget::animate()
{
//    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
    if (world != nullptr)
    {
        world->update(speed);
    }
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), Qt::white);
    QFont font;
    font.setPixelSize(24);
    painter.setFont(font);
    if (world != nullptr)
    {
        world->render(&painter);
    }
    painter.end();
}
