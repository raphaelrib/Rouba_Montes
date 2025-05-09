#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


typedef struct carta{
    int numero;
    int naipe; //1-COPAS, 2-OUROS, 3-ESPADAS, 4-PAUS
}carta;


////////////////////////////////////////////////
//Structs para Pilha
typedef struct elementoPILHA{
    carta *cartapilha;
    struct elementoPILHA *anterior;
}elementoPILHA;

typedef struct pilha{
    elementoPILHA *topo;
}pilha;


////////////////////////////////////////////////
//Structs para Lista
typedef struct elementoLISTA{
    carta *cartalista;
    struct elementoLISTA *proximo;
}elementoLISTA;

typedef struct lista{
    int tamanho;
    elementoLISTA *inicio;
}lista;


////////////////////////////////////////////////
//Structs para o Jogador

typedef struct player{
    char nome[30];
    pilha *monte;  //MÃO
    int tamanho;
}player;


//APENAS RETORNA O NOME DO NAIPE
const char* naipe(carta* c){

    if(c->naipe == 1){
        return "COPAS";
    }

    else if(c->naipe == 2){
        return "OUROS";
    }

    else if(c->naipe == 3){
        return "ESPADAS";
    }

    else if(c->naipe == 4){
        return "PAUS";
    }
}


////////////////////////////////////////////////
//Funções Dinâmicas para Epilhar 
pilha* criapilha();
void empilha(pilha* p, carta *c);
carta* desempilha(pilha* p, carta* c);


////////////////////////////////////////////////
//Funções Dinâmicas para Lista
lista *crialista();
void inserirultimaposicao(lista* l, carta* c);
carta* removernaposicao(lista *l, carta* c);
int recuperarporchave(lista *l, carta *chave);


////////////////////////////////////////////////
//Funções para o jogo
void imprimeinicio();
carta* criacarta(int j, int k);
player* criajogador(int quantidade);
pilha* criabaralho(pilha * baralho, int qbaralhos);
char mostracarta(int numero);
int imprimeescolha(int contajogadores, int contadescarte);
void imprimejogadores(player jogador[], int q);
void imprimirDescarte(lista* descarte);
pilha* embaralhar(pilha *baralho, int quantidade);
void jogar(player jogador[], int q, pilha* baralho, int qbaralhos, lista* descarte);
void ordenadescarte(lista* l);
void ordenamontes(player jogador[], int q);
void liberar(lista* l, pilha* p, player jogador[], int quantidade);
void SAIR(lista* l, pilha* p, player jogador[], int quantidade);
void estatisticas(player jogador[], int i);



int main (){
    
    
    int qbaralhos = 0, qjogadores = 0, escolha = 1, a = 0;
    int* k;

    do{
        imprimeinicio();


        printf("\nDigite a quantidade de jogadores: ");
        scanf("%d", &qjogadores);
        printf("\nDigite a quantidade de baralhos: ");
        scanf("%d", &qbaralhos);

        //NECESSARIO PARA ELIMINAR Oa ENTER DO SCANF
        getchar();
        
        player* jogador = criajogador(qjogadores);

        for(int i=0; i<qjogadores; i++){

            printf("\nDigite o nome do jogador %d", i+1);
            fgets(jogador[i].nome, 30, stdin);
            printf("\n");
        }

        pilha* baralho = criapilha();
        baralho = criabaralho(baralho, qbaralhos);/////////
        lista* descarte = crialista();

        carta* caux;

        for(int i=0; i<qbaralhos*52; i++){//////////////
            
            caux = desempilha(baralho, caux);
            empilha(jogador[1].monte, caux);
            jogador[1].tamanho++;
        }        

        //jogar(jogador, qjogadores, baralho, qbaralhos, descarte);
        getchar();
        //PARA COLOCAR OS JOGADORES NA ORDEM DE VENCEDORES

        //k é utilizado apenas para definir a ordem de vencedores
        k =(int*) malloc(qjogadores*sizeof(int));
        for(int i=0; i<qjogadores; i++){
            k[i] = i+1;
        }

        int i = 0, j = 0, aux = 0, n = qjogadores;

        for(j = n-1; j>0; j--){
            for(i = 0; i<j; i++){
                if(jogador[i+1].tamanho > jogador[i].tamanho){
                    aux = k[i+1];
                    k[i+1] = k[i];
                    k[i] = aux;
                }
            }
        }

        //system("clear");
        imprimeinicio();

        getchar();

        for(int i=0; i<qjogadores; i++){
            a = k[i]-1;
            ordenamontes(jogador, a);
            estatisticas(jogador, a);
        }

        imprimirDescarte(descarte);

        printf("\n\nDeseja jogar novamente ?");
        printf("Digite 1 para SIM e 0 para SAIR");
        scanf("%d", &escolha);
        
        if(escolha != 0){
            liberar(descarte, baralho, jogador, qjogadores);
        }
        else{
            SAIR(descarte, baralho, jogador, qjogadores);
        }

    }while(escolha != 0);
    
    return 0;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇÕES PARA PILHA DINÂMICA
pilha* criapilha(){

    pilha *p = (pilha*) malloc(sizeof(pilha));
    p->topo = NULL;
    return p;
}


void empilha(pilha* p, carta *c){

    elementoPILHA *novo = (elementoPILHA*) malloc(sizeof(elementoPILHA));
    novo->cartapilha = criacarta(0, 0);
    
    novo->cartapilha = c;
    novo->anterior = p->topo;
    p->topo = novo;
}


carta* desempilha(pilha* p, carta* c) {
    if (p->topo == NULL) {
        return NULL;
    }

    elementoPILHA *aux = p->topo;
    c = aux->cartapilha;

    p->topo = aux->anterior;
    free(aux);

    return c;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇÕES PARA LISTA DINÂMICA
lista *crialista(){

    lista *l = (lista*) malloc(sizeof(lista));
    l->tamanho = 0;
    l->inicio = NULL;
    return l;
}


void inserirultimaposicao(lista* l, carta* c){

    elementoLISTA *aux = l->inicio;
    elementoLISTA *novo = (elementoLISTA*) malloc(sizeof(elementoLISTA));
    novo->cartalista = criacarta(0, 0);

    if(l->tamanho == 0){
        l->inicio = novo;
        novo->proximo = NULL;
        novo->cartalista = c;
        l->tamanho++;
        return;
    }

    while(aux->proximo != NULL){
        aux = aux->proximo;
    }

    aux->proximo = novo;
    novo->cartalista = c;
    novo->proximo = NULL;
    l->tamanho++;

    ordenadescarte(l);

}



carta* removernaposicao(lista *l, carta* c){

    int posicao = recuperarporchave(l, c);

    elementoLISTA *ant = l->inicio;
    elementoLISTA *prox = l->inicio;
    carta* caux = criacarta(0, 0);

    if(posicao == 1){
        l->inicio = prox->proximo;
    }

    else{
        for(int i=1; i<posicao; i++){
            ant = prox;
            prox = prox->proximo;
        }
        ant->proximo = prox->proximo;
    }

    caux = prox->cartalista;
    l->tamanho--;
    free(prox);
    return caux;
}


int recuperarporchave(lista *l, carta *chave){

    int posicao = 1;
    elementoLISTA *aux = l->inicio;

    for(int i=0; i<=l->tamanho; i++){
        if(aux != NULL){
            if(aux->cartalista->numero == chave->numero && aux->cartalista->naipe == chave->naipe){
                return posicao;
            }
            posicao++;
        aux = aux->proximo;
        }
    }

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇÕES PARA O JOGO

void imprimeinicio(){

    printf("\n\n\033[1;31m************************************************************");
    printf("\n************************************************************");
    printf("\n***                                                      ***");
    printf("\n***                                                      ***");
    printf("\n***                    \033[1;34m ROUBA MONTES \033[1;31m                    ***");
    printf("\n***                                                      ***");
    printf("\n***                                                      ***");
    printf("\n************************************************************");
    printf("\n************************************************************");
    printf("\033[0m");
}


carta* criacarta(int j, int k){  //1-COPAS, 2-OUROS, 3-ESPADAS, 4-PAUS

    carta *c = (carta*) malloc(sizeof(carta));
    c->numero = k;
    c->naipe = j;
    return c;
}


player* criajogador(int quantidade){

    player* jogador = (player*) malloc(quantidade*sizeof(player));

    for(int i=0 ; i<quantidade; i++){
        jogador[i].monte = (pilha*) malloc(sizeof(pilha));
        jogador[i].monte->topo = NULL;
        jogador[i].tamanho = 0;
    }
    return jogador;
}


pilha* criabaralho(pilha * baralho, int qbaralhos){

    int i = 0, j = 0, k = 0, cartastotal = 0;
    carta* c;

    cartastotal = qbaralhos * 52;

    for(i=0; i<qbaralhos; i++){
        for(j=1; j<5; j++){
            for(k=1; k<14; k++){
                c = criacarta(j, k);
                empilha(baralho, c);
            }
        }
    }

    return baralho = embaralhar(baralho, cartastotal);

}


//RETORNA AS CARTAS QUE SÃO LETRAS
char mostracarta(int numero){
    if(numero == 1){
        return 'A';
    }

    else if(numero == 11){
        return 'J';
    }

    else if(numero == 12){
        return 'Q';
    }

    else if(numero == 13){
        return 'K';
    }
}


//IMPRIME A AÇÃO DO JOGADOR
int imprimeescolha(int contajogadores, int contadescarte){

    int escolha = 0;

    ////////////////adicionar cartas dos jogadores e da area de descarte a mostra
    
    if(contadescarte > 0 && contajogadores > 0){
        do{
            printf("\nSuas opcoes:");
            printf("\n1 - Roubar apenas o jogador.");
            printf("\n2 - Colocar a carta no descarte.");
            printf("\n3 - Roubar apenas do descarte.");
            printf("\n4 - Roubar tudo.");
            printf("\nEscolha uma opcao:");
            scanf("%d", &escolha);

        }while(escolha < 1 && escolha > 4);
        return escolha;
    }

    else if(contadescarte > 0 && contajogadores == 0){
        do{
            printf("\nSuas opcoes:");
            printf("\n1 - Roubar do descarte.");
            printf("\n2 - Colocar a carta no descarte.");
            printf("\nEscolha uma opcao:");
            scanf("%d", &escolha);

        }while(escolha != 1 && escolha != 2);
        return escolha*10;
    }

    else if(contadescarte == 0 && contajogadores > 0){
        do{
            printf("\nSuas opcoes:");
            printf("\n1 - Roubar apenas o jogador.");  //CRIAR CODIÇÃO IF CONTAJOGADORES > 1
            printf("\n2 - Colocar a carta no descarte.");
            printf("\nEscolha uma opcao:");
            scanf("%d", &escolha);

        }while(escolha != 1 && escolha != 2);
        return escolha;
    }
}


//IMPRIME A CARTA DO TOPO DE CADA MONTE DOS JOGADORES
void imprimejogadores(player jogador[], int q){

    //system("clear");
    imprimeinicio();
    carta* cartaaux;
    elementoLISTA* auxlista;
    const char* texto;
    int qcartas = 0;  

    printf("\n\n\033[1;34mJOGADORES");
    for(int i=0; i<q; i++){
        
        if (jogador[i].monte->topo != NULL) {
            cartaaux = jogador[i].monte->topo->cartapilha;
            texto = naipe(cartaaux);

            if(cartaaux->naipe == 1 || cartaaux->naipe == 2){
                printf("\n\033[1;34m%s->\033[1;31m %d - %s ", jogador[i].nome, cartaaux->numero, texto);
            }

            else if(cartaaux->naipe == 3 || cartaaux->naipe == 4){
                printf("\n\033[1;34m%s->\033[0m %d - %s ", jogador[i].nome, cartaaux->numero, texto);
            }

            qcartas = jogador[i].tamanho;
            qcartas = qcartas-(qcartas%10);
            //Apenas para definir mais ou menos uma quantidade de cartas para os jogadores
            if(qcartas > 0 || (jogador[i].tamanho > 0 && jogador[i].tamanho < 11)){
                printf("\033[1;34mMONTE >%d\033[0m", qcartas);
            }  
        }
        else{
            printf("\n\033[1;34m%s\033[1;34m-> SEM PILHA\033[0m", jogador[i].nome);
        }
    }
    printf("\033[0m");
}

//MOSTRA O DESCARTE
void imprimirDescarte(lista *descarte) {
    elementoLISTA *auxlista;
    carta *cartaaux;
    const char *texto;
    
    printf("\n\n\033[1;34mDESCARTE");

    if (descarte->inicio != NULL) {
        auxlista = descarte->inicio;

        while (auxlista != NULL) {
            cartaaux = auxlista->cartalista;
            texto = naipe(cartaaux);

            if (cartaaux->naipe == 1 || cartaaux->naipe == 2) {
                printf("\n\033[0m->\033[1;31m %d - %s ", cartaaux->numero, texto);
            }

            if (cartaaux->naipe == 3 || cartaaux->naipe == 4) {
                printf("\n\033[0m->\033[0m %d - %s ", cartaaux->numero, texto);
            }

            auxlista = auxlista->proximo;
        }

    } else {
        printf("\n\033[1;34m-> DESCARTE VAZIO");
    }

    printf("\033[0m");  // Resetar as cores para os valores padrão
    printf("\n\n");
}


pilha* embaralhar(pilha *baralho, int quantidade){
    srand((unsigned)time(NULL));

    elementoPILHA* PILHAaux;
    elementoPILHA* PILHAdavez;
    carta* CARTAaux = criacarta(0, 0);

    int i = 0, j=0, A = 0;

    for(i=0; i<quantidade; i++)
    {

        PILHAaux = baralho->topo;
        PILHAdavez = baralho->topo;
        A = 1 + (rand() % quantidade);

        if(i > 0){
            for(j=0; j<i; j++){
                PILHAdavez = PILHAdavez->anterior;
            }
        }//PASSA POR TODOS AS CARTAS, FAZ A TROCA UMA A UMA GARANTINDO QUE TODAS FORAM TROCADAS/EMBARALHADAS

        if(A > 1){
            for(j=1; j<A; j++){
                PILHAaux = PILHAaux->anterior;
            }
        }

        CARTAaux = PILHAaux->cartapilha;
        PILHAaux->cartapilha = PILHAdavez->cartapilha;
        PILHAdavez->cartapilha = CARTAaux;
    }
    
    return baralho;
    
}


//TODAS AS MANIPULAÇÕES DO JOGO COLOCADAS AQUI 
void jogar(player jogador[], int q, pilha* baralho, int qbaralhos, lista* descarte){

    carta* c = criacarta(0, 0);
    carta* caux;
    pilha *pilhaaux;
    pilha *pilhaaux2;
    elementoLISTA* listaaux;

    int i = 0, j = 0, k = 0, contadescarte = 0, contajogadores = 0, rodadaextra = 0, escolha = 0;
    char letra;
    const char* auxnaipe;
    int total = 0;

    //Criação e Atribuição das cartas ao monte (baralho).
    baralho = criabaralho(baralho, qbaralhos);

    imprimejogadores(jogador, q);
    imprimirDescarte(descarte);
    
    do{
        printf("\n\033[1;34mVEZ DE %s\033[0m\n", jogador[i].nome);

        //Reseta variáveis.
        contadescarte = 0, contajogadores = 0, rodadaextra = 0;
        pilhaaux = jogador[i].monte;
        listaaux = descarte->inicio;

        printf("\nPressione uma tecla para retirar uma carta do baralho.");
        getchar();

        //Mostra carta retirada
        c = desempilha(baralho, c);

        auxnaipe = naipe(c);
        

        if(c->numero != 1 || c->numero != 11 || c->numero != 12 || c->numero != 13){
            printf("\nCarta retirada: %d - %s", c->numero, auxnaipe);
        }
        else{
            letra = mostracarta(c->numero);
            printf("\nCarta retirada: %c - %d", letra, c->naipe);
        }
        

        //Testa se existe alguma carta disponível no descarte para roubar.
        for(j=0; j<descarte->tamanho; j++){
            if(listaaux != NULL){
                if(listaaux->cartalista->numero == c->numero){
                    contadescarte++;
                }
                listaaux = listaaux->proximo;
            }
        }
        listaaux = descarte->inicio;
        
        //Testa se existe algum monte entre os jogadores disponível para roubar.
        for(j=0; j<q; j++){
            //i é o jogador da vez
            if(i != j){
                if(jogador[j].monte->topo != NULL){
                    if(c->numero == jogador[j].monte->topo->cartapilha->numero){
                        contajogadores++;
                    }
                }
            }
        }

        //Caso não haja carta a ser roubada
        if(contadescarte == 0 && contajogadores == 0){
            if(pilhaaux->topo != NULL && c->numero == pilhaaux->topo->cartapilha->numero){
                printf("\nCarta inserida no topo do seu monte.");
                empilha(pilhaaux, c);
            }
            else{
                printf("\nCarta inserida no descarte, o topo do seu monte e diferente.");
                inserirultimaposicao(descarte, c);
            }
            getchar();
        }

        //Caso haja cartas a serem roubadas
        else{
            escolha = imprimeescolha(contajogadores, contadescarte);
            getchar();

            //PARA ROUBAR AS CARTAS DE UM JOGADOR
            if(escolha == 1 || escolha == 4){
                for(j=0; j<q; j++){
                    //i é o jogador da vez
                    if(i != j && jogador[j].monte->topo != NULL){
                        if(c->numero == jogador[j].monte->topo->cartapilha->numero){
                            pilhaaux2 = jogador[j].monte;
                            pilha* temp = (pilha*) malloc(sizeof(pilha));
                            total = jogador[j].tamanho;

                            //Desempilhando pelo topo
                            for(int i=0; i<total; i++){
                                caux = desempilha(pilhaaux2, caux);
                                empilha(temp, caux);
                            }

                            //Desempilhando, agora pela primeira carta
                            for(int i=0; i<total; i++){
                                caux = desempilha(temp, caux);
                                empilha(pilhaaux, caux);
                                jogador[i].tamanho++;
                            }
                            free(temp);
                        }
                    }
                }
                if(escolha != 4){
                    empilha(pilhaaux, c);
                    jogador[i].tamanho++;
                    rodadaextra++;
                }
            }

            //ROUBA DO DESCARTE
            if (escolha == 3 || escolha == 4 || escolha == 10) {

                while (listaaux != NULL) {
                    if (listaaux->cartalista->numero == c->numero) {
                        caux = removernaposicao(descarte, listaaux->cartalista);
                        empilha(pilhaaux, caux);
                        jogador[i].tamanho++;
                    }
                    listaaux = listaaux->proximo;
                }

                empilha(pilhaaux, c);
                jogador[i].tamanho++;
                rodadaextra++;
            }

            else if(escolha == 2 || escolha == 20){
                printf("\nCarta inserida no descarte.");
                inserirultimaposicao(descarte, c);
            }
        }
        
        imprimejogadores(jogador, q);
        imprimirDescarte(descarte);

        i++;
        if(rodadaextra > 0){
            i--;
            printf("\nJOGUE DE NOVO %s\n", jogador[i].nome);
        }
        else if(i == q){
            i = 0;
        }

    }while(baralho->topo != NULL);
}


void ordenadescarte(lista* l){

    int i = 0, j = 0, n = l->tamanho;
    elementoLISTA* aux;
    carta* caux = criacarta(0, 0);
    aux = l->inicio;

    for(j=n-1; j>0; j--){
        aux = l->inicio;
        for(i = 0; i<j; i++){
            if(aux->proximo->cartalista->numero < aux->cartalista->numero){
                caux = aux->proximo->cartalista;
                aux->proximo->cartalista = aux->cartalista;
                aux->cartalista = caux;
            }
            aux = aux->proximo;
        }
    }
}


void ordenamontes(player jogador[], int q){
    
    if(jogador[q].monte->topo == NULL){
        return;
    }

    int COPAS;

    int i = 0, j = 0, n = jogador[q].tamanho, ant = 0, prox = 0;
    elementoPILHA* aux;
    carta* caux = criacarta(0, 0);
    aux = jogador[q].monte->topo;



    for(j = n-1; j>0; j--){
        aux = jogador[q].monte->topo;

        for(i = 0; i<j; i++){
            if(aux->cartapilha->naipe < aux->anterior->cartapilha->naipe){
                caux = aux->cartapilha;
                aux->cartapilha = aux->anterior->cartapilha;
                aux->anterior->cartapilha = caux;
            }
            aux = aux->anterior;
        }
    }

    for(j = n-1; j>0; j--){
        aux = jogador[q].monte->topo;

        for(i = 0; i<j; i++){
            if(aux->cartapilha->naipe == 1){
                if(aux->cartapilha->numero < aux->anterior->cartapilha->numero){
                    caux = aux->cartapilha;
                    aux->cartapilha = aux->anterior->cartapilha;
                    aux->anterior->cartapilha = caux;
                    ant++;
                }
                aux = aux->anterior;
            }
        }
    }
    
    prox = ant;

    for(j = n-1; j>0; j--){
        aux = jogador[q].monte->topo;
        for(int k=0; k<=ant; k++){
            aux = aux->anterior;
        }

        for(i = ant; i<j; i++){
            if(aux->cartapilha->naipe == 2){
                if(aux->cartapilha->numero < aux->anterior->cartapilha->numero){
                    caux = aux->cartapilha;
                    aux->cartapilha = aux->anterior->cartapilha;
                    aux->anterior->cartapilha = caux;
                    prox++;
                }
                aux = aux->anterior;
            }
        }
    }

    ant = prox;

    for(j = n-1; j>0; j--){
        aux = jogador[q].monte->topo;
        for(int k=0; k<=ant; k++){
            aux = aux->anterior;
        }

        for(i = ant; i<j; i++){
            if(aux->cartapilha->naipe == 3){
                if(aux->cartapilha->numero < aux->anterior->cartapilha->numero){
                    caux = aux->cartapilha;
                    aux->cartapilha = aux->anterior->cartapilha;
                    aux->anterior->cartapilha = caux;
                    prox++;
                }
                aux = aux->anterior;
            }
        }
    }

    ant = prox;

    for(j = n-1; j>0; j--){
        aux = jogador[q].monte->topo;
        for(int k=0; k<=ant; k++){
            aux = aux->anterior;
        }

        for(i = ant; i<j; i++){
            if(aux->cartapilha->naipe == 4){
                if(aux->cartapilha->numero < aux->anterior->cartapilha->numero){
                    caux = aux->cartapilha;
                    aux->cartapilha = aux->anterior->cartapilha;
                    aux->anterior->cartapilha = caux;
                    prox++;
                }
                aux = aux->anterior;
            }
        }
    }
}


void liberar(lista* l, pilha* p, player jogador[], int quantidade){

    while(p->topo != NULL){
        elementoPILHA *aux = p->topo;
        p->topo = aux->anterior;
        aux->anterior = NULL;
        free(aux);
    }

    while(p->topo != NULL){
        for(int i=0; i<quantidade; i++){
            for(int j=0; j<jogador[i].tamanho; i++){
                elementoPILHA *aux = jogador[i].monte->topo;
                jogador[i].monte->topo = aux->anterior;
                aux->anterior = NULL;
                free(jogador[i].monte->topo->cartapilha);
                free(aux);
            }
            free(jogador[i].monte);
        }
    }

    for(int i=0; i<l->tamanho; i++){
        elementoLISTA *aux = l->inicio;
        l->inicio = aux->proximo;
        free(aux->cartalista);
        free(aux);

        if(l->inicio == NULL){
            break;
        }
    }
    
    free(p);
    free(l);
    free(jogador);
}


void SAIR(lista* l, pilha* p, player jogador[], int quantidade){

    liberar(l, p, jogador, quantidade);
    exit('0');
}


void estatisticas(player jogador[], int i){

    printf("\n\nJOGADOR\n%s", jogador[i].nome);
    printf("\nMonte: %d CARTAS NA MAO\n", jogador[i].tamanho);

    if(jogador[i].tamanho == 0){
        return;
    }
    
    elementoPILHA* aux = jogador[i].monte->topo;

    const char* texto;
    carta *cartaaux;

    if(jogador[i].monte->topo != NULL){
        for(int j=0; j<jogador[i].tamanho; j++){
            cartaaux = aux->cartapilha;
            texto = naipe(cartaaux);
            if (cartaaux->naipe == 1 || cartaaux->naipe == 2) {
                    printf("\n\033[1;31m-> %d - %s \033[0m", cartaaux->numero, texto);
            }

            if (cartaaux->naipe == 3 || cartaaux->naipe == 4) {
                printf("\n\033[0m-> %d - %s \033[0m", cartaaux->numero, texto);
            }
            aux = aux->anterior;
        }
    }
}

//Eu estive aqui \(*_*)/ \(*_*)/ \(*_*)/ \(*_*)/ \(*_*)/ \(*_*)/ \(*_*)/
