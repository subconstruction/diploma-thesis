#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "SortingThread.h"

#include <QThread>
#include <QProgressBar>
#include <QLabel>
#include <QtWidgets>
#include <QLayout>
#include <QTimer>

#include <ctime>
#include <math.h>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <windows.h>

double start;

int arraySize = 25;

int* values = new int[arraySize];

SortingThread* sortingThread;

std::vector<std::pair<QProgressBar*, QLabel*>> MainWindow::createProgressBarsAndLabels(QWidget *parent, int max_int)
{
    std::vector<std::pair<QProgressBar*, QLabel*>> progressBarLabelPairs;

    for (int ordinal = 1; ordinal <= max_int; ++ordinal) {
        QProgressBar* progressBar = new QProgressBar(parent);
        progressBar->setObjectName(QString("item_bar%1").arg(ordinal));

        QLabel* label = new QLabel(parent);
        label->setObjectName(QString("item_label%1").arg(ordinal));

        int progressBarX = 160 + ordinal * 30;
        int progressBarY = 120;

        progressBar->setGeometry(progressBarX, progressBarY, 16, 231);
        progressBar->setOrientation(Qt::Vertical);
        progressBar->setStyleSheet("border-color: #74c8ff; border-radius: 4px");
        progressBar->setTextVisible(0);

        int labelX = progressBarX;
        int labelY = 360;
        label->setGeometry(labelX, labelY, 21, 16);

        progressBarLabelPairs.push_back(std::make_pair(progressBar, label));
    }

    return progressBarLabelPairs;
}

void MainWindow::deleteProgressBarsAndLabels(int max_int)
{
    for (int ordinal = 1; ordinal <= max_int; ++ordinal) {
        QString progressBarName = QString("item_bar%1").arg(ordinal);
        QString labelName = QString("item_label%1").arg(ordinal);

        QProgressBar* progressBar = findChild<QProgressBar*>(progressBarName);
        QLabel* label = findChild<QLabel*>(labelName);

        if (progressBar) {
            progressBar->deleteLater();
        }

        if (label) {
            label->deleteLater();
        }
    }
}


void MainWindow::setProgressBarsAndLabelsToValue(const int array[])
{
    for (int i = 0; i < arraySize; ++i) {
        QString progressBarName = QString("item_bar%1").arg(i + 1);
        QString labelName = QString("item_label%1").arg(i + 1);

        QProgressBar* progressBar = findChild<QProgressBar*>(progressBarName);
        QLabel* label = findChild<QLabel*>(labelName);

        if (progressBar && label) {
            progressBar->setValue(array[i]);
            label->setText(QString::number(array[i]));
        }
    }

    QString p = "Swap Count: ";
    int count = sortingThread->swap_operations_count;
    QString countString = QString::number(count);
    QString labelText = p + countString;
    QLabel* label = findChild<QLabel*>("swap_count");
    label->setText(labelText);
}

void MainWindow::shuffleValues()
{
    sortingThread->quit();

    QLabel* label = findChild<QLabel*>("swap_count");
    label->setText("Swap Count: 0");

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < arraySize; ++i) {
        values[i] = rand() % 101;
    }

    setProgressBarsAndLabelsToValue(values);
}

void MainWindow::disableAll() {
    ui->bubble_sort->setEnabled(false);
    ui->shaker_sort->setEnabled(false);
    ui->insertion_sort->setEnabled(false);
    ui->bub_and_sel_sort->setEnabled(false);
    ui->merge_sort->setEnabled(false);
    ui->quick_sort->setEnabled(false);
    ui->gnome_sort->setEnabled(false);
    ui->heap_sort->setEnabled(false);
    ui->selection_sort->setEnabled(false);
}

void MainWindow::enableAll() {
    ui->bubble_sort->setEnabled(true);
    ui->shaker_sort->setEnabled(true);
    ui->insertion_sort->setEnabled(true);
    ui->bub_and_sel_sort->setEnabled(true);
    ui->merge_sort->setEnabled(true);
    ui->quick_sort->setEnabled(true);
    ui->gnome_sort->setEnabled(true);
    ui->heap_sort->setEnabled(true);
    ui->selection_sort->setEnabled(true);
}

void MainWindow::onSortingFinished(int* sortedArray, int size)
{
    double end = clock();

    std::copy(sortedArray, sortedArray + size, values);
    setProgressBarsAndLabelsToValue(values);

    enableAll();

    QString p = "Runtime: ";
    QString e = " ms";

    QString countString = QString::number(end - sortingThread->swap_operations_count*sortingThread->sleepTime - start);
    QString labelText = p + countString + e;
    QLabel* label = findChild<QLabel*>("runtime_label");
    label->setText(labelText);
}

void MainWindow::updateLabel(int val)
{
    const int value = val*10;

    QString text = QString("%1ms").arg(value);
    ui->label->setText(text);

    sortingThread->setSleepTime(value);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::vector<std::pair<QProgressBar*, QLabel*>> progressBarsAndLabels = createProgressBarsAndLabels(this, 25);

    for (const auto& pair : progressBarsAndLabels) {
        pair.first->show();
        pair.second->show();
    }

    sortingThread = new SortingThread(this);

    connect(ui->speed_slider, SIGNAL(valueChanged(int)), this, SLOT(updateLabel(int)));
    connect(sortingThread, &SortingThread::sortingFinished, this, &MainWindow::onSortingFinished);
    connect(ui->merge_sort, &QPushButton::clicked, this, [this]() {
        if (ui->shouldShuffle->checkState()) shuffleValues();

        sortingThread->quit();
        sortingThread->assignMethod("merge");

        sortingThread->setArray(values, arraySize);
        start = clock();
        sortingThread->start();
    });
    connect(ui->array_update, &QPushButton::clicked, this, [this]() {
        sortingThread->quit();
        sortingThread->swap_operations_count = 0;

        const int newSize = ui->array_size_t->value();
        if (newSize < 1) {
            MessageBoxA(0, "Minimum Size Is 0", "An Invalid Value Entered", 0);
            return;
        }

        disableAll();
        deleteProgressBarsAndLabels(arraySize);

        arraySize = newSize;
        values = new int[arraySize];

        std::vector<std::pair<QProgressBar*, QLabel*>> progressBarsAndLabels = createProgressBarsAndLabels(this, arraySize);

        for (const auto& pair : progressBarsAndLabels) {
            pair.first->show();
            pair.second->show();
        }

        shuffleValues();
        QThread::msleep(200);

        srand(static_cast<unsigned int>(time(nullptr)));

        for (int i = 0; i < newSize; ++i) {
            values[i] = rand() % 101;
        }

        setProgressBarsAndLabelsToValue(values);

        enableAll();
    });
    connect(ui->heap_sort, &QPushButton::clicked, this, [this]() {
        if (ui->shouldShuffle->checkState()) shuffleValues();

        sortingThread->quit();
        sortingThread->assignMethod("heap");

        sortingThread->setArray(values, arraySize);
        start = clock();
        sortingThread->start();
    });

    connect(ui->bubble_sort, &QPushButton::clicked, this, [this]() {
        if (ui->shouldShuffle->checkState()) shuffleValues();

        sortingThread->quit();
        sortingThread->assignMethod("bubble");

        sortingThread->setArray(values, arraySize);
        start = clock();
        sortingThread->start();
    });

    connect(ui->quick_sort, &QPushButton::clicked, this, [this]() {
        if (ui->shouldShuffle->checkState()) shuffleValues();

        sortingThread->quit();
        sortingThread->assignMethod("quick");

        sortingThread->setArray(values, arraySize);
        start = clock();
        sortingThread->start();
    });

    connect(ui->shaker_sort, &QPushButton::clicked, this, [this]() {
        if (ui->shouldShuffle->checkState()) shuffleValues();

        sortingThread->quit();
        sortingThread->assignMethod("shaker");

        sortingThread->setArray(values, arraySize);
        start = clock();
        sortingThread->start();
    });

    connect(ui->insertion_sort, &QPushButton::clicked, this, [this]() {
        if (ui->shouldShuffle->checkState()) shuffleValues();

        sortingThread->quit();
        sortingThread->assignMethod("insertion");

        sortingThread->setArray(values, arraySize);
        start = clock();
        sortingThread->start();
    });

    connect(ui->gnome_sort, &QPushButton::clicked, this, [this]() {
        if (ui->shouldShuffle->checkState()) shuffleValues();

        sortingThread->quit();
        sortingThread->assignMethod("gnome");

        sortingThread->setArray(values, arraySize);
        start = clock();
        sortingThread->start();
    });

    connect(ui->selection_sort, &QPushButton::clicked, this, [this]() {
        if (ui->shouldShuffle->checkState()) shuffleValues();

        sortingThread->quit();
        sortingThread->assignMethod("selection");

        sortingThread->setArray(values, arraySize);
        start = clock();
        sortingThread->start();
    });

    connect(ui->bub_and_sel_sort, &QPushButton::clicked, this, [this]() {
        if (ui->shouldShuffle->checkState()) shuffleValues();

        sortingThread->quit();
        sortingThread->assignMethod("bub_and_sel");

        sortingThread->setArray(values, arraySize);
        start = clock();
        sortingThread->start();
    });

    connect(ui->shuffle_values, &QPushButton::clicked, this, &MainWindow::shuffleValues);

    shuffleValues();

    setProgressBarsAndLabelsToValue(values);

    setMaximumHeight(490);
    setMinimumSize(800, 490);
}

MainWindow::~MainWindow()
{
    using std::cout;

    cout << "Init Destructor [1]";

    delete ui;
    delete[] values;
}
