#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <string>
#include <stdexcept>
#include <iostream>

/**
 * @brief Implementação de um vetor (array dinâmico) genérico.
 * @details Permite armazenamento contíguo de elementos com redimensionamento automático.
 * @tparam T O tipo do dado a ser armazenado no vetor.
 */
template<typename T>
class Vetor {
    public:
        /**
        * @brief Construtor padrão. Cria um vetor vazio.
        */
        Vetor() : dados(nullptr), capacidade_(0), tamanho_(0) {}

        /**
        * @brief Construtor que cria um vetor com um tamanho e valor inicial definidos.
        * @param tamanho_req O tamanho inicial do vetor.
        * @param valor_inicial O valor com o qual preencher todas as posições.
        */
        Vetor(size_t tamanho_req, const T& valor_inicial) {
            dados = new T[tamanho_req];
            capacidade_ = tamanho_req;
            tamanho_ = tamanho_req;
            for (size_t i = 0; i < tamanho_; ++i) {
                dados[i] = valor_inicial;
            }
        }

        /**
        * @brief Destrutor. Libera a memória alocada para os dados do vetor.
        */
        ~Vetor() {
            delete[] dados;
        }

        /**
        * @brief Adiciona um novo elemento ao final do vetor.
        * @details Se necessário, redimensiona o vetor para acomodar o novo elemento.
        * @param valor O elemento a ser adicionado.
        */
        void adicionar(const T& valor) {
            if (tamanho_ == capacidade_) {
                redimensionar(capacidade_ == 0 ? 1 : capacidade_ * 2);
            }
            dados[tamanho_++] = valor;
        }

        /**
        * @brief Acessa um elemento do vetor por seu índice (versão não-const).
        * @param i O índice do elemento a ser acessado.
        * @return Uma referência ao elemento no índice especificado.
        * @throw std::out_of_range se o índice for inválido.
        */
        T& operator[](size_t i) {
            if (i >= tamanho_) throw std::out_of_range("Índice do vetor inválido");
            return dados[i];
        }

        /**
        * @brief Acessa um elemento do vetor por seu índice (versão const).
        * @param i O índice do elemento a ser acessado.
        * @return Uma referência constante ao elemento no índice especificado.
        * @throw std::out_of_range se o índice for inválido.
        */
        const T& operator[](size_t i) const {
            if (i >= tamanho_) throw std::out_of_range("Índice do vetor inválido");
            return dados[i];
        }

        /**
        * @brief Retorna o número de elementos atualmente no vetor.
        * @return O tamanho do vetor.
        */
        size_t tamanho() const {
            return tamanho_;
        }

    private:
        /**
        * @brief Redimensiona o armazenamento interno do vetor.
        * @param nova_capacidade A nova capacidade desejada para o vetor.
        */
        void redimensionar(size_t nova_capacidade) {
            T* novo_dados = new T[nova_capacidade];
            size_t elementos_a_copiar = (tamanho_ < nova_capacidade) ? tamanho_ : nova_capacidade;
            
            for (size_t i = 0; i < elementos_a_copiar; ++i) {
                novo_dados[i] = dados[i];
            }
            
            delete[] dados;
            dados = novo_dados;
            capacidade_ = nova_capacidade;
        }

        T* dados; ///< Ponteiro para o array de dados alocado dinamicamente.
        size_t capacidade_; ///< A capacidade total de armazenamento do vetor.
        size_t tamanho_;    ///< O número de elementos atualmente no vetor.
};

/**
 * @brief Implementação de uma Árvore AVL auto-balanceada.
 * @details Garante que as operações de inserção e busca tenham complexidade de
 * O(log N) no pior caso, mantendo a árvore balanceada através de rotações.
 * @tparam Chave O tipo da chave de ordenação.
 * @tparam Valor O tipo do valor a ser armazenado.
 */
template<typename Chave, typename Valor>
class ArvoreAVL {
    public:
        /**
        * @brief Construtor padrão. Cria uma árvore vazia.
        */
        ArvoreAVL() : raiz(nullptr) {}

        /**
        * @brief Destrutor. Libera a memória de todos os nós da árvore.
        */
        ~ArvoreAVL() { limpar(raiz); }

        /**
        * @brief Insere um par chave-valor na árvore.
        * @details Se a chave já existir, o valor é atualizado.
        * @param chave A chave para inserção.
        * @param valor O valor associado à chave.
        */
        void inserir(Chave chave, Valor valor) { raiz = inserir(raiz, chave, valor); }

        /**
        * @brief Busca um valor associado a uma chave.
        * @param chave A chave a ser buscada.
        * @return Um ponteiro para o valor se a chave for encontrada, ou nullptr caso contrário.
        */
        Valor* buscar(Chave chave) const {
            No* resultado = buscar(raiz, chave);
            return (resultado == nullptr) ? nullptr : &(resultado->valor);
        }
        
        /**
        * @brief Busca todos os valores cujas chaves estão dentro de um intervalo.
        * @param inicio A chave de início do intervalo (inclusivo).
        * @param fim A chave de fim do intervalo (inclusivo).
        * @param resultado Um vetor onde os valores encontrados serão armazenados.
        */
        void buscarEmFaixa(Chave inicio, Chave fim, Vetor<Valor>& resultado) const {
            buscarEmFaixa(raiz, inicio, fim, resultado);
        }
        
        /**
        * @brief Executa uma função de limpeza para cada valor na árvore.
        * @details Útil para liberar memória quando os valores são ponteiros.
        * @param deletarValor Um ponteiro de função que sabe como deletar um objeto do tipo Valor.
        */
        void limparValores(void (*deletarValor)(Valor)) {
            limparValores(raiz, deletarValor);
        }

        /**
        * @brief Realiza uma travessia em ordem na árvore, executando uma função em cada nó.
        * @param visitar Um ponteiro de função a ser executado para cada valor na árvore, em ordem.
        */
        void travessiaEmOrdem(void (*visitar)(Valor)) const {
            travessiaEmOrdem(raiz, visitar);
        }

    private:
        /**
        * @brief Estrutura interna para representar um nó da Árvore AVL.
        */
        struct No {
            Chave chave;
            Valor valor;
            No *esquerda;
            No *direita;
            int altura;

            No(Chave c, Valor v) : chave(c), valor(v), esquerda(nullptr), direita(nullptr), altura(1) {}
        };

        No* raiz; ///< Ponteiro para o nó raiz da árvore.

        /**
        * @brief Função auxiliar para obter a altura de um nó.
        * @param N O nó cuja altura será calculada.
        * @return A altura do nó, ou 0 se o nó for nulo.
        */
        int altura(No* N) const {
            if (N == nullptr) return 0;
            return N->altura;
        }
        
        /**
        * @brief Função auxiliar para obter o maior de dois inteiros.
        */
        int max(int a, int b) const {
            return (a > b) ? a : b;
        }

        /**
        * @brief Realiza uma rotação simples à direita.
        */
        No* rotacaoDireita(No* y);

        /**
        * @brief Realiza uma rotação simples à esquerda.
        */
        No* rotacaoEsquerda(No* x);

        /**
        * @brief Calcula o fator de balanceamento de um nó.
        * @return O fator de balanceamento (altura da subárvore esquerda - altura da direita).
        */
        int fatorBalanceamento(No* N) const;

        /**
        * @brief Função auxiliar recursiva para inserir um nó na árvore.
        */
        No* inserir(No* no, Chave chave, Valor valor);
        
        /**
        * @brief Função auxiliar recursiva para buscar um nó na árvore.
        */
        No* buscar(No* no, Chave chave) const;

        /**
        * @brief Função auxiliar recursiva para buscar nós em um intervalo.
        */
        void buscarEmFaixa(No* no, Chave inicio, Chave fim, Vetor<Valor>& resultado) const;

        /**
        * @brief Função auxiliar recursiva para deletar todos os nós da árvore.
        */
        void limpar(No* no);

        /**
        * @brief Função auxiliar recursiva para limpar os valores armazenados.
        */
        void limparValores(No* no, void (*deletarValor)(Valor));

        /**
        * @brief Função auxiliar recursiva para a travessia em ordem.
        */
        void travessiaEmOrdem(No* no, void (*visitar)(Valor)) const;
};

// Implementações das funções privadas da ArvoreAVL que não são triviais
template<typename Chave, typename Valor>
typename ArvoreAVL<Chave, Valor>::No* ArvoreAVL<Chave, Valor>::rotacaoDireita(No* y) {
    No* x = y->esquerda;
    No* T2 = x->direita;
    x->direita = y;
    y->esquerda = T2;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    return x;
}

template<typename Chave, typename Valor>
typename ArvoreAVL<Chave, Valor>::No* ArvoreAVL<Chave, Valor>::rotacaoEsquerda(No* x) {
    No* y = x->direita;
    No* T2 = y->esquerda;
    y->esquerda = x;
    x->direita = T2;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    return y;
}

template<typename Chave, typename Valor>
int ArvoreAVL<Chave, Valor>::fatorBalanceamento(No* N) const {
    if (N == nullptr) return 0;
    return altura(N->esquerda) - altura(N->direita);
}

template<typename Chave, typename Valor>
typename ArvoreAVL<Chave, Valor>::No* ArvoreAVL<Chave, Valor>::inserir(No* no, Chave chave, Valor valor) {
    if (no == nullptr) return(new No(chave, valor));

    if (chave < no->chave)
        no->esquerda = inserir(no->esquerda, chave, valor);
    else if (chave > no->chave)
        no->direita = inserir(no->direita, chave, valor);
    else {
        no->valor = valor;
        return no;
    }

    no->altura = 1 + max(altura(no->esquerda), altura(no->direita));
    int balance = fatorBalanceamento(no);

    if (balance > 1 && chave < no->esquerda->chave)
        return rotacaoDireita(no);
    if (balance < -1 && chave > no->direita->chave)
        return rotacaoEsquerda(no);
    if (balance > 1 && chave > no->esquerda->chave) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }
    if (balance < -1 && chave < no->direita->chave) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }

    return no;
}

template<typename Chave, typename Valor>
typename ArvoreAVL<Chave, Valor>::No* ArvoreAVL<Chave, Valor>::buscar(No* no, Chave chave) const {
    if (no == nullptr || no->chave == chave) return no;
    if (chave < no->chave) return buscar(no->esquerda, chave);
    else return buscar(no->direita, chave);
}

template<typename Chave, typename Valor>
void ArvoreAVL<Chave, Valor>::buscarEmFaixa(No* no, Chave inicio, Chave fim, Vetor<Valor>& resultado) const {
    if (no == nullptr) return;
    if (inicio < no->chave) buscarEmFaixa(no->esquerda, inicio, fim, resultado);
    if (inicio <= no->chave && no->chave <= fim) resultado.adicionar(no->valor);
    if (fim > no->chave) buscarEmFaixa(no->direita, inicio, fim, resultado);
}

template<typename Chave, typename Valor>
void ArvoreAVL<Chave, Valor>::limpar(No* no) {
    if (no != nullptr) {
        limpar(no->esquerda);
        limpar(no->direita);
        delete no;
    }
}

template<typename Chave, typename Valor>
void ArvoreAVL<Chave, Valor>::limparValores(No* no, void (*deletarValor)(Valor)) {
    if (no != nullptr) {
        limparValores(no->esquerda, deletarValor);
        limparValores(no->direita, deletarValor);
        deletarValor(no->valor);
    }
}

template<typename Chave, typename Valor>
void ArvoreAVL<Chave, Valor>::travessiaEmOrdem(No* no, void (*visitar)(Valor)) const {
    if (no == nullptr) return;
    travessiaEmOrdem(no->esquerda, visitar);
    visitar(no->valor);
    travessiaEmOrdem(no->direita, visitar);
}


/**
 * @brief Estrutura para armazenar um par de chave-valor.
 */
template<typename Chave, typename Valor>
struct Par {
    Chave chave;
    Valor valor;
};

/**
 * @brief Estrutura para um nó da lista encadeada usada para tratar colisões.
 */
template<typename Chave, typename Valor>
struct NoHash {
    Par<Chave, Valor> par;
    NoHash* proximo = nullptr;
};

/**
 * @brief Implementação de um HashMap usando a técnica de Encadeamento Separado.
 * @details Mapeia chaves para valores com tempo médio de inserção e busca de O(1).
 * @tparam Chave O tipo da chave.
 * @tparam Valor O tipo do valor a ser armazenado.
 */
template<typename Chave, typename Valor>
class HashMap {
    public:
        /**
        * @brief Construtor do HashMap.
        * @param cap_inicial A quantidade inicial de baldes (buckets).
        */
        HashMap(size_t cap_inicial = 101) : 
            capacidade(cap_inicial), 
            tabela(cap_inicial, nullptr),
            tamanho(0)
        {}

        /**
        * @brief Destrutor. Libera a memória de todos os nós do HashMap.
        */
        ~HashMap() {
            for (size_t i = 0; i < capacidade; ++i) {
                NoHash<Chave, Valor>* atual = tabela[i];
                while (atual != nullptr) {
                    NoHash<Chave, Valor>* a_deletar = atual;
                    atual = atual->proximo;
                    delete a_deletar;
                }
            }
        }

        /**
        * @brief Insere um par chave-valor no mapa.
        * @details Se a chave já existir, o valor é atualizado.
        * @param chave A chave para inserção.
        * @param valor O valor associado à chave.
        */
        void inserir(const Chave& chave, const Valor& valor) {
            size_t indice = funcaoHash(chave);
            
            NoHash<Chave, Valor>* atual = tabela[indice];
            while(atual != nullptr) {
                if (atual->par.chave == chave) {
                    atual->par.valor = valor; // Atualiza o valor se a chave já existe
                    return;
                }
                atual = atual->proximo;
            }

            // Insere um novo nó no início da lista encadeada
            NoHash<Chave, Valor>* novo_no = new NoHash<Chave, Valor>();
            novo_no->par.chave = chave;
            novo_no->par.valor = valor;
            novo_no->proximo = tabela[indice];
            tabela[indice] = novo_no;
            tamanho++;
        }

        /**
        * @brief Busca um valor associado a uma chave.
        * @param chave A chave a ser buscada.
        * @return Um ponteiro para o valor se a chave for encontrada, ou nullptr caso contrário.
        */
        Valor* buscar(const Chave& chave) const {
            size_t indice = funcaoHash(chave);
            
            NoHash<Chave, Valor>* atual = tabela[indice];
            while (atual != nullptr) {
                if (atual->par.chave == chave) {
                    return &(atual->par.valor); // Retorna o endereço do valor
                }
                atual = atual->proximo;
            }
            return nullptr; // Não encontrado
        }
        
        /**
        * @brief Executa uma função de limpeza para cada valor no mapa.
        * @param deletarValor Um ponteiro de função que sabe como deletar um objeto do tipo Valor.
        */
        void limparValores(void (*deletarValor)(Valor)) {
            for (size_t i = 0; i < capacidade; ++i) {
                NoHash<Chave, Valor>* atual = tabela[i];
                while (atual != nullptr) {
                    deletarValor(atual->par.valor);
                    atual = atual->proximo;
                }
            }
        }

        /**
        * @brief Coleta todos os valores armazenados no HashMap.
        * @param resultado Um vetor onde os valores coletados serão adicionados.
        */
        void getValores(Vetor<Valor>& resultado) const {
            for (size_t i = 0; i < capacidade; ++i) {
                NoHash<Chave, Valor>* atual = tabela[i];
                while (atual != nullptr) {
                    resultado.adicionar(atual->par.valor);
                    atual = atual->proximo;
                }
            }
        }
    private:
        size_t capacidade;  ///< Número de baldes (buckets) na tabela.
        Vetor<NoHash<Chave, Valor>*> tabela; ///< O vetor de ponteiros para as listas encadeadas.
        size_t tamanho;     ///< Número total de elementos no mapa.

        /**
        * @brief Função de hash para chaves do tipo std::string (algoritmo djb2).
        * @param chave A chave a ser convertida em hash.
        * @return O valor de hash.
        */
        size_t funcaoHash(const std::string& chave) const {
            unsigned long hash = 5381;
            for (char c : chave) {
                hash = ((hash << 5) + hash) + c; // hash * 33 + c
            }
            return hash % capacidade;
        }

        /**
        * @brief Função de hash para chaves do tipo int.
        * @param chave A chave a ser convertida em hash.
        * @return O valor de hash.
        */
        size_t funcaoHash(int chave) const {
            return static_cast<size_t>(chave) % capacidade;
        }
};

#endif // ESTRUTURAS_H