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
    void onHeapSortButtonClicked();
    void onMergeSortButtonClicked();
    void onShakerSortButtonClicked(const int array[13]);
    void onInsertionSortButtonClicked(const int array[13]);
    void onBubbleSortButtonClicked(const int array[13]);
    void onQuickSortButtonClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setProgressBarsAndLabelsToValue(const int array[13]);
    void bubbleSortAndVisualize(int array[], int size);
    void shakerSortAndVisualize(int array[], int size);
    void insertionSortAndVisualize(int array[], int size);
    void gnomeSortAndVisualize(int array[], int size);
    void selectionSortAndVisualize(int array[], int size);
    void onSortingFinished(int* sortedArray, int size);
    int partition(int arr[], int low, int high);
    void quickSort(int arr[], int low, int high);
    void quickSortAndVisualize(int array[], int size);
    void heapify(int array[], int size, int root, int& comparisons, int& swaps);
    void heapSortAndVisualize(int array[], int size);

private:
    Ui::MainWindow *ui;
};
