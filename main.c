#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define LIN 10
#define COL 20
#define QTD_BOMBAS 40


void limparTela() {
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}


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


// Abrir coordenada
void abrirCelula(int i, int j) {
    if (coordenadaEhValida(i, j) && jogo[i][j].estaAberta == 0){
        jogo[i][j].estaAberta = 1;
        if (jogo[i][j].vizinhos == 0) {
            abrirCelula(i-1, j);
            abrirCelula(i+1, j);
            abrirCelula(i, j-1);
            abrirCelula(i, j+1);
        }
    }
}


// Verificar vitória ou derrota
int ganhou() {
    int quantidade = 0;
    for (int i = 0; i < LIN; i++) {
        for (int j = 0; j < COL; j++){
            if (jogo[i][j].estaAberta == 0 && jogo[i][j].eBomba == 0)
                quantidade += 1;
        }
    }
    return quantidade;
}


// Entrada das coordenadas pelo usuário
void jogar() {
    int i, j; // i: linha; j: coluna
    limparTela();
    do {
        imprimir();
        do {
            printf("Digite as coordenadas IxJ (Ex.: 9x19): ");
            scanf("%dx%d", &i, &j);
            if (coordenadaEhValida(i, j) == 0){
                printf("\nCoordenada inválida!\n");
                continue;
            }
            if (jogo[i][j].estaAberta == 1) 
                printf("\nCoordenada já aberta!\n");
        } while(coordenadaEhValida(i, j) == 0 || jogo[i][j].estaAberta == 1);
        abrirCelula(i, j);
        limparTela();
    } while (ganhou() != 0 && jogo[i][j].eBomba == 0);
    
    if (jogo[i][j].eBomba == 1)
        printf("\nPerdeu!\n");
    else
        printf("\nParabéns!\n");
    imprimir();
}


// Função main
int main(int argc, char const *argv[]) {

    inicializarJogo();
    sortearBombas();
    contarBombas();
    
    jogar();

    return 0;
}
