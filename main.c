#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LIN 10
#define COL 20
#define QTD_BOMBAS 40


/*
    Celula
    eBomba: true ou false
    estaAberta: true ou false
    vizinhos: 0 a 4
*/
typedef struct {
    int eBomba;
    int estaAberta;
    int vizinhos;
} Celula;



Celula jogo[LIN][COL];



// Inicializar matriz jogo
void inicializarJogo() {
    for (int i = 0; i < LIN; i++) {
        for (int j = 0; j < COL; j++) {
            jogo[i][j].eBomba = 0;
            jogo[i][j].estaAberta = 0;
            jogo[i][j].vizinhos = 0;
        }
    }
}


// Sortear N bombas
void sortearBombas() {
    int i, j, k;
    srand(time(NULL));
    for (k = 0; k < QTD_BOMBAS; k++) {
        i = rand() % LIN;
        j = rand() % COL;
        if (jogo[i][j].eBomba == 0)
            jogo[i][j].eBomba = 1;
        else
            k--;
    }
}


// Verifica se a coordenada ij é válida
int coordenadaEhValida(int i, int j) {
    if (i >= 0 && i < LIN && j >= 0 && j < COL)
        return 1;
    return 0;
}


// Retorna a quantidade de bombas vizinhas da coordenada ij
int quantBombasVizinhas(int i, int j) {
    int quantidade = 0;
    if (coordenadaEhValida(i-1, j) && jogo[i-1][j].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i+1, j) && jogo[i+1][j].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i, j-1) && jogo[i][j-1].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i, j+1) && jogo[i][j+1].eBomba)
        quantidade += 1;
    return quantidade;
}


// Contar bombas viznhas
void contarBombas() {
    for (int i = 0; i < LIN; i++) {
        for (int j = 0; j < COL; j++)
            jogo[i][j].vizinhos = quantBombasVizinhas(i, j);
    }
}


void printLinhaDeSeparacao(){
    printf("\n   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
}
void printTitulo() {
    printf("\n==================================  CAMPO MINADO  ==================================\n\n");
}

// Imprimir matriz jogo
void imprimir() {
    printTitulo();

    printf("   ");
    for (int j = 0; j < COL; j++){
        if (j < 10)
            printf("  %d ", j); 
        else
            printf(" %d ", j);   
    }
    printLinhaDeSeparacao();
    for (int i = 0; i < LIN; i++) {
        printf("%d  |", i);
        for (int j = 0; j < COL; j++){
            if (jogo[i][j].estaAberta){
                if (jogo[i][j].eBomba)
                    printf(" * ");
                else
                    printf(" %d ", jogo[i][j].vizinhos);
            } else {
                printf("   ");
            }
            printf("|");
        }
        printLinhaDeSeparacao();
    }
}


int main(int argc, char const *argv[]) {

    inicializarJogo();
    sortearBombas();
    contarBombas();
    imprimir();

    return 0;
}
