#include <iostream>
#include "unionFind.hpp"
#include "heap.hpp"

const int MAX_ARESTAS = 10000;

int main() {
    int n, m;
    std::cin >> n >> m;

    Aresta arestas[MAX_ARESTAS];
    for (int i = 0; i < m; ++i) {
        std::cin >> arestas[i].u >> arestas[i].v >> arestas[i].custo;
    }

    // ÁRVORE GERADORA MÍNIMA
    // Inicialização
    int custo = 0;
    int quant_arestas = 0;

    // Union-Find
    UnionFind dsu(n);
    for (int i = 0; i < n; ++i) {
        dsu.Make(i);
    }

    // Heap das arestas
    Heap heap(m);
    for (int i = 0; i < m; ++i) {
        heap.Inserir(arestas[i]);
    }

    // Árvore
    while (!heap.Vazio()) {
        Aresta e = heap.Remover();  // e(u,v,c)
        int u = e.u;
        int v = e.v;

        if (dsu.Find(u) != dsu.Find(v)) {
            dsu.Union(u, v);
            custo += e.custo;
            quant_arestas++;
        }
    }

    std::cout << custo << std::endl;

    return 0;
}
