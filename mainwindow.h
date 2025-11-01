#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include "trem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void updateInterface(int,int,int);

private:
    Ui::MainWindow *ui;

    // Vetor de 6 trens
    Trem *trens[6];

    // Vetor de 7 mutexes [cite: 56]
    QMutex mutexes[7];
};

#endif // MAINWINDOW_H