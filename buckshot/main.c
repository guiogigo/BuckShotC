#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

/*
MUDANÇAS ATUAIS:
- Melhoria no código da função jogada
- Adicionados os seguintes itens:
    - Lata
    - Lupa
    - Cigarro
    - Algemas
    - Faca
*/

struct player {
    int life;
    int cor;
    int qtdItem;
    int lupas;
    int cigarros;
    int latas;
    int algemas;
    int algemado;
    int facas;
};

struct bala {
    int carregada;
    int cor;
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
    p1->cor = 9; //Azul
    p2->cor = 12; //Vermelho

    struct arma shotgun;
    shotgun = carregarArma();

    receberItens(p1);
    receberItens(p2);
    //receberItens(p2);

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
        if(bala.carregada) bala.cor = 12;
        else bala.cor = 8;
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
    p->algemas = 0;
    p->algemado = 0;
    p->facas = 0;
    p->qtdItem = 0;
    return p;
};

void printBalas(struct arma* arma) {
    int qtd = arma->qtd;
    for(int i=0; i<qtd; i++) {
        printf("[");
        color(arma->balas[i].cor);
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
    /*for(int i=0; i<arma->qtd; i++) {
        printf("[%d]", arma->random[i].carregada);
    }*/
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

    struct player *jogador;
    if((*rodada)%2) { //Significa que a rodada é do player 2
        jogador = p2;
        player = 2;
    }
    else { //Significa que a rodada é do player 1
        jogador = p1;
        player = 1;
    }

    color(jogador->cor);

    printf("Rodada do PLAYER %d\n", player);

    exibirItens(jogador);

    do {
        color(15);
        printf("[0] - Atirar em VOCE\n");
        printf("[1] - Atirar no OUTRO\n");
        printf("[2] - Usar um item\n\n");
        printf("Qual voce prefere? ");
        scanf("%d", &choice);
        if(choice < 0 || choice > 2) printf("Insira uma valor de 0 a 2.\n");
        if(choice == 2) {
            item = usarItens(jogador, arma);

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
        if(p1->algemado || p2->algemado) {
            p1->algemado = 0;
            p2->algemado = 0;
        } else (*rodada)++;
    }
    else {
        jogador->life -= arma->random[tiro].carregada;
        arma->prox++;
    }

    system("cls");
    color(15);
}

void receberItens(struct player *player) {
    if(player->qtdItem < 8) {
        player->cigarros+=2;
        player->lupas+= 2;
        player->latas+=2;
        player->algemas+=2;
        player->facas+=2;
        player->qtdItem+= 10;
    }
}

void exibirItens(struct player *player) {
    int qtd = 0;
    color(player->cor);
    printf("========<ITENS>========\n");
    printf("[LATA: %d]\t", player->latas);
    printf("[ALGEMAS: %d]\t\n", player->algemas);
    printf("[LUPA: %d]\t", player->lupas);
    printf("[CIGARRO: %d]\t\n", player->cigarros);
    printf("[FACA: %d]\t\n", player->facas);
    printf("=======================\n");
    printf("\n");
    color(15);
}

int usarItens(struct player *player, struct arma *shotgun) {
    int choice, bala;

    do {
        system("cls");
        printBalas(shotgun);
        printf("[Vida atual: %d]\n", player->life);
        color(player->cor);
        printf("========<ITENS>========\n");
        printf("[1] - [LATA: %d]\t", player->latas);
        printf("[2] - [ALGEMAS: %d]\t\n", player->algemas);
        printf("[3] - [LUPA: %d]\t", player->lupas);
        printf("[4] - [CIGARRO: %d]\t\n", player->cigarros);
        printf("[5] - [FACA: %d]\t\n", player->facas);
        printf("=======================\n");
        printf("[0] - Voltar\n");
        printf("=======================\n");

        printf("Qual item voce vai usar? ");
        scanf("%d", &choice);

        if(choice == 0 || choice == 120) return 0;
        if(choice < 1 || choice > 5) printf("Insira uma valor de 1 a 5.\n");


    switch (choice) {
    case 1:
        player->latas--;
        player->qtdItem--;
        bala = shotgun->prox;

        color(15);
        printf("Voce usou a lata e removeu uma [");
        color(shotgun->random[bala].cor);
        printf("#");
        color(15);
        printf("] da arma\n");

        shotgun->prox++;

        color(player->cor);
        continuar();
        return 0;
    case 2:
        player->algemas--;
        player->qtdItem--;
        player->algemado = 1;

        color(15);
        printf("Voce algemou o OUTRO, ");
        color(12);
        printf("pode atirar sem medo...\n");
        color(15);

        color(player->cor);
        continuar();
        return 0;
    case 3:
        player->lupas--;
        player->qtdItem--;
        bala = shotgun->prox;
        color(15);
        printf("Voce usou a lupa e encontrou: [");
        color(shotgun->random[bala].cor);
        printf("#");
        color(15);
        printf("], a escolha e sua.\n");

        color(player->cor);
        continuar();
        return 0;
    case 4:
        color(15);
        if(player->life < 5) {
            player->life++;
            player->qtdItem--;
            player->cigarros--;

            printf("Sua nova vida e: ");
            color(10);
            printf("[%d]\n", player->life);
        } else printf("Sua vida ja esta no maximo\n");

        color(player->cor);
        continuar();
        return 0;
    case 5:
        player->facas--;
        player->qtdItem--;
        shotgun->random[shotgun->prox].carregada *= 2;

        color(15);
        printf("Voce cortou o cano da sua arma, vai causar mais dano...\n");

        color(player->cor);
        continuar();
        return 0;
    }
    choice = NULL;
    } while(choice < 1 || choice > 4);
}

void continuar() {
    printf("Pressione ENTER para continuar:");
    fflush(stdin);
    getchar();
}
