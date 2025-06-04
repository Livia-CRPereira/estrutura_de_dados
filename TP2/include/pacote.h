#ifndef PACOTE_H
#define PACOTE_H

#include "estruturas_auxiliares.h"
#include "armazem.h"
#include "time.h"

enum Estado_do_pacote{
nao_postado,
chegada_escalonada,
chegou_nao_armazenado,
armazenado,
alocado_para_transporte,
sendo_transportado,
entregue
};


class Pacote{
    public:
        Pacote();
        Pacote(int id_pacote, Time tempo_postagem_pacote, const std::string& remetente_str,
           const std::string& destinatario_str, const std::string& tipo_str,
           int armazem_origem_id_int, int armazem_destino_id_int);
        ~Pacote();


        int getId();
        Time getTempoPostagem();
        Time getTempoArmazenado();
        Time getTempoTransportado();
        Time getTempoEspera();
        std::string getRemetente();
        std::string getDestinatario();
        std::string getTipo();
        int getArmazemOrigem();
        int getArmazemDestino();
        Estado_do_pacote getEstado();
        Vetor<Time> getMomentos();
        ListaEncadeada<int> getRota();

        // Métodos Setters/Modificadores
        void setEstado(Estado_do_pacote novoEstado);
        void adicionaMomento(Time tempo_evento); // Registra o tempo de uma alteração no pacote
        void acumularTempoArmazenado(Time delta_tempo);
        void acumularTempoEmTransito(Time delta_tempo);
        void acumularTempoEsperaManipulacao(Time delta_tempo); // Novo
        void setRota(const ListaEncadeada<int>& nova_rota);
        void avancaNaRota(); // Remove o primeiro armazém da rota (já visitado)
        int getProximoArmazemRota() const; // Retorna o ID do próximo armazém na rota
        bool chegouAoDestinoFinal() const; // Verifica se o pacote chegou ao seu destino final

        Time getTempoUltimoEntrada();
        Time getTempoEsperadoEstadia();

    private:
        int id;
        Time tempo_postagem;
        Time tempo_armazenado, tempo_transportado, tempo_espera;

        std::string remetente, destinatario, tipo;
        int armazemOrigem_id, armazemDestino_id;
        Estado_do_pacote estado_atual;

        Vetor<Time> momentos_pacote_alterado;

        ListaEncadeada<int> rota;

        Time ultimo_tempo_entrada; 
        Time tempo_esperado_estadia;

};

#endif