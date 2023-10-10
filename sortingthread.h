#ifndef SORTINGTHREAD_H
#define SORTINGTHREAD_H

#include <QObject>
#include <QThread>

class SortingThread : public QThread
{
    Q_OBJECT

public:
    explicit SortingThread(QObject* parent = nullptr);
    void setArray(int* arr, int size);
    void merge(int arr[], int l, int m, int r);
    void mergeSort(int arr[], int l, int r);
    void mergeSort(int arr[], int size);

signals:
    void sortingFinished(int* sortedArray, int size);

protected:
    void run() override;

private:
    int* array;
    int arraySize;
};

#endif
