#include <stdio.h>
#include <sistema.h>

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