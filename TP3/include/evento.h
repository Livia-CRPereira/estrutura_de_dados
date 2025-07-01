#ifndef EVENTO_H
#define EVENTO_H

#include <string>
#include <iostream>
#include <iomanip>

/**
 * @brief Enumeração dos tipos de evento logístico.
 * @details Representa os possíveis tipos de eventos que um pacote pode sofrer,
 * [cite_start]conforme especificado no enunciado do trabalho prático[cite: 15, 16, 17, 18, 19, 20].
 */
enum TipoEvento { RG, AR, RM, UR, TR, EN, INDEFINIDO };

/**
 * @brief Representa um único evento no sistema logístico.
 * @details Armazena todos os dados possíveis para qualquer tipo de evento, como
 * data, hora, tipo, pacote associado e informações de localização.
 */
class Evento {
    public:
        int dataHora;           ///< A data e hora em que o evento ocorreu.
        TipoEvento tipo;        ///< O tipo do evento (RG, AR, etc.).
        int idPacote;           ///< O identificador do pacote associado a este evento.

        // --- Atributos opcionais, presentes dependendo do tipo de evento ---
        std::string remetente;    ///< Nome do cliente remetente (apenas para eventos RG).
        std::string destinatario; ///< Nome do cliente destinatário (apenas para eventos RG).
        int armazemOrigem;      ///< ID do armazém de origem.
        int armazemDestino;     ///< ID do armazém de destino.
        int secaoDestino;       ///< ID da seção de destino dentro de um armazém.

        /**
        * @brief Construtor padrão.
        * @details Inicializa um Evento com valores padrão/inválidos.
        */
        Evento() : dataHora(-1), tipo(INDEFINIDO), idPacote(-1),
                armazemOrigem(-1), armazemDestino(-1), secaoDestino(-1) {}
        
        /**
        * @brief Imprime o evento formatado na saída padrão.
        * @details A formatação da saída varia de acordo com o tipo do evento,
        * garantindo a conformidade com o formato especificado no trabalho.
        */
        void imprimir() const {
            // Formata dataHora para ter 7 dígitos com zeros
            std::cout << std::setw(7) << std::setfill('0') << dataHora << " EV ";
            
            std::string tipoStr;
            switch (tipo) {
                case RG: tipoStr = "RG"; break;
                case AR: tipoStr = "AR"; break;
                case RM: tipoStr = "RM"; break;
                case UR: tipoStr = "UR"; break;
                case TR: tipoStr = "TR"; break;
                case EN: tipoStr = "EN"; break;
                case INDEFINIDO: tipoStr = "??"; break;
            }

            // Formata idPacote para ter 3 dígitos com zeros
            std::cout << tipoStr << " " << std::setw(3) << std::setfill('0') << idPacote << " ";

            switch (tipo) {
                case RG:
                    // Imprime nomes sem formatação de largura, e IDs com formatação
                    std::cout << remetente << " " 
                            << destinatario << " " 
                            << std::setw(3) << std::setfill('0') << armazemOrigem << " " 
                            << std::setw(3) << std::setfill('0') << armazemDestino;
                    break;
                case AR:
                case RM:
                case UR:
                    std::cout << std::setw(3) << std::setfill('0') << armazemDestino << " " << std::setw(3) << std::setfill('0') << secaoDestino;
                    break;
                case TR:
                    std::cout << std::setw(3) << std::setfill('0') << armazemOrigem << " " << std::setw(3) << std::setfill('0') << armazemDestino;
                    break;
                case EN:
                    std::cout << std::setw(3) << std::setfill('0') << armazemDestino;
                    break;
                case INDEFINIDO:
                    break;
            }
            std::cout << std::endl;
        }
};

#endif // EVENTO_H