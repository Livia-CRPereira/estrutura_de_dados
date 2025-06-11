#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "estruturas_auxiliares.h"
#include "armazem.h"
#include "pacote.h"
#include "transporte.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm> // Para std::sort
#include <sstream>   // Para o formatador

// --- Estruturas de Evento ---

/**
 * @brief Enumera os tipos de eventos que podem ocorrer na simulação.
 */
enum TipoEvento {
    EVENTO_PACOTE,      ///< Um evento relacionado à chegada de um pacote.
    EVENTO_TRANSPORTE   ///< Um evento de despacho de um veículo de transporte.
};

/**
 * @brief Estrutura base para todos os eventos na simulação.
 * @details Contém o tempo em que o evento ocorre e seu tipo.
 */
struct Evento {
    int tempo;
    TipoEvento tipo;
    Evento(int t, TipoEvento tp) : tempo(t), tipo(tp) {}
    virtual ~Evento() {}
};

/**
 * @brief Evento que representa a chegada de um pacote a um armazém.
 */
struct EventoEntregaPacote : public Evento {
    int id_pacote;
    EventoEntregaPacote(int t, int id_p) : Evento(t, EVENTO_PACOTE), id_pacote(id_p) {}
};

/**
 * @brief Evento que representa a partida de um transporte de uma origem para um destino.
 */
struct EventoTransporte : public Evento {
    int id_origem;
    int id_destino;
    EventoTransporte(int t, int id_o, int id_d) : Evento(t, EVENTO_TRANSPORTE), id_origem(id_o), id_destino(id_d) {}
};

/**
 * @brief Functor de comparação para a fila de prioridades de eventos (minHeap).
 * @details Ordena os eventos primariamente pelo tempo. Em caso de empate,
 * eventos de pacote têm prioridade sobre eventos de transporte.
 */
struct EventoPtrComparador {
    bool operator()(const Evento* a, const Evento* b) const {
        if (a->tempo != b->tempo) return a->tempo < b->tempo;
        // Critério de desempate: Pacotes antes de transportes
        if (a->tipo != b->tipo) return a->tipo < b->tipo;
        // Critérios de desempate secundários para consistência
        if (a->tipo == EVENTO_TRANSPORTE) {
            auto* ta = static_cast<const EventoTransporte*>(a);
            auto* tb = static_cast<const EventoTransporte*>(b);
            if (ta->id_origem != tb->id_origem) return ta->id_origem < tb->id_origem;
        }
        return false; // Mantém a ordem se tudo for igual
    }
};

/**
 * @brief Motor central da simulação de logística, responsável por gerenciar o tempo,
 * os eventos e o estado de todos os componentes do sistema.
 * @details Tipo Abstrato de Dados que apresenta uma série de funções que podem ser
 * separadas nas seguintes categorias para explicar o funcionamento da simulação:
 *
 * 1. Funções de Configuração e Preparação:
 * Carregam os parâmetros da simulação, constroem a rede de armazéns e
 * preparam o estado inicial para a execução.
 * - Escalonador(): Construtor que inicializa o estado do sistema.
 * - carregarConfiguracao(): Define os parâmetros globais, como capacidade e
 * latência dos transportes, e cria os objetos Armazem.
 * - adicionarConexao(): Constrói a topologia da rede, conectando os armazéns.
 * - adicionarPacote(): Adiciona os pacotes que serão o "trabalho" da simulação.
 * - prepararSimulacao(): Etapa crucial que ocorre antes da execução. Ela
 * calcula a rota para cada pacote e insere os primeiros eventos (chegada de
 * pacotes e os primeiros despachos de transporte) na fila de prioridades,
 * dando o pontapé inicial no sistema.
 *
 * 2. Funções de Utilitários e Consulta:
 * Funções de utilidade necessárias para a implementação, que fornecem acesso
 * a componentes do sistema ou realizam tarefas de apoio.
 * - getPacotePorId(), getArmazemPorId(): Métodos rápidos para encontrar um objeto
 * específico a partir de seu ID.
 * - imprimirEvento(): Formata e exibe no console cada passo importante da
 * simulação, criando um log detalhado do que aconteceu em cada instante de tempo.
 * - inicializarSecoesDosArmazens(): Garante que cada armazém tenha as "seções"
 * (pilhas de pacotes) corretas, uma para cada vizinho direto na rede.
 *
 * 3. Algoritmos Fundamentais:
 * O coração lógico da simulação, onde as decisões e cálculos importantes são feitos.
 * - calcularRotaParaPacote(): Para cada pacote, o sistema usa um algoritmo de
 * Busca em Largura (BFS) sobre o grafo da rede para encontrar o caminho mais
 * curto (em número de saltos) entre a origem e o destino. Essa rota é
 * armazenada no próprio pacote.
 * - Lógica de Transporte (em `processarOndaDeTransporte`): Este é o algoritmo
 * mais complexo. Ele simula a operação de um veículo em uma rota:
 * 1. Esvazia a seção inteira do armazém em uma estrutura temporária.
 * 2. Calcula os custos de tempo para remover cada pacote, penalizando os
 * que estavam mais no fundo da pilha.
 * 3. Aplica a regra de negócio: os pacotes mais antigos (menor tempo de
 * postagem) têm prioridade para embarcar.
 * 4. Embarca os pacotes prioritários até o limite da capacidade.
 * 5. Re-empilha os pacotes que ficaram para trás na seção de origem.
 * 6. Agenda a chegada dos pacotes transportados no próximo armazém.
 *
 * 4. Motor da Simulação (Loop de Eventos):
 * Gerencia o fluxo principal da simulação, processando eventos em ordem cronológica.
 * - executar(): É o loop principal. A simulação não avança o tempo de forma
 * linear (t++), mas sim "salta" no tempo para o próximo evento na fila de
 * prioridades (uma min-heap). Ele processa todos os eventos que ocorrem
 * no mesmo instante ("onda de eventos") antes de avançar para o próximo.
 * - processarEventoEntrega(): Trata da chegada de um pacote. Se for a postagem
 * inicial, o armazena na origem. Se for uma chegada intermediária, avança a
 * rota do pacote e o armazena no novo local. Se for o destino final,
 * finaliza o processo para aquele pacote.
 * - processarOndaDeTransporte(): Trata do despacho de veículos. Executa a
 * lógica de transporte (descrita no item 3) e, crucialmente, re-agenda
 * o próximo evento de transporte para aquela mesma rota no futuro, garantindo
 * a continuidade do fluxo logístico.
 *
 * 5. Geração de Resultados:
 * O objetivo final da simulação é entregar todos os pacotes e, no processo,
 * gerar dados. A principal forma de avaliação é o log de eventos.
 * - A função `imprimirEvento` é chamada em cada ação significativa (armazenado,
 * removido, em trânsito, entregue), criando um histórico completo.
 * - O critério de parada da simulação (`pacotes_entregues < pacotes.tamanho()`)
 * garante que o sistema rode até que o objetivo seja cumprido.
 * - Ao final, o log gerado pode ser usado para analisar a eficiência,
 * tempos de espera e gargalos do sistema configurado.
 *
 * 6. Lógica de Negócio e Regras Específicas:
 * Detalhes que definem o comportamento único desta simulação.
 * - Simulação de Eventos Discretos: O tempo avança por eventos, não de forma contínua.
 * - Processamento em "Ondas": Todos os eventos de um mesmo instante são
 * agrupados e processados, com prioridade para eventos de pacote.
 * - Armazenamento LIFO: Os armazéns funcionam como pilhas (Last-In, First-Out),
 * o que cria a necessidade de manobras e custos de remoção.
 * - Prioridade de Embarque: Pacotes mais antigos sempre têm prioridade, simulando
 * uma política de "First-Come, First-Served" em relação à postagem.
 * - Rearmazenamento Condicional: A ordem de re-empilhamento dos pacotes que não
 * viajaram é invertida para otimizar a próxima remoção (os mais antigos,
 * que provavelmente viajarão em breve, ficam mais perto do topo).
 */
class Escalonador {
    public:
        /**
        * @brief Construtor do Escalonador.
        */
        Escalonador();

        /**
        * @brief Destrutor do Escalonador.
        * @details Garante que todos os eventos alocados dinamicamente na fila sejam liberados.
        */
        ~Escalonador();

        /**
        * @brief Carrega os parâmetros de configuração da simulação.
        * @param interv_transportes Intervalo de tempo entre despachos de veículos na mesma rota.
        * @param cst_remocao Custo de tempo para remover um pacote de uma pilha.
        * @param num_armazens O número total de armazéns na simulação.
        */
        void carregarConfiguracao(int interv_transportes, int cst_remocao, int num_armazens);
        
        /**
        * @brief Adiciona uma conexão (aresta) não-direcionada entre dois armazéns.
        * @param u O ID do primeiro armazém.
        * @param v O ID do segundo armazém.
        */
        void adicionarConexao(int u, int v);
        
        /**
        * @brief Adiciona um pacote à lista de pacotes a serem processados na simulação.
        * @param p O objeto Pacote a ser adicionado.
        */
        void adicionarPacote(const Pacote& p);

        /**
        * @brief Adiciona um transporte à lista de transportes a serem processados na simulação.
        * @param t O objeto Transporte a ser adicionado.
        */
        void adicionarTransporte(const Transporte& t);
        
        /**
        * @brief Prepara a simulação para execução.
        * @details Calcula as rotas para todos os pacotes e popula a fila de eventos inicial.
        */
        void prepararSimulacao();
        
        /**
        * @brief Executa o loop principal da simulação.
        * @details Processa eventos da fila em ondas até que todos os pacotes sejam entregues.
        */
        void executar();

    private:
        int intervalo_transportes; // Intervalo de tempo entre a saída de dois transportes na mesma rota.
        int custo_remocao; // Custo de tempo para remover um pacote de uma pilha no armazém.
        int tempo_sistema; // O tempo atual do sistema na simulação.
        int min_postagem_global; // O menor tempo de postagem entre todos os pacotes, usado para cálculos de tempo.
        size_t pacotes_entregues; // Contador para o número de pacotes que chegaram ao destino final.

        
        Grafo<int, int> rede_armazens; // Grafo que representa a rede de armazéns e suas conexões.
        Vetor<Transporte> transportes; // Vetor que armazena todos os objetos Transporte (rotas).
        Vetor<Armazem> armazens; // Vetor que armazena todos os objetos Armazem.
        Vetor<Pacote> pacotes; // Vetor que armazena todos os objetos Pacote da simulação.
        minHeap<Evento*, EventoPtrComparador> fila_eventos; // Fila de prioridade (min-heap) que gerencia todos os eventos futuros.

        /**
        * @brief Processa todos os eventos de transporte que ocorrem no mesmo instante de tempo.
        * @param onda Um vetor de ponteiros para EventoTransporte.
        */
        void processarOndaDeTransporte(Vetor<Evento*>& onda);
        
        /**
        * @brief Processa um único evento de chegada de pacote.
        * @param evento O ponteiro para o evento de entrega do pacote.
        */
        void processarEventoEntrega(EventoEntregaPacote* evento);
        
        /**
        * @brief Garante que todos os armazéns tenham as seções corretas para seus vizinhos.
        */
        void inicializarSecoesDosArmazens();
        
        /**
        * @brief Calcula a rota mais curta para um pacote usando BFS (Busca em Largura).
        * @param pacote O pacote para o qual a rota será calculada e definida.
        */
        void calcularRotaParaPacote(Pacote& pacote);
        
        /**
        * @brief Retorna um ponteiro para um objeto Pacote com base em seu ID.
        * @param id O ID do pacote a ser encontrado.
        * @return Ponteiro para o Pacote, ou nullptr se não for encontrado.
        */
        Pacote* getPacotePorId(int id);
        
        /**
        * @brief Retorna um ponteiro para um objeto Armazem com base em seu ID.
        * @param id O ID do armazém a ser encontrado.
        * @return Ponteiro para o Armazem, ou nullptr se não for encontrado.
        */
        Armazem* getArmazemPorId(int id);

        /**
        * @brief Retorna um ponteiro para um objeto Transporte com base nos limites da rota.
        * @param id_origem O ID do armazém de origem.
        * @param id_destino O ID do armazém de destino.
        * @return Ponteiro para o Transporte, ou nullptr se não for encontrado.
        */
        Transporte* getTransportePorRota(int id_origem, int id_destino);

        /**
        * @brief Imprime uma mensagem de evento formatada no console.
        * @param tempo O tempo do sistema em que o evento ocorreu.
        * @param id_pacote O ID do pacote relacionado ao evento.
        * @param msg A mensagem descritiva do evento.
        */
        void imprimirEvento(int tempo, int id_pacote, const std::string& msg);
};

#endif // ESCALONADOR_H