#ifndef VETOR_H
#define VETOR_H

#include <stdexcept>
#include <iostream>

/**
 * @brief Classe genérica de vetor dinâmico, ou redimensionável.
 * @details Implementa um vetor com alocação dinâmica, capaz de armazenar elementos de qualquer tipo.
*/
template <typename T>
class Vetor {
    public:
        /**
         * @brief Construtor padrão.
        */
        Vetor() : dados(nullptr), capacidade(0), tamanho(0) {}

        /**
         * @brief Construtor com capacidade inicial.
         * @param tam Capacidade inicial do vetor.
        */
        Vetor(int tam) : capacidade(tam), tamanho(0) {
            if (tam < 0) throw std::invalid_argument("Capacidade negativa.");
            dados = (capacidade > 0) ? new T[capacidade] : nullptr;
        }

        /**
         * @brief Construtor de cópia.
        */
        Vetor(const Vetor<T>& outro) : capacidade(outro.capacidade), tamanho(outro.tamanho) {
            dados = (capacidade > 0) ? new T[capacidade] : nullptr;
            for (int i = 0; i < tamanho; ++i) {
                dados[i] = outro.dados[i];
            }
        }

        /**
         * @brief Operador de atribuição por cópia.
         * @param outro Índice do elemento.
         * @return Vetor atualizado.
        */
        Vetor<T>& operator=(const Vetor<T>& outro) {
            if (this != &outro) {
                T* novo = (outro.capacidade > 0) ? new T[outro.capacidade] : nullptr;
                for (int i = 0; i < outro.tamanho; ++i) {
                    novo[i] = outro.dados[i];
                }
                delete[] dados;
                dados = novo;
                capacidade = outro.capacidade;
                tamanho = outro.tamanho;
            }
            return *this;
        }

        /**
         * @brief Destrutor.
        */
        ~Vetor() {
            delete[] dados;
        }

        /**
         * @brief Insere um elemento no final do vetor.
         * @param valor Elemento a ser inserido.
        */
        void adicionar(const T& valor) {
            if (tamanho >= capacidade) {
                redimensionar(capacidade == 0 ? 4 : capacidade * 2);
            }
            dados[tamanho++] = valor;
        }

        /**
         * @brief Acessa um elemento por índice (com verificação).
         * @param i Índice do elemento.
         * @return Referência ao elemento.
        */
        T& operator[](int i) {
            if (i < 0 || i >= tamanho) {
                throw std::out_of_range("Índice fora do intervalo.");
            }
            return dados[i];
        }

        /**
         * @brief Acessa um elemento constante por índice (com verificação).
         * @param i Índice do elemento.
         * @return Referência ao elemento.
        */
        const T& operator[](int i) const {
            /*
            A existência de duas funções de sobrecarga permite que o compilador decida automaticamente
            entre os dois, dependendo se o elemento do vetor a ser modificado é mutável ou constante.
            */
            if (i < 0 || i >= tamanho) {
                throw std::out_of_range("Índice fora do intervalo.");
            }
            return dados[i];
        }

        /**
         * @brief Retorna o número de elementos armazenados.
         * @return  Tamanho do Vetor.
        */
        int getTamanho() const {
            return tamanho;
        }

        /**
         * @brief Remove todos os elementos do vetor e libera a memória.
        */
        void limpar() {
            delete[] dados;
            dados = nullptr;
            capacidade = 0;
            tamanho = 0;
        }

        /**
         * @brief Retorna o último elemento do vetor.
         * @return Valor (chave) do último elemento do vetor.
        */
        const T& ultimo() const {
            if (tamanho == 0) throw std::out_of_range("Vetor vazio.");
            return dados[tamanho - 1];
        }

    private:
        T* dados;
        int capacidade;
        int tamanho;

        /**
         * @brief Redimensiona a capacidade do vetor.
         * @param novaCapacidade Nova capacidade desejada.
         */
        void redimensionar(int novaCapacidade) {
            if (novaCapacidade <= capacidade) return;
            T* novo = new T[novaCapacidade];
            for (int i = 0; i < tamanho; ++i) {
                novo[i] = dados[i];
            }
            delete[] dados;
            dados = novo;
            capacidade = novaCapacidade;
        }
};

#endif // VETOR_H


