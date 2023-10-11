#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>

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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setProgressBarsAndLabelsToValue(const int array[]);
    void onSortingFinished(int* sortedArray, int size);
    void deleteProgressBarsAndLabels(int max_int);

    std::vector<std::pair<QProgressBar*, QLabel*>> createProgressBarsAndLabels(QWidget *parent, int max_int);
private:
    Ui::MainWindow *ui;
};
