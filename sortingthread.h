#ifndef SORTINGTHREAD_H
#define SORTINGTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>

class SortingThread : public QThread
{
    Q_OBJECT

public slots:
    void stopSorting();

public:
    int sleepTime = 0;

    //rule constructions
    explicit SortingThread(QObject* parent = nullptr); // disable implicit conversions

    void setArray(int* arr, int size);
    void setSleepTime(int time);
    void assignMethod(const std::string &method);
    void swap_values(int array[], int a, int b);

    // @region: sorting [start]

    //merge
    void merge(int arr[], int l, int m, int r);
    void mergeSort(int arr[], int size);
    void mergeSort(int arr[], int l, int r);

    //bubble
    void bubbleSortAndVisualize(int array[], int size);

    //shaker
    void shakerSortAndVisualize(int array[], int size);

    //insertion
    void insertionSortAndVisualize(int array[], int size);

    //gnome
    void gnomeSortAndVisualize(int array[], int size);

    //selection
    void selectionSortAndVisualize(int array[], int size);

    //quick sort
    int partition(int arr[], int low, int high);
    void quickSort(int arr[], int low, int high);
    void quickSortAndVisualize(int array[], int size);

    //heap sort
    void heapify(int array[], int size, int root, int& comparisons, int& swaps);
    void heapSortAndVisualize(int array[], int size);


    // @region: sortring [end]

    int swap_operations_count;
    bool stopRequested = false; // Flag to indicate a stop request


signals:
    void sortingFinished(int* sortedArray, int size, int swap_operations_count) ;


protected:
    void run() override; // starting component

private:
    QMutex mutex; // Add a mutex to ensure thread-safety

private:
    int* array;
    int arraySize;

    std::string sortingMethod;
};

#endif
