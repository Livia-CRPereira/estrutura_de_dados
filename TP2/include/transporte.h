#ifndef TRANSPORTE_H
#define TRANSPORTE_H

#include "estruturas_auxiliares.h"
#include "pacote.h"
#include "time.h"

class Transporte{
    public:
        Transporte();
        ~Transporte();
        
        Vetor<Pacote> getPacotes();
        Time getTempo();
        int getCapacidadeMaxPacotes() const;

        void setPacote(Pacote pacote);
        void setTempo(Time tempo);
        void setCapacidadeMax(int cap);

    private:
        Vetor<Pacote> pacotes_sendo_transportados;
        int capacidade_max_pacote;
        Time tempo_do_transporte;
};

#endif