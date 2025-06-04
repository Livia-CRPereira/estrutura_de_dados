#ifndef SET_HPP
#define SET_HPP

#include <bits/stdc++.h>
using namespace std;

typedef struct s_table_elem{
    string dado;
    bool vazio;
    bool retirada;

} ElementoTabela;

class StringSet{

    public:
        StringSet(int tamanho);
        ~StringSet();

        void Inserir(string s);
        void Remover(string s);
        bool Pertence(string s);
        
        StringSet* Intersecao(StringSet* S);
        StringSet* Uniao(StringSet* S);
        StringSet* DiferencaSimetrica(StringSet* S);

        void Imprimir();

    private:
        size_t tamanhoOriginal;
        size_t tamanhoTabela;
        size_t tamanhoConjunto;
        ElementoTabela* tabela;

        int Hash(string s) {
            int hash = 0;
            for (char letra : s)
                hash = ((hash << 5) + hash) + letra; // hash * 33 + c
            return hash % tamanhoTabela;
        }


        int Rehash(int pos) {
            return (pos + 1) % tamanhoTabela;
        };


        void Resize(size_t novoTamanho) {
            ElementoTabela* tabelaAntiga = tabela;
            size_t tamanhoAntigo = tamanhoTabela;

            tabela = new ElementoTabela[novoTamanho];
            tamanhoTabela = novoTamanho;
            tamanhoConjunto = 0;

            for (size_t i = 0; i < novoTamanho; i++){
                tabela[i].vazio = true;
                tabela[i].retirada = false;
            }

            for (size_t i = 0; i < tamanhoAntigo; ++i){
                if (!tabelaAntiga[i].vazio && !tabelaAntiga[i].retirada){
                    Inserir(tabelaAntiga[i].dado);
                }
            }

            delete[] tabelaAntiga;
        };

};


#endif