#include "consulta.h"
#include <iostream>

Consulta::Consulta(OrganizadorEventos* org) : organizador(org) {}

// IMPRESSÃO
void imprimirEventoPtr(const Evento* evento) {
    if (evento) {
        evento->imprimir();
    }
}

// Consulta PC
void Consulta::processarConsultaPacote(const Vetor<std::string>& tokens) {
    // std::cout << "\n--- Processando Consulta PC: " << tokens[0] << " " << tokens[1] << " " << tokens[2] << " ---" << std::endl;

    int dataHora = std::stoi(tokens[0]);
    int idPacote = std::stoi(tokens[2]);

    // Imprime a linha da consulta original
    std::cout << std::setw(6) << std::setfill('0') << dataHora << " PC " << tokens[2] << std::endl;

    const ArvoreAVL<long long, int>& indice = organizador->getIndicePacoteTempo();
    long long chave_inicio = (long long)idPacote * 10000000000LL;
    long long chave_fim = chave_inicio + 9999999999LL;

    // std::cout << "[DEBUG ConsultaPC] Buscando na Arvore em faixa. Chave inicio: " << chave_inicio << ", Chave fim: " << chave_fim << std::endl;
    Vetor<int> indices_encontrados;
    indice.buscarEmFaixa(chave_inicio, chave_fim, indices_encontrados);
    // std::cout << "[DEBUG ConsultaPC] Encontrados " << indices_encontrados.tamanho() << " eventos na arvore para este pacote." << std::endl;

    const Vetor<Evento>& baseDeEventos = organizador->getBaseEventos();
    Vetor<const Evento*> eventos_para_imprimir;

    for (size_t i = 0; i < indices_encontrados.tamanho(); ++i) {
        int indice_evento = indices_encontrados[i];
        const Evento& evento = baseDeEventos[indice_evento];
        if (evento.dataHora <= dataHora) {
            eventos_para_imprimir.adicionar(&evento);
        }
    }
    // std::cout << "[DEBUG ConsultaPC] Apos filtrar por data, " << eventos_para_imprimir.tamanho() << " eventos serao impressos." << std::endl;

    std::cout << eventos_para_imprimir.tamanho() << std::endl;
    for (size_t i = 0; i < eventos_para_imprimir.tamanho(); ++i) {
        eventos_para_imprimir[i]->imprimir();
    }
}

// Consulta CL
void Consulta::processarConsultaCliente(const Vetor<std::string>& tokens) {
    int dataHora = std::stoi(tokens[0]);
    std::string nomeCliente = tokens[2];

    std::cout << std::setw(6) << std::setfill('0') << dataHora << " CL " << nomeCliente << std::endl;

    const auto& indice_clientes = organizador->getIndiceClientes();
    Vetor<int>** ptr_vetor_pacotes = indice_clientes.buscar(nomeCliente);

    if (ptr_vetor_pacotes == nullptr) {
        std::cout << 0 << std::endl;
        return;
    }

    Vetor<int>* vetor_pacotes = *ptr_vetor_pacotes;
    const auto& baseDeEventos = organizador->getBaseEventos();
    const auto& indice_pacote_tempo = organizador->getIndicePacoteTempo();

    // Passo 1: Criar uma árvore temporária para armazenar e ordenar os resultados.
    // A chave será a data/hora + índice, para garantir ordem e unicidade.
    // O valor será um ponteiro para o evento original (sem duplicar dados).
    ArvoreAVL<long long, const Evento*> arvore_resultados;
    int contador_resultados = 0;

    // Passo 2: Para cada pacote, encontrar o primeiro e último evento e INSERIR na árvore de resultados.
    for (size_t i = 0; i < vetor_pacotes->tamanho(); ++i) {
        int id_pacote = (*vetor_pacotes)[i];

        Vetor<int> indices_todos_eventos;
        long long chave_busca_inicio = (long long)id_pacote * 10000000000LL;
        long long chave_busca_fim = chave_busca_inicio + 9999999999LL;
        indice_pacote_tempo.buscarEmFaixa(chave_busca_inicio, chave_busca_fim, indices_todos_eventos);

        Vetor<int> indices_validos;
        for (size_t j = 0; j < indices_todos_eventos.tamanho(); ++j) {
            int indice = indices_todos_eventos[j];
            if (baseDeEventos[indice].dataHora <= dataHora) {
                indices_validos.adicionar(indice);
            }
        }
        
        if (indices_validos.tamanho() > 0) {
            // Insere o primeiro evento na árvore de resultados
            int indice_primeiro = indices_validos[0];
            const Evento* primeiro_evento = &baseDeEventos[indice_primeiro];
            long long chave_primeiro = (long long)primeiro_evento->dataHora * 100000 + indice_primeiro;
            arvore_resultados.inserir(chave_primeiro, primeiro_evento);
            contador_resultados++;
            
            // Se houver mais de um evento, insere o último também
            if (indices_validos.tamanho() > 1) {
                int indice_ultimo = indices_validos[indices_validos.tamanho() - 1];
                const Evento* ultimo_evento = &baseDeEventos[indice_ultimo];
                long long chave_ultimo = (long long)ultimo_evento->dataHora * 100000 + indice_ultimo;
                // A busca pode retornar o mesmo ponteiro, mas a inserção com chave diferente é segura.
                if (indice_primeiro != indice_ultimo) {
                    arvore_resultados.inserir(chave_ultimo, ultimo_evento);
                    contador_resultados++;
                }
            }
        }
    }

    // Passo 3: Imprimir os resultados fazendo a travessia em ordem na árvore.
    std::cout << contador_resultados << std::endl;
    arvore_resultados.travessiaEmOrdem(imprimirEventoPtr);
}