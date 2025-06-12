#include "estruturas_auxiliares.h"

// --- Implementação do Algoritmo Quicksort ---

/**
 * @brief Função auxiliar para trocar dois elementos de posição em um Vetor.
 */
template <typename T>
void trocar(Vetor<T>& vetor, int i, int j) {
    T temp = vetor[i];
    vetor[i] = vetor[j];
    vetor[j] = temp;
}

/**
 * @brief Particiona o vetor para o Quicksort e retorna o índice do pivô.
 * @details Usa o esquema de partição de Lomuto, onde o último elemento é o pivô.
 */
template <typename T, typename Comparator>
int particionar(Vetor<T>& vetor, int baixo, int alto, Comparator comp) {
    T pivo = vetor[alto]; // Escolhe o último elemento como pivô
    int i = (baixo - 1); // Índice do menor elemento

    for (int j = baixo; j <= alto - 1; j++) {
        // Se o elemento atual for menor que o pivô (de acordo com o comparador)
        if (comp(vetor[j], pivo)) {
            i++; // Incrementa o índice do menor elemento
            trocar(vetor, i, j);
        }
    }
    trocar(vetor, i + 1, alto);
    return (i + 1);
}

/**
 * @brief A função recursiva que implementa o Quicksort.
 */
template <typename T, typename Comparator>
void quickSortRecursivo(Vetor<T>& vetor, int baixo, int alto, Comparator comp) {
    if (baixo < alto) {
        // pi é o índice da partição, vetor[pi] está agora no lugar certo
        int pi = particionar(vetor, baixo, alto, comp);

        // Ordena separadamente os elementos antes e depois da partição
        quickSortRecursivo(vetor, baixo, pi - 1, comp);
        quickSortRecursivo(vetor, pi + 1, alto, comp);
    }
}

/**
 * @brief Função principal para ordenar um Vetor<T> usando o Quicksort.
 * @details Funciona de maneira semelhante ao std::sort, aceitando um vetor e um comparador.
 * @tparam T O tipo dos elementos no vetor.
 * @tparam Comparator O tipo da função de comparação (geralmente um lambda).
 * @param vetor O Vetor<T> a ser ordenado.
 * @param comp A função de comparação que retorna true se o primeiro argumento vem antes do segundo.
 */
template <typename T, typename Comparator>
void quickSort(Vetor<T>& vetor, Comparator comp) {
    if (vetor.tamanho() > 1) {
        quickSortRecursivo(vetor, 0, vetor.tamanho() - 1, comp);
    }
}