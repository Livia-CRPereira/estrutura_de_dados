#ifndef LISTAADJ_HPP
#define LISTAADJ_HPP

#include <iostream>
#include <string>

class TipoItem {
    public:
        TipoItem();
        void SetChave(int c);
        int GetChave() const;

    private:
        int chave;
};

class TipoCelula {
    public:
        TipoCelula();

    private:
        TipoItem item;
        TipoCelula* prox;

        friend class ListaAdjacencia;
};

class ListaAdjacencia {
    public:
        ListaAdjacencia();
        ~ListaAdjacencia();

        void AdicionaVertice();
        void SetVizinho(int root, int vizinho);
        void GetVizinhos(int root);
        int GetQuantidadeVertices();
        int GetQuantidadeArestas();
        int GetTamanho(int v);

    private:
        struct ListaDeVizinhos {
            TipoCelula* primeiroVizinho;
            ListaDeVizinhos* proxVertice;
        };

        ListaDeVizinhos* primeiroVertice;      
        int numVertices;       
        int numArestas;      
            
};

#endif


