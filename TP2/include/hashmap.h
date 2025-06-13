#ifndef HASHMAP_H
#define HASHMAP_H

#include "estruturas_auxiliares.h"

/**
 * @brief Estrutura para armazenar pares de chave-valor nos baldes do HashMap.
 */
template<typename K, typename V>
struct ParChaveValor {
    K chave;
    V valor;
};

/**
 * @brief Implementação de um HashMap usando a técnica de Encadeamento Separado.
 * @details Mapeia chaves de tipo K para valores de tipo V com tempo médio de
 * inserção e busca de O(1).
 * @tparam K O tipo da chave (deve ser um tipo integral para o hash padrão).
 * @tparam V O tipo do valor a ser armazenado.
 */
template<typename K, typename V>
class HashMap {
    public:
        /**
        * @brief Construtor do HashMap.
        * @param capacidade A quantidade de baldes (buckets) a serem criados.
        * Um número primo maior que o número esperado de itens é uma boa escolha.
        */
        HashMap(size_t capacidade = 101) : num_elementos(0), capacidade_buckets(capacidade) {
            buckets.adicionar(ListaEncadeada<ParChaveValor<K, V>>()); // Adiciona uma lista vazia
            buckets = Vetor<ListaEncadeada<ParChaveValor<K, V>>>(capacidade);
        }

        /**
        * @brief Insere um par chave-valor no mapa.
        * @details Se a chave já existir, o valor é atualizado.
        * @param chave A chave para inserção.
        * @param valor O valor associado à chave.
        */
        void inserir(const K& chave, const V& valor) {
            size_t index = hash(chave) % capacidade_buckets;
            ListaEncadeada<ParChaveValor<K, V>>& lista = buckets[index];

            // Verifica se a chave já existe para atualizar o valor
            typename ListaEncadeada<ParChaveValor<K, V>>::No* atual = lista.getInicio();
            while (atual != nullptr) {
                if (atual->dado.chave == chave) {
                    atual->dado.valor = valor; // Atualiza e retorna
                    return;
                }
                atual = atual->proximo;
            }

            // Se não encontrou, insere um novo par
            ParChaveValor<K, V> novo_par;
            novo_par.chave = chave;
            novo_par.valor = valor;
            lista.adicionar(novo_par);
            num_elementos++;
        }

        /**
        * @brief Busca um valor associado a uma chave.
        * @param chave A chave a ser buscada.
        * @return Um ponteiro para o valor se a chave for encontrada, ou nullptr caso contrário.
        */
        V* buscar(const K& chave) {
            size_t index = hash(chave) % capacidade_buckets;
            ListaEncadeada<ParChaveValor<K, V>>& lista = buckets[index];

            typename ListaEncadeada<ParChaveValor<K, V>>::No* atual = lista.getInicio();
            while (atual != nullptr) {
                if (atual->dado.chave == chave) {
                    return &(atual->dado.valor); // Retorna o endereço do valor
                }
                atual = atual->proximo;
            }

            return nullptr; // Não encontrado
        }

    private:
        /**
        * @brief Função de hash simples para chaves integrais.
        * @param chave A chave a ser convertida em hash.
        * @return O valor de hash.
        */
        size_t hash(const K& chave) const {
            // Para tipos integrais, a própria chave é um bom hash.
            return static_cast<size_t>(chave);
        }

        Vetor<ListaEncadeada<ParChaveValor<K, V>>> buckets;
        size_t num_elementos;
        size_t capacidade_buckets;
};

#endif // HASHMAP_H