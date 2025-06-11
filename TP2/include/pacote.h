#ifndef PACOTE_H
#define PACOTE_H

#include "estruturas_auxiliares.h"
#include "armazem.h"

/**
 * @brief Enumera os possíveis estados de um pacote dentro do sistema logístico.
 */
enum Estado_do_pacote{
    NAO_FOI_POSTADO,           ///< O pacote foi criado, mas ainda não entrou na simulação.
    CHEGADA_ESCALONADA_ARMAZEM, ///< O pacote foi postado e está a caminho do primeiro armazém.
    ARMAZENADO_EM_ARMAZEM,      ///< O pacote está fisicamente em uma seção de um armazém.
    ALOCADO_PARA_TRANSPORTE,    ///< O pacote foi removido de uma seção e aguarda o transporte.
    ENTREGUE                    ///< O pacote chegou ao seu destino final e saiu do sistema.
};


/**
 * @brief Representa um pacote, a entidade fundamental movida pela simulação.
 * @details Esta classe encapsula todos os dados de um pacote individual, incluindo
 * seus pontos de origem e destino, sua rota calculada, sua localização atual,
 * seu estado e diversas métricas de tempo para análise de desempenho.
 */
class Pacote{
    public:
        /**
         * @brief Construtor padrão.
         * @details Cria um pacote com valores padrão/inválidos.
         */
        Pacote() : id(-1), tempo_postagem(0), custo_remocao(0),
                   tempo_em_espera(0), tempo_em_transporte(0), tempo_total_no_sistema(0), tempo_chegada_armazem_atual(0),
                   armazem_origem(-1), armazem_destino(-1),
                   armazem_atual(-1), estado_atual(NAO_FOI_POSTADO), proximo_passo_na_rota(nullptr) {};

        /**
         * @brief Construtor parametrizado.
         * @param id O ID único do pacote.
         * @param tempo_postagem O momento da simulação em que o pacote é postado.
         * @param armazem_origem O ID do armazém de origem.
         * @param armazem_destino O ID do armazém de destino final.
         * @param custo_remocao O tempo/custo base para manusear este pacote em um armazém.
         */
        Pacote(int id, int tempo_postagem, int armazem_origem, int armazem_destino, int custo_remocao){
            this->id = id;
            this->tempo_postagem = tempo_postagem;
            this->armazem_origem = armazem_origem;
            this->armazem_destino = armazem_destino;
            this->custo_remocao = custo_remocao;
            this->proximo_passo_na_rota = nullptr;

            tempo_em_espera = tempo_em_transporte = tempo_total_no_sistema = tempo_chegada_armazem_atual = 0;
            estado_atual = NAO_FOI_POSTADO;
        };
        
        /**
         * @brief Destrutor da classe Pacote.
         */
        ~Pacote(){};

        // Setters
        void setId(int x){ id = x;};
        void setTempoPostagem(int t) { tempo_postagem = t; };
        void setArmazemOrigem(int x){ armazem_origem = x; };
        void setTempoChegadaAtual(int tempo) { tempo_chegada_armazem_atual = tempo; } 
        void setArmazemDestino(int x){ armazem_destino = x; };
        void setArmazemAtual(int x){ armazem_atual = x; };
        void setEstado(Estado_do_pacote e){ estado_atual = e; };
        
        // Acumuladores de tempo
        void addTempoEspera(int t){ tempo_em_espera += t; };
        void addTempoTransporte(int t){ tempo_em_transporte += t; };
        void addTempoTotal(int t){ tempo_total_no_sistema += t; };

        // Getters
        int getId()const{ return id;};
        int getTempoPostagem()const{ return tempo_postagem;};
        int getTempoEspera()const{ return tempo_em_espera;};
        int getCusto()const{ return custo_remocao;}
        int getTempoTransporte()const{ return tempo_em_transporte;};
        int getTempoTotal()const{ return tempo_total_no_sistema;};
        int getTempoChegadaAtual() const { return tempo_chegada_armazem_atual; }
        int getArmazemDestino()const{ return armazem_destino;};
        int getArmazemOrigem()const{ return armazem_origem;};
        int getArmazemAtual()const{ return armazem_atual;};
        Estado_do_pacote getEstado()const{ return estado_atual;};

        /**
         * @brief Define a rota completa que o pacote deve seguir.
         * @param r Uma ListaEncadeada de IDs de armazéns que compõem a rota.
         */
        void setRota(const ListaEncadeada<int>& r) {
            rota = r;
            proximo_passo_na_rota = rota.getInicio();
            if (proximo_passo_na_rota) {
                armazem_atual = proximo_passo_na_rota->dado;
            }
        }

        /**
         * @brief Avança o pacote para o próximo passo da sua rota.
         * @details Atualiza o ponteiro interno da rota e o armazém atual do pacote.
         */
        void avancarNaRota() {
            if (proximo_passo_na_rota) {
                proximo_passo_na_rota = proximo_passo_na_rota->proximo;
                if (proximo_passo_na_rota) {
                    armazem_atual = proximo_passo_na_rota->dado;
                } else {
                    // Se não há próximo passo, significa que chegou ao destino
                    armazem_atual = armazem_destino;
                }
            }
        }

        /**
         * @brief Consulta qual é o próximo armazém na rota do pacote.
         * @return O ID do próximo armazém. Se já estiver no último trecho, retorna o destino final.
         */
        int getProximoArmazem() {
            if (proximo_passo_na_rota && proximo_passo_na_rota->proximo) {
                return proximo_passo_na_rota->proximo->dado;
            }
            // Se não há um "próximo do próximo", o próximo é o destino final.
            return getArmazemDestino();
        }


    private:
        int id; // Número do pacote
        int tempo_postagem; // Tempo de entrada no sistema
        int custo_remocao; // Tempo para remover um pacote armazenado

        int tempo_em_espera, tempo_em_transporte, tempo_total_no_sistema; // Estatísticas de tempo específicas para cada situação do pacote
        int tempo_chegada_armazem_atual; // Estatística de tempo em que o pacote chegou no armazém que está no momento

        int armazem_origem, armazem_destino; // Informações em id sobre remetente e destinatário
        int armazem_atual; // Localização atual (em id) do pacote

        Estado_do_pacote estado_atual; // Estado atual do pacote

        ListaEncadeada<int> rota; // ids dos armazens por onde tem que passar
        typename ListaEncadeada<int>::No* proximo_passo_na_rota; // Ponteiro para o nó atual na rota
};

#endif // PACOTE_H