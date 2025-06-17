// VPL - Testes para entrega
/*
#include <iostream>
#include <fstream>
#include <string>

#include "escalonador.h"
#include "pacote.h"
#include "transporte.h"
#include "armazem.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: ./bin/tp2.out <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::ifstream arquivo_entrada(argv[1]);
    if (!arquivo_entrada.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << argv[1] << "'" << std::endl;
        return 1;
    }

    int capacidade, latencia, intervalo, custo_remocao;
    arquivo_entrada >> capacidade >> latencia >> intervalo >> custo_remocao;

    if (!arquivo_entrada) {
        std::cerr << "Erro: Arquivo de entrada com formato inválido ou faltando parâmetros iniciais." << std::endl;
        return 1;
    }

    int numero_armazens;
    arquivo_entrada >> numero_armazens;
    
    Escalonador escalonador;
    escalonador.carregarConfiguracao(intervalo, custo_remocao, numero_armazens);

    for (int i = 0; i < numero_armazens; ++i) {
        for (int j = 0; j < numero_armazens; ++j) {
            int conexao;
            arquivo_entrada >> conexao;
            
            if (!arquivo_entrada) {
                std::cerr << "Erro: Arquivo de entrada incompleto ou mal formatado na matriz de adjacência." << std::endl;
                return 1;
            }
            
            if (conexao == 1) {
                if (i < j) {
                    escalonador.adicionarConexao(i, j);
                }
                Transporte novo_transporte(i, j, capacidade, latencia);
                escalonador.adicionarTransporte(novo_transporte);
            }
        }
    }

    int numero_pacotes;
    arquivo_entrada >> numero_pacotes;

    for (int i = 0; i < numero_pacotes; ++i) {
        int tempo_postagem, id_original, origem, destino;
        std::string str_pac, str_org, str_dst;
        arquivo_entrada >> tempo_postagem >> str_pac >> id_original >> str_org >> origem >> str_dst >> destino;

        if (!arquivo_entrada) {
            std::cerr << "Erro: Arquivo de entrada nao contem a quantidade de pacotes declarada." << std::endl;
            return 1;
        }

        Pacote novo_pacote(i, tempo_postagem, origem, destino, custo_remocao);
        escalonador.adicionarPacote(novo_pacote);
    }

    arquivo_entrada.close();

    escalonador.prepararSimulacao();
    escalonador.executar();

    return 0;
}
*/

// Análise experimental
#include <iostream>
#include <fstream>
#include <string>
#include <chrono> // Para medir o tempo

#include "escalonador.h"
#include "pacote.h"
#include "transporte.h"
#include "armazem.h"

int main(int argc, char* argv[]) {
    // Inicia a contagem de tempo
    auto inicio_execucao = std::chrono::high_resolution_clock::now();

    if (argc != 2) {
        std::cerr << "Uso: ./bin/simulador <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::ifstream arquivo_entrada(argv[1]);
    if (!arquivo_entrada.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << argv[1] << "'" << std::endl;
        return 1;
    }

    int intervalo, custo_remocao;
    arquivo_entrada >> intervalo >> custo_remocao;

    int numero_armazens, numero_rotas;
    arquivo_entrada >> numero_armazens >> numero_rotas;

    Escalonador escalonador;
    escalonador.carregarConfiguracao(intervalo, custo_remocao, numero_armazens);

    // Adiciona as conexões para o cálculo de rotas via BFS
    // Apenas uma vez para cada par, para não duplicar no grafo
    Vetor<bool> conexoes_adicionadas(numero_armazens * numero_armazens, false);

    // Lê as rotas de transporte com latências individuais
    for (int i = 0; i < numero_rotas; ++i) {
        int origem, destino, capacidade, latencia;
        arquivo_entrada >> origem >> destino >> capacidade >> latencia;
        
        if (!arquivo_entrada) {
            std::cerr << "Erro: Arquivo de entrada incompleto na seção de rotas." << std::endl;
            return 1;
        }

        if (origem < destino && !conexoes_adicionadas[origem * numero_armazens + destino]) {
            escalonador.adicionarConexao(origem, destino);
            conexoes_adicionadas[origem * numero_armazens + destino] = true;
        }

        Transporte novo_transporte(origem, destino, capacidade, latencia);
        escalonador.adicionarTransporte(novo_transporte);
    }

    int numero_pacotes;
    arquivo_entrada >> numero_pacotes;

    for (int i = 0; i < numero_pacotes; ++i) {
        int tempo_postagem, id_original, origem, destino;
        std::string str_pac, str_org, str_dst;
        arquivo_entrada >> tempo_postagem >> str_pac >> id_original >> str_org >> origem >> str_dst >> destino;

        if (!arquivo_entrada) {
            std::cerr << "Erro: Arquivo de entrada nao contem a quantidade de pacotes declarada." << std::endl;
            return 1;
        }

        Pacote novo_pacote(i, tempo_postagem, origem, destino, custo_remocao);
        escalonador.adicionarPacote(novo_pacote);
    }

    arquivo_entrada.close();

    escalonador.prepararSimulacao();
    escalonador.executar();

    // --- SALVANDO ESTATÍSTICAS DETALHADAS ---
    std::ofstream arq_stats("resultados/stats_pacotes.csv");
    arq_stats << "id_pacote,tempo_total,tempo_espera,tempo_transporte\n";
    for(int i = 0; i < numero_pacotes; ++i) {
        Pacote* p = escalonador.getPacotePorId(i);
        if (p) {
            arq_stats << p->getId() << ","
                      << p->getTempoTotal() << ","
                      << p->getTempoEspera() << ","
                      << p->getTempoTransporte() << "\n";
        }
    }
    arq_stats.close();

    // Finaliza e imprime o tempo de execução
    auto fim_execucao = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim_execucao - inicio_execucao;
    std::cerr << duracao.count() << std::endl;

    return 0;
}
