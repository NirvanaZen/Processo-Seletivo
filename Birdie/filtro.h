#ifndef FILTRO_H_INCLUDED
#define FILTRO_H_INCLUDED

#define MAX 500

void armazena_produto(int id, char titulo[MAX]);

//      Forma mais rustica de classificador: procura se o titulo contem a palavra chave procurada
//      Nesse classificador sera categorizado como smartphone acessorios de smartphone que tenha
// essa palavra em seu titulo. Tambem nao classifica como smartphone titulos que sejam celulares
// smartphone mas que nao contem essa palavra em seu
void classificador_fraco(void);


void classificador_medio(void);

void gera_resultado(char *nome_arquivo);
void esvazia_lista(void);

#endif // FILTRO_H_INCLUDED