#include <stdio.h>
#include <stdlib.h>
#include "filtro.h"

int main(int argc, char *argv[])
{
    FILE *dados;
    char titulo_temp[MAX];
    unsigned int id_tmp;

    if (argc != 2)
    {
        printf("Numero de entrada errado! Deve conter 1 arquivo de dados de entrada\n\n");
        exit(1);
    }
    
    dados = fopen(argv[1], "r");

    fscanf(dados, "%*[^\n]s"); // ignora primeira linha

    while(!feof(dados))
    {
        fscanf(dados, "%u %[^\n]s", &id_tmp, titulo_temp); // Le dados do arquivo

        armazena_produto(id_tmp, titulo_temp);
    }
//  Terminou de guardar os produtos na Lista
    fclose(dados);

    classificador_fraco();

    gera_resultado("resultados_fracos.tsv");

    classificador_medio();

    gera_resultado("resultados_medios.tsv");

    esvazia_lista();

    return 0;
}