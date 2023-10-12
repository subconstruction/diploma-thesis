#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>

#include <QMouseEvent>
#include <QWidget>
#include <QPoint>
#include <QSize>
#include <QPushButton>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void shuffleValues();
    void disableAll();
    void enableAll();
    void updateLabel(int val);

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            if (!resizing) {
                dragStartPosition = event->pos();
                dragging = true;
            }
        } else if (event->button() == Qt::RightButton) {
            dragStartPosition = event->pos();
            resizing = true;
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        if (dragging) {
            QPoint delta = event->pos() - dragStartPosition;
            move(pos() + delta);
        } else if (resizing) {
            int newWidth = size().width() + (event->pos().x() - dragStartPosition.x());
            int newHeight = size().height() + (event->pos().y() - dragStartPosition.y());
            resize(newWidth, newHeight);
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            dragging = false;
        } else if (event->button() == Qt::RightButton) {
            resizing = false;
        }
    }


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setProgressBarsAndLabelsToValue(const int array[]);
    void onSortingFinished(int* sortedArray, int size);
    void deleteProgressBarsAndLabels(int max_int);

    std::vector<std::pair<QProgressBar*, QLabel*>> createProgressBarsAndLabels(QWidget *parent, int max_int);
private:
    Ui::MainWindow *ui;

    QPushButton* closeBtn;
    QPushButton* minimizeBtn;
    QPoint dragStartPosition;

    bool dragging;
    bool resizing;
};


