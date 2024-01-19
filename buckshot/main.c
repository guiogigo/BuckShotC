#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

/*
MUDANÇAS ATUAIS:
- Melhoria no código da função jogada
*/

struct player {
    int life;
    int qtdItem;
    int lupas;
    int cigarros;
    int latas;
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
void continuar();

void jogada(int*,struct player*,struct player*,struct arma*);
void printBalas(struct arma*);
void printHud(struct player *,struct player *,struct arma*);
void receberItens(struct player*);
void exibirItens(struct player *);
int usarItens(struct player *,struct arma *);

void main() {
    struct player *p1, *p2;
    p1 = criarPlayer();
    p2 = criarPlayer();


    struct arma shotgun;
    shotgun = carregarArma();

    receberItens(p1);
    receberItens(p2);
    receberItens(p2);

    int *rodada = 0;
    //int *recargas = 1;
    while(p1->life > 0 && p2->life > 0) {
            /*
        for(int i=0; i<shotgun.qtd; i++) {
            printf("[%d]", shotgun.random[i].carregada);
        }
        printBalas(&shotgun);
        printf("[P1: %d]-x-[P2: %d]\n", p1->life, p2->life);
            */
        //printf("Recargas: %d\n", recargas);

        jogada(&rodada, p1, p2, &shotgun);
        if((shotgun.prox) == (shotgun.qtd)) {
            shotgun = carregarArma();
            //recargas++;
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
    p->lupas = 0;
    p->cigarros = 0;
    p->latas = 0;
    p->qtdItem = 0;
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

void printHud(struct player *p1, struct player *p2, struct arma* arma) {
    for(int i=0; i<arma->qtd; i++) {
        printf("[%d]", arma->random[i].carregada);
    }
    printBalas(arma);
    printf("[P1: %d]-x-[P2: %d]\n", p1->life, p2->life);
}

void color(int n) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, n);
}

void jogada(int *rodada, struct player *p1, struct player *p2, struct arma* arma) {


    int choice, player, item;
    int tiro = arma->prox;
    printHud(p1, p2, arma);


    if((*rodada)%2) { //Significa que a rodada é do player 2
        color(12);
        player = 2;
    }
    else { //Significa que a rodada é do player 1
        color(9);
        player = 1;
    }

    printf("Rodada do PLAYER %d\n", player);

    if(player == 1)exibirItens(p1);
    else exibirItens(p2);


    do {
        printf("[0] - Atirar em VOCE\n");
        printf("[1] - Atirar no OUTRO\n");
        printf("[2] - Usar um item\n");
        printf("Qual voce prefere? ");
        scanf("%d", &choice);
        if(choice < 0 || choice > 2) printf("Insira uma valor de 0 a 2.\n");
        if(choice == 2) {
            if(player == 1) item = usarItens(p1, arma);
            else item = usarItens(p2, arma);

            if(!item){
                color(15);
                system("cls");
                return;
            }
        }
    } while(choice < 0 || choice > 1);



    if(choice == 1) {
        if(player == 1) p2->life -= arma->random[tiro].carregada;
        else p1->life -= arma->random[tiro].carregada;
        arma->prox++;
        (*rodada)++;
    }
    else {
        if(player == 1) p1->life -= arma->random[tiro].carregada;
        else p2->life -= arma->random[tiro].carregada;
        arma->prox++;
    }



    system("cls");
    color(15);
}

void receberItens(struct player *player) {
    if(player->qtdItem < 8) {
        player->cigarros++;
        player->lupas+= 4;
        player->latas+= 3;
        player->qtdItem+= 2;
    }
}

void exibirItens(struct player *player) {
    int qtd = 0;
    printf("========<ITENS>========\n");
    printf("[LATA: %d]\t", player->latas);
    printf("[ALGEMAS: %d]\t\n", qtd);
    printf("[LUPA: %d]\t", player->lupas);
    printf("[CIGARRO: %d]\t\n", player->cigarros);
    printf("=======================\n");
    printf("\n");
}

int usarItens(struct player *player, struct arma *shotgun) {
    int qtd = 0;
    int choice, bala;

    do {
        system("cls");
        printf("========<ITENS>========\n");
        printf("[1] - [LATA: %d]\t", player->latas);
        printf("[2] - [ALGEMAS: %d]\t\n", qtd);
        printf("[3] - [LUPA: %d]\t", player->lupas);
        printf("[4] - [CIGARRO: %d]\t\n", player->cigarros);
        printf("=======================\n");
        printf("[0] - Voltar\n");
        printf("=======================\n");

        printf("Qual item voce vai usar? ");
        scanf("%d", &choice);

        if(choice == 0 || choice == 120) return 0;
        if(choice < 1 || choice > 4) printf("Insira uma valor de 0 a 3.\n");


    switch (choice) {
    case 1:
        player->latas--;
        player->qtdItem--;
        bala = shotgun->prox;
        printf("Você usou a lata e removeu: [%d]\n", shotgun->random[bala].carregada);
        shotgun->prox++;

        continuar();
        break;
    case 3:
        player->lupas--;
        player->qtdItem--;
        bala = shotgun->prox;
        printf("Você usou a lupa e encontrou: [%d]\n", shotgun->random[bala].carregada);
        /*for(int i=bala; bala<shotgun->qtd; bala++) {
        printf("[%d]", shotgun->random[bala].carregada);
        }
        printf("\n");*/

        continuar();
        break;
    case 4:
        if(player->life < 5) {
            player->life++;
            player->qtdItem--;
            player->cigarros--;
            printf("Sua vida atual agora é: [%d]\n", player->life);
        } else printf("Sua vida ja esta no maximo\n");

        continuar();
        break;
    }
    choice = NULL;
    } while(choice < 1 || choice > 4);
}

void continuar() {
    printf("Pressione ENTER para continuar:");
    fflush(stdin);
    getchar();
}
