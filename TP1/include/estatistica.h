#ifndef ESTATISTICA_H
#define ESTATISTICA_H

#include "vetor.h"

/**
 * @file
 * @brief Define dois Tipos Abstratos de Dados (TADs) usados na análise de desempenho de algoritmos de ordenação.
 * 
 * @details
 * O arquivo apresenta dois Tipos Abstratos de Dados (TADs) utilizados para estruturar os dados de avaliação de parâmetros
 * em algoritmos de ordenação, com foco em determinar a eficiência de estratégias (em relação a quebras e partições)
 * entre QuickSort e InsertionSort.
 * 
 * Conceitos:
 * - MPS (Minimum Partition Size): Tamanho mínimo da partição a partir do qual se torna mais vantajoso trocar 
 *                                  o InsertionSort pelo QuickSort.
 * - LQ (Limiar de Quebras): Quantidade de quebras (quantidade de vezes em que v[i] > v[i + 1]) que define 
 *                          quando passar a utilizar o QuickSort.
 * 
 * Cada TAD contém os seguintes atributos:
 * - valorMPS ou valorLQ: Valor do limiar sendo testado.
 * - algoritmo: Nome do algoritmo de ordenação utilizado na medição.
 * - custo: Valor do custo computado com base em pesos de comparações, movimentações e chamadas recursivas.
 * - comparacoes: Número de comparações realizadas durante a ordenação.
 * - movimentacoes: Número de movimentações de elementos realizadas.
 * - chamadas: Número de chamadas ao algoritmo de ordenação.
 */


/**
 * @brief Estrutura de estatísticas para uma variação de MPS.
 * @details Guarda os dados de desempenho para um valor específico de MPS.
*/
class EstatisticaMPS {
    public:

        /**
         * @brief Construtor com parâmetros.
        */
        EstatisticaMPS(){
            valorMPS = comparacoes = movimentacoes = chamadas = 0;
        }

        /**
         * @brief Retorna o valorMPS.
         * @return Valor do limiar de Partição testado.
        */
        int getValor()const {return valorMPS;}

        /**
         * @brief Retorna o custo.
         * @return Valor do custo com base em comparaçõesm movimentações e chamadas.
        */
        double getCusto()const {return custo;}

        /**
         * @brief Retorna o número de comparações.
         * @return Quantidade de comparações feitas entre elementos no processo de ordenação.
        */
        int getCmp()const {return comparacoes;}

        /**
         * @brief Retorna o número de movimentações.
         * @return Quantidade de trocas feitas entre elementos no processo de ordenação.
        */
        int getMove()const {return movimentacoes;}

        /**
         * @brief Retorna o número de chamadas.
         * @return Quantidade de chamadas a funç~eos de ordenação no processo de ordenação.
        */
        int getCall()const {return chamadas;}

        /**
         * @brief Define o valorMPS.
         * @param v valor do limiar de Partição utilizado.
        */
        void setValor(int v){valorMPS = v;}

        /**
         * @brief Define o custo.
         * @param a, b, c pesos de comparações, movimentaç~eos e chamadas no custo total.
        */
        void setCusto(double a, double b, double c){
            custo = ((a * comparacoes) + (b * movimentacoes) + (c * chamadas));
        }

        /**
         * @brief Define a quantidade de comparações.
         * @param v quantidade de comparações (já calculada).
        */
        void setCmp(int v){comparacoes = v;}

        /**
         * @brief Define a quantidade de movimentações.
         * @param v quantidade de movimentações (já calculada).
        */
        void setMove(int v){movimentacoes = v;}

        /**
         * @brief Define a quantidade de chamadas.
         * @param v quantidade de chamadas (já calculada).
        */
        void setCall(int v){chamadas = v;}

    private:
        int valorMPS;
        double custo;      
        int comparacoes;
        int movimentacoes;
        int chamadas;

};

/**
 * @brief Enumeração para tipo de algoritmo usado.
 * @details Sigla qs para Quicksort e in para InsertionSort.
*/
enum TipoAlgoritmo { qs, in };

/**
 * @brief Estrutura de estatísticas para uma variação de LQ.
 * @details Guarda os dados de desempenho para um valor específico de LQ com um algoritmo específico.
*/
class EstatisticaLQ {
    public:

        /**
         * @brief Construtor com parâmetros.
        */
        EstatisticaLQ(){
            algoritmo = qs; //arbitrariamente padrão
            valorLQ = comparacoes = movimentacoes = chamadas = 0;
        }

        /**
         * @brief Retorna o algoritmo.
         * @return Algorítimo utilizado para se chegar aos resultados.
        */
        TipoAlgoritmo getAlgoritmo()const {return algoritmo;}

        /**
         * @brief Retorna o valorLQ.
         * @return Valor do limiar de Quebras testado.
        */
        int getValor()const {return valorLQ;}
        
        /**
         * @brief Retorna o custo.
         * @return Valor do custo com base em comparaçõesm movimentações e chamadas.
        */
        double getCusto()const {return custo;}

        /**
         * @brief Retorna o número de comparações.
         * @return Quantidade de comparações feitas entre elementos no processo de ordenação.
        */
        int getCmp()const {return comparacoes;}

        /**
         * @brief Retorna o número de movimentações.
         * @return Quantidade de trocas feitas entre elementos no processo de ordenação.
        */
        int getMove()const {return movimentacoes;}

        /**
         * @brief Retorna o número de chamadas.
         * @return Quantidade de chamadas a funç~eos de ordenação no processo de ordenação.
        */
        int getCall()const {return chamadas;}

        /**
         * @brief Retorna o algoritmo.
         * @param tipo Algorítimo utilizado para se chegar aos resultados.
        */
        void setAlgoritmo(TipoAlgoritmo tipo){algoritmo = tipo;}

        /**
         * @brief Define o valorMPS.
         * @param v valor do limiar de Quebras utilizado.
        */
        void setValor(int v){valorLQ = v;}
        
        /**
         * @brief Define o custo.
         * @param a, b, c pesos de comparações, movimentaç~eos e chamadas no custo total.
        */
        void setCusto(double a, double b, double c){
            custo = ((a * comparacoes) + (b * movimentacoes) + (c * chamadas));
        }

        /**
         * @brief Define a quantidade de comparações.
         * @param v quantidade de comparações (já calculada).
        */
        void setCmp(int v){comparacoes = v;}

        /**
         * @brief Define a quantidade de movimentações.
         * @param v quantidade de movimentações (já calculada).
        */
        void setMove(int v){movimentacoes = v;}

        /**
         * @brief Define a quantidade de chamadas.
         * @param v quantidade de chamadas (já calculada).
        */
        void setCall(int v){chamadas = v;}
    
    private:
        TipoAlgoritmo algoritmo;  
        int valorLQ;
        double custo;              
        int comparacoes;
        int movimentacoes;
        int chamadas;
};

#endif // ESTATISTICA_H



