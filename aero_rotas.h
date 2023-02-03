#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define R0 6371
#define R 6381
#define MAX 20
#define LMAX 100


typedef struct
{
    char ICAO[5], IATA[4], cidade[MAX];
    int LatGrau,LatMin,LatSeg,LongGrau,LongMin,LongSeg,TimeZone;
    char LongDir,LatDir;
} TipoDadoA;

typedef struct aeroporto
{
    TipoDadoA aeroporto;
    struct aeroporto *prox;
} AERO;

typedef struct
{
    char strin[5][MAX];
    char a_strin[LMAX];
} TipoString;

typedef struct
{
    float dist;
    char airline[MAX], codigo[MAX], aero_p[MAX], hora_p[MAX], aero_c[MAX], hora_c[MAX];
} TipoDadoR;

typedef struct rota
{
    TipoDadoR x;
    struct rota *prox;
} ROTA;

typedef struct info
{
    char nome[MAX];
    struct info *prox;
} INFO;

/*  Funcoes relativas a lista AERO*/
AERO *ler_aero(AERO *base,FILE *fp);
AERO *inicia_aero(void);
AERO *insere_aero(AERO *base,AERO *ap_antes,TipoDadoA z);
void mostrar_aero(AERO *base);
AERO *ponto_insercao_aero(AERO *base, TipoDadoA x);
int ListaVazia(AERO *base);
int ordem_a_aero(AERO* x, TipoDadoA y);
AERO* finaliza_aero(AERO*base);

/*  Funcoes relativas a lista ROTA*/
ROTA* inicia_rota(void);
ROTA* ler_rota(FILE *fich, AERO*base_a, ROTA *base);
ROTA* alocar_memoria_rota(AERO*base_a, ROTA *base, TipoString s);
ROTA* ponto_insercao_rota(ROTA*base, ROTA*novo);
int ordem_a_rota(ROTA *atual, ROTA *novo);
ROTA* insere_rota(ROTA *base, ROTA *anterior, ROTA *novo);
void mostrar_rota(ROTA*base);
ROTA* finaliza_rota(ROTA*base);

/*  Funcoes relativas a lista INFO*/
int existe_aero(AERO*base, char nome[MAX]);
int existe_info(INFO*base, char nome[MAX]);
INFO* alocar_memoria_info(INFO*base, char nome[MAX]);
INFO* ponto_insercao_info(INFO*base, INFO*novo);
int ordem_a_info(INFO *atual, INFO *novo);
INFO* insere_info(INFO *base, INFO *anterior, INFO *novo);
void mostrar_info(INFO *base);
INFO* finaliza_info(INFO*base);

/*  Funcoes relativas a distancia entre dois aeroportos*/
AERO* procura_partida(AERO*base, ROTA*novo);
AERO* procura_chegada(AERO*base, ROTA*novo);
float latitude(AERO*endereco);
float longitude(AERO*endereco);
void vetor_OX(float coord[2], float vetor[3]);
float produto_interno(float vetor1[3], float vetor2[3]);
float norma(float vetor[3]);
float distancia(AERO*base, ROTA*novo);

/*  Funcoes relativas a ordenação da lista AERO conforme a linha de comando*/
void troca_2_rotas(ROTA *a,ROTA *b);
void ordena_hora_crescente (ROTA *base);
float transforma_horario(char hora[MAX]);
void ordena_hora_decrescente (ROTA *base);

/*  Funcoes relativas a linha de comando*/
int num_rota_direta(ROTA* base, char origem[MAX], char destino[MAX]);
void imprimi_elemento(TipoDadoR x);
void imprimi_n_elementos(ROTA* base,int i);
void rotas_diretas(ROTA*base,char origem[MAX], char destino[MAX]);
void ligacao(ROTA *base, char origem[MAX], char destino[MAX]);
int num_origem_rota(ROTA* base, char origem[MAX]);
void ligacao_horarias(ROTA *base, char origem[MAX], char destino[MAX]);
void ligacao_horaria_dist(ROTA *base, char origem[MAX], char destino[MAX]);

/*  Funcoes relativas a inicialização e finalização do programa*/
void menu(AERO* base_aero,ROTA* base_rota, int argc, char *argv[]);
AERO* inicia_e_le_aero(AERO *base_aero,FILE *fp_aero);
ROTA* inicia_e_le_rota(ROTA *base_rota,AERO *base_aero, FILE *fp_rota);
void teste_abertura(FILE *fp);
void encerra_prog(AERO* base_aero,FILE* fp_aero,ROTA* base_rota,FILE *fp_rota);
void help(void);
