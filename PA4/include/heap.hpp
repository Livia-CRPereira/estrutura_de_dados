#include <iostream>

// Heap de arestas ordenada pelo custo
class Heap {
    public:
        Heap(int maxsize);
        ~Heap();
    
        void Inserir(Aresta a);
        Aresta Remover(); // Remove e retorna a menor aresta
        bool Vazio();
    
    private:
        Aresta* data;
        int tamanho;
    
        int GetAncestral(int posicao);
        int GetSucessorEsq(int posicao);
        int GetSucessorDir(int posicao);
};
