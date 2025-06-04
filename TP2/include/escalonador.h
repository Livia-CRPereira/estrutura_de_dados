#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "estruturas_auxiliares.h" // Inclui Vetor, DefaultLess, minHeap, ListaEncadeada, Grafo, Pilha
#include "armazem.h"                // Inclui Armazem
#include "pacote.h"                 // Inclui Pacote
#include "transporte.h"             // Inclui Transporte
#include "time.h"                   // Inclui Time

// Enumeração para os tipos de eventos
enum TipoEvento {
    EVENTO_INVALIDO = 0,
    EVENTO_CHEGADA_PACOTE,    // Pacote chega a um armazém (postagem ou após transporte)
    EVENTO_INICIO_TRANSPORTE, // Pacotes são alocados e iniciam o transporte entre armazéns
    EVENTO_FIM_SIMULACAO      // Um evento para sinalizar o fim da simulação (opcional, pode ser implícito)
    // Adicione outros tipos de eventos conforme a complexidade do sistema (ex: manipulação interna)
};

// Estrutura para representar um evento no escalonador
struct EventoSimulacao {
    Time tempo;          // Instante de ocorrência do evento [cite: 40, 41]
    TipoEvento tipo;     // Tipo do evento [cite: 41]
    int id_pacote;       // ID do pacote envolvido (se aplicável, -1 se não)
    int id_armazem_origem; // ID do armazém de origem do evento (se aplicável, ex: para transporte)
    int id_armazem_destino; // ID do armazém de destino do evento (se aplicável, ex: para chegada, ou destino de transporte)

    // Construtor para facilitar a criação de eventos
    EventoSimulacao(Time t = Time(), TipoEvento te = EVENTO_INVALIDO, int ip = -1, int iao = -1, int iad = -1)
        : tempo(t), tipo(te), id_pacote(ip), id_armazem_origem(iao), id_armazem_destino(iad) {}

    // Sobrecarga do operador < para que DefaultLess<EventoSimulacao> funcione com a minHeap
    // Compara eventos pelo tempo de ocorrência
    bool operator<(const EventoSimulacao& other) const {
        return tempo < other.tempo;
    }
};

class Escalonador {
    public:
        Escalonador();
        ~Escalonador();

        // Inicia e executa a simulação principal
        void simuladorArmazens(); // Renomeado de simuladorArmazens para refletir a função principal

        // 1. Inicializa o estado do sistema e agenda eventos iniciais
        void inicializa();

        // 2. Insere um evento na fila de prioridade
        void insereEvento(const EventoSimulacao& evento);

        // 3. Retira o próximo evento da fila de prioridade
        EventoSimulacao retiraProximoEvento(); // Retorna o evento, não apenas o remove

        // 4. Finaliza a simulação e gera relatórios
        void finaliza();

    private:
        // Fila de prioridade de eventos, usando EventoSimulacao e o comparador padrão (DefaultLess)
        minHeap<EventoSimulacao> fila_de_prioridade; // [cite: 42]
        Time relogio_simulacao; // Relógio da simulação [cite: 39]
        bool condicao_termino; // Flag para controlar o loop da simulação [cite: 51]

        // Componentes do sistema simulado que o escalonador gerencia
        Grafo<std::string, double> topologia_armazens; // Grafo de conexão entre armazéns [cite: 6, 16]
        Vetor<Armazem> armazens; // Coleção de todos os objetos Armazem
        Vetor<Pacote> pacotes_no_sistema; // Coleção de todos os pacotes no sistema (para acesso por ID)

        // transporte -> carregar

        // ID para o próximo pacote que entrar no sistema (para garantir unicidade)
        int proximo_id_pacote;

        // Métodos internos para processar diferentes tipos de eventos
        void processaChegadaPacote(const EventoSimulacao& evento);
        void processaInicioTransporte(const EventoSimulacao& evento);

        // Métodos auxiliares de gerenciamento e estatísticas
        // Para simplificar a inicialização, alguns métodos serão chamados aqui.
        void carregaTopologiaArmazens();
        void carregaPacotesIniciais(); // Lê o arquivo de entrada de pacotes [cite: 18]
        void agendarTransportesPeriodicos();

        bool todasSecoesArmazensVazias() const;
        void atualizarEstatisticas(); // [cite: 51]
        void gerarRelatorios(); // [cite: 51]
};

#endif // ESCALONADOR_H