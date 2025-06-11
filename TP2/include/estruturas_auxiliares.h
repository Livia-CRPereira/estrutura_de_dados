#ifndef ESTRUTURAS_AUXILIARES_H
#define ESTRUTURAS_AUXILIARES_H

#include <stdexcept> // Necessário para std::out_of_range
#include <iostream>  // Para std::cerr
#include <cstddef>   // Necessário para size_t

// ------ VETOR ------
/**
* @brief Classe genérica de vetor dinâmico, ou redimensionável. 
* @details Implementa um vetor que gerencia seu próprio armazenamento, 
* permitindo redimensionamento automático quando novos elementos são adicionados.
* Funciona de maneira semelhante ao std::vector da biblioteca padrão do C++.
 * @tparam T O tipo de dado a ser armazenado no vetor
*/
template<typename T>
class Vetor {
    public:
        /**
         * @brief Construtor padrão.
         * @details Cria um vetor vazio sem capacidade ou elementos.
        */
        Vetor() : dados(nullptr), capacidade(0), tamanho_(0) {}

        /**
         * @brief Construtor de cópia.
         * @param outro O vetor a ser copiado.
         * @details Cria um novo vetor como uma cópia profunda de um vetor existente.
        */
        Vetor(const Vetor<T>& outro) : dados(nullptr), capacidade(0), tamanho_(0) {
            if (outro.tamanho_ > 0) {
                redimensionar(outro.capacidade);
                tamanho_ = outro.tamanho_;
                for (size_t i = 0; i < tamanho_; ++i) {
                    dados[i] = outro.dados[i];
                }
            }
        }

        /**
         * @brief Construtor que inicializa o vetor com um tamanho específico.
         * @param tamanho_inicial O número de elementos a serem alocados.
         * @details Os elementos são inicializados com o valor padrão do tipo T.
        */
        Vetor(size_t tamanho_inicial) : dados(nullptr), capacidade(0), tamanho_(0) {
            if (tamanho_inicial > 0) {
                dados = new T[tamanho_inicial];
                capacidade = tamanho_inicial;
                tamanho_ = tamanho_inicial;
            }
        }

        /**
         * @brief Operador de atribuição por cópia.
         * @param outro O vetor do qual os dados serão copiados.
         * @return Uma referência para o vetor atualizado.
        */
        Vetor<T>& operator=(const Vetor<T>& outro) {
            if (this != &outro) { 
                delete[] dados;
                dados = nullptr;
                capacidade = 0;
                tamanho_ = 0;
                if (outro.tamanho_ > 0) {
                    redimensionar(outro.capacidade);
                    tamanho_ = outro.tamanho_;
                    for (size_t i = 0; i < tamanho_; ++i) {
                        dados[i] = outro.dados[i];
                    }
                }
            }
            return *this;
        }

        /**
         * @brief Construtor que inicializa o vetor com um tamanho e valor específicos.
         * @param tamanho_inicial O número de elementos a serem alocados.
         * @param valor_inicial O valor para preencher os elementos.
        */
        Vetor(size_t tamanho_inicial, const T& valor_inicial) : dados(nullptr), capacidade(0), tamanho_(0) {
            if (tamanho_inicial > 0) {
                dados = new T[tamanho_inicial];
                capacidade = tamanho_inicial;
                tamanho_ = tamanho_inicial;
                for (size_t i = 0; i < tamanho_; ++i) {
                    dados[i] = valor_inicial;
                }
            }
        }

        /**
         * @brief Destrutor.
         * @details Libera a memória alocada dinamicamente pelo vetor.
        */
        ~Vetor() {
            delete[] dados;
        }

        /**
         * @brief Obtém um ponteiro para o primeiro elemento.
         * @return Ponteiro para o primeiro elemento.
        */
        T* begin() {
            return dados;
        }

        /**
         * @brief Obtém um ponteiro constante para o primeiro elemento.
         * @return Ponteiro constante para o primeiro elemento.
        */
        const T* begin() const {
            return dados;
        }

        /**
         * @brief Obtém um ponteiro para a posição após o último elemento.
         * @return Ponteiro para a posição após o último elemento.
        */
        T* end() {
            return dados + tamanho_;
        }

        /**
         * @brief Obtém um ponteiro constante para a posição após o último elemento.
         * @return Ponteiro constante para a posição após o último elemento.
        */
        const T* end() const {
            return dados + tamanho_;
        }

        /**
         * @brief Adiciona um elemento ao final do vetor.
         * @param valor O valor a ser adicionado.
         * @details Se a capacidade atual não for suficiente, o vetor é redimensionado.
        */
        void adicionar(const T& valor) {
            if (tamanho_ == capacidade)
                redimensionar(capacidade == 0 ? 1 : capacidade * 2);
            dados[tamanho_++] = valor;
        }

        /**
         * @brief Operador de acesso a elemento por índice.
         * @param i O índice do elemento a ser acessado.
         * @return Uma referência ao elemento no índice especificado.
         * @throws std::out_of_range se o índice for inválido.
        */
        T& operator[](size_t i) {
            if (i >= tamanho_) throw std::out_of_range("Índice inválido");
            return dados[i];
        }

        /**
         * @brief Operador de acesso a elemento por índice (versão constante).
         * @param i O índice do elemento a ser acessado.
         * @return Uma referência constante ao elemento no índice especificado.
         * @throws std::out_of_range se o índice for inválido.
        */
        const T& operator[](size_t i) const {
            if (i >= tamanho_) throw std::out_of_range("Índice inválido"); 
            return dados[i]; 
        }

        /**
         * @brief Retorna o número de elementos no vetor.
         * @return O número de elementos armazenados atualmente.
        */
        size_t tamanho() const {
            return tamanho_;
        }

    private:
        T* dados;
        size_t capacidade;
        size_t tamanho_;

        /**
         * @brief Redimensiona o armazenamento interno do vetor.
         * @param nova_capacidade A nova capacidade de armazenamento.
         * @details Aloca um novo bloco de memória, copia os elementos existentes,
         * libera o bloco antigo e atualiza a capacidade.
        */
        void redimensionar(size_t nova_capacidade) {
            T* novo_dados = new T[nova_capacidade];
            for (size_t i = 0; i < tamanho_; ++i)
                novo_dados[i] = dados[i];
            delete[] dados;
            dados = novo_dados;
            capacidade = nova_capacidade;
        }
};

// ------ HEAP MÍNIMA ------
/**
 * @brief Functor de comparação padrão para o minHeap.
 * @details Define a operação "menor que" padrão usando o operador < do tipo T.
 * Pode ser substituído por um comparador customizado na declaração do heap.
 * @tparam T O tipo de dado a ser comparado.
 */
template <typename T>
struct DefaultLess {
    /**
     * @brief Sobrecarga do operador de chamada de função.
     * @param a O primeiro elemento para comparação.
     * @param b O segundo elemento para comparação.
     * @return true se a for menor que b, false caso contrário.
     */
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

/**
 * @brief Classe genérica que implementa um heap mínimo (Min Heap).
 * @details Esta estrutura de dados é uma árvore binária especializada onde o nó pai
 * é sempre menor ou igual aos seus filhos, garantindo que o menor elemento
 * esteja sempre na raiz. A ordem é definida por um objeto Comparador.
 * @tparam T O tipo de dado a ser armazenado no heap.
 * @tparam Comparador O tipo do functor de comparação que define a ordem dos elementos.
 */
template <typename T, typename Comparador = DefaultLess<T>>
class minHeap {
    public:
        /**
         * @brief Construtor padrão.
         * @details Cria um heap mínimo vazio, sem capacidade ou elementos.
         */
        minHeap() : dados(nullptr), capacidade(0), tamanho_(0) {}

        /**
         * @brief Destrutor.
         * @details Libera toda a memória alocada dinamicamente pelo heap.
         */
        ~minHeap() {
            delete[] dados;
        }

        /**
         * @brief Insere um novo elemento no heap.
         * @param e O elemento a ser inserido.
         * @details Adiciona o elemento e o reposiciona para manter a propriedade do heap.
         * Se necessário, o armazenamento interno é redimensionado.
         */
        void inserir(const T& e) {
            if (tamanho_ == capacidade)
                redimensionar(capacidade == 0 ? 4 : capacidade * 2);
            dados[tamanho_] = e;
            subir(tamanho_);
            ++tamanho_;
        }

        /**
         * @brief Retorna uma referência ao menor elemento (a raiz do heap).
         * @return Uma referência constante ao menor elemento.
         * @throws std::runtime_error se o heap estiver vazio.
         */
        const T& verMinimo() const {
            if (vazia()) {
                throw std::runtime_error("Heap vazia. Nao e possivel ver o minimo.");
            }
            return dados[0];
        }

        /**
         * @brief Remove e retorna o menor elemento do heap.
         * @return O menor elemento, que foi removido da estrutura.
         * @throws std::runtime_error se o heap estiver vazio.
         */
        T removerMinimo() {
            if (vazia()) throw std::runtime_error("Heap vazia");
            T minimo = dados[0];
            dados[0] = dados[--tamanho_];
            descer(0);
            return minimo;
        }

        /**
         * @brief Verifica se o heap está vazio.
         * @return true se o heap não contiver elementos, false caso contrário.
         */
        bool vazia() const {
            return tamanho_ == 0;
        }

        /**
         * @brief Retorna o número de elementos no heap.
         * @return O tamanho atual do heap.
         */
        size_t tamanho() const { return tamanho_; }

    private:
        T* dados;
        size_t capacidade;
        size_t tamanho_;
        Comparador comp; 

        /**
         * @brief Troca a posição de dois elementos no array interno.
         * @param i O índice do primeiro elemento.
         * @param j O índice do segundo elemento.
         */
        void trocar(size_t i, size_t j) {
            T temp = dados[i];
            dados[i] = dados[j];
            dados[j] = temp;
        }

        /**
         * @brief Move um elemento para cima no heap (heapify-up) para manter a propriedade do heap.
         * @param i O índice do elemento a ser movido para cima.
         */
        void subir(size_t i) {
            while (i > 0) {
                size_t pai = (i - 1) / 2;
                if (comp(dados[i], dados[pai])) { 
                    trocar(i, pai);
                    i = pai;
                } else break;
            }
        }

        /**
         * @brief Move um elemento para baixo no heap (heapify-down) para manter a propriedade do heap.
         * @param i O índice do elemento a ser movido para baixo.
         */
        void descer(size_t i) {
            size_t menor = i;
            size_t esq = 2 * i + 1;
            size_t dir = 2 * i + 2;

            if (esq < tamanho_ && comp(dados[esq], dados[menor]))
                menor = esq;
            if (dir < tamanho_ && comp(dados[dir], dados[menor]))
                menor = dir;

            if (menor != i) {
                trocar(i, menor);
                descer(menor);
            }
        }

        /**
         * @brief Redimensiona o array interno que armazena os elementos do heap.
         * @param nova_capacidade A nova capacidade de armazenamento.
         */
        void redimensionar(size_t nova_capacidade) {
            T* novo_dados = new T[nova_capacidade];
            for (size_t i = 0; i < tamanho_; ++i)
                novo_dados[i] = dados[i];
            delete[] dados;
            dados = novo_dados;
            capacidade = nova_capacidade;
        }
};

// ------ LISTA ENCADEADA ------
/**
 * @brief Classe genérica que implementa uma lista simplesmente encadeada.
 * @details Armazena uma coleção de elementos em nós ligados sequencialmente.
 * Permite inserção eficiente no início e no fim da lista.
 * @tparam T O tipo de dado a ser armazenado na lista.
 */
template <typename T>
class ListaEncadeada {
    public:
        /**
         * @brief Estrutura interna que representa um nó na lista encadeada.
         * @details Cada nó contém um dado e um ponteiro para o próximo nó na sequência.
         */
        struct No {
            T dado;
            No* proximo;

            /**
             * @brief Construtor do nó.
             * @param d O dado a ser armazenado no nó.
             */
            No(const T& d) : dado(d), proximo(nullptr) {}
        };

        /**
         * @brief Construtor padrão.
         * @details Cria uma lista encadeada vazia.
         */
        ListaEncadeada() : inicio(nullptr), fim(nullptr), tamanho(0) {}

        /**
         * @brief Construtor de cópia.
         * @param outro A lista a ser copiada.
         * @details Cria uma nova lista como uma cópia profunda de uma lista existente.
         */
        ListaEncadeada(const ListaEncadeada<T>& outro) : inicio(nullptr), fim(nullptr), tamanho(0) {
            No* atual_outro = outro.inicio;
            while (atual_outro != nullptr) {
                this->adicionar(atual_outro->dado); 
                atual_outro = atual_outro->proximo;
            }
        }

        /**
         * @brief Operador de atribuição por cópia.
         * @param outro A lista da qual os dados serão copiados.
         * @return Uma referência para a lista atualizada (`*this`).
         */
        ListaEncadeada<T>& operator=(const ListaEncadeada<T>& outro) {
            if (this == &outro) {
                return *this; 
            }
            // Limpa a lista atual
            while (inicio != nullptr) {
                No* temp = inicio;
                inicio = inicio->proximo;
                delete temp;
            }
            fim = nullptr;
            tamanho = 0;

            // Copia os elementos da outra lista
            No* atual_outro = outro.inicio;
            while (atual_outro != nullptr) {
                this->adicionar(atual_outro->dado);
                atual_outro = atual_outro->proximo;
            }

            return *this;
        }

        /**
         * @brief Destrutor.
         * @details Libera a memória de todos os nós alocados pela lista.
         */
        ~ListaEncadeada() {
            while (inicio != nullptr) {
                No* temp = inicio;
                inicio = inicio->proximo;
                delete temp;
            }
        }

        /**
         * @brief Adiciona um elemento ao final da lista.
         * @param valor O valor a ser adicionado.
         */
        void adicionar(const T& valor) {
            No* novo = new No(valor);
            if (!inicio)
                inicio = fim = novo;
            else {
                fim->proximo = novo;
                fim = novo;
            }
            ++tamanho;
        }

        /**
         * @brief Adiciona um elemento no início da lista.
         * @param valor O valor a ser adicionado.
         */
        void adicionarNoInicio(const T& valor) {
            No* novo = new No(valor);
            novo->proximo = inicio;
            inicio = novo;
            if (!fim) {
                fim = novo;
            }
            ++tamanho;
        }

        /**
         * @brief Obtém o primeiro elemento da lista sem removê-lo.
         * @return O dado do primeiro elemento.
         * @throws std::runtime_error se a lista estiver vazia.
         */
        T getPrimeiro() const {
            if (!inicio) throw std::runtime_error("Lista vazia");
            return inicio->dado;
        }

        /**
         * @brief Remove e retorna o primeiro elemento da lista.
         * @return O dado do elemento que foi removido.
         * @throws std::runtime_error se a lista estiver vazia.
         */
        T removerPrimeiro() {
            if (!inicio) throw std::runtime_error("Lista vazia");
            No* temp = inicio;
            T valor = temp->dado;
            inicio = inicio->proximo;
            if (!inicio) fim = nullptr; 
            delete temp;
            --tamanho;
            return valor;
        }

        /**
         * @brief Verifica se a lista está vazia.
         * @return true se a lista não contiver elementos, false caso contrário.
         */
        bool vazia() const {
            return inicio == nullptr;
        }

        /**
         * @brief Retorna um ponteiro para o nó inicial da lista.
         * @return Um ponteiro para o primeiro nó.
         */
        No* getInicio() const { return inicio; }
        
        /**
         * @brief Retorna o número de elementos na lista.
         * @return O tamanho atual da lista.
         */
        int getTamanho() const { return tamanho; }

        /**
         * @brief Busca o primeiro nó que contém um valor específico.
         * @param valor_procurado O valor a ser encontrado na lista.
         * @return Um ponteiro para o primeiro nó com o valor especificado, ou nullptr se não encontrado.
         */
        No* buscarNoPeloValor(const T& valor_procurado) const {
            No* atual = inicio;
            while (atual != nullptr) {
                if (atual->dado == valor_procurado) {
                    return atual;
                }
                atual = atual->proximo;
            }
            return nullptr; 
        }

        /**
         * @brief Obtém o valor do nó seguinte a um nó com um valor específico.
         * @param valor_no_atual O valor do nó cujo sucessor se deseja obter.
         * @return O dado contido no nó seguinte.
         * @throws std::runtime_error se o valor não for encontrado ou se for o último da lista.
         */
        T getValorDoProximo(const T& valor_no_atual) const {
            No* no_atual = buscarNoPeloValor(valor_no_atual);

            if (no_atual == nullptr) {
                throw std::runtime_error("O valor informado não foi encontrado na lista.");
            }
            if (no_atual->proximo == nullptr) {
                throw std::runtime_error("Não há próximo elemento após o valor fornecido.");
            }
            return no_atual->proximo->dado;
        }

        /**
         * @brief Retorna o elemento em um índice específico da lista.
         * @param indice O índice do elemento a ser retornado (base 0).
         * @return O dado no índice especificado.
         * @throws std::out_of_range se o índice for negativo ou maior ou igual ao tamanho da lista.
         */
        T getElementoEmIndice(int indice) const {
            if (indice < 0 || indice >= tamanho) {
                throw std::out_of_range("Índice fora dos limites da lista.");
            }
            No* atual = inicio;
            for (int i = 0; i < indice; ++i) {
                atual = atual->proximo;
            }
            return atual->dado;
        }

    private:
        No* inicio;
        No* fim;
        int tamanho;
};

// ------ GRAFO ------
/**
 * @brief Estrutura que representa uma aresta ponderada em um grafo.
 * @details Armazena o índice do vértice de destino e o peso da aresta.
 * @tparam T_Peso O tipo de dado para o peso da aresta (ex: int, double).
 */
template <typename T_Peso>
struct Aresta {
    int idx_destino;    
    T_Peso peso;

    /**
     * @brief Construtor para uma aresta com destino e peso.
     * @param d O índice do vértice de destino.
     * @param p O peso da aresta.
     */
    Aresta(int d, T_Peso p) : idx_destino(d), peso(p) {}
    
    /**
     * @brief Construtor padrão.
     * @details Inicializa a aresta com um destino inválido (-1) e peso padrão.
     */
    Aresta() : idx_destino(-1), peso(T_Peso()) {} 
};

/**
 * @brief Estrutura que representa um vértice em um grafo.
 * @details Contém um nome (ou identificador) e uma lista de adjacências (arestas).
 * @tparam T_NomeVertice O tipo de dado para o nome/identificador do vértice.
 * @tparam T_Peso O tipo de dado para o peso das arestas que partem deste vértice.
 */
template <typename T_NomeVertice, typename T_Peso>
struct Vertice {
    T_NomeVertice nome;
    Vetor<Aresta<T_Peso>> adjacentes; 
};

/**
 * @brief Classe genérica para um grafo não-direcionado e ponderado.
 * @details Implementa um grafo usando uma lista de adjacências. Os vértices são
 * armazenados em um vetor e cada vértice contém um vetor de arestas para seus vizinhos.
 * @tparam T_NomeVertice O tipo de dado para o nome/identificador do vértice.
 * @tparam T_Peso O tipo de dado para o peso das arestas.
 */
template <typename T_NomeVertice, typename T_Peso>
class Grafo {
    public:
        /**
         * @brief Construtor padrão.
         * @details Cria um grafo vazio.
         */
        Grafo() {}

        /**
         * @brief Adiciona um novo vértice ao grafo.
         * @param nome O nome ou identificador do vértice a ser adicionado.
         * @return O índice inteiro atribuído ao novo vértice.
         */
        int adicionarVertice(const T_NomeVertice& nome) {
            Vertice<T_NomeVertice, T_Peso> v;
            v.nome = nome;
            vertices.adicionar(v);
            return vertices.tamanho() - 1;
        }

        /**
         * @brief Adiciona uma aresta não-direcionada entre dois vértices.
         * @param u O índice do primeiro vértice.
         * @param v O índice do segundo vértice.
         * @param peso O peso da aresta entre u e v.
         */
        void adicionarAresta(int u, int v, T_Peso peso) {
            if (u < 0 || static_cast<size_t>(u) >= vertices.tamanho() || v < 0 || static_cast<size_t>(v) >= vertices.tamanho()) {
                std::cerr << "Erro: Índices de vértice inválidos para adicionarAresta." << std::endl;
                return;
            }

            // Adiciona a aresta nos dois sentidos para grafo não-direcionado
            vertices[static_cast<size_t>(u)].adjacentes.adicionar(Aresta<T_Peso>(v, peso));
            vertices[static_cast<size_t>(v)].adjacentes.adicionar(Aresta<T_Peso>(u, peso));
        }

        /**
         * @brief Retorna a lista de adjacências (vizinhos) de um vértice.
         * @param v O índice do vértice.
         * @return Uma referência constante para o vetor de arestas do vértice.
         */
        const Vetor<Aresta<T_Peso>>& vizinhos(int v) const {
            if (v < 0 || static_cast<size_t>(v) >= vertices.tamanho()) {
                static const Vetor<Aresta<T_Peso>> empty_adjacentes; 
                std::cerr << "Erro: Índice de vértice inválido para vizinhos." << std::endl;
                return empty_adjacentes;
            }
            return vertices[static_cast<size_t>(v)].adjacentes;
        }

        /**
         * @brief Obtém o nome de um vértice a partir de seu índice.
         * @param v O índice do vértice.
         * @return O nome do vértice. Retorna um valor padrão se o índice for inválido.
         */
        T_NomeVertice getNome(int v) const {
            if (v < 0 || static_cast<size_t>(v) >= vertices.tamanho()) {
                return T_NomeVertice();
            }
            return vertices[static_cast<size_t>(v)].nome;
        }

        /**
         * @brief Retorna o número total de vértices no grafo.
         * @return A quantidade de vértices.
         */
        int quantidadeVertices() const {
            return vertices.tamanho();
        }

        /**
         * @brief Obtém o índice de um vértice a partir de seu nome.
         * @param nome O nome do vértice a ser procurado.
         * @return O índice do vértice, ou -1 se não for encontrado.
         */
        int getIndice(const T_NomeVertice& nome) const {
            for (size_t i = 0; i < vertices.tamanho(); ++i) {
                if (vertices[i].nome == nome) {
                    return static_cast<int>(i);
                }
            }
            return -1;
        }

        /**
         * @brief Obtém o peso da aresta entre dois vértices.
         * @param u O índice do vértice de origem.
         * @param v O índice do vértice de destino.
         * @return O peso da aresta. Retorna um valor padrão se a aresta não existir ou os índices forem inválidos.
         */
        T_Peso getPesoAresta(int u, int v) const {
            if (u < 0 || static_cast<size_t>(u) >= vertices.tamanho() || v < 0 || static_cast<size_t>(v) >= vertices.tamanho()) {
                return T_Peso();
            }
            for (const auto& aresta : vertices[static_cast<size_t>(u)].adjacentes) {
                if (aresta.idx_destino == v) {
                    return aresta.peso;
                }
            }
            return T_Peso(); // Retorna peso padrão se não encontrar a aresta
        }


    private:
        /**
         * @brief O vetor que armazena todos os vértices do grafo.
         */
        Vetor<Vertice<T_NomeVertice, T_Peso>> vertices;
};


// ------ PILHA ------
/**
 * @brief Classe genérica que implementa uma pilha (Stack).
 * @details Implementa uma estrutura de dados LIFO (Last-In, First-Out) 
 * utilizando uma lista encadeada. As operações de empilhar e desempilhar
 * ocorrem no topo da pilha.
 * @tparam T O tipo de dado a ser armazenado na pilha.
 */
template<typename T>
class Pilha {
    public:
        /**
         * @brief Construtor padrão.
         * @details Cria uma pilha vazia.
         */
        Pilha() : topo_(nullptr) {}

        /**
         * @brief Construtor de cópia.
         * @param outro A pilha a ser copiada.
         * @details Cria uma nova pilha como uma cópia profunda de outra,
         * mantendo a ordem original dos elementos.
         */
        Pilha(const Pilha<T>& outro) : topo_(nullptr) {
            if (outro.topo_ == nullptr) {
                return;
            }
            // Usa uma pilha auxiliar para inverter a ordem duas vezes,
            // garantindo que a cópia tenha a mesma ordem da original.
            Pilha<T> aux;
            No* atual_outro = outro.topo_;
            while (atual_outro != nullptr) {
                aux.empilhar(atual_outro->dado);
                atual_outro = atual_outro->proximo;
            }
            while (!aux.vazia()) {
                this->empilhar(aux.desempilhar());
            }
        }
    
        /**
         * @brief Operador de atribuição por cópia.
         * @param outro A pilha da qual os dados serão copiados.
         * @return Uma referência para a pilha atualizada.
         */
        Pilha<T>& operator=(const Pilha<T>& outro) {
            if (this == &outro) {
                return *this; 
            }

            // Limpa a pilha atual antes de copiar.
            while (!vazia()) {
                desempilhar();
            }

            // Copia os elementos da outra pilha, preservando a ordem.
            if (outro.topo_ != nullptr) {
                Pilha<T> aux;
                No* atual_outro = outro.topo_;
                while (atual_outro != nullptr) {
                    aux.empilhar(atual_outro->dado);
                    atual_outro = atual_outro->proximo;
                }
                while (!aux.vazia()) {
                    this->empilhar(aux.desempilhar());
                }
            }
            
            return *this;
        }

        /**
         * @brief Destrutor.
         * @details Libera a memória de todos os nós da pilha.
         */
        ~Pilha() {
            while (!vazia()) {
                desempilhar();
            }
        }

        /**
         * @brief Calcula e retorna o número de elementos na pilha.
         * @return O número de elementos na pilha.
         * @details A complexidade desta operação é O(n), pois percorre a lista.
         */
        size_t tamanho() const {
            size_t count = 0;
            No* current = topo_;
            while (current != nullptr) {
                count++;
                current = current->proximo;
            }
            return count;
        }
        
        /**
         * @brief Adiciona um item no topo da pilha (operação "push").
         * @param item O item a ser adicionado.
         */
        void empilhar(const T& item) {
            topo_ = new No(item, topo_);
        }

        /**
         * @brief Remove e retorna o item do topo da pilha (operação "pop").
         * @return O item que estava no topo.
         * @throws std::runtime_error se a pilha estiver vazia.
         */
        T desempilhar() {
            if (vazia()) {
                throw std::runtime_error("Pilha vazia");
            }
            No* velho_topo = topo_;
            T valor = velho_topo->dado;
            topo_ = velho_topo->proximo;
            delete velho_topo;
            return valor;
        }

        /**
         * @brief Verifica se a pilha está vazia.
         * @return true se a pilha não contiver elementos, false caso contrário.
         */
        bool vazia() const {
            return topo_ == nullptr;
        }

        /**
         * @brief Retorna o elemento em um índice específico, contando a partir do topo.
         * @param indice O índice do elemento desejado (0 para o topo, 1 para o segundo, etc.).
         * @return O dado no índice especificado.
         * @throws std::out_of_range se o índice for inválido.
         */
        T getElementoEmIndice(int indice) const {
            if (indice < 0 || static_cast<size_t>(indice) >= tamanho()) {
                throw std::out_of_range("Indice fora dos limites da pilha.");
            }
            No* atual = topo_;
            for (int i = 0; i < indice; ++i) {
                atual = atual->proximo;
            }
            return atual->dado;
        }
    
    private:
        /**
         * @brief Estrutura interna que representa um nó na pilha.
         * @details Cada nó contém o dado e um ponteiro para o nó abaixo dele.
         */
        struct No {
            T dado;
            No* proximo;

            /**
             * @brief Construtor do nó.
             * @param d O dado a ser armazenado no nó.
             * @param p Ponteiro para o próximo nó (o nó que estava no topo anteriormente).
             */
            No(const T& d, No* p = nullptr) : dado(d), proximo(p) {}
        };

        /**
         * @brief Ponteiro para o nó no topo da pilha.
         */
        No* topo_;
};


#endif // ESTRUTURAS_AUXILIARES_H