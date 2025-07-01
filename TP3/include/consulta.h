#ifndef CONSULTA_H
#define CONSULTA_H

#include "organizador_eventos.h"
#include "estruturas.h"
#include <string>

/**
 * @brief Processa as consultas solicitadas e gera a saída formatada.
 * @details Esta classe é responsável por interpretar as linhas de comando de consulta (PC e CL),
 * utilizar os índices pré-processados pelo OrganizadorEventos para coletar os dados
 * e imprimir os resultados na saída padrão conforme o formato especificado.
 */
class Consulta {
public:
    /**
     * @brief Construtor da classe Consulta.
     * @param organizador Um ponteiro para a instância do OrganizadorEventos,
     * usado para acessar a base de dados e os índices.
     */
    Consulta(OrganizadorEventos* organizador);

    /**
     * @brief Processa uma consulta de histórico de pacote (PC).
     * @details Busca e imprime todos os eventos associados a um determinado pacote
     * até o momento da consulta.
     * @param tokens Um vetor de strings contendo os campos da linha de consulta.
     */
    void processarConsultaPacote(const Vetor<std::string>& tokens);

    /**
     * @brief Processa uma consulta de pacotes de um cliente (CL).
     * @details Busca e imprime os eventos relevantes de todos os pacotes associados
     * a um determinado cliente até o momento da consulta.
     * @param tokens Um vetor de strings contendo os campos da linha de consulta.
     */
    void processarConsultaCliente(const Vetor<std::string>& tokens);

private:
    /**
     * @brief Ponteiro para o gerenciador central de eventos e dados.
     */
    OrganizadorEventos* organizador;
};

#endif // CONSULTA_H