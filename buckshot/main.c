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

struct arma {
    struct bala *balas;
    struct bala *random;
    int qtd;
    int prox;
};



struct bala* criaBalas(int);
struct arma carregarArma();
struct player* criarPlayer();
void color(int);
void jogada(int*,struct player*,struct player*,struct arma*);
void printBalas(struct arma*);

void main() {
    struct player *p1, *p2;
    p1 = criarPlayer();
    p2 = criarPlayer();


    struct arma shotgun;
    shotgun = carregarArma();

    int *rodada = 0;
    while(p1->life > 0 && p2->life > 0) {
        for(int i=0; i<shotgun.qtd; i++) {
            printf("[%d]", shotgun.random[i]);
        }
        printBalas(&shotgun);
        printf("[P1: %d]-x-[P2: %d]\n", p1->life, p2->life);
        jogada(&rodada, p1, p2, &shotgun);
        if((shotgun.prox) == (shotgun.qtd)) {
            shotgun = carregarArma();
        }
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

struct bala *criaBalas(int qtd) {
    srand(time(NULL));

    struct bala *balas;
    balas = (struct bala*)malloc((qtd) * sizeof(struct bala));

    struct bala bala;
    for(int i=0; i<(qtd); i++) {
        bala.carregada = rand()%2;
        balas[i] = bala;
    }
    return balas;
}

struct arma carregarArma() {
    srand(time(NULL));

    struct arma arma;
    struct bala *lista, *random, temp;
    int i,j;

    int qtd;
    qtd = 3 + rand()%6;
    lista = criaBalas(qtd);
    random = criaBalas(qtd);

    arma.balas = lista;
    arma.random = random;
    arma.qtd = qtd;
    arma.prox = 0;

    for(i=0; i<qtd; i++) {
        arma.random[i] = arma.balas[i];
    }

    for(i=qtd-1; i>0; i--) {
        j = rand()%(qtd);
        temp = arma.random[i];
        arma.random[i] = arma.random[j];
        arma.random[j] = temp;
    }


    return arma;
}

struct player* criarPlayer(){
    struct player *p;
    p = (struct player*)malloc(sizeof(struct player));
    p->life = 5;
    return p;
};

void printBalas(struct arma* arma) {
    int qtd = arma->qtd;
    for(int i=0; i<qtd; i++) {
        printf("[");
        if(arma->balas[i].carregada) color(12);
        else color(8);
        printf("#");
        color(15);
        printf("]");
    }
    printf("\n");
};

void printVida(struct player* p1, struct player* p2) {
    printf("[P1: %d]-x-[P2: %d]\n", p1->life, p2->life);
}

void color(int n) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, n);
}

void jogada(int *rodada, struct player *p1, struct player *p2, struct arma* arma) {
    int choice;
    int tiro = arma->prox;

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
            p2->life -= arma->random[tiro].carregada;
            arma->prox++;
            (*rodada)++;
        }
        else {
            p1->life -= arma->random[tiro].carregada;
            arma->prox++;
        }
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
            p1->life -= arma->random[tiro].carregada;
            arma->prox++;
            (*rodada)++;
        }
        else {
            p2->life -= arma->random[tiro].carregada;
            arma->prox++;
        }
    }
    system("cls");
    color(15);
}
