#ifndef ORGANIZADOR_EVENTOS_H
#define ORGANIZADOR_EVENTOS_H

#include "estruturas.h"
#include "evento.h"

/**
 * @brief Gerencia o processamento de eventos e a manutenção dos índices de busca.
 * @details Esta classe é o núcleo do sistema, responsável por receber os eventos,
 * armazená-los de forma centralizada e atualizar as estruturas de dados (índices)
 * que permitem a execução eficiente das consultas.
 */
class OrganizadorEventos {
    public:
        /**
        * @brief Construtor padrão da classe.
        */
        OrganizadorEventos();

        /**
        * @brief Destrutor da classe.
        * @details Responsável por liberar a memória alocada dinamicamente pelos índices.
        */
        ~OrganizadorEventos();

        /**
        * @brief Processa uma linha de evento do arquivo de entrada.
        * @details Analisa os tokens de uma linha, cria um objeto Evento e atualiza
        * todos os índices de busca com a nova informação.
        * @param tokens Um vetor de strings contendo os campos da linha de evento.
        */
        void processarEvento(const Vetor<std::string>& tokens);

        /**
        * @brief Fornece acesso à base de eventos.
        * @return Uma referência constante para o vetor que armazena todos os eventos.
        */
        const Vetor<Evento>& getBaseEventos() const;

        /**
        * @brief Fornece acesso ao índice de clientes.
        * @return Uma referência constante para o HashMap que mapeia nomes de clientes a seus pacotes.
        */
        const HashMap<std::string, Vetor<int>*>& getIndiceClientes() const;

        /**
        * @brief Fornece acesso ao índice de estado atual dos pacotes.
        * @return Uma referência constante para o HashMap que mapeia um ID de pacote ao seu último evento.
        */
        const HashMap<int, int>& getIndiceEstadoAtual() const;

        /**
        * @brief Fornece acesso ao índice de pacote-tempo.
        * @return Uma referência constante para a Árvore AVL que armazena todos os eventos de forma ordenada.
        */
        const ArvoreAVL<long long, int>& getIndicePacoteTempo() const;

    private:
        // --- Armazenamento Central ---
        Vetor<Evento> baseDeEventos; ///< Armazenamento principal e único de todos os objetos de evento. [cite: 151, 152]

        // --- Índices para Aceleração de Consultas ---
        
        /**
        * @brief Índice para a consulta de histórico de pacote (PC).
        * @details Uma Árvore AVL que agrupa eventos por pacote e os ordena por tempo,
        * permitindo buscas eficientes por faixa de chave (pacote + tempo).
        */
        ArvoreAVL<long long, int> indicePacoteTempo;

        /**
        * @brief Índice para encontrar os pacotes associados a um cliente (consulta CL).
        * @details Mapeia o nome de um cliente para um ponteiro de vetor contendo os IDs
        * de todos os pacotes com os quais ele está associado.
        */
        HashMap<std::string, Vetor<int>*> indiceClientes;

        /**
        * @brief Índice para encontrar rapidamente o estado mais recente de um pacote.
        * @details Mapeia um ID de pacote diretamente para o índice de seu último evento
        * registrado na baseDeEventos, otimizando a busca pelo "estado atual".
        */
        HashMap<int, int> indiceEstadoAtual;

        // --- Funções Auxiliares ---
        
        /**
        * @brief Converte uma string para o tipo de evento enumerado.
        * @param s A string a ser convertida (ex: "RG", "AR").
        * @return O valor do enum TipoEvento correspondente.
        */
        TipoEvento stringParaTipoEvento(const std::string& s);

        /**
        * @brief Converte uma string para um inteiro de forma segura.
        * @param s A string a ser convertida.
        * @return O valor inteiro correspondente.
        */
        int extrairInt(const std::string& s);
};

#endif // ORGANIZADOR_EVENTOS_H