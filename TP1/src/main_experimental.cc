/*
Coeficientes na escala original:
a (comparações): 0.000034
b (movimentações): -0.000022
c (chamadas): -0.000198
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "ordenador.h"

// Parâmetros de custo
const double a = -0.261878;   // Comparações
const double b = 0.398545;  // Movimentações
const double c = 0.000080;  // Chamadas

// Definidos por -DKEYSZ e -DPLSZ na compilação
#ifndef KEYSZ
#define KEYSZ 16
#endif

#ifndef PLSZ
#define PLSZ 16
#endif

typedef struct {
    char chave[KEYSZ];
    char payload[PLSZ];
} Registro;

double timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int chave_para_int(char *chave) {
    return atoi(chave);  // Assume que a chave é composta apenas por dígitos
}

int le_arquivo(const char *filename, Registro **vetor_registros) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int capacidade = 1000;
    int tamanho = 0;
    *vetor_registros = (Registro *)malloc(capacidade * sizeof(Registro));
    if (!*vetor_registros) {
        fprintf(stderr, "Erro de malloc\n");
        exit(EXIT_FAILURE);
    }

    char buffer_key[KEYSZ];
    char buffer_payload[PLSZ];

    while (fscanf(fp, "%31s %31s", buffer_key, buffer_payload) == 2) {
        // Redimensiona vetor se necessário
        if (tamanho >= capacidade) {
            capacidade *= 2;
            *vetor_registros = (Registro *)realloc(*vetor_registros, capacidade * sizeof(Registro));
            if (!*vetor_registros) {
                fprintf(stderr, "Erro de realloc\n");
                exit(EXIT_FAILURE);
            }
        }

        // Copia strings com segurança
        strncpy((*vetor_registros)[tamanho].chave, buffer_key, KEYSZ);
        strncpy((*vetor_registros)[tamanho].payload, buffer_payload, PLSZ);

        // Simula uso do payload (impacto no tempo de leitura)
        volatile int dummy = 0;
        for (int j = 0; j < PLSZ - 1 && (*vetor_registros)[tamanho].payload[j] != '\0'; j++) {
            dummy += (*vetor_registros)[tamanho].payload[j];
        }

        tamanho++;
    }

    fclose(fp);
    return tamanho;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    double ini_leitura = timestamp();

    Registro *registros;
    int n = le_arquivo(argv[1], &registros);

    int *valores = (int *)malloc(n * sizeof(int));
    if (!valores) {
        fprintf(stderr, "Erro de malloc para valores\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        valores[i] = chave_para_int(registros[i].chave);
    }

    int *copia = (int *)malloc(n * sizeof(int));
    if (!copia) {
        fprintf(stderr, "Erro de malloc para copia\n");
        return EXIT_FAILURE;
    }
    memcpy(copia, valores, n * sizeof(int));

    double fim_leitura = timestamp();

    Ordenador ordenador;
    ordenador.setPesos(a, b, c);

    double ini_calib = timestamp();
    int limiarParticao = ordenador.determinaLimiarParticao(valores, n, 0.1);
    int limiarQuebras  = ordenador.determinaLimiarQuebras(42, valores, limiarParticao, n, 0.1);
    double fim_calib   = timestamp();

    double ini_sort = timestamp();
    ordenador.resetCounters();
    ordenador.OrdenadorUniversal(copia, n, limiarParticao, limiarQuebras);
    double fim_sort = timestamp();

    double tempo_leitura = fim_leitura - ini_leitura;
    double tempo_calib   = fim_calib   - ini_calib;
    double tempo_sort    = fim_sort    - ini_sort;
    double tempo_total   = fim_sort    - ini_leitura;

    double custo = a * ordenador.getCmp() + b * ordenador.getMove() + c * ordenador.getCalls();

    // Saída no formato CSV
    printf("%d,%d,%.6f,%.6f,%.6f,%.6f,%.6f\n",
           limiarParticao, limiarQuebras, custo,
           tempo_leitura, tempo_calib, tempo_sort, tempo_total);

    free(registros);
    free(valores);
    free(copia);

    return 0;
}
