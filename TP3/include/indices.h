#ifndef INDICES_H
#define INDICES_H

#include <string>

/**
 * @brief Estrutura para armazenar o índice do primeiro e do último evento de um pacote.
 * @details Utilizada em estratégias de indexação para otimizar o acesso a eventos chave.
 */
struct InfoPacote {
    int indicePrimeiroEvento = -1;  ///< O índice do primeiro evento do pacote na base de dados.
    int indiceUltimoEvento = -1;    ///< O índice do último evento do pacote na base de dados.

    /**
     * @brief Construtor para inicializar os índices.
     * @param primeiro O índice do primeiro evento.
     * @param ultimo O índice do último evento.
     */
    InfoPacote(int primeiro, int ultimo) 
        : indicePrimeiroEvento(primeiro), indiceUltimoEvento(ultimo) {}

    /**
     * @brief Construtor padrão.
     */
    InfoPacote() = default; 
};

/**
 * @brief Estrutura para armazenar um par de nomes de remetente e destinatário.
 */
struct ParNomes {
    std::string remetente;      ///< Nome do cliente remetente.
    std::string destinatario;   ///< Nome do cliente destinatário.
};

#endif // INDICES_H