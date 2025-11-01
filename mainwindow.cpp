#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Criar um vetor de ponteiros para os mutexes
    // (necessário para passar para o construtor do trem)
    QMutex* mutex_ptr[7];
    for (int i = 0; i < 7; i++) {
        mutex_ptr[i] = &mutexes[i];
    }

    // --- COORDENADAS DE INÍCIO - AJUSTE PARA O SEU .UI ---
    const int x1 = 60,  x2 = 230, x3 = 400;
    const int y1 = 30,  y2 = 150, y3 = 270;

    //Cria os 6 trens (ID, x_inicial, y_inicial, ponteiro_mutexes)
    trens[0] = new Trem(1, x1, y1, mutex_ptr);      // Trem 1 (Verde)
    trens[1] = new Trem(2, x2, y1, mutex_ptr);      // Trem 2 (Vermelho)
    trens[2] = new Trem(3, x3, y1, mutex_ptr);      // Trem 3 (Azul)
    trens[3] = new Trem(4, x1, y2, mutex_ptr);      // Trem 4 (Laranja)
    trens[4] = new Trem(5, x3, y2, mutex_ptr);      // Trem 5 (Roxo)
    trens[5] = new Trem(6, x1-170, y3, mutex_ptr);  // Trem 6 (Preto)


    // Conecta o sinal UPDATEGUI de CADA trem ao slot UPDATEINTERFACE
    for (int i = 0; i < 6; i++) {
        connect(trens[i], SIGNAL(updateGUI(int,int,int)), SLOT(updateInterface(int,int,int)));
    }

    // Conecta os SLIDERS aos SLOTS setVelocidade dos trens
    // (Os nomes 'slider_tremX' devem ser os mesmos que você definiu no .ui)
    connect(ui->slider_trem1, SIGNAL(valueChanged(int)), trens[0], SLOT(setVelocidade(int)));
    connect(ui->slider_trem2, SIGNAL(valueChanged(int)), trens[1], SLOT(setVelocidade(int)));
    connect(ui->slider_trem3, SIGNAL(valueChanged(int)), trens[2], SLOT(setVelocidade(int)));
    connect(ui->slider_trem4, SIGNAL(valueChanged(int)), trens[3], SLOT(setVelocidade(int)));
    connect(ui->slider_trem5, SIGNAL(valueChanged(int)), trens[4], SLOT(setVelocidade(int)));
    connect(ui->slider_trem6, SIGNAL(valueChanged(int)), trens[5], SLOT(setVelocidade(int)));

    // Inicia os trens automaticamente [cite: 60]
    for (int i = 0; i < 6; i++) {
        trens[i]->start();
    }
}

//Função que será executada quando o sinal UPDATEGUI for emitido
void MainWindow::updateInterface(int id, int x, int y){
    // (Os nomes 'label_tremX' devem ser os mesmos que você definiu no .ui)
    switch(id){
    case 1:
        ui->label_trem1->setGeometry(x,y,21,17);
        break;
    case 2:
        ui->label_trem2->setGeometry(x,y,21,17);
        break;
    case 3:
        ui->label_trem3->setGeometry(x,y,21,17);
        break;
    case 4:
        ui->label_trem4->setGeometry(x,y,21,17);
        break;
    case 5:
        ui->label_trem5->setGeometry(x,y,21,17);
        break;
    case 6:
        ui->label_trem6->setGeometry(x,y,21,17);
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    // Encerra as threads antes de fechar
    for (int i = 0; i < 6; i++) {
        trens[i]->terminate();
    }
    delete ui;
}