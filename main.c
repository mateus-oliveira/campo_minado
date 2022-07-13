#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define LIN 10
#define COL 20
#define QTD_BOMBAS 40
#define LIM_AJUDA 10


// limpar texto no terminal
void limparTela() {
    system("clear");
}

/*
    Celula
    eBomba: 0 ou 1
    estaAberta: 0 ou 1
    vizinhos: 0 a 8
*/
typedef struct {
    int eBomba;
    int estaAberta;
    int vizinhos;
} Celula;

Celula campo[LIN][COL];


// Função de ajuda ao jogador
void ajuda(int * count){
    int i, j, k = 1;
    srand(time(NULL));

    if (*count < LIM_AJUDA){
        while (k > 0) {
            i = rand() % LIN;
            j = rand() % COL;
            if (campo[i][j].eBomba == 0 && campo[i][j].estaAberta == 0){
                campo[i][j].estaAberta = 1;
                (*count) += 1;
                k = -1;
            }
        }
        printf("Céclula aberta: %dx%d\n\n", i, j);
    } else {
        printf("É permitido pedir ajuda somente %d vezes!\n\n", LIM_AJUDA);
    }
}


void tempo(clock_t inicio){
    clock_t fim;
    fim = clock();
    printf("Seu tempo é: %.2f s\n\n", (fim - inicio) / (float) 1000);
}


// Inicializar matriz campo
void inicializarJogo() {
    for (int i = 0; i < LIN; i++) {
        for (int j = 0; j < COL; j++) {
            campo[i][j].eBomba = 0;
            campo[i][j].estaAberta = 0;
            campo[i][j].vizinhos = 0;
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
        if (campo[i][j].eBomba == 0)
            campo[i][j].eBomba = 1;
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
    if (coordenadaEhValida(i-1, j) && campo[i-1][j].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i+1, j) && campo[i+1][j].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i, j-1) && campo[i][j-1].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i, j+1) && campo[i][j+1].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i-1, j-1) && campo[i-1][j-1].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i-1, j+1) && campo[i-1][j+1].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i+1, j-1) && campo[i+1][j-1].eBomba)
        quantidade += 1;
    if (coordenadaEhValida(i+1, j+1) && campo[i+1][j+1].eBomba)
        quantidade += 1;
    return quantidade;
}


// Contar bombas viznhas
void contarBombas() {
    for (int i = 0; i < LIN; i++) {
        for (int j = 0; j < COL; j++)
            campo[i][j].vizinhos = quantBombasVizinhas(i, j);
    }
}


void printLinhaDeSeparacao(){
    printf("\n   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
}
void printTitulo() {
    printf("\n==================================  CAMPO MINADO  ==================================\n\n");
}


// Imprimir matriz campo
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
            if (campo[i][j].estaAberta){
                if (campo[i][j].eBomba)
                    printf(" * ");
                else
                    printf(" %d ", campo[i][j].vizinhos);
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
    if (coordenadaEhValida(i, j) && campo[i][j].estaAberta == 0){
        campo[i][j].estaAberta = 1;
        if (campo[i][j].vizinhos == 0) {
            abrirCelula(i-1, j);
            abrirCelula(i+1, j);
            abrirCelula(i, j-1);
            abrirCelula(i, j+1);
            abrirCelula(i-1, j-1);
            abrirCelula(i-1, j+1);
            abrirCelula(i+1, j-1);
            abrirCelula(i+1, j+1);
        }
    }
}


void salvarESair(){
  
}


void menuJogo(clock_t inicio, int * count, void (*callback)()){
    int opcao;

    limparTela();
    
    printf("1 - Continuar\n2 - Ajuda\n3 - Tempo de Jogo\n4 - Salvar e Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    limparTela();
    switch(opcao) {
        case 1:
            (*callback) ();
            break;
        case 2:
            ajuda(count);
            (*callback) ();
            break;
        case 3:
            tempo(inicio);
            (*callback) ();
            break;
        case 4:
            salvarESair();
            break;
        default:
            break;
    }
}


// Verificar vitória ou derrota
int ganhou() {
    int quantidade = 0;
    for (int i = 0; i < LIN; i++) {
        for (int j = 0; j < COL; j++){
            if (campo[i][j].estaAberta == 0 && campo[i][j].eBomba == 0)
                quantidade += 1;
        }
    }
    return quantidade;
}


// Entrada das coordenadas pelo usuário
void jogar() {
    int i, j; // i: linha; j: coluna
    void (*callback) () = &imprimir;
    static int countAjuda = 0;
    clock_t inicio;
    inicio = clock();
    limparTela();
    do {
        imprimir();
        do {
            printf("Digite as coordenadas IxJ (Ex.: 9x19) ou -1 para o menu: ");
            scanf("%d", &i);
            if (i == -1){
                menuJogo(inicio, &countAjuda, callback);
                continue;
            } else {
                scanf("x%d", &j);
            }
            if (coordenadaEhValida(i, j) == 0){
                printf("\nCoordenada inválida!\n");
                continue;
            }
            if (campo[i][j].estaAberta == 1) 
                printf("\nCoordenada já aberta!\n");
        } while (coordenadaEhValida(i, j) == 0 || campo[i][j].estaAberta == 1);
        abrirCelula(i, j);
        limparTela();
    } while (ganhou() != 0 && campo[i][j].eBomba == 0);
    
    if (campo[i][j].eBomba == 1)
        printf("\nPerdeu!\n");
    else
        printf("\nParabéns!\n");
    imprimir();
}


// Função que inicia um novo jogo
void novoJogo(){
    inicializarJogo();
    sortearBombas();
    contarBombas();
    jogar();
}


void continuarJogo(){
  
}


void modoAutonomo(){
  
}


void recordes(){
  
}


void menuInicial(){
    int opcao;
    
    printf("1 - Novo Jogo\n2 - Continuar\n3 - Modo Autônomo\n4 - Listar Recordes\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1: 
            novoJogo();
            break;
        case 2:
            continuarJogo();
            break;
        case 3:
            modoAutonomo();
            break;
        case 4:
            recordes();
            break;
        default:
            break;
    }
}


// Função main
int main(int argc, char const *argv[]) {
    menuInicial();
    return 0;
}
