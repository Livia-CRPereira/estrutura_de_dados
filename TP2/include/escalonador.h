#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "estruturas_auxiliares.h"
#include "pacote.h"
#include "armazem.h"
#include "transporte.h"
#include "hashmap.h"

// --- Estrutura de Evento ---

enum TipoId { PACOTE, TRANSPORTE };
enum TipoEvento { CHEGADA_PACOTE, SAIDA_TRANSPORTE };

/**
 * @brief Representa uma ação pontual e agendada que move a simulação para frente.
 * @details O Evento é a unidade fundamental na Simulação de Eventos Discretos (SED).
 * Cada objeto Evento encapsula tudo que é necessário para descrever uma ação futura:
 * o que vai acontecer, quando, e quais entidades estão envolvidas. Eles são inseridos
 * em uma fila de prioridades (min-heap) para serem processados em ordem cronológica.
 *
 * A lógica de priorização é controlada pela 'chave' (long long), que é construída
 * de forma a resolver empates de tempo de maneira determinística e correta.
*/
struct Evento {
    long long chave;
    int tempo;
    int id;
    int origem;
    int destino;
    TipoId tipo_id;
    TipoEvento tipo_evento;

    // Construtor padrão
    Evento() : chave(0), tempo(0), id(-1), origem(-1), destino(-1), tipo_id(PACOTE), tipo_evento(CHEGADA_PACOTE) {}

    Evento(int t, int obj_id, int org, int dest, TipoId t_id, TipoEvento t_evento)
        : tempo(t), id(obj_id), origem(org), destino(dest), tipo_id(t_id), tipo_evento(t_evento)
    {
        if (tipo_id == PACOTE) {
            // Chave para evento de PACOTE (chegada)
            chave = (long long)tempo * 10000000LL + (long long)id * 10LL + 1LL;
        } else { // tipo_id == TRANSPORTE
            // Chave para evento de TRANSPORTE (saída)
            chave = (long long)tempo * 10000000LL + (long long)origem * 10000LL + (long long)destino * 10LL + 2LL;
        }
    }

    // Sobrecarga do operador '<' para o MinHeap saber como comparar
    bool operator<(const Evento& outro) const {
        return this->chave < outro.chave;
    }
};

/**
 * @brief Functor de comparação para a fila de prioridades de eventos (minHeap).
 * @details Garante que a minHeap ordene os ponteiros de Evento com base em suas
 * chaves numéricas, e não pelos endereços de memória. A menor chave tem a
 * maior prioridade.
 */
struct ComparadorEventos {
    bool operator()(const Evento* a, const Evento* b) const {
        return a->chave < b->chave;
    }
};

/**
 * @brief Motor central da simulação de logística, responsável por gerenciar o tempo,
 * os eventos e o estado de todos os componentes do sistema.
 * @details Esta classe implementa a lógica da Simulação de Eventos Discretos (SED).
 * Ela não avança o tempo passo a passo, mas "salta" diretamente para o próximo
 * momento importante, tornando a simulação eficiente. Suas responsabilidades são:
 *
 * 1.  Configuração: Ler os dados do problema e preparar o cenário inicial.
 * 2.  Execução do Loop de Eventos: Manter o "motor" da simulação rodando,
 * processando um evento de cada vez em ordem cronológica estrita.
 * 3.  Processamento de Lógicas de Negócio: Implementar as regras do sistema
 * Hanoi, como o armazenamento LIFO e a priorização de pacotes.
 * 4.  Agendamento de Novos Eventos: As ações tomadas em um evento (como
 * despachar um caminhão) geram novos eventos futuros (a chegada desse
 * caminhão), que são inseridos na fila para manter a simulação viva.
 */
class Escalonador {
    public:
        /**
         * @brief Construtor do Escalonador, inicializa o estado do sistema.
         */
        Escalonador();
    
        /**
         * @brief Destrutor do Escalonador.
         * @details Libera a memória de todos os eventos que ainda possam estar na fila.
         */
        ~Escalonador();
    
        /**
         * @brief Carrega os parâmetros globais da simulação.
         * @param interv_transp Intervalo de tempo entre despachos de veículos na mesma rota.
         * @param custo_rem Custo de tempo para remover um único pacote de uma pilha.
         * @param num_armazens O número total de armazéns na simulação.
         */
        void carregarConfiguracao(int interv_transp, int custo_rem, int num_armazens);
        
        /**
         * @brief Adiciona uma conexão (aresta) não-direcionada ao grafo da rede.
         * @param u O ID do primeiro armazém.
         * @param v O ID do segundo armazém.
         */
        void adicionarConexao(int u, int v);
        
        /**
         * @brief Adiciona um pacote à lista de pacotes a serem simulados.
         * @param p O objeto Pacote a ser adicionado.
         */
        void adicionarPacote(const Pacote& p);
    
        /**
         * @brief Adiciona uma rota de transporte ao sistema.
         * @param t O objeto Transporte que define a rota e suas propriedades.
         */
        void adicionarTransporte(const Transporte& t);
    
        /**
         * @brief Prepara a simulação para execução.
         * @details Calcula a rota para cada pacote via BFS e insere os eventos iniciais
         * (postagem de pacotes e os primeiros transportes) na fila de prioridades.
         */
        void prepararSimulacao();
        
        /**
         * @brief Executa o loop principal da simulação.
         * @details Retira um evento da fila de cada vez, avança o tempo e o processa,
         * até que todos os pacotes sejam entregues.
         */
        void executar();
    
        /**
         * @brief Retorna um ponteiro para um objeto Pacote com base em seu ID.
         * @param id O ID do pacote a ser encontrado.
         * @return Ponteiro para o Pacote, ou nullptr se não for encontrado.
         */
        Pacote* getPacotePorId(int id);
    private:
        // --- Variáveis de Estado ---
        int tempo_sistema;              // O tempo atual do sistema na simulação.
        int pacotes_entregues;          // Contador para o critério de parada da simulação.
        int total_pacotes_na_simulacao; // Número total de pacotes a serem entregues.
    
        // --- Parâmetros Globais ---
        int intervalo_transportes; // Intervalo de tempo entre a saída de dois transportes na mesma rota.
        int custo_remocao;         // Custo de tempo para remover um pacote de uma pilha no armazém.
    
        // --- Estruturas de Dados Principais ---
        Grafo<int, int> rede_armazens;             // Grafo que representa a rede de armazéns e suas conexões.
        Vetor<Armazem*> armazens;                   // Vetor que armazena todos os ponteiros para Armazem.
        Vetor<Pacote*> pacotes;                     // Vetor que armazena todos os ponteiros para Pacote da simulação.
        Vetor<Transporte*> transportes;             // Vetor que armazena todos os ponteiros para Transporte (rotas).
        minHeap<Evento*, ComparadorEventos> fila_eventos; // Fila de prioridade (min-heap) que gerencia todos os eventos futuros.
    
        // --- Mapas para acesso rápido ---
        HashMap<int, Pacote*> mapa_pacotes;
        HashMap<int, Armazem*> mapa_armazens;
        HashMap<long long, Transporte*> mapa_transportes;
    
        // --- Métodos de Processamento de Eventos ---
        /**
         * @brief Processa um único evento de chegada de pacote.
         * @details Verifica se é o destino final ou um armazém intermediário e
         * armazena o pacote ou finaliza sua jornada.
         * @param evento O ponteiro para o evento de chegada do pacote.
         */
        void processarEventoChegada(Evento* evento);
    
        /**
         * @brief Processa um único evento de transporte.
         * @details Simula a remoção LIFO de pacotes, prioriza o embarque, despacha
         * os pacotes e re-agenda o próximo transporte para a rota.
         * @param evento O ponteiro para o evento de transporte.
         */
        void processarEventoTransporte(Evento* evento);
    
        // --- Funções Auxiliares ---
        /**
         * @brief Calcula a rota mais curta para um pacote usando BFS (Busca em Largura).
         * @param pacote O pacote para o qual a rota será calculada e definida.
         */
        void calcularRotaParaPacote(Pacote& pacote);
    
        /**
         * @brief Imprime uma mensagem de evento formatada no console.
         * @param tempo O tempo do sistema em que o evento ocorreu.
         * @param id_pacote O ID do pacote relacionado ao evento.
         * @param msg A mensagem descritiva do evento.
         */
        void imprimirEvento(int tempo, int id_pacote, const std::string& msg);
    
        /**
         * @brief Formata um número com zeros à esquerda para exibição.
         * @param numero O número a ser formatado.
         * @param largura A largura total do campo.
         * @return Uma string com o número formatado.
         */
        std::string formatarNumero(int numero, int largura);
    

        
        /**
         * @brief Retorna um ponteiro para um objeto Armazem com base em seu ID.
         * @param id O ID do armazém a ser encontrado.
         * @return Ponteiro para o Armazem, ou nullptr se não for encontrado.
         */
        Armazem* getArmazemPorId(int id);
    
        /**
         * @brief Retorna um ponteiro para um objeto Transporte com base nos IDs da rota.
         * @param id_origem O ID do armazém de origem.
         * @param id_destino O ID do armazém de destino.
         * @return Ponteiro para o Transporte, ou nullptr se não for encontrado.
         */
        Transporte* getTransportePorRota(int id_origem, int id_destino);
};

#endif // ESCALONADOR_H