#include "aero_rotas.h"

int existe_aero(AERO*base, char nome[MAX])  /*  Procura o nome (string) na lista de aeroporotos (AERO).*/
{
    int i;                                  /*  Caso o nome exista, isto e', caso exista informacao sobre esse aeroporto,*/
    AERO*aux;                               /*retorna 1, do contrario retorna 0.*/

    i=0;
    aux = base;
    while(aux!=NULL)
    {
        if( strcmp(aux->aeroporto.cidade,nome) == 0 )
        {
            i++;
        }
        aux = aux->prox;
    }
    return i;
}

int existe_info(INFO*base, char nome[MAX])   /*  Procura o nome (string) na lista de nomes sem informacao (INFO).*/
{
    int i;                                   /*  Caso o nome exista retorna 1, do contrario retorna 0.*/
    INFO*aux;

    i=0;
    aux = base;
    while(aux!=NULL)
    {
        if( strcmp(aux->nome, nome) == 0 )
        {
            i++;
        }
        aux = aux->prox;
    }
    return i;
}

INFO* alocar_memoria_info(INFO*base, char nome[MAX])   /*  A função aloca memoria para uma lista (INFO) que guarda os nomes*/
{
    INFO *novo, *aux;                                  /*dos aeroportos que nao existem na lista AERO*/

    novo=(INFO*)calloc(1, sizeof(INFO));
    if(novo==(INFO*)NULL)
    {
        fprintf(stdout, "erro ao alocar memoria calloc\n");
        exit(0);
    }
    strcpy(novo->nome, nome);
    novo->prox = NULL;

    aux = ponto_insercao_info(base, novo);
    base = insere_info(base, aux, novo);

    return base;
}

INFO* ponto_insercao_info(INFO*base, INFO*novo)  /*  Procura o ponto de insercao de um elemento em ordem alfabetica*/
{
    INFO *atual, *anterior;

    anterior = NULL;
    atual = base;

    while((atual!=NULL)&&(ordem_a_info(atual, novo)<=0))
    {
        anterior = atual;
        atual = atual->prox;
    }
    return anterior;
}

int ordem_a_info(INFO *atual, INFO *novo)   /*  Entre 2 nomes (cidades) mostra qual viria primeiro em ordem alfabetica*/
{
    return strcmp(atual->nome, novo->nome);
}

INFO* insere_info(INFO *base, INFO *anterior, INFO *novo)  /*  Insere elemento na lista INFO*/
{

    if(anterior == NULL)
    {
        novo->prox = base;
        return novo;
    }
    novo->prox = anterior->prox;
    anterior->prox = novo;
    return base;
}

void mostrar_info(INFO *base)  /*  Imprimi elesmentos da lista INFO*/
{
    while(base!=(INFO*)NULL)
    {
        fprintf(stdout, "Nao ha informacoes a respeito de %s em aeroportos.txt\n", base->nome);
        base = base->prox;
    }
    printf("\n");
}

INFO* finaliza_info(INFO*base)  /*  Encerra lista, liberando a memoria antes utilizada*/
{
    INFO* aux;
    while(base!=NULL)
    {
        aux = base;
        base = base->prox;
        free(aux);
    }
    return (NULL);
}



ROTA* inicia_rota(void)  /*  Inicia lista das rotas*/
{
    return (NULL);
}

ROTA* ler_rota(FILE *fich, AERO *base_a, ROTA *base)  /*  Le ficheiros "rotas.txt"*/
{
    char linha[LMAX];
    int retorno, i;
    TipoString s;
    INFO *base_i = NULL;  /*  Inicializa a lista INFO.*/

    while( fgets(linha, LMAX, fich) )
    {
        for(i=0; i<5; i++)
        {
            s.strin[i][0]='\0';
        }
        retorno = sscanf(linha,"%s %s %s %s %s", s.strin[0], s.strin[1], s.strin[2], s.strin[3], s.strin[4]);
        if(retorno>0)
        {
            if(strcmp(s.strin[0], "AIRLINE:")==0)      /*  Caso a primeira string for identica a "AIRLINE:",*/
            {
                s.a_strin[0]='\0';                     /*a linha informa sobre uma airline e as proximas guardam o seu nome.*/
                for(i=1; i<retorno; i++)               /*  O nome da airline será usado na lista ate a situacao ocorrer novamente.*/
                {
                    strcat(s.a_strin, s.strin[i]);     /*  O numero de strings é indicado por retorno*/
                    strcat(s.a_strin, " ");
                }
            }
            else    /*  Caso contrario, a linha informa sobre uma rota*/
            {
                if((existe_aero(base_a, s.strin[1])==0)&&(existe_info(base_i, s.strin[1])==0)) /*  Avalia se os aeroportos da rota existem em AERO ou INFO.*/
                {
                    base_i = alocar_memoria_info(base_i, s.strin[1]);                          /*  Se nao existe em AERO e INFO, é alocado em INFO.*/
                }
                if((existe_aero(base_a, s.strin[3])==0)&&(existe_info(base_i, s.strin[3])==0))
                {
                    base_i = alocar_memoria_info(base_i, s.strin[3]);
                }
                if((existe_aero(base_a, s.strin[1])==1) && (existe_aero(base_a, s.strin[3])==1))
                {
                    base = alocar_memoria_rota(base_a, base, s);                               /*  Se existem em AERO, a rota é alocada em ROTA.*/
                }
            }
        }
    }
    mostrar_info(base_i);
    base_i = finaliza_info(base_i);
    return base;
}

ROTA *alocar_memoria_rota(AERO*base_a, ROTA *base, TipoString s) /*  A função aloca memoria para uma lista (ROTA)*/
{
    ROTA *novo;                                                  /*que guarda informações das rotas*/
    ROTA *aux;

    novo=(ROTA*)calloc(1, sizeof(ROTA));
    if(novo==(ROTA*)NULL)
    {
        fprintf(stdout, "erro ao alocar memoria calloc\n");
        exit(0);
    }
    strcpy(novo->x.codigo, s.strin[0]);
    strcpy(novo->x.aero_p, s.strin[1]);
    strcpy(novo->x.hora_p, s.strin[2]);
    strcpy(novo->x.aero_c, s.strin[3]);
    strcpy(novo->x.hora_c, s.strin[4]);
    strcpy(novo->x.airline, s.a_strin);
    novo->prox = NULL;
    novo->x.dist = distancia(base_a, novo); /*  A distancia entre os aeropostos tambem e' guardada em ROTA.*/

    aux = ponto_insercao_rota(base, novo);
    base = insere_rota(base, aux, novo);
    return base;
}

ROTA* ponto_insercao_rota(ROTA*base, ROTA*novo)   /*  Procura o ponto de insercao de um elemento em ordem alfabetica*/
{
    ROTA *atual, *anterior;

    anterior = NULL;
    atual = base;

    while((atual!=NULL)&&(ordem_a_rota(atual, novo)<=0))
    {
        anterior = atual;
        atual = atual->prox;
    }
    return anterior;
}

int ordem_a_rota(ROTA *atual, ROTA *novo)  /*  Entre 2 nomes (cidades) mostra qual viria primeiro em ordem alfabetica*/
{
    return strcmp(atual->x.aero_p, novo->x.aero_p);
}

ROTA* insere_rota(ROTA *base, ROTA *anterior, ROTA *novo)  /*  Insere elemento na lista ROTA*/
{

    if(anterior == NULL)
    {
        novo->prox = base;
        return novo;
    }
    novo->prox = anterior->prox;
    anterior->prox = novo;
    return base;
}

void mostrar_rota(ROTA *base)  /*  Imprimi elesmentos da lista ROTA*/
{
    while(base!=(ROTA*)NULL)
    {
        imprimi_elemento(base->x);
        base = base->prox;
    }
    printf("\n");
}

void imprimi_elemento(TipoDadoR x)  /*  Imprimi um elemento TipoDadoR*/
{
    fprintf(stdout, "\n\n%s %s %s %s %s %.3fKm %s\n\n",x.codigo,x.aero_p,x.hora_p,
            x.aero_c,x.hora_c,x.dist,x.airline);
}

ROTA* finaliza_rota(ROTA*base)  /*  Encerra lista, liberando a memoria antes utilizada*/
{
    ROTA* aux;
    while(base!=NULL)
    {
        aux = base;
        base = base->prox;
        free(aux);
    }
    return (NULL);
}



AERO *inicia_aero(void) /*Inicia lista dos aeroportos*/
{
    return (NULL);
}

AERO *insere_aero(AERO *base, AERO *ap_antes, TipoDadoA z) /* Insere elemento na lista dos aeroportos*/
{
    AERO *ap_local;

    ap_local=(AERO*)calloc(1,sizeof(AERO));
    if(ap_local == NULL)
    {
        puts("erro ao alocar memoria calloc");
        return base;
    }
    ap_local->aeroporto = z;
    ap_local->prox = NULL;

    if(ap_antes == NULL)
    {
        ap_local->prox = base;
        return ap_local;
    }
    ap_local->prox = ap_antes->prox;
    ap_antes ->prox = ap_local;
    return base;
}

void mostrar_aero(AERO * base)  /*Imprimi elesmentos da lista dos aeroportos (AERO)*/
{
    AERO *aux;

    aux = base ;
    while(aux != NULL)
    {
        printf("%s %s %d %d %d %c %d %d %d %c %s %d\n",aux->aeroporto.ICAO, aux->aeroporto.IATA,aux->aeroporto.LatGrau,
               aux->aeroporto.LatMin, aux->aeroporto.LatSeg, aux->aeroporto.LatDir, aux->aeroporto.LongGrau, aux->aeroporto.LongMin,
               aux->aeroporto.LongSeg,aux->aeroporto.LongDir,aux->aeroporto.cidade,aux->aeroporto.TimeZone);
        aux = aux-> prox;
    }
    printf("\n");
}

AERO *ponto_insercao_aero(AERO *base, TipoDadoA x)  /*Procura o ponto de insercao de um elemento em ordem alfabetica*/
{
    AERO *anterior,*atual;

    anterior = NULL;
    atual = base;

    while((atual != NULL) && ordem_a_aero(atual,x) > 0)
    {
        anterior = atual;
        atual = atual->prox;
    }
    return anterior;
}

int ListaVazia(AERO *base)  /*Testa se a lista tem elementos ou esta vazia*/
{
    return (base == NULL);
}

int ordem_a_aero(AERO* x, TipoDadoA y) /*Entre 2 cidades mostra qual viria primeiro em ordem alfabetica*/
{
    int i = 0;
    while (x->aeroporto.cidade[i]==y.cidade[i])
        i++;
    return(x->aeroporto.cidade[i]<=y.cidade[i]);
}

AERO *ler_aero(AERO *base,FILE *fp) /*Le ficheiros "aeroportos.txt"*/
{
    TipoDadoA x ;
    AERO *P_Inser;
    while(fscanf(fp,"%s %s %d %d %d %c %d %d %d %c %s %d",x.ICAO,x.IATA,&x.LatGrau,&x.LatMin,&x.LatSeg,
                 &x.LatDir,&x.LongGrau,&x.LongMin, &x.LongSeg,&x.LongDir,x.cidade,&x.TimeZone) != EOF)
    {
        P_Inser = ponto_insercao_aero(base,x);
        base = insere_aero(base,P_Inser,x);
    }
    return base;
}

AERO* finaliza_aero(AERO*base) /*Encerra lista, liberando a memoria antes utilizada*/
{
    AERO* aux;
    while(base!=NULL)
    {
        aux = base;
        base = base->prox;
        free(aux);
    }
    return (NULL);
}



float distancia(AERO*base, ROTA*novo)  /*  A função devolve a distância entre dois aeroportos*/
{
    AERO *endereco1,*endereco2;
    float coord1[2], coord2[2], vetOA[3], vetOB[3], ang, dist;

    endereco1 = procura_partida(base, novo);
    endereco2 = procura_chegada(base, novo);


    coord1[0] = latitude(endereco1);
    coord1[1] = longitude(endereco1);
    coord2[0] = latitude(endereco2);
    coord2[1] = longitude(endereco2);

    vetor_OX(coord1, vetOA);
    vetor_OX(coord2, vetOB);

    ang = acos(produto_interno(vetOA,vetOB)/(norma(vetOA)*norma(vetOB))); /*  Angulo entre os vetores vetOA e vetOB*/
    dist=R*ang;

    return dist;
}

AERO* procura_partida(AERO*base, ROTA*novo)  /*  Procura e devolve o endereço da memoria em AERO*/
{
    /*sobre o aeroporto de partida de novo (ROTA)*/
    while((base!=NULL) && (strcmp(novo->x.aero_p, base->aeroporto.cidade)!=0))
    {
        base = base->prox;
    }

    return base;
}

AERO* procura_chegada(AERO*base, ROTA*novo)  /*  Procura e devolve o endereço da memoria em AERO*/
{
    /*sobre o aeroporto de chegada de novo (ROTA)*/
    while((base!=NULL) && (strcmp(novo->x.aero_c, base->aeroporto.cidade)!=0))
    {
        base = base->prox;
    }

    return base;
}

float latitude(AERO*endereco)  /*  Retorna latitude em radianos*/
{
    float x, z;
    /*  x e' a latitude do aeroporto em graus*/
    x = endereco->aeroporto.LatGrau + ((endereco->aeroporto.LatMin)/60) + ((endereco->aeroporto.LatSeg)/3600);
    z = ((x*acos(-1))/180); /*  z 'e a latitude do aeroporto em radianos*/
    /*  acos(-1)==pi*/
    if(endereco->aeroporto.LatDir=='N')
    {
        return z;
    }
    else
    {
        return (-z);
    }
}

float longitude(AERO*endereco)  /*  Retorna longitude em radianos*/
{
    float x, z;
    /*  x e' a longitude do aeroporto em graus*/
    x = endereco->aeroporto.LongGrau + ((endereco->aeroporto.LongMin)/60) + ((endereco->aeroporto.LongSeg)/3600);
    z = ((x*acos(-1))/180); /*  z e' a latitude do aeroporto em radianos*/
    /*  acos(-1)==pi*/
    if(endereco->aeroporto.LongDir=='E')
    {
        return z;
    }
    else
    {
        return (-z);
    }
}

void vetor_OX(float coord[2], float vetor[3])  /*  Cria um vetor com as coordenadas do aeroporto (latitude e longitude)*/
{
    vetor[0]=R*cos(coord[0])*cos(coord[1]);
    vetor[1]=R*cos(coord[0])*sin(coord[1]);
    vetor[2]=R*sin(coord[0]);
}

float produto_interno(float vetor1[3], float vetor2[3])  /*  Calcula o produto interno de dois vetores*/
{
    float z;
    z=(vetor1[0]*vetor2[0])+(vetor1[1]*vetor2[1])+(vetor1[2]*vetor2[2]);
    return z;
}

float norma(float vetor[3])  /*  Calcula a norma de um vetor*/
{
    float z;
    z=sqrt(produto_interno(vetor,vetor));
    return z;
}



void troca_2_rotas(ROTA *a, ROTA *b)  /*Troca dois elementos da lista*/
{
    TipoDadoR save;

    save = b->x;
    b->x = a->x;
    a->x = save;
}

void ordena_hora_crescente (ROTA *base)  /*Ordena toda a pilha por ordem crescente de horario de partida*/
{
    ROTA *atual,*pesquisa;

    atual = base;
    pesquisa = atual->prox;

    while(pesquisa != NULL)
    {

        while(pesquisa != NULL)
        {

            if((transforma_horario(atual->x.hora_p)) > (transforma_horario(pesquisa->x.hora_p)))
            {
                troca_2_rotas(atual,pesquisa);
            }
            else pesquisa = pesquisa->prox;
        }
        atual = atual->prox;
        pesquisa = atual->prox;
    }
}

float transforma_horario(char str_hora[MAX])  /*Le uma string no formato hh:mmPM\AM e transforma*/
{
    float hora,min;                          /* em um numero unico de horas em uma escala de 0 a 24*/
    char periodo[3];

    sscanf(str_hora,"%f:%f%s",&hora,&min,periodo);

    hora = hora + (min/60);

    if(periodo[0] == 'P')
        hora = hora + 12.00;

    return hora;
}

void ordena_hora_decrescente (ROTA *base)  /*Ordena toda a pilha por ordem crescente de horario de partida*/
{
    ROTA *atual,*pesquisa;

    atual = base;
    pesquisa = atual->prox;

    while(pesquisa != NULL)
    {

        while(pesquisa != NULL)
        {

            if((transforma_horario(atual->x.hora_p)) < (transforma_horario(pesquisa->x.hora_p)))
            {
                troca_2_rotas(atual,pesquisa);
            }
            else pesquisa = pesquisa->prox;
        }
        atual = atual->prox;
        pesquisa = atual->prox;
    }
}



int num_rota_direta(ROTA* base, char origem[MAX], char destino[MAX])  /*    Coloca os voos correspondentes a pesquisa no "topo" da lista*/
{
    ROTA *atual,*pesquisa;                                            /*e retorna o numero de voos que foram selecionados na pesquisa*/
    int i =0;

    atual = base;
    pesquisa = atual;

    while(pesquisa != NULL)
    {

        if(  (strcmp(origem,pesquisa->x.aero_p) == 0) && (strcmp(destino,pesquisa->x.aero_c) == 0) )
        {
            troca_2_rotas(atual,pesquisa);
            atual = atual ->prox;
            i++;
        }
        pesquisa = pesquisa->prox;
    }

    return i;
}

void imprimi_n_elementos(ROTA* base,int i)  /*  Imprimi a partir da "base" 'n' elementos*/
{
    int j;
    ROTA* atual;

    atual = base;

    for(j =1; j<=i; j++)
    {
        imprimi_elemento(atual->x);
        atual = atual->prox;
    }
}

void rotas_diretas(ROTA*base, char origem[MAX], char destino[MAX])  /*  Busca uma rota direta*/
{
    int i =0;

    i = num_rota_direta(base,origem,destino);
    imprimi_n_elementos(base,i);

    if (i == 0)
        printf("\n\nNAO EXISTEM ROTAS PARA O TRECHO EM QUESTAO\n\n\n");

}



int num_origem_rota(ROTA* base, char origem[MAX])  /*   Coloca os voos correspondentes a pesquisa no "topo" da lista*/
{
    ROTA *atual,*pesquisa;                         /*e retorna o numero de voos que foram selecionados na pesquisa*/
    int i =0;                                      /*   Analisa apenas a origem */

    atual = base;
    pesquisa = atual;

    while(pesquisa != NULL)
    {

        if( strcmp(origem,pesquisa->x.aero_p) == 0)
        {
            troca_2_rotas(atual,pesquisa);
            atual = atual ->prox;
            i++;
        }
        pesquisa = pesquisa->prox;
    }

    return i;
}

void ligacao(ROTA *base, char origem[MAX], char destino[MAX])  /*Procura e imprimi voos com uma ligacao*/
{
    int j,i = 0;
    int k = 0;
    ROTA *atual,*pesquisa;

    i = num_origem_rota(base,origem);

    atual = base;
    pesquisa = atual->prox;

    for(j=0; j<i; j++)
    {
        while(pesquisa != NULL)
        {
            if(  (strcmp(destino,pesquisa->x.aero_c) == 0) && (strcmp(atual->x.aero_c,pesquisa->x.aero_p) == 0)  )
            {
                imprimi_elemento(atual->x);
                imprimi_elemento(pesquisa->x);
                printf("\n\n\n");
                k++;
            }
            pesquisa = pesquisa->prox;
        }
        atual = atual->prox;
        pesquisa = atual ->prox;
    }
    if(k == 0)
        printf("\n\nNAO EXISTEM ROTAS COM 1 LIGACAO DE %s PARA %s\n\n", origem, destino);

}

void ligacao_horaria(ROTA *base, char origem[MAX], char destino[MAX])  /*Procura e imprimi voos com uma ligacao e ligacao horaria*/
{
    int j,i = 0;
    int k = 0;
    ROTA *atual,*pesquisa;

    i = num_origem_rota(base,origem);

    atual = base;
    pesquisa = atual->prox;

    for(j=0; j<i; j++)
    {
        while(pesquisa != NULL)
        {
            if(  (strcmp(destino,pesquisa->x.aero_c) == 0) && (strcmp(atual->x.aero_c,pesquisa->x.aero_p) == 0)
                    && (transforma_horario(atual->x.hora_c)) < transforma_horario(pesquisa->x.hora_p) )
            {

                imprimi_elemento(atual->x);
                imprimi_elemento(pesquisa->x);
                printf("\n\n");
                k++;
            }
            pesquisa = pesquisa->prox;
        }
        atual = atual->prox;
        pesquisa = atual ->prox;
    }
    if(k == 0)
        printf("\n\nNAO EXISTEM ROTAS COM 1 LIGACAO HORARIA DE %s PARA %s\n\n", origem, destino);

}

void ligacao_horaria_dist(ROTA *base, char origem[MAX], char destino[MAX])  /*Procura e imprimi o voos com uma ligacao e ligacao horaria de menor distancia*/
{
    int j,k = 0, i = 0;
    ROTA *atual,*pesquisa;
    TipoDadoR trecho_1,trecho_2;
    float dist_total = 0.000;

    i = num_origem_rota(base,origem);
    atual = base;
    pesquisa = atual->prox;

    for(j=0; j<i; j++)
    {
        while(pesquisa != NULL)
        {
            if(  (strcmp(destino,pesquisa->x.aero_c) == 0) && (strcmp(atual->x.aero_c,pesquisa->x.aero_p) == 0)
                    && (transforma_horario(atual->x.hora_c)) < transforma_horario(pesquisa->x.hora_p) )
            {

                if(  ((atual->x.dist + pesquisa->x.dist) < dist_total) || dist_total == 0.00  )
                {
                    dist_total = atual->x.dist + pesquisa->x.dist;
                    trecho_1 = atual->x;
                    trecho_2 = pesquisa->x;
                    k++;
                }
            }
            pesquisa = pesquisa->prox;
        }
        atual = atual->prox;
        pesquisa = atual ->prox;
    }
    if(k == 0)
        printf("\n\nNAO EXISTEM ROTAS COM 1 LIGACAO HORARIA DE %s PARA %s\n\n", origem, destino);

    else
    {
        imprimi_elemento(trecho_1);
        imprimi_elemento(trecho_2);
        printf("\n\n\n");
    }
}



void menu(AERO* base_aero,ROTA* base_rota, int argc, char *argv[])  /*Responsavel pro receber os argumentos da linha de comando e ativar as respectivas funcoes*/
{
    if(strcmp(argv[1], "-aeroportos") == 0)
        mostrar_aero(base_aero);

    else if(strcmp(argv[1],"-voos") == 0)
        mostrar_rota(base_rota);

    else if( (strcmp(argv[3],"-L") == 0) && (strcmp(argv[4],"0") == 0) && (argc == 5) )
        rotas_diretas(base_rota,argv[1],argv[2]);

    else if( (strcmp(argv[3],"-L") == 0) && (strcmp(argv[4],"0") == 0)  && (strcmp(argv[5],"-TC") == 0))
    {
        ordena_hora_crescente(base_rota);
        rotas_diretas(base_rota,argv[1],argv[2]);
    }
    else if( (strcmp(argv[3],"-L") == 0) && (strcmp(argv[4],"0") == 0)  && (strcmp(argv[5],"-TD") == 0))
    {
        ordena_hora_decrescente(base_rota);
        rotas_diretas(base_rota,argv[1],argv[2]);
    }
    else if( (strcmp(argv[3],"-L") == 0) && (strcmp(argv[4],"1") == 0) && (argc == 5) )
        ligacao(base_rota,argv[1],argv[2]);

    else if( (strcmp(argv[3],"-L") == 0) && (strcmp(argv[4],"1") == 0)  && (strcmp(argv[5],"-TC") == 0) && argc == 6)
        ligacao_horaria(base_rota,argv[1],argv[2]);

    else if( (strcmp(argv[3],"-L") == 0) && (strcmp(argv[4],"1") == 0)  && (strcmp(argv[5],"-TC") == 0)
             && (strcmp(argv[6],"-D") == 0) )

        ligacao_horaria_dist(base_rota,argv[1],argv[2]);

    else help();
}

AERO* inicia_e_le_aero(AERO *base_aero,FILE *fp_aero)  /*Inicia e preenche a lista dos aeroportos*/
{
    base_aero = inicia_aero();
    base_aero = ler_aero(base_aero,fp_aero);
    return base_aero;

}

ROTA* inicia_e_le_rota(ROTA *base_rota,AERO *base_aero, FILE *fp_rota) /*Inicia e preenche a lista das rotas*/
{
    base_rota = inicia_rota();
    base_rota = ler_rota(fp_rota, base_aero, base_rota);
    return base_rota;

}

void encerra_prog(AERO* base_aero,FILE* fp_aero,ROTA* base_rota,FILE *fp_rota)  /*Encerra o programa fechando todos ficheiros e apagando listas*/
{
    fclose(fp_aero);
    fclose(fp_rota);
    base_rota = finaliza_rota(base_rota);
    base_aero = finaliza_aero(base_aero);
}

void teste_abertura(FILE *fp) /*Testa a abertura de ficheiros, informando erros na abertura*/
{
    if(fp == NULL)
    {
        puts("Erro na abertura do ficheiro");
        exit(0);
    }
}

void help(void)  /*Funcao voltada a orientacao do utilizador sobre o funcionamento do programa*/
{

    printf("\n\n\n                                                 BEM VINDO AO MENU DE AJUDA\n\n");
    printf("  PARA :\n");
    printf("          INFORMACOES DOS AEROPORTOS                                               :            -aeroportos\n\n");
    printf("          INFORMACOES DOS VOOS DIRETOS                                             :            -voos\n\n");
    printf("          PEDIDO DE ROTA DIRETA                                                    :            ORIGEM DESTINO -L 0  \n\n");
    printf("          PEDIDO DE ROTA DIRETA  EM ORDEM CRESCENTE DE HORARIO DE PARTIDA          :            ORIGEM DESTINO -L 0 -TC\n\n");
    printf("          PEDIDO DE ROTA DIRETA ORDEM DECRESCENTE DE HORARIO DE PARTIDA            :            ORIGEM DESTINO -L 0 -TD\n\n");
    printf("          PEDIDO DE ROTA COM UMA LIGACAO                                           :            ORIGEM DESTINO -L 1\n\n");
    printf("          PEDIDO DE ROTA COM UMA LIGACAO HORARIA                                   :            ORIGEM DESTINO -L 1 -TC\n\n");
    printf("          PEDIDO DE ROTA COM UMA LIGACAO HORARIA E COM A MENOR DISTANCIA           :            ORIGEM DESTINO -L 1 -TC -D\n\n\n\n\n");
    printf("  'ORIGEM' E 'DESTINO' CORRESPONDEM AO NOME DAS CIDADES DE ORIGEM E DESTINO ESCRITAS COM LETRAS MAIUSCULAS\n\n\n\n\n");
}
