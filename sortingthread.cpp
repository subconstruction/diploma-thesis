#include "SortingThread.h"

#include <QThread>

#include <iostream>
#include <algorithm>

SortingThread::SortingThread(QObject* parent) : QThread(parent), array(nullptr), arraySize(0), sortingMethod("bubble"), stopRequested(false), swap_operations_count(0) {}

void SortingThread::setArray(int* arr, int size)
{
    array = arr;
    arraySize = size;
}

void SortingThread::assignMethod(const std::string &method) {
    sortingMethod = method;
}

void SortingThread::setSleepTime(int time) {
    sleepTime = time;
}

void SortingThread::swap_values(int array[], int a, int b) {
    ++swap_operations_count;

    const int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

void SortingThread::bubbleSortAndVisualize(int array[], int size)
{
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                QMutexLocker locker(&mutex);
                if (stopRequested) {
                    stopRequested = false;
                    return;
                }

                swap_values(array, j, j + 1);

                emit sortingFinished(array, arraySize, swap_operations_count);
                QThread::msleep(sleepTime);
            }
        }
    }
}

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

        emit sortingFinished(arr, arraySize, ++swap_operations_count);

        QThread::msleep(sleepTime);
    }

    while (i < n1) {

        arr[k] = L[i];
        i++;
        k++;

        emit sortingFinished(arr, arraySize, ++swap_operations_count);

        QThread::msleep(sleepTime);
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;

        emit sortingFinished(arr, arraySize, ++swap_operations_count);

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

void SortingThread::stopSorting()
{
    mutex.lock();
    stopRequested = true;
    mutex.unlock();
}

void SortingThread::shakerSortAndVisualize(int array[], int size)
{
    int left = 0;
    int right = size - 1;
    bool swapped = true;

    while (swapped) {
        swapped = false;

        for (int i = left; i < right; ++i) {
            if (array[i] > array[i + 1]) {
                swap_values(array, i, i + 1);
                swapped = true;

                emit sortingFinished(array, arraySize, swap_operations_count);
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
                swap_values(array, i, i - 1);
                swapped = true;

                emit sortingFinished(array, arraySize, swap_operations_count);
                QThread::msleep(sleepTime);
            }
        }

        ++left;
    }
}

void SortingThread::insertionSortAndVisualize(int array[], int size)
{
    for (int i = 1; i < size; ++i) {
        int key = array[i];
        int j = i - 1;

        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            --j;
            swap_operations_count++;
            //QThread::msleep(sleepTime);
        }
        array[j + 1] = key;
        swap_operations_count++;

        emit sortingFinished(array, arraySize, swap_operations_count);
        QThread::msleep(sleepTime);
    }
}

void SortingThread::gnomeSortAndVisualize(int array[], int size)
{
    int index = 0;

    while (index < size) {
        if (index == 0) {
            ++index;
        }

        if (array[index] >= array[index - 1]) {
            ++index;
        } else {
            swap_values(array, index, index - 1);
            --index;

            emit sortingFinished(array, arraySize, swap_operations_count);
            QThread::msleep(sleepTime);
        }
    }
}

void SortingThread::selectionSortAndVisualize(int array[], int size)
{
    for (int i = 0; i < size - 1; ++i) {
        int minIndex = i;

        for (int j = i + 1; j < size; ++j) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            swap_values(array, i, minIndex);

            emit sortingFinished(array, arraySize, swap_operations_count);
            QThread::msleep(sleepTime);
        }
    }
}

void SortingThread::quickSort(int arr[], int low, int high)
{
    if (low < high) {
        int pivot = partition(arr, low, high);

        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

int SortingThread::partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap_values(arr, i, j);

            emit sortingFinished(arr, arraySize, swap_operations_count);
            QThread::msleep(sleepTime);
        }
    }
    swap_values(arr, i + 1, high);

    emit sortingFinished(arr, arraySize, swap_operations_count);
    QThread::msleep(sleepTime);

    return (i + 1);
}

void SortingThread::quickSortAndVisualize(int array[], int size)
{
    quickSort(array, 0, size - 1);
}

void SortingThread::heapify(int array[], int size, int root, int& comparisons, int& swaps)
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
        swap_values(array, root, largest);
        swaps++;

        emit sortingFinished(array, arraySize, swap_operations_count);
        QThread::msleep(sleepTime);

        heapify(array, size, largest, comparisons, swaps);
    }
}

void SortingThread::heapSortAndVisualize(int array[], int size)
{
    int comparisons = 0;
    int swaps = 0;

    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(array, size, i, comparisons, swaps);
    }

    for (int i = size - 1; i > 0; i--) {
        swap_values(array, 0, i);
        swaps++;

        emit sortingFinished(array, arraySize, swap_operations_count);
        QThread::msleep(sleepTime);

        heapify(array, i, 0, comparisons, swaps);
    }
}

void SortingThread::run()
{
    swap_operations_count = 0;

    if (sortingMethod == "bubble") {
        bubbleSortAndVisualize(array, arraySize);
    }

    if (sortingMethod == "shaker") {
        shakerSortAndVisualize(array, arraySize);
    }

    if (sortingMethod == "selection") {
        selectionSortAndVisualize(array, arraySize);
    }

    if (sortingMethod == "bub_and_sel") {
        selectionSortAndVisualize(array, arraySize);
    }

    if (sortingMethod == "heap") {
        heapSortAndVisualize(array, arraySize);
    }

    if (sortingMethod == "gnome") {
        gnomeSortAndVisualize(array, arraySize);
    }

    if (sortingMethod == "quick") {
        quickSortAndVisualize(array, arraySize);
    }

    if (sortingMethod == "merge") {
        mergeSort(array, arraySize);
    }

    if (sortingMethod == "insertion") {
        insertionSortAndVisualize(array, arraySize);
    }

    emit sortingFinished(array, arraySize, 0);
}
