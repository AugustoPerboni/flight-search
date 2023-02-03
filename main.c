#include <stdio.h>
#include <stdlib.h>
#include "aero_rotas.c"


int main(int argc, char *argv[])
{

    FILE *fp_rota, *fp_aero;
    ROTA *base_rota = NULL;
    AERO *base_aero = NULL;

    fp_aero = fopen("aeroportos.txt", "r");
    teste_abertura(fp_aero);
    fp_rota = fopen("rotas.txt", "r");
    teste_abertura(fp_rota);

    base_aero = inicia_e_le_aero(base_aero,fp_aero);
    base_rota = inicia_e_le_rota(base_rota,base_aero,fp_rota);

    menu(base_aero, base_rota, argc, argv);

    encerra_prog(base_aero,fp_aero,base_rota,fp_rota);
    return 0;
}
