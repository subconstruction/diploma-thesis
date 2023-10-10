#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "SortingThread.h"

#include <QThread>
#include <QTimer>

#include <cstdlib>
#include <ctime>

unsigned const short sleepTime = 100;

int values[13];
int defaultValues[13] = {10, 51, 28, 92, 66, 57, 21, 13, 19, 44, 13, 72, 41};

SortingThread* sortingThread;

void MainWindow::setProgressBarsAndLabelsToValue(const int array[13])
{
    for (int i = 0; i < 13; ++i) {
        QString progressBarName = QString("item_bar%1").arg(i + 1);
        QString labelName = QString("item_label%1").arg(i + 1);

        QProgressBar* progressBar = findChild<QProgressBar*>(progressBarName);
        QLabel* label = findChild<QLabel*>(labelName);

        if (progressBar && label) {
            progressBar->setValue(array[i]);
            label->setText(QString::number(array[i]));
        }
    }
}

void MainWindow::bubbleSortAndVisualize(int array[], int size)
{
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                std::swap(array[j], array[j + 1]);

                setProgressBarsAndLabelsToValue(array);
                QThread::msleep(sleepTime);
            }
        }
    }
}

void MainWindow::shakerSortAndVisualize(int array[], int size)
{
    int left = 0;
    int right = size - 1;
    bool swapped = true;

    while (swapped) {
        swapped = false;

        for (int i = left; i < right; ++i) {
            if (array[i] > array[i + 1]) {
                std::swap(array[i], array[i + 1]);
                swapped = true;

                setProgressBarsAndLabelsToValue(array);
                QThread::msleep(sleepTime);
            }
        }

        if (!swapped) {
            break;
        }

        swapped = false;
        --right;

        for (int i = right; i > left; --i) {
            if (array[i] < array[i - 1]) {
                std::swap(array[i], array[i - 1]);
                swapped = true;

                setProgressBarsAndLabelsToValue(array);
                QThread::msleep(sleepTime);
            }
        }

        ++left;
    }
}

void MainWindow::insertionSortAndVisualize(int array[], int size)
{
    for (int i = 1; i < size; ++i) {
        int key = array[i];
        int j = i - 1;

        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = key;

        setProgressBarsAndLabelsToValue(array);
        QThread::msleep(sleepTime);
    }
}

void MainWindow::gnomeSortAndVisualize(int array[], int size)
{
    int index = 0;

    while (index < size) {
        if (index == 0) {
            ++index;
        }

        if (array[index] >= array[index - 1]) {
            ++index;
        } else {
            std::swap(array[index], array[index - 1]);
            --index;

            setProgressBarsAndLabelsToValue(array);
            QThread::msleep(sleepTime);
        }
    }
}

void MainWindow::selectionSortAndVisualize(int array[], int size)
{
    for (int i = 0; i < size - 1; ++i) {
        int minIndex = i;

        for (int j = i + 1; j < size; ++j) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            std::swap(array[i], array[minIndex]);

            setProgressBarsAndLabelsToValue(array);
            QThread::msleep(sleepTime);
        }
    }
}

void MainWindow::onBubbleSortButtonClicked(const int array[13])
{
    int* nonConstArray = const_cast<int*>(array);

    bubbleSortAndVisualize(nonConstArray, 13);
}

void MainWindow::onShakerSortButtonClicked(const int array[13])
{
    int* nonConstArray = const_cast<int*>(array);

    shakerSortAndVisualize(nonConstArray, 13);
}

void MainWindow::onInsertionSortButtonClicked(const int array[13])
{
    int* nonConstArray = const_cast<int*>(array);

    insertionSortAndVisualize(nonConstArray, 13);
}

void MainWindow::shuffleValues()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < 13; ++i) {
        values[i] = rand() % 101;
    }

    setProgressBarsAndLabelsToValue(values);
}

void MainWindow::onMergeSortButtonClicked()
{
    ui->merge_sort->setEnabled(false);

    sortingThread->setArray(values, 13);
    sortingThread->start();
}

void MainWindow::onSortingFinished(int* sortedArray, int size)
{
    std::copy(sortedArray, sortedArray + size, values);
    setProgressBarsAndLabelsToValue(values);

    ui->merge_sort->setEnabled(true);
}

void MainWindow::quickSort(int arr[], int low, int high)
{
    if (low < high) {
        int pivot = partition(arr, low, high);

        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

int MainWindow::partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);

            setProgressBarsAndLabelsToValue(arr);
            QThread::msleep(sleepTime);
        }
    }
    std::swap(arr[i + 1], arr[high]);

    setProgressBarsAndLabelsToValue(arr);
    QThread::msleep(sleepTime);

    return (i + 1);
}

void MainWindow::quickSortAndVisualize(int array[], int size)
{
    quickSort(array, 0, size - 1);
}

void MainWindow::onQuickSortButtonClicked()
{
    int* nonConstArray = new int[13];
    std::copy(values, values + 13, nonConstArray);

    quickSortAndVisualize(nonConstArray, 13);
    delete[] nonConstArray;
}

void MainWindow::heapify(int array[], int size, int root, int& comparisons, int& swaps)
{
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    if (left < size && array[left] > array[largest]) {
        largest = left;
    }

    if (right < size && array[right] > array[largest]) {
        largest = right;
    }

    if (largest != root) {
        std::swap(array[root], array[largest]);
        swaps++;

        setProgressBarsAndLabelsToValue(array);
        QThread::msleep(sleepTime);

        heapify(array, size, largest, comparisons, swaps);
    }
}

void MainWindow::onHeapSortButtonClicked()
{
    int* nonConstArray = new int[13];
    std::copy(values, values + 13, nonConstArray);

    heapSortAndVisualize(nonConstArray, 13);
    delete[] nonConstArray;
}

void MainWindow::heapSortAndVisualize(int array[], int size)
{
    int comparisons = 0;
    int swaps = 0;

    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(array, size, i, comparisons, swaps);
    }

    for (int i = size - 1; i > 0; i--) {
        std::swap(array[0], array[i]);
        swaps++;

        setProgressBarsAndLabelsToValue(array);
        QThread::msleep(sleepTime);

        heapify(array, i, 0, comparisons, swaps);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sortingThread = new SortingThread(this);

    connect(sortingThread, &SortingThread::sortingFinished, this, &MainWindow::onSortingFinished);
    connect(ui->merge_sort, &QPushButton::clicked, this, &MainWindow::onMergeSortButtonClicked);
    connect(ui->heap_sort, &QPushButton::clicked, this, &MainWindow::onHeapSortButtonClicked);

    connect(ui->bubble_sort, &QPushButton::clicked, this, [this]() {
        onBubbleSortButtonClicked(values);
    });

    connect(ui->quick_sort, &QPushButton::clicked, this, [this]() {
        onQuickSortButtonClicked();
    });

    connect(ui->shaker_sort, &QPushButton::clicked, this, [this]() {
        onShakerSortButtonClicked(values);
    });

    connect(ui->insertion_sort, &QPushButton::clicked, this, [this]() {
        onInsertionSortButtonClicked(values);
    });

    connect(ui->gnome_sort, &QPushButton::clicked, this, [this]() {
        gnomeSortAndVisualize(values, 13);
    });

    connect(ui->selection_sort, &QPushButton::clicked, this, [this]() {
        selectionSortAndVisualize(values, 13);
    });

    connect(ui->bub_and_sel_sort, &QPushButton::clicked, this, [this]() {
        selectionSortAndVisualize(values, 13);
    });

    connect(ui->shuffle_values, &QPushButton::clicked, this, &MainWindow::shuffleValues);

    std::copy(std::begin(defaultValues), std::end(defaultValues), std::begin(values));

    setProgressBarsAndLabelsToValue(values);

    setFixedSize(800, 490);
}

MainWindow::~MainWindow()
{
    delete ui;
}
