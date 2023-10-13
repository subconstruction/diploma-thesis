#ifndef DRAGGING_H
#define DRAGGING_H

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>

class BorderlessWindow : public QWidget
{
public:
    BorderlessWindow(QWidget* parent = nullptr) : QWidget(parent)
    {
        setWindowFlags(Qt::FramelessWindowHint);
    }

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            initialMousePos = event->globalPos();
            initialWindowPos = this->pos();
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (event->buttons() & Qt::LeftButton)
        {
            int dx = event->globalX() - initialMousePos.x();
            int dy = event->globalY() - initialMousePos.y();
            int newX = initialWindowPos.x() + dx;
            int newY = initialWindowPos.y() + dy;

            this->move(newX, newY);
        }
    }

private:
    QPoint initialMousePos;
    QPoint initialWindowPos;
};

#endif // DRAGGING_H
