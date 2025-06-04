#include "graph.hpp"

Grafo::Grafo() {}

Grafo::~Grafo() {}

void Grafo::InsereVertice() {
    vertices.AdicionaVertice();
}

void Grafo::InsereAresta(int v, int w) {
    vertices.SetVizinho(v, w);
}

int Grafo::QuantidadeVertices() {
    return vertices.GetQuantidadeVertices();
}

int Grafo::QuantidadeArestas() {
    return vertices.GetQuantidadeArestas();
}

int Grafo::GrauMinimo() {
    int n = vertices.GetQuantidadeVertices();
    if (n == 0) return 0;

    int minGrau = vertices.GetTamanho(0);
    for (int i = 1; i < n; i++) {
        int grau = vertices.GetTamanho(i);
        if (grau < minGrau) {
            minGrau = grau;
        }
    }
    return minGrau;
}

int Grafo::GrauMaximo() {
    int n = vertices.GetQuantidadeVertices();
    if (n == 0) return 0;

    int maxGrau = vertices.GetTamanho(0);
    for (int i = 1; i < n; i++) {
        int grau = vertices.GetTamanho(i);
        if (grau > maxGrau) {
            maxGrau = grau;
        }
    }
    return maxGrau;
}


void Grafo::ImprimeVizinhos(int v) {
    vertices.GetVizinhos(v);
}
