#include "heap.hpp"

Heap::Heap(int maxsize){
    tamanho = 0;
    data = new int[maxsize];
}
Heap::~Heap(){
    delete[] data;
}

void Heap::Inserir(int x){
    data[tamanho] = x;
    int currentPosition = tamanho;
    int parentPosition = GetAncestral(currentPosition);

    
    while ((currentPosition > 0) && (data[currentPosition] < data[parentPosition])){
        int aux = data[currentPosition];
        data[currentPosition] =  data[parentPosition];
        data[parentPosition] = aux;

        currentPosition = parentPosition;
        parentPosition = (currentPosition - 1)/2;

    }
    tamanho++;
}
int Heap::Remover() {

    int aux = data[0];

    data[0] = data[tamanho - 1];
    tamanho = tamanho - 1;

    int posicao = 0;
    int sucEsq = GetSucessorEsq(posicao);
    int sucDir = GetSucessorDir(posicao);

    int menorSucessor;
    if (sucEsq < tamanho && sucDir < tamanho){
        menorSucessor = (data[sucEsq] < data[sucDir]) ? sucEsq : sucDir;
    }else if (sucEsq < tamanho){
        menorSucessor = sucEsq;
    }else if (sucDir < tamanho){
        menorSucessor = sucDir;
    }else{
        menorSucessor = -1; // sem filhos
    }

    while (menorSucessor != -1 && data[posicao] > data[menorSucessor]){
        int auxT = data[posicao];

        data[posicao] = data[menorSucessor];
        data[menorSucessor] = auxT;

        posicao = menorSucessor;

        sucEsq = GetSucessorEsq(posicao);
        sucDir = GetSucessorDir(posicao);

        if (sucEsq < tamanho && sucDir < tamanho){
            menorSucessor = (data[sucEsq] < data[sucDir]) ? sucEsq : sucDir;
        }else if (sucEsq < tamanho){
            menorSucessor = sucEsq;
        }else if (sucDir < tamanho){
            menorSucessor = sucDir;
        }else{
            menorSucessor = -1;
        }
    }
    std::cout << aux << " ";

    return aux;
}


bool Heap::Vazio(){
    if (tamanho == 0) return 1;
    else return 0;

}

int Heap::GetAncestral(int posicao){return (posicao - 1)/2;}
int Heap::GetSucessorEsq(int posicao){return 2 * posicao + 1;}
int Heap::GetSucessorDir(int posicao){return 2 * posicao + 2;}