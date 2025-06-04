#ifndef ORDENADOR_H
#define ORDENADOR_H

#include <iostream> // Utilizado para cin e cout
#include <iomanip>  // Utilizado para precisão de casas decimais  
#include "vetor.h"
#include "estatistica.h"

/**
 * @brief Estrutura de ordenação e avaliação de desempenho.
 * @details 
 * Tipo Abstrato de Dados que apresenta uma série de funções que podem ser separadas em algumas categorias:
 * 1. Básicas: Construtor, Destrutor e atribuição de atributos necessários para cálculo de custo.
 * 2. Auxiliares: Funções de utilidades necessárias para a implementação e que são mais gerais, sendo inspiradas
 * principalmente em funções pré-existentes em bibliotecas, tais como swap e floor padrão, que podem ter sofrido 
 * modificações para melhor atender aos objetivos da implementação. 
 * 3. Algorítimos de ordenação: Algorítimos criados com o objetivo de ordenar um vetor. 
 * 4. Ordenador Universal: Função definida com o objetivo de escolher entre algorítimos de ordenação com base em
 * parâmetros de organização e estado atual do vetor a ser odenado. 
 * 5. Avaliadores: Avaliadores de desempenho os algorítimos de ordenação em relação a vários valores de Quebras
 * e Partições testados para um mesmo vetor a fim de se definir, por fim, quais os melhores limiares de escolha.
 * 6. Pontos Extras:  
*/
class Ordenador {
    public:

        /**
         * @brief Construtor padrão
        */
        Ordenador();
        // Como Vetor usa memória dinâmica interna e tem destrutor, não precisa fazer nada extra

        // Setter dos pesos

        /**
         * @brief Define os pesos utilizados no cálculo do custo.
         * @param a_ Peso das comparações.
         * @param b_ Peso das movimentações.
         * @param c_ Peso das chamadas recursivas.
        */
        void setPesos(double a_, double b_, double c_);

        // Auxiliares

        /**
         * @brief Cria uma cópia de um vetor.
         * @param original Vetor original a ser copiado.
         * @param tam Tamanho do vetor.
         * @return Ponteiro para o novo vetor alocado com os mesmos valores.
        */
        int* copiaVetor(const int* original, int tam) const;

        /**
         * @brief Reseta os contadores internos (cmp, move, calls).
        */
        void resetCounters();

        /**
         * @brief Conta o número de quebras em um vetor (elementos fora de ordem).
         * @param v Vetor de inteiros.
         * @param tam Tamanho do vetor.
         * @return Número de quebras encontradas.
        */
        int contaQuebras(int* v, int tam);

        /**
         * @brief Troca os valores de dois inteiros.
         * @param a Ponteiro para o primeiro inteiro.
         * @param b Ponteiro para o segundo inteiro.
        */
        void swap(int *a, int *b);

        /**
         * @brief Implementa o floor personalizado para números negativos.
         * @param num Número real.
         * @return Parte inteira inferior do número.
        */
        double customFloor(double num);

        /**
         * @brief Realiza trocas aleatórias no vetor para criar um número de quebras.
         * @param vetor Vetor a ser modificado.
         * @param tam Tamanho do vetor.
         * @param limiarQuebras Quantidade de quebras desejada.
        */
        void shuffle(int* vetor, int tam, int limiarQuebras);

        // Algorítimos de ordenação

        /**
         * @brief Algoritmo de ordenação por inserção.
         * @param v Vetor a ser ordenado.
         * @param ini Índice inicial.
         * @param tam Índice final.
        */
        void insercao(int *v, int ini, int tam);

        /**
         * @brief Calcula a mediana entre três inteiros.
         * @param a Primeiro inteiro.
         * @param b Segundo inteiro.
         * @param c Terceiro inteiro.
         * @return Valor da mediana.
        */
        int median(int a, int b, int c);

        /**
         * @brief Realiza particionamento em três partes para o Quicksort.
         * @param A Vetor a ser particionado.
         * @param l Índice esquerdo.
         * @param r Índice direito.
         * @param i Ponteiro para armazenar o índice i resultante.
         * @param j Ponteiro para armazenar o índice j resultante.
        */
        void partition3(int *A, int l, int r, int *i, int *j);

        /**
         * @brief 
         * Versão recursiva do algoritmo Quicksort utilizando Inserção 
         * e mediana de valores para Partição com limiar de partição para 
         * definição de quando utilizar Inserção ou Quicksort.
         * @param v Vetor a ser ordenado.
         * @param l Índice inicial.
         * @param r Índice final.
         * @param minTamParticao Tamanho mínimo para aplicar Quicksort.
        */
        void quickSortRec(int *v, int l, int r, int minTamParticao);

        // Ordenador universal

        /**
         * @brief Ordenador que escolhe o método de ordenação com base nas quebras.
         * @param copia Vetor a ser ordenado.
         * @param tam Tamanho do vetor.
         * @param minTamParticao Limiar de tamanho para usar Quicksort.
         * @param limiarQuebras Quantidade limite de quebras para usar Insertion.
        */
        void OrdenadorUniversal(int *copia, int tam, int minTamParticao, double limiarQuebras);

        // MPS

        /**
         * @brief Calcula a diferença de custo entre os extremos da próxima faixa de MPS.
         * @param est Vetor de estatísticas.
         * @param minIdx Índice inferior.
         * @param maxIdx Índice superior.
         * @return Diferença entre o custo dos dois valores de MPS limites da próxima faixa.
        */
        float calculaDiffMPS(const Vetor<EstatisticaMPS>& est, int minIdx, int maxIdx);

        /**
         * @brief Determina o melhor limiar de partição (MPS) com base no custo.
         * @param v Vetor a ser analisado.
         * @param tam Tamanho do vetor.
         * @param limiarCusto Limiar de diferença aceitável de custo.
         * @return Melhor valor de MPS encontrado.
        */
        int determinaLimiarParticao(int *v, int tam, double limiarCusto);

        /**
         * @brief Calcula nova faixa de busca para o MPS baseado no melhor índice atual.
         * @param est Vetor de estatísticas.
         * @param melhorIdx Índice do melhor MPS até agora.
         * @param numMPS Número de amostras MPS.
         * @param minMPS Referência para novo mínimo MPS.
         * @param maxMPS Referência para novo máximo MPS.
         * @param passoMPS Referência para novo passo MPS.
         * @param minIdx Referência para o índice do mps que será o novo valor mínimo.
         * @param maxIdx Referência para o índice do mps que será o novo valor máximo.
        */
        void calculaNovaFaixaParticao(const Vetor<EstatisticaMPS>& est, int melhorIdx, int numMPS, 
                                      int& minMPS, int& maxMPS, int& passoMPS, int& minIdx, int& maxIdx);

        // LQ

        /**
         * @brief Calcula a diferença de custo entre os extremos da próxima faixa de LQ.
         * @param est Vetor de estatísticas.
         * @param minIdx Índice mínimo.
         * @param maxIdx Índice máximo.
         * @return Diferença entre o custo dos dois valores de LQ limites da próxima faixa (considerando Insertion).
         */
        float calculaDiffLQ(const Vetor<EstatisticaLQ>& est, int minIdx, int maxIdx);

        /**
         * @brief Determina o limiar de quebras ideal com base no custo.
         * @param seed Semente para geração de aleatoriedade.
         * @param v Vetor de entrada.
         * @param limiarParticao Limiar de partição usado na ordenação.
         * @param tam Tamanho do vetor.
         * @param limiarCusto Diferença de custo aceitável.
         * @return Melhor valor de limiar de quebras encontrado.
         */
        int determinaLimiarQuebras(int seed, int* v, int limiarParticao, int tam, double limiarCusto);

        /**
         * @brief Calcula a nova faixa de LQ com base no melhor índice obtido.
         * @param est Vetor de estatísticas.
         * @param melhorIdx Índice do melhor LQ.
         * @param numLQ Número de amostras.
         * @param minLQ Referência para novo mínimo LQ.
         * @param maxLQ Referência para novo máximo LQ.
         * @param passoLQ Referência para novo passo LQ.
         * @param minIdx Referência para o índice do lq que será o novo valor mínimo.
         * @param maxIdx Referência para o índice do lq que será o novo valor máximo.
         */
        void calculaNovaFaixaQuebras(const Vetor<EstatisticaLQ>& est, int melhorIdx, int numLQ, 
                                     int& minLQ, int& maxLQ, int& passoLQ, int& minIdx, int& maxIdx);

        int getCmp(){return cmp;}
        int getMove(){return move;}
        int getCalls(){return calls;}

    private:
        double a, b, c; // pesos de custo
        int cmp, move, calls; // contadores globais por execução

        int limiarMPS, limiarLQ; // limiares modificados a cada iter
        float diffMPS, diffLQ; // diferenças modificadas a cada iter

};

#endif // ORDENADOR_H