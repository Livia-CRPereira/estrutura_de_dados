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

    int numero_armazens;
    arquivo_entrada >> numero_armazens;

    Escalonador escalonador;
    // Configuração com parâmetros globais
    escalonador.carregarConfiguracao(intervalo, custo_remocao, numero_armazens);

    // Ler a matriz, adicionar conexões no grafo E criar os objetos Transporte
    for (int i = 0; i < numero_armazens; ++i) {
        for (int j = 0; j < numero_armazens; ++j) {
            int conexao;
            arquivo_entrada >> conexao;
            
            if (conexao == 1) {
                // Adiciona a conexão para o cálculo de rotas (BFS)
                // Adiciona só uma vez (i < j) para evitar duplicar arestas no grafo
                if (i < j) {
                    escalonador.adicionarConexao(i, j);
                }
                // Cria e adiciona o objeto Transporte para a rota i -> j
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

        Pacote novo_pacote(i, tempo_postagem, origem, destino, custo_remocao);
        escalonador.adicionarPacote(novo_pacote);
    }

    arquivo_entrada.close();

    escalonador.prepararSimulacao();
    escalonador.executar();

    return 0;
}