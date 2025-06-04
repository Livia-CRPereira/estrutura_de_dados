#include "listaAdj.hpp"

// TipoItem

// Construtor
TipoItem::TipoItem() {
    chave = -1;  // Inicializa a chave como -1 (valor indicativo de um item não inicializado)
}

// Setter da chave
void TipoItem::SetChave(int c) {
    chave = c;
}

// Getter da chave
int TipoItem::GetChave() const {
    return chave;
}

// Tipo Celula
TipoCelula::TipoCelula() {
    item.SetChave(-1);
    prox = nullptr;  // Inicializa o ponteiro prox como nullptr
}

// Lista de Adjacencia
ListaAdjacencia::ListaAdjacencia() {
    primeiroVertice = nullptr;
    numVertices = 0;
    numArestas = 0;
}

ListaAdjacencia::~ListaAdjacencia() {
    ListaDeVizinhos* vAtual = primeiroVertice;
    while (vAtual != nullptr) {
        ListaDeVizinhos* vProx = vAtual->proxVertice;

        TipoCelula* cAtual = vAtual->primeiroVizinho;
        while (cAtual != nullptr) {
            TipoCelula* cProx = cAtual->prox;
            delete cAtual;
            cAtual = cProx;
        }

        delete vAtual;
        vAtual = vProx;
    }
}


void ListaAdjacencia::AdicionaVertice() {
    ListaDeVizinhos* novoVertice = new ListaDeVizinhos;
    novoVertice->primeiroVizinho = nullptr;
    novoVertice->proxVertice = nullptr;

    if (primeiroVertice == nullptr) {
        primeiroVertice = novoVertice;
    } else {
        ListaDeVizinhos* atual = primeiroVertice;
        while (atual->proxVertice != nullptr) {
            atual = atual->proxVertice;
        }
        atual->proxVertice = novoVertice;
    }

    numVertices++;
}

void ListaAdjacencia::SetVizinho(int root, int vizinho) {
    // 1. Navega até o vértice de índice "root"
    ListaDeVizinhos* atual = primeiroVertice;
    for (int i = 0; i < root; i++) {
        if (atual == nullptr) {
            throw "Erro: índice de vértice inválido.";
        }
        atual = atual->proxVertice;
    }

    // 2. Cria nova célula com o vizinho
    TipoCelula* nova = new TipoCelula;
    nova->item.SetChave(vizinho);
    nova->prox = nullptr;

    // 3. Insere no final da lista de vizinhos
    if (atual->primeiroVizinho == nullptr) {
        atual->primeiroVizinho = nova;
    } else {
        TipoCelula* aux = atual->primeiroVizinho;
        while (aux->prox != nullptr) {
            aux = aux->prox;
        }
        aux->prox = nova;
    }

    numArestas++;
}

void ListaAdjacencia::GetVizinhos(int root){
    // 1. Navega até o vértice de índice "root"
    ListaDeVizinhos* atual = primeiroVertice;
    for (int i = 0; i < root; i++) {
        if (atual == nullptr) {
            throw "Erro: índice de vértice inválido.";
        }
        atual = atual->proxVertice;
    }
    if (atual->primeiroVizinho == nullptr) std::cout << "Sem vizinhos";
    else{
        TipoCelula* aux = atual->primeiroVizinho;
        while (aux != nullptr) {
            std::cout << aux->item.GetChave() << " ";
            aux = aux->prox;
        }
    }
    
}
int ListaAdjacencia::GetQuantidadeVertices(){
    return numVertices;
}
int ListaAdjacencia::GetQuantidadeArestas(){
    return numArestas/2;
}
int ListaAdjacencia::GetTamanho(int v){
    ListaDeVizinhos* atual = primeiroVertice;
    for (int i = 0; i < v; i++) {
        if (!atual) {
            throw "Erro: índice de vértice inválido.";
        }
        atual = atual->proxVertice;
    }

    TipoCelula* aux = atual->primeiroVizinho;
    int tamanho = 0;
    while (aux != nullptr) {
        tamanho++;
        aux = aux->prox;
    }
    return tamanho;

}





