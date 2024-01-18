#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

struct player {
    int life;
};

struct bala {
    int carregada;
};

int* criaBalas(int*);
struct player* criarPlayer();
void printBalas(struct bala*,int*);
void color(int);
void jogada(int*,struct player*,struct player*);

void main() {
    struct player *p1, *p2;
    p1 = criarPlayer();
    p2 = criarPlayer();

    int *qtd;
    struct bala *balas;
    balas = criaBalas(&qtd);
    //printBalas(balas);

    int *rodada = 0;
    while(p1->life > 0 && p2->life > 0) {
        printBalas(balas,&qtd);
        printf("[P1: %d]-x-[P2: %d]\n", p1->life, p2->life);
        jogada(&rodada, p1, p2);

    }
    if(p1->life > p2->life) {
        color(10);
        printf("O PLAYER 1 VENCEU");
    } else {
        color(10);
        printf("O PLAYER 2 VENCEU");
    }
    color(15);
}

int* criaBalas(int*qtd) {
    srand(time(NULL));

    struct bala *balas;
    *qtd = 3 + rand()%6;

    balas = (struct bala*)malloc((*qtd) * sizeof(struct bala));

    struct bala bala;
    for(int i=0; i<(*qtd); i++) {
        bala.carregada = rand()%2;
        balas[i] = bala;
    }
    return balas;
}

struct player* criarPlayer(){
    struct player *p;
    p = (struct player*)malloc(sizeof(struct player));
    p->life = 5;
    return p;
};

void printBalas(struct bala* balas,int*qtd) {
    for(int i=0; i<(*qtd); i++) {
        printf("[");
        if(balas[i].carregada) color(8);
        else color(12);
        printf("#");
        color(15);
        printf("]");
    }
    printf("\n");
}

void printVida(struct player* p1, struct player* p2) {
    printf("[P1: %d]-x-[P2: %d]\n", p1->life, p2->life);
}

void color(int n) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, n);
}

void jogada(int *rodada, struct player *p1, struct player *p2) {
    int choice;

    if((*rodada)%2 == 0) {
        color(9);
        printf("Rodada do PLAYER 1\n");
        printf("[0] - Atirar em VOCE\n");
        printf("[1] - Atirar no OUTRO\n");

        do {
            printf("Qual voce prefere? ");
            scanf("%d", &choice);
            if(choice < 0 || choice > 1) printf("Insira uma valor entre 0 e 1.\n");
        } while(choice < 0 || choice > 1);

        if(choice) {
            p2->life--;
            (*rodada)++;
        }
        else p1->life--;
    }
    else {
        color(12);
        printf("Rodada do PLAYER 2\n");
        printf("[0] - Atirar em VOCE\n");
        printf("[1] - Atirar no OUTRO\n");

        do {
            printf("Qual voce prefere? ");
            scanf("%d", &choice);
            if(choice < 0 || choice > 1) printf("Insira uma valor entre 0 e 1.\n");
        } while(choice < 0 || choice > 1);

        if(choice) {
            p1->life--;
            (*rodada)++;
        }
        else p2->life--;
    }
    system("cls");
    color(15);
}
