#include "trem.h"
#include <QtCore>
#include <QMutexLocker>

// Construtor
// ID, Posição X, Posição Y, Vetor de Mutexes
Trem::Trem(int ID, int x, int y, QMutex **mutexes){
    this->ID = ID;
    this->x = x;
    this->y = y;
    this->sleepTime = 101; // Inicia com velocidade média (slider em 100)
    this->mutexes = mutexes;
}

// Slot para definir a velocidade
void Trem::setVelocidade(int sliderValue){
    // O slider vai de 0 (parado) a 200 (máximo) [cite: 62]
    // O sleepTime deve ser o inverso:
    // slider = 0 -> sleepTime = 201 (loop vai parar)
    // slider = 200 -> sleepTime = 1 (velocidade máxima)
    this->sleepTime = 201 - sliderValue;
}


//Função a ser executada após executar trem->START
void Trem::run(){

    // --- COORDENADAS DE EXEMPLO - AJUSTE PARA O SEU .UI ---
    // Malha 3x2 (superior) e 2x2 (inferior)
    const int x1 = 60,  x2 = 230, x3 = 400, x4 = 570;
    const int y1 = 30,  y2 = 150, y3 = 270;

    /*
     * Mapeamento dos Mutexes para as Regiões Críticas (conforme Figura 2):
     * mutexes[0]: Região 0
     * mutexes[1]: Região 1
     * mutexes[2]: Região 2
     * mutexes[3]: Região 3
     * mutexes[4]: Região 4
     * mutexes[5]: Região 5
     * mutexes[6]: Região 6
    */

    while(true){
        // Se o slider está em 0, o trem para [cite: 62]
        if (sleepTime > 200) {
            msleep(100); // Apenas dorme 100ms e checa de novo
            continue;    // Pula o resto do loop
        }

        switch(ID){
        case 1: // Trem 1 (Verde, Top-Left, Horário)
            if (y == y1 && x < x2)      // Topo
                x+=10;
            else if (x == x2 && y < y2) { // Direita -> Entra na RC 0
                QMutexLocker locker(mutexes[0]);
                y+=10;
            }
            else if (y == y2 && x > x1) { // Baixo -> Entra na RC 2
                QMutexLocker locker(mutexes[2]);
                x-=10;
            }
            else                        // Esquerda
                y-=10;
            break;

        case 2: // Trem 2 (Vermelho, Top-Mid, Horário)
            if (y == y1 && x < x3)      // Topo
                x+=10;
            else if (x == x3 && y < y2) { // Direita -> Entra na RC 1
                QMutexLocker locker(mutexes[1]);
                y+=10;
            }
            else if (y == y2 && x > x2) { // Baixo -> Entra na RC 3
                QMutexLocker locker(mutexes[3]);
                x-=10;
            }
            else {                      // Esquerda -> Entra na RC 0
                QMutexLocker locker(mutexes[0]);
                y-=10;
            }
            break;

        case 3: // Trem 3 (Azul, Top-Right, Horário)
            if (y == y1 && x < x4)      // Topo
                x+=10;
            else if (x == x4 && y < y2) // Direita
                y+=10;
            else if (y == y2 && x > x3) { // Baixo -> Entra na RC 4
                QMutexLocker locker(mutexes[4]);
                x-=10;
            }
            else {                      // Esquerda -> Entra na RC 1
                QMutexLocker locker(mutexes[1]);
                y-=10;
            }
            break;

        case 4: // Trem 4 (Laranja, Bot-Left, Horário)
            if (y == y2 && x < x2) {    // Topo -> Entra na RC 2 e 3
                QMutexLocker locker(mutexes[2]);
                QMutexLocker locker2(mutexes[3]);
                x+=10;
            }
            else if (x == x2 && y < y3) { // Direita -> Entra na RC 5
                QMutexLocker locker(mutexes[5]);
                y+=10;
            }
            else if (y == y3 && x > x1) // Baixo
                x-=10;
            else                        // Esquerda
                y-=10;
            break;

        case 5: // Trem 5 (Roxo, Bot-Right, Horário)
            if (y == y2 && x < x4) {    // Topo -> Entra na RC 4
                 QMutexLocker locker(mutexes[4]);
                x+=10;
            }
            else if (x == x4 && y < y3) // Direita
                y+=10;
            else if (y == y3 && x > x3) // Baixo
                x-=10;
            else {                      // Esquerda -> Entra na RC 5 e 6
                QMutexLocker locker(mutexes[5]);
                QMutexLocker locker2(mutexes[6]);
                y-=10;
            }
            break;

        case 6: // Trem 6 (Preto, Externo, Anti-Horário)
            if (y == y3 && x < (x4+170))
                x+=10;
            else if (x == (x4+170) && y > (y1-100))
                y-=10;
            else if (y == (y1-100) && x > (x1-170))
                x-=10;
            else
                y+=10;
            break;

        default:
            break;
        }
        emit updateGUI(ID, x,y);    //Emite um sinal para atualizar a GUI
        msleep(sleepTime);
    }
}