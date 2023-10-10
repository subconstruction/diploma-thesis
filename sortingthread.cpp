#include "SortingThread.h"
#include <algorithm>

SortingThread::SortingThread(QObject* parent) : QThread(parent), array(nullptr), arraySize(0) {}

void SortingThread::setArray(int* arr, int size)
{
    array = arr;
    arraySize = size;
}

const int sleepTime = 35; // чисто для мержа костыль

void SortingThread::merge(int arr[], int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;

        emit sortingFinished(arr, arraySize);

        QThread::msleep(sleepTime);
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;

        emit sortingFinished(arr, arraySize);

        QThread::msleep(sleepTime);
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;

        emit sortingFinished(arr, arraySize);

        QThread::msleep(sleepTime);
    }
}

void SortingThread::mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

void SortingThread::mergeSort(int arr[], int size)
{
    mergeSort(arr, 0, size - 1);
}

void SortingThread::run()
{
    mergeSort(array, arraySize);

    emit sortingFinished(array, arraySize);
}
