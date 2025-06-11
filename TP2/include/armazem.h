#ifndef ARMAZEM_H
#define ARMAZEM_H

#include <string>    // Para std::string
#include <stdexcept>  // Para std::runtime_error e std::out_of_range
#include <algorithm> // Para std::find (opcional, mas bom ter)
#include <iostream>  // Para std::cerr

#include "estruturas_auxiliares.h" 
#include "pacote.h"               

/**
 * @brief Representa um armazém ou centro de distribuição na rede logística.
 * @details Um armazém gerencia o armazenamento de pacotes em seções, onde cada
 * seção é uma pilha LIFO (Last-In, First-Out) de pacotes destinados a um
 * armazém vizinho específico. Ele também possui uma pilha auxiliar para
 * manobras internas de pacotes.
 */
class Armazem {
    public:
        /**
         * @brief Construtor padrão.
         * @details Cria um armazém com um ID inválido (-1).
         */
        Armazem() : id(-1) {}

        /**
         * @brief Construtor parametrizado.
         * @param armazem_id O ID único para este armazém.
         * @param armazens_vizinhos Um array de inteiros com os IDs dos armazéns vizinhos.
         * @param num_vizinhos O número de armazéns vizinhos no array.
         */
        Armazem(int armazem_id, const int* armazens_vizinhos, int num_vizinhos) :
            id(armazem_id) {

            for (int i = 0; i < num_vizinhos; ++i) {
                id_dos_vizinhos.adicionar(armazens_vizinhos[i]);
                secoes.adicionar(Pilha<Pacote>());
            }
        }

        /**
         * @brief Destrutor da classe Armazem.
         */
        ~Armazem() {}

        /**
         * @brief Define o ID do armazém.
         * @param x O novo ID.
         */
        void setId(int x){ id = x;}
        
        /**
         * @brief Obtém o ID do armazém.
         * @return O ID do armazém.
         */
        int getId() const { return id;}

        /**
         * @brief Armazena um pacote na seção correta.
         * @details Encontra a seção correspondente ao próximo destino do pacote e o empilha.
         * @param pacote O pacote a ser armazenado.
         * @param id_destino O ID do próximo armazém na rota do pacote.
         */
        void armazenaPacote(Pacote& pacote, int id_destino) {
            try {
                int secao_idx = getSecaoIndex(id_destino);
                secoes[static_cast<size_t>(secao_idx)].empilhar(pacote);
                pacote.setEstado(ARMAZENADO_EM_ARMAZEM);
            } catch (const std::runtime_error& e) {
                std::cerr << "Erro ao armazenar pacote no armazém " << id << ": " << e.what() << std::endl;
            }
        }

        /**
         * @brief Recupera o pacote do topo de uma seção específica.
         * @param proximo_armazem_destino_id O ID do armazém vizinho que define a seção.
         * @return O pacote removido do topo da pilha.
         * @throws std::runtime_error se a seção estiver vazia.
         */
        Pacote recuperaPacoteDaSecao(int proximo_armazem_destino_id) {
            try {
                int secao_idx = getSecaoIndex(proximo_armazem_destino_id);
                if (secoes[secao_idx].vazia()) {
                    throw std::runtime_error("Seção vazia, impossível recuperar pacote.");
                }
                Pacote pacote_removido = secoes[secao_idx].desempilhar();
                pacote_removido.setEstado(ALOCADO_PARA_TRANSPORTE);
                return pacote_removido;
            } catch (const std::runtime_error& e) {
                std::cerr << "Erro ao recuperar pacote do armazém " << id << ": " << e.what() << std::endl;
                throw; 
            }
        }

        /**
         * @brief Move um pacote de uma seção para a pilha auxiliar.
         * @details Usado para acessar pacotes que não estão no topo de uma seção.
         * @param proximo_armazem_destino_id ID que define a seção de origem.
         * @param tempo_atual_simulacao O tempo atual da simulação (pode ser usado para logs).
         * @param custo_movimento O custo de tempo a ser adicionado ao pacote por esta operação.
         */
        void empilharNaAuxiliar(int proximo_armazem_destino_id, int tempo_atual_simulacao, int custo_movimento) {
            try {
                int secao_idx = getSecaoIndex(proximo_armazem_destino_id);
                if (secoes[secao_idx].vazia()) {
                    throw std::runtime_error("Seção vazia, impossível empilhar na auxiliar.");
                }
                Pacote pacote_movido = secoes[secao_idx].desempilhar();
                pilha_auxiliar.empilhar(pacote_movido);

                pacote_movido.addTempoEspera(custo_movimento);
                pacote_movido.addTempoTotal(custo_movimento);
            } catch (const std::runtime_error& e) {
                std::cerr << "Erro em armazém " << id << " ao empilhar na auxiliar: " << e.what() << std::endl;
                throw;
            }
        }

        /**
         * @brief Move um pacote da pilha auxiliar de volta para sua seção de origem.
         * @param proximo_armazem_destino_id ID que define a seção de destino.
         * @param tempo_atual_simulacao O tempo atual da simulação.
         * @param custo_movimento O custo de tempo a ser adicionado ao pacote por esta operação.
         */
        void reempilharDaAuxiliar(int proximo_armazem_destino_id, int tempo_atual_simulacao, int custo_movimento) {
            try {
                int secao_idx = getSecaoIndex(proximo_armazem_destino_id);
                if (pilha_auxiliar.vazia()) {
                    throw std::runtime_error("Pilha auxiliar vazia, impossível reempilhar.");
                }
                Pacote pacote_reempilhado = pilha_auxiliar.desempilhar();
                secoes[secao_idx].empilhar(pacote_reempilhado);

                pacote_reempilhado.addTempoEspera(custo_movimento);
                pacote_reempilhado.addTempoTotal(custo_movimento);

            } catch (const std::runtime_error& e) {
                std::cerr << "Erro em armazém " << id << " ao reempilhar da auxiliar: " << e.what() << std::endl;
                throw;
            }
        }

        /**
         * @brief Remove e retorna um pacote da pilha auxiliar.
         * @param tempo_atual_simulacao O tempo atual da simulação.
         * @param custo_movimento O custo de tempo a ser adicionado ao pacote.
         * @return O pacote removido da pilha auxiliar.
         * @throws std::runtime_error se a pilha auxiliar estiver vazia.
         */
        Pacote desempilharDaAuxiliar(int tempo_atual_simulacao, int custo_movimento) {
            if (pilha_auxiliar.vazia()) {
                throw std::runtime_error("Pilha auxiliar vazia, impossível desempilhar.");
            }
            Pacote pacote_desempilhado = pilha_auxiliar.desempilhar();

            pacote_desempilhado.addTempoEspera(custo_movimento);
            pacote_desempilhado.addTempoTotal(custo_movimento);
            return pacote_desempilhado;
        }

        /**
         * @brief Adiciona dinamicamente um novo vizinho e a seção de armazenamento correspondente.
         * @param id_vizinho O ID do novo armazém vizinho.
         */
        void adicionarVizinhoESecao(int id_vizinho) {
            for (size_t i = 0; i < id_dos_vizinhos.tamanho(); ++i) {
                if (id_dos_vizinhos[i] == id_vizinho) {
                    return; // Vizinho já existe
                }
            }
            id_dos_vizinhos.adicionar(id_vizinho);
            secoes.adicionar(Pilha<Pacote>());
        }

        /**
         * @brief Verifica se uma seção específica está vazia.
         * @param proximo_armazem_destino_id ID que define a seção.
         * @return true se a seção estiver vazia, false caso contrário.
         */
        bool isSecaoVazia(int proximo_armazem_destino_id) const {
            try {
                int secao_idx = getSecaoIndex(proximo_armazem_destino_id);
                return secoes[secao_idx].vazia();
            } catch (const std::runtime_error&) {
                return true; 
            }
        }

        /**
         * @brief Verifica se todas as seções de armazenamento e a pilha auxiliar estão vazias.
         * @return true se o armazém estiver completamente vazio, false caso contrário.
         */
        bool todasSecoesVazias() const {
            for (size_t i = 0; i < secoes.tamanho(); ++i) {
                if (!secoes[i].vazia()) {
                    return false;
                }
            }
            return pilha_auxiliar.vazia();
        }

        /**
         * @brief Obtém a quantidade de pacotes em uma seção específica.
         * @param proximo_armazem_destino_id ID que define a seção.
         * @return O número de pacotes na seção.
         */
        int getQuantidadePacotesNaSecao(int proximo_armazem_destino_id) const {
            try {
                int secao_idx = getSecaoIndex(proximo_armazem_destino_id);
                return secoes[secao_idx].tamanho();
            } catch (const std::runtime_error&) {
                return 0;
            }
        }

        /**
         * @brief Obtém uma referência para uma seção (pilha) específica.
         * @param proximo_armazem_destino_id ID que define a seção.
         * @return Uma referência para a pilha de pacotes da seção.
         */
        Pilha<Pacote>& getSecaoRef(int proximo_armazem_destino_id) {
            int secao_idx = getSecaoIndex(proximo_armazem_destino_id);
            return secoes[secao_idx];
        }

        /**
         * @brief Obtém uma referência para a pilha auxiliar.
         * @return Uma referência para a pilha auxiliar de pacotes.
         */
        Pilha<Pacote>& getPilhaAuxiliarRef() { return pilha_auxiliar; }

        /**
         * @brief Remove um lote de pacotes de uma seção para transporte.
         * @param proximo_armazem_destino_id ID que define a seção de origem.
         * @param capacidade O número máximo de pacotes a remover.
         * @param tempo_atual_simulacao O tempo atual da simulação.
         * @param custo_remocao_base O custo de tempo a ser adicionado a cada pacote removido.
         * @return Um Vetor contendo os pacotes removidos.
         */
        Vetor<Pacote> removerPacotesParaTransporte(int proximo_armazem_destino_id, int capacidade, int tempo_atual_simulacao, int custo_remocao_base) {
            Vetor<Pacote> pacotes_para_transporte;
            try {
                int secao_idx = getSecaoIndex(proximo_armazem_destino_id);
                Pilha<Pacote>& secao = secoes[secao_idx];

                int pacotes_removidos_count = 0;
                while (!secao.vazia() && pacotes_removidos_count < capacidade) {
                    Pacote pacote_removido = secao.desempilhar();
                    pacote_removido.addTempoEspera(custo_remocao_base);
                    pacote_removido.addTempoTotal(custo_remocao_base);
                    pacotes_para_transporte.adicionar(pacote_removido);
                    pacotes_removidos_count++;
                }
            } catch (const std::runtime_error& e) {
                std::cerr << "Erro em Armazem::removerPacotesParaTransporte: " << e.what() << std::endl;
                throw;
            }
            return pacotes_para_transporte;
        }

        /**
         * @brief Obtém o índice da seção com base no ID do próximo armazém.
         * @param proximo_armazem_destino_id O ID do armazém vizinho para o qual o pacote se destina.
         * @return O índice (base 0) do vetor de seções.
         * @throws std::runtime_error se o ID de destino não corresponder a nenhum vizinho conhecido.
         */
        int getSecaoIndex(int proximo_armazem_destino_id) const {
            for (size_t i = 0; i < id_dos_vizinhos.tamanho(); ++i) {
                if (id_dos_vizinhos[i] == proximo_armazem_destino_id) {
                    return i;
                }
            }
            throw std::runtime_error("Armazém de destino não é um vizinho conhecido para este armazém.");
        }

    private:
        int id; // ID único do armazém.
        Vetor<int> id_dos_vizinhos; // Vetor que armazena os IDs dos armazéns vizinhos. A ordem é importante.
        Vetor<Pilha<Pacote>> secoes; // Vetor de seções, onde cada seção é uma pilha. A ordem corresponde a `id_dos_vizinhos`
        Pilha<Pacote> pilha_auxiliar; // Pilha temporária usada para manobras internas de pacotes.
};

#endif // ARMAZEM_H