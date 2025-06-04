#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 10000

// Funções de ordenação
void insertion_sort(int* v, int n) {
    for (int i = 1; i < n; i++) {
        int chave = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > chave) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

void quicksort(int* v, int inicio, int fim) {
    if (inicio < fim) {
        int pivo = v[fim];
        int i = inicio - 1;
        for (int j = inicio; j < fim; j++) {
            if (v[j] <= pivo) {
                i++;
                int temp = v[i]; v[i] = v[j]; v[j] = temp;
            }
        }
        int temp = v[i + 1]; v[i + 1] = v[fim]; v[fim] = temp;
        int q = i + 1;
        quicksort(v, inicio, q - 1);
        quicksort(v, q + 1, fim);
    }
}

// Utilitários
void copiar_vetor(int* origem, int* destino, int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}

void gerar_vetor_ordenado(int* v, int n) {
    for (int i = 0; i < n; i++) {
        v[i] = i;
    }
}

void gerar_vetor_invertido(int* v, int n) {
    for (int i = 0; i < n; i++) {
        v[i] = n - i;
    }
}

void gerar_vetor_aleatorio(int* v, int n) {
    for (int i = 0; i < n; i++) {
        v[i] = rand() % 100000;
    }
}

// Tempo em segundos
double tempo_execucao(void (*func)(int*, int), int* v, int n) {
    clock_t ini = clock();
    func(v, n);
    return (double)(clock() - ini) / CLOCKS_PER_SEC;
}

double tempo_exec_quick(int* v, int n) {
    clock_t ini = clock();
    quicksort(v, 0, n - 1);
    return (double)(clock() - ini) / CLOCKS_PER_SEC;
}


int main() {
    srand(time(NULL));
    int ordenado[TAMANHO], invertido[TAMANHO], aleatorio[TAMANHO];
    int v[TAMANHO];

    gerar_vetor_ordenado(ordenado, TAMANHO);
    gerar_vetor_invertido(invertido, TAMANHO);
    gerar_vetor_aleatorio(aleatorio, TAMANHO);

    printf("==== Resultados de tempo (s) ====\n");

    // Ordenado
    copiar_vetor(ordenado, v, TAMANHO);
    double t_ins_ord = tempo_execucao(insertion_sort, v, TAMANHO);
    copiar_vetor(ordenado, v, TAMANHO);
    double t_qs_ord = tempo_exec_quick(v, TAMANHO);
    printf("Vetor ordenado:\n  Insertion: %.6f s\n  Quicksort: %.6f s\n", t_ins_ord, t_qs_ord);

    // Invertido
    copiar_vetor(invertido, v, TAMANHO);
    double t_ins_inv = tempo_execucao(insertion_sort, v, TAMANHO);
    copiar_vetor(invertido, v, TAMANHO);
    double t_qs_inv = tempo_exec_quick(v, TAMANHO);
    printf("Vetor invertido:\n  Insertion: %.6f s\n  Quicksort: %.6f s\n", t_ins_inv, t_qs_inv);

    // Aleatório
    copiar_vetor(aleatorio, v, TAMANHO);
    double t_ins_ale = tempo_execucao(insertion_sort, v, TAMANHO);
    copiar_vetor(aleatorio, v, TAMANHO);
    double t_qs_ale = tempo_exec_quick(v, TAMANHO);
    printf("Vetor aleatório:\n  Insertion: %.6f s\n  Quicksort: %.6f s\n", t_ins_ale, t_qs_ale);

    return 0;
}
