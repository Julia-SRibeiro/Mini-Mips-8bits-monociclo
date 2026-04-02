#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void carrega_dat (memoria_dados* mem_dados){
    char arq[50];

    printf("Nome do arquivo .dat: ");
    limpa_buffer();
    scanf("%s", arq);

    // Abre arquivo no modo leitura
    FILE* arquivo = fopen(arq, "r");
    if(arquivo == NULL){
        printf("Erro: nao foi possivel abrir o arquivo.");
        return;
    }

    // Zera memoria de dados
    if (mem_dados->dados != NULL) {
        free(mem_dados->dados);
    }

    // Alocacao de memoria (256 x tamanho do tipo instrucao)
    mem_dados -> dados = (int*) malloc(sizeof(int) * MAX);
    if (mem_dados->dados == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    // Verifica arquivo.dat e salva na memoria de dados
    mem_dados -> tamanho = 0;
    int valor, linha=0;
    while (fscanf(arquivo, "%d", &valor) == 1){
        linha++;
        if (valor < -128 || valor > 127) {
            printf("Linha %d ignorada. Valor %d fora de [-128, 127].\n", linha, valor);
            printf("%d: %d\n", linha, valor);
            mem_dados -> tamanho++;
            continue;
        }
        if (mem_dados -> tamanho >= MAX) {
            printf("Memoria cheia!");
            break;
        }

        mem_dados -> dados[mem_dados -> tamanho] = valor;
        mem_dados -> tamanho++;
    }

    printf("%d dados carregados.\n", mem_dados->tamanho);
    fclose(arquivo);
}

void print_mem_dat(memoria_dados* imp_dados){
   if (imp_dados->dados == NULL) {
       printf("Memoria de dados nao carregada.\n");
       return;
   }

   int total = imp_dados->tamanho;
   int zeros = 0, nao_zeros = 0;


   // Conta valores
   for (int i = 0; i < total; i++) {
       if (imp_dados->dados[i] != 0)
           nao_zeros++;
       else
           zeros++;
   }


   printf("\nMemoria de dados (%d posicao(oes) carregada(s), %d nao-zero):\n",
          total, nao_zeros);


   if (nao_zeros == 0) {
       printf("  (todas as posicoes sao zero)\n");
       printf("Total: %d posicao(oes)\n", total);
       return;
   }


   printf("+-------+--------+\n");
   printf("| Addr  |  Valor |\n");
   printf("+-------+--------+\n");


   for (int i = 0; i < total; i++) {
       int v = imp_dados->dados[i];
       if (v != 0)
           printf("| %5d | %6d |\n", i, v);
   }


   printf("+-------+--------+\n");
   printf("  (%d posicao(oes) com valor zero omitida(s))\n", zeros);
   printf("Total: %d posicao(oes)\n", total);
}

void salva_dat(memoria_dados* mem_dados) {
    if (mem_dados->dados == NULL) {
        printf("Erro: Memoria de dados nao carregada.\n");
        return;
    }

    char arq[50];
    printf("Nome do arquivo de saida .dat: ");
    limpa_buffer();
    scanf("%s", arq);

    FILE* arquivo = fopen(arq, "w");
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel criar o arquivo '%s'.\n", arq);
        return;
    }

    // Salva cada valor da memória em uma linha
    for (int i = 0; i < mem_dados->tamanho; i++) {
        fprintf(arquivo, "%d\n", mem_dados->dados[i]);
    }

    fclose(arquivo);
    printf("Memoria de dados salva em '%s' (%d posicoes).\n", arq, mem_dados->tamanho);
}