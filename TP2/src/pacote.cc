#include "pacote.h"

// Construtor padrão (pode ser usado para pacotes sem dados iniciais, mas o outro construtor é mais comum)
Pacote::Pacote()
    : id(-1), tempo_postagem(), tempo_armazenado(), tempo_transportado(), tempo_espera(),
      remetente(""), destinatario(""), tipo(""),
      armazemOrigem_id(-1), armazemDestino_id(-1),
      estado_atual(nao_postado), ultimo_tempo_entrada(), tempo_esperado_estadia() {
}

Pacote(int id_pacote, Time tempo_postagem_pacote, const std::string& remetente_str,
        const std::string& destinatario_str, const std::string& tipo_str,
        int armazem_origem_id_int, int armazem_destino_id_int){
    id = id_pacote;
    tempo_postagem = tempo_postagem_pacote;
    remetente = remetente_str;
    destinatario = destinatario_str;
    tipo = tipo_str;
    armazemOrigem_id = armazem_origem_id_int;
    armazemDestino_id = armazem_destino_id_int;
}

// Destrutor
Pacote::~Pacote() {
    // Nada especial para desalocar aqui, pois Vetor e ListaEncadeada já cuidam da memória.
}

// Métodos getTempors
int Pacote::getId() {
    return id;
}

Time Pacote::getTempoPostagem() {
    return tempo_postagem;
}

Time Pacote::getTempoArmazenado() {
    return tempo_armazenado;
}

Time Pacote::getTempoTransportado() {
    return tempo_transportado;
}

Time Pacote::getTempoEspera() {
    return tempo_espera;
}

std::string Pacote::getRemetente() {
    return remetente;
}

std::string Pacote::getDestinatario() {
    return destinatario;
}

std::string Pacote::getTipo() {
    return tipo;
}

int Pacote::getArmazemOrigem() {
    return armazemOrigem_id; 
}

int Pacote::getArmazemDestino() {
    return armazemDestino_id;
}

Estado_do_pacote Pacote::getEstado() {
    return estado_atual;
}

Vetor<Time> Pacote::getMomentos() {
    return momentos_pacote_alterado;
}

ListaEncadeada<int> Pacote::getRota() {
    return rota;
}

void Pacote::setEstado(Estado_do_pacote novoEstado) {
    estado_atual = novoEstado;
}

void Pacote::adicionaMomento(Time tempo_evento) {
    momentos_pacote_alterado.adicionar(tempo_evento);
    ultimo_tempo_entrada = tempo_evento; 
}

void Pacote::acumularTempoArmazenado(Time delta_tempo) {
    tempo_armazenado = tempo_armazenado + delta_tempo;
}

void Pacote::acumularTempoEmTransito(Time delta_tempo) {
    tempo_transportado = tempo_transportado + delta_tempo;
}

void Pacote::acumularTempoEsperaManipulacao(Time delta_tempo) {
    tempo_espera = tempo_espera + delta_tempo;
}

void Pacote::setRota(const ListaEncadeada<int>& nova_rota) {
    rota = nova_rota;

}

void Pacote::avancaNaRota() {
    if (!rota.vazia()) {
        rota.removerPrimeiro(); // Remove o armazém atual/já visitado da rota
    }
}

int Pacote::getProximoArmazemRota() const {
    if (!rota.vazia()) {
        return rota.getPrimeiro();
    }
    return -1;
}


bool Pacote::chegouAoDestinoFinal() const {
    return rota.vazia();
}

Time Pacote::getTempoUltimoEntrada() {
    return ultimo_tempo_entrada;
}

Time Pacote::getTempoEsperadoEstadia() {
    return tempo_esperado_estadia;
}

