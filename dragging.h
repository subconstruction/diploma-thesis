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
        setWindowFlags(Qt::FramelessWindowHint); // Remove window frame (title bar, border)

        // Set a background color (you can customize this)
        this->setStyleSheet("background-color: lightgray;");

        // You can add other widgets and elements to this window as needed
    }

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            // Store the initial mouse position for dragging
            initialMousePos = event->globalPos();
            initialWindowPos = this->pos();
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (event->buttons() & Qt::LeftButton)
        {
            // Calculate the new window position based on the mouse movement
            int dx = event->globalX() - initialMousePos.x();
            int dy = event->globalY() - initialMousePos.y();
            int newX = initialWindowPos.x() + dx;
            int newY = initialWindowPos.y() + dy;

            // Move the window to the new position
            this->move(newX, newY);
        }
    }

private:
    QPoint initialMousePos;
    QPoint initialWindowPos;
};

#endif // DRAGGING_H
