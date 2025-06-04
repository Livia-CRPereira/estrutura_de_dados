#include "Set.hpp"

StringSet::StringSet(int tamanho){

    tamanhoOriginal = tamanho;
    tamanhoTabela = tamanho;
    tamanhoConjunto = 0;
    tabela = new ElementoTabela[tamanhoTabela];
    
    for (size_t i = 0; i < tamanhoTabela; i++){
        tabela[i].vazio = true;
        tabela[i].retirada = false;
    }

}

StringSet::~StringSet(){

    delete[] tabela;
}

void StringSet::Inserir(string s){
    if (Pertence(s)) return;

    double fatorCarga = 0.7;

    if ((double)tamanhoConjunto / tamanhoTabela >= fatorCarga){
        Resize(tamanhoTabela * 2);
    }

    int h = Hash(s);
    while (!tabela[h].vazio && !tabela[h].retirada){
        h = Rehash(h);
    }

    tabela[h].dado = s;
    tabela[h].vazio = false;
    tabela[h].retirada = false;
    tamanhoConjunto++;
}


void StringSet::Remover(string s){

    int h = Hash(s);

    for (size_t i = 0; i < tamanhoTabela; ++i){

        int pos = (h + i) % tamanhoTabela;

        if (tabela[pos].vazio) return;

        if (!tabela[pos].retirada && tabela[pos].dado == s){

            tabela[pos].retirada = true;
            tamanhoConjunto--;
            return;
        }
    }
}

bool StringSet::Pertence(string s){
    int h = Hash(s);

    for (size_t i = 0; i < tamanhoTabela; ++i){
        int pos = (h + i) % tamanhoTabela;

        if (tabela[pos].vazio) return false;

        if (!tabela[pos].retirada && tabela[pos].dado == s){
            return true;
        }
    }
    return false;
}


StringSet* StringSet::Intersecao(StringSet* S){

    StringSet* result = new StringSet(min(tamanhoTabela, S->tamanhoTabela));

    for (size_t i = 0; i < tamanhoTabela; ++i){
        if (!tabela[i].vazio && 
            !tabela[i].retirada && 
            S->Pertence(tabela[i].dado)) result->Inserir(tabela[i].dado);
    }
    return result;
}

StringSet* StringSet::Uniao(StringSet* S){

    StringSet* result = new StringSet(tamanhoConjunto + S->tamanhoConjunto);

    for (size_t i = 0; i < tamanhoTabela; ++i){
        if (!tabela[i].vazio && 
            !tabela[i].retirada) result->Inserir(tabela[i].dado);
    }

    for (size_t i = 0; i < S->tamanhoTabela; ++i){
        if (!S->tabela[i].vazio 
            && !S->tabela[i].retirada) result->Inserir(S->tabela[i].dado);
    }

    return result;
}

StringSet* StringSet::DiferencaSimetrica(StringSet* S){
    StringSet* result = new StringSet(tamanhoTabela + S->tamanhoTabela);

    for (size_t i = 0; i < tamanhoTabela; ++i){
        if (!tabela[i].vazio && 
            !tabela[i].retirada && 
            !S->Pertence(tabela[i].dado)) result->Inserir(tabela[i].dado);
    }

    for (size_t i = 0; i < S->tamanhoTabela; ++i){
        if (!S->tabela[i].vazio && 
            !S->tabela[i].retirada && 
            !Pertence(S->tabela[i].dado)) result->Inserir(S->tabela[i].dado);
    }

    return result;
}

void StringSet::Imprimir(){
    
    cout << "{ ";
    bool primeiro = true;
    for (size_t i = 0; i < tamanhoTabela; ++i){
        if (!tabela[i].vazio && !tabela[i].retirada){
            if (!primeiro) cout << ", ";
            cout << tabela[i].dado;
            primeiro = false;
        }
    }
    cout << " }" << endl;
    
}