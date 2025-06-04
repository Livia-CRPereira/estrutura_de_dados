#ifndef ARMAZEM_H
#define ARMAZEM_H

#include "estruturas_auxiliares.h"
#include "pacote.h"

class Armazem{
    public:
        Armazem();
        Armazem(int armazem_id, std::string armazem_nome, int quant_pacotes);
        ~Armazem();

        int getId();
        std::string getNome();

        void armazenaPacote(Pacote pacote);
        Pacote recuperaPacote();

        bool isSecaoVazia(int proximo_armazem_destino_id) const;
        bool todasSecoesVazias() const;
        int getQuantidadePacotesNaSecao(int proximo_armazem_destino_id) const;
        
    private:
        int id;
        std::string nome;
        Pilha<Pacote> pilha_de_pacotes;
};

#endif