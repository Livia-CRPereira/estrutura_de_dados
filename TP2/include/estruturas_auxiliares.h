#ifndef ESTRUTURAS_AUXILIARES_H
#define ESTRUTURAS_AUXILIARES_H

#include <string>
#include <stdexcept>
#include <iostream> 

template<typename T>
class Vetor {
    public:
        Vetor() : dados(nullptr), capacidade(0), tamanho_(0) {}

        ~Vetor() {
            delete[] dados;
        }

        void adicionar(const T& valor) {
            if (tamanho_ == capacidade)
                redimensionar(capacidade == 0 ? 1 : capacidade * 2);
            dados[tamanho_++] = valor;
        }

        T& operator[](size_t i) {
            if (i >= tamanho_) throw std::out_of_range("Índice inválido");
            return dados[i];
        }

        const T& operator[](size_t i) const {
            if (i >= tamanho_) throw std::out_of_range("Índice inválido"); 
            return dados[i]; 
        }

        size_t tamanho() const {
            return tamanho_;
        }

    private:
        T* dados;
        size_t capacidade;
        size_t tamanho_;

        void redimensionar(size_t nova_capacidade) {
            T* novo_dados = new T[nova_capacidade];
            for (size_t i = 0; i < tamanho_; ++i)
                novo_dados[i] = dados[i];
            delete[] dados;
            dados = novo_dados;
            capacidade = nova_capacidade;
        }
};


template <typename T>
struct DefaultLess {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

template <typename T, typename Comparador = DefaultLess<T>>
class minHeap {
    public:
        minHeap() : dados(nullptr), capacidade(0), tamanho_(0) {}

        ~minHeap() {
            delete[] dados;
        }

        void inserir(const T& e) {
            if (tamanho_ == capacidade)
                redimensionar(capacidade == 0 ? 4 : capacidade * 2);
            dados[tamanho_] = e;
            subir(tamanho_);
            ++tamanho_;
        }

        T removerMinimo() {
            if (vazia()) throw std::runtime_error("Heap vazia");
            T minimo = dados[0];
            dados[0] = dados[--tamanho_];
            descer(0);
            return minimo;
        }

        bool vazia() const {
            return tamanho_ == 0;
        }

    private:
        T* dados;
        size_t capacidade;
        size_t tamanho_;
        Comparador comp; 

        void trocar(size_t i, size_t j) {
            T temp = dados[i];
            dados[i] = dados[j];
            dados[j] = temp;
        }

        void subir(size_t i) {
            while (i > 0) {
                size_t pai = (i - 1) / 2;
                if (comp(dados[i], dados[pai])) { 
                    trocar(i, pai);
                    i = pai;
                } else break;
            }
        }

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

        void redimensionar(size_t nova_capacidade) {
            T* novo_dados = new T[nova_capacidade];
            for (size_t i = 0; i < tamanho_; ++i)
                novo_dados[i] = dados[i];
            delete[] dados;
            dados = novo_dados;
            capacidade = nova_capacidade;
        }
};

template <typename T>
class ListaEncadeada {
    public:
        ListaEncadeada() : inicio(nullptr), fim(nullptr), tamanho(0) {}

        ~ListaEncadeada() {
            while (inicio != nullptr) {
                No* temp = inicio;
                inicio = inicio->proximo;
                delete temp;
            }
        }

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

        T getPrimeiro() const {
            if (!inicio) throw std::runtime_error("Lista vazia");
            return inicio->dado;
        }

        T removerPrimeiro() {
            if (!inicio) throw std::runtime_error("Lista vazia");
            No* temp = inicio;
            T valor = temp->dado;
            inicio = inicio->proximo;
            if (!inicio) fim = nullptr; // Se a lista ficou vazia
            delete temp;
            --tamanho;
            return valor;
        }

        bool vazia() const {
            return inicio == nullptr;
        }

        No* getInicio() const { return inicio; }
        int getTamanho() const { return tamanho; }

    private:
        struct No {
            T dado;
            No* proximo;
            No(const T& d) : dado(d), proximo(nullptr) {}
        };
        No* inicio;
        No* fim;
        int tamanho;
};

template <typename T_Peso>
struct Aresta {
    int idx_destino;    
    T_Peso peso;

    Aresta(int d, T_Peso p) : destino(d), peso(p) {}
};

template <typename T_NomeVertice, typename T_Peso>
struct Vertice {
    T_NomeVertice nome;
    Vetor<Aresta<T_Peso>> adjacentes; 
};

template <typename T_NomeVertice, typename T_Peso>
class Grafo {
    public:
        Grafo() {}

        int adicionarVertice(const T_NomeVertice& nome) {
            Vertice<T_NomeVertice, T_Peso> v;
            v.nome = nome;
            vertices.adicionar(v);
            return vertices.tamanho() - 1;
        }

        void adicionarAresta(int u, int v, T_Peso peso) {
            if (u < 0 || u >= vertices.tamanho() || v < 0 || v >= vertices.tamanho()) {
                std::cerr << "Erro: Índices de vértice inválidos para adicionarAresta." << std::endl;
                return;
            }

            vertices[u].adjacentes.adicionar(Aresta<T_Peso>(v, peso));
            vertices[v].adjacentes.adicionar(Aresta<T_Peso>(u, peso));
        }

        const Vetor<Aresta<T_Peso>>& vizinhos(int v) const {
            if (v < 0 || v >= vertices.tamanho()) {
                static const Vetor<Aresta<T_Peso>> empty_adjacentes; 
                std::cerr << "Erro: Índice de vértice inválido para vizinhos." << std::endl;
                return empty_adjacentes;
            }
            return vertices[v].adjacentes;
        }

        T_NomeVertice getNome(int v) const {
            if (v < 0 || v >= vertices.tamanho()) {
                return T_NomeVertice();
            }
            return vertices[v].nome;
        }

        int quantidadeVertices() const {
            return vertices.tamanho();
        }

        int getIndice(const T_NomeVertice& nome) const {
            for (int i = 0; i < vertices.tamanho(); ++i) {
                if (vertices[i].nome == nome) {
                    return i;
                }
            }
            return -1;
        }

        T_Peso getPesoAresta(int u, int v) const {
            if (u < 0 || u >= vertices.tamanho() || v < 0 || v >= vertices.tamanho()) {
                return T_Peso();
            }
            for (const auto& aresta : vertices[u].adjacentes) {
                if (aresta.destino == v) {
                    return aresta.peso;
                }
            }
            return T_Peso();
        }


    private:
        Vetor<Vertice<T_NomeVertice, T_Peso>> vertices;
};

template<typename T>
class Pilha {
    public:
        Pilha() : topo_(nullptr) {}

        ~Pilha() {
            while (!vazia()) desempilhar();
        }

        void empilhar(const T& item) {
            topo_ = new No(item, topo_);
        }

        T desempilhar() {
            if (vazia()) throw std::runtime_error("Pilha vazia");
            No* velho_topo = topo_;
            T valor = velho_topo->dado;
            topo_ = velho_topo->proximo;
            delete velho_topo;
            return valor;
        }

        bool vazia() const {
            return topo_ == nullptr;
        }
    
    private:
        struct No {
            T dado;
            No* proximo;
            No(const T& d, No* p = nullptr) : dado(d), proximo(p) {}
        };

        No* topo_;
};


#endif