#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "filtro.h"

#define N_Bloqueados 7

// Para variar um poucos a palavra chave, mas ainda sim conseguir itens dentro da categoria smartphone
// foi criado o vetor abaixo, contendo algumas palavra chaves que pode ser relacionada a principal.
char *Palavras_Chaves[] = { "Smartphone",
                            "Celular",
                            "iPhone",
                            "Android",
                            "Samsung",
                            "Motorola",
                            "Lg ",       // Com espaço no final para nao pegar no meio de palavras, tipo "algo"
                            "Zenfone",
                            "Xiaomi",
};

char *Palavras_Bloqueadas[] = { "Suporte",
                                "Braçadeira",
                                "Kit",
                                "Capa",
                                "Bumper",
                                "Protetores",
                                "Pelicula",
                                "Película",
                                "Tablet"
};

typedef struct produto_
{
    unsigned int ID;
    char *TITULO;
    char eh_smart; // char gasta menos memoria
    struct produto_ *prox;
} Produto;

// Variaveis Globais sao iniciadas com 0
struct lista_
{
    Produto *ini;
    Produto *fim;
    Produto sentinela; // iniciado com 0 por ser global
} Lista;

void armazena_produto(int id, char titulo[MAX])
{
    Produto *novo;
    novo = (Produto *)malloc(sizeof(Produto));
    novo->TITULO = (char *)malloc((strlen(titulo) + 1) * sizeof(char)); // +1 eh para incluir o '\0'

    strcpy(novo->TITULO, titulo); // Copia titulo para struct

    novo->ID = id; // Copia id para struct
    novo->eh_smart = 0; 

    if (Lista.ini == NULL) // Lista vazia, inicia lista
    {
        Lista.ini = novo;
        Lista.fim = novo;
        novo->prox = &(Lista.sentinela);
    }
    else // Encadeia o novo produto no final da lista
    {
        novo->prox = &(Lista.sentinela); // ultimo elemento sempre aponta para sentinela
        Lista.fim->prox = novo; // ultimo elemento passa apontar para o novo
        Lista.fim = novo; // fim da lista passa a ser o novo elemento
    }
    return;
}

char busca_chave(char *titulo, char *chave)
{
    int indice_titulo, indice_chave, indice_aux;

    indice_titulo = 0;
    indice_chave = 0;

    while (titulo[indice_titulo] != '\0') // varre todo o titulo
    {
        while ( tolower(titulo[indice_titulo]) != tolower(chave[indice_chave])  // nao diferencia maiuscula de minuscula
                && titulo[indice_titulo] != '\0') // nao chegou no final do titulo
        {
            indice_titulo++; // avanca o indice no titulo ateh achar uma letra igual a primeira letra da chave
        }
        indice_aux = indice_titulo;

        while ( tolower(titulo[indice_aux]) == tolower(chave[indice_chave]) )
        {
            indice_aux++;
            indice_chave++; // se achou a palavra chave inteira, no final o indice chave aponta para '\0'

            if (chave[indice_chave] == '\0') // achou a palavra chave
            {
                return 1;
            }
        }
        indice_chave = 0; // nao achou a palavra chave, reseta o indice pro comeco
        indice_titulo ++; // avanca o indice do titulo para procurar em outras posicoes
    }

    return 0; //  nao achou a palavra chave
}

void classificador_fraco(void)
{
    Produto *aux;
    char *chave;
    aux = Lista.ini;

    chave = Palavras_Chaves[0];
    while (aux != &(Lista.sentinela))
    {
        aux->eh_smart = busca_chave(aux->TITULO, chave);
        aux = aux->prox;
    }

    return;
}

void classificador_medio(void)
{
    Produto *aux;
    char *chave;
    int i;
    aux = Lista.ini;

    while (aux != &(Lista.sentinela))
    {
        
        chave = Palavras_Chaves[0];
        aux->eh_smart = busca_chave(aux->TITULO, chave);

//      chaves alternativas para encontrar celulares: 1-Celular, 2-iPhone, 3-Android
        i = 1;
        for (; i<=3; i++)
        {
            if (aux->eh_smart == 0) // Ainda nao foi classificado
            {
                chave = Palavras_Chaves[i];
                aux->eh_smart = busca_chave(aux->TITULO, chave);
            }
        }

//      chaves eliminadas por nao se tratar de smartphone: 0-Suporte, 1-Bracadeira, 2-Kit
        i = 0;
        for (; i<N_Bloqueados; i++)
        {
            if (aux->eh_smart == 1) // Foi classificado mas nao deveria
            {
                chave = Palavras_Bloqueadas[i];
                if (busca_chave(aux->TITULO, chave) == 1)
                    aux->eh_smart = 0;
            }
        }

        aux = aux->prox;
    }

    return;
}

void gera_resultado(char *nome_arquivo)
{
    FILE *resultado;
    Produto *aux;
    
    resultado = fopen(nome_arquivo, "w");
    aux = Lista.ini;

    fprintf(resultado, "ID\t\tCLASSIFICAÇÃO\tTÍTULO\n");

    while (aux != &(Lista.sentinela))
    {
        if(aux->eh_smart == 1)
        {
            fprintf(resultado, "%u\t(smartphone)\t%s\n", aux->ID, aux->TITULO);
        }
        else
        {
            fprintf(resultado, "%u\t(não-smartphone)\t%s\n", aux->ID, aux->TITULO);
        }

        aux = aux->prox;
    }

    fclose(resultado);
    return;
}

void esvazia_lista(void)
{
    Produto *aux;
    aux = Lista.ini;

    while (aux != &(Lista.sentinela))
    {
        Lista.ini = Lista.ini->prox;
        free(aux->TITULO);
        free(aux);
        aux = Lista.ini;
    }
    return;
}