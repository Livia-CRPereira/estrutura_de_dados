#include "transporte.h"


Transporte::Transporte()
    : capacidade_max_pacote(0), tempo_do_transporte() {
}

Transporte::~Transporte() {
}

Vetor<Pacote> Transporte::getPacotes() {
    return pacotes_sendo_transportados;
}

Time Transporte::getTempo() {
    return tempo_do_transporte;
}

int Transporte::getCapacidadeMaxPacotes() const {
    return capacidade_max_pacote;
}

void Transporte::setPacote(Pacote pacote) {
    pacotes_sendo_transportados.adicionar(pacote);
}

void Transporte::setTempo(Time tempo) {
    tempo_do_transporte = tempo;
}

void Transporte::setCapacidadeMax(int cap) {
    capacidade_max_pacote = cap;
}