#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

// Incluindo seus TADs e estruturas
#include "estruturas_auxiliares.h"
#include "pacote.h"
#include "armazem.h"
#include "transporte.h"
#include "ordenacao.h" 

// --- Definição dos Tipos de Evento ---

enum TipoEvento {
    EVENTO_CHEGADA_PACOTE,
    EVENTO_TRANSPORTE
};

struct Evento {
    long long chave;
    int tempo;
    TipoEvento tipo;
    virtual ~Evento() {}
};

struct EventoChegadaPacote : public Evento {
    int id_pacote;

    EventoChegadaPacote(int t, int id) : id_pacote(id) {
        this->tempo = t;
        this->tipo = EVENTO_CHEGADA_PACOTE;
        this->chave = (long long)this->tempo * 10000000LL + 
                      (long long)this->id_pacote * 10LL + 1;
    }
};

struct EventoTransporte : public Evento {
    int id_origem;
    int id_destino;

    EventoTransporte(int t, int origem, int destino) : id_origem(origem), id_destino(destino) {
        this->tempo = t;
        this->tipo = EVENTO_TRANSPORTE;
        this->chave = (long long)this->tempo * 10000000LL + 
                      (long long)this->id_origem * 10000LL + 
                      (long long)this->id_destino * 10LL + 2;
    }
};

// --- Comparador para o MinHeap ---
// O minHeap precisa saber como comparar ponteiros para Evento,
// então criamos um functor que compara as chaves dentro dos objetos.
struct ComparadorEventos {
    bool operator()(const Evento* a, const Evento* b) const {
        return a->chave < b->chave;
    }
};


// --- Classe Principal do Simulador ---

class Escalonador {
public:
    Escalonador();
    ~Escalonador();

    void carregarConfiguracao(int interv_transp, int custo_rem, int num_armazens);
    void adicionarConexao(int u, int v);
    void adicionarPacote(const Pacote& p);
    void adicionarTransporte(const Transporte& t); // Reintroduzido

    void prepararSimulacao();
    void executar();

private:
    // Variáveis de Estado
    int tempo_sistema;
    int pacotes_entregues;
    int total_pacotes_na_simulacao;

    // Parâmetros Globais
    int intervalo_transportes;
    int custo_remocao;

    // Estruturas de Dados Principais
    Grafo<int, int> rede_armazens;
    Vetor<Armazem> armazens;
    Vetor<Pacote> pacotes;
    Vetor<Transporte> transportes; // Reintroduzido
    minHeap<Evento*, ComparadorEventos> fila_eventos;

    // Métodos de Processamento
    void processarEventoChegada(EventoChegadaPacote* evento);
    void processarEventoTransporte(EventoTransporte* evento);

    // Funções Auxiliares
    void calcularRotaParaPacote(Pacote& pacote);
    void imprimirEvento(int tempo, int id_pacote, const std::string& msg);
    std::string formatarNumero(int numero, int largura);
    Pacote* getPacotePorId(int id);
    Armazem* getArmazemPorId(int id);
    Transporte* getTransportePorRota(int id_origem, int id_destino); // Novo helper
};

#endif // ESCALONADOR_H