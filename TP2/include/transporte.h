#ifndef TRANSPORTE_H
#define TRANSPORTE_H

/**
 * @brief Representa uma rota ou modal de transporte entre dois armazéns.
 * @details Esta classe encapsula as propriedades de uma rota de transporte,
 * como origem, destino, capacidade de carga e o tempo de viagem. Ela também
 * gera um ID único para a rota e controla seu estado diário na simulação.
 */
class Transporte{
    public:
        /**
         * @brief Construtor parametrizado para criar uma rota de transporte.
         * @param origem_id ID do local de origem.
         * @param destino_id ID do local de destino.
         * @param capacidade_max_pacotes Número máximo de pacotes que a rota suporta.
         * @param tempo_transporte Tempo necessário para percorrer a rota.
         */
        Transporte(int origem_id, int destino_id, int capacidade_max_pacotes, int tempo_transporte){
            this->origem_id = origem_id;
            this->destino_id = destino_id;
            this->capacidade_max_pacotes = capacidade_max_pacotes;
            this->tempo_transporte = tempo_transporte;
            setid_transporte();
            setRealizacaoDiaria(false);
        };

        /**
         * @brief Construtor padrão.
         * @details Inicializa uma rota de transporte com valores padrão ou inválidos.
         */
        Transporte() : origem_id(-1), destino_id(-1), capacidade_max_pacotes(0), tempo_transporte(0), realizacao_diaria(false) {
            setid_transporte();
        };
        
        /**
         * @brief Destrutor da classe Transporte.
         */
        ~Transporte(){};

        // Getters
        int getid_transporte() const { return id_transporte;};
        int getOrigemId() const{ return origem_id;};
        int getDestinoId() const{ return destino_id;};
        int getCapacidadeMaxPacotes() const{ return capacidade_max_pacotes;};
        int getTempoTransportePorSegmento() const{ return tempo_transporte;};
        bool getRealizacaoDiaria() const{ return realizacao_diaria;};

        // Setters
        /**
         * @brief Define o ID único do transporte.
         * @details Cria um ID combinando matematicamente os IDs de origem e destino.
         * Por exemplo, para uma rota com origem 12 e destino 34, o ID gerado será 1234.
         */
        void setid_transporte(){ 
            int fator = 1;
            int temp = destino_id;
            if (temp <= 0) { // Garante que o fator seja pelo menos 10 se o destino for 0
                fator = 10;
            } else {
                while (temp > 0) {
                    fator *= 10;
                    temp /= 10;
                }
            }

            int resultado = origem_id * fator + destino_id;
            id_transporte = resultado;
        };

        void setOrigemId(int id){ origem_id = id;};
        void setDestinoId(int id){ destino_id = id;};
        void setCapacidadeMaxPacotes(int cap){ capacidade_max_pacotes = cap;};
        void setTempoTransportePorSegmento(int tempo){ tempo_transporte = tempo;};
        void setRealizacaoDiaria(bool realizada){ realizacao_diaria = realizada;};

    private:
        int id_transporte; // ID único gerado a partir da origem e destino.
        int origem_id; // ID do local de origem da rota.
        int destino_id; // ID do local de destino da rota.
        int capacidade_max_pacotes; // Número máximo de pacotes que podem ser transportados de uma vez.
        int tempo_transporte; // Tempo (em unidades de simulação) para completar a viagem.
        bool realizacao_diaria; // Flag para controlar se a rota já foi usada no dia atual.
};

#endif // TRANSPORTE_H