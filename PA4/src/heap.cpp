#include "unionFind.hpp"
#include "heap.hpp"

Heap::Heap(int maxsize) {
    tamanho = 0;
    data = new Aresta[maxsize];
}
Heap::~Heap() {
    delete[] data;
}

void Heap::Inserir(Aresta x) {
    data[tamanho] = x;
    int pos = tamanho;
    int pai = GetAncestral(pos);

    while (pos > 0 && data[pos].custo < data[pai].custo) {
        std::swap(data[pos], data[pai]);
        pos = pai;
        pai = GetAncestral(pos);
    }

    tamanho++;
}

Aresta Heap::Remover() {
    Aresta min = data[0];
    data[0] = data[tamanho - 1];
    tamanho--;

    int pos = 0;

    while (true) {
        int esq = GetSucessorEsq(pos);
        int dir = GetSucessorDir(pos);
        int menor = pos;

        if (esq < tamanho && data[esq].custo < data[menor].custo)
            menor = esq;
        if (dir < tamanho && data[dir].custo < data[menor].custo)
            menor = dir;

        if (menor != pos) {
            std::swap(data[pos], data[menor]);
            pos = menor;
        } else {
            break;
        }
    }

    return min;
}

bool Heap::Vazio() {
    return tamanho == 0;
}

int Heap::GetAncestral(int pos) { return (pos - 1) / 2; }
int Heap::GetSucessorEsq(int pos) { return 2 * pos + 1; }
int Heap::GetSucessorDir(int pos) { return 2 * pos + 2; }