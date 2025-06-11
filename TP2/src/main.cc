#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "escalonador.h"
#include "pacote.h"
#include "transporte.h"

int main(int argc, char* argv[]) {
    // 1. Verificar se o nome do arquivo foi passado como argumento
    if (argc != 2) {
        std::cerr << "Erro: Uso incorreto." << std::endl;
        std::cerr << "Execute como: ./bin/tp2.out <caminho_para_o_arquivo_de_entrada>" << std::endl;
        return 1;
    }

    // 2. Abrir o arquivo de entrada
    std::string nome_arquivo = argv[1];
    std::ifstream arquivo_entrada(nome_arquivo);

    if (!arquivo_entrada.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << nome_arquivo << "'" << std::endl;
        return 1;
    }

    // 3. Ler os parâmetros da simulação 
    int capacidade, latencia, intervalo, custo_remocao;
    arquivo_entrada >> capacidade >> latencia >> intervalo >> custo_remocao;

    int numero_armazens;
    arquivo_entrada >> numero_armazens;

    // 4. Criar e configurar o escalonador
    Escalonador escalonador;
    escalonador.carregarConfiguracao(intervalo, custo_remocao, numero_armazens);


    // 5. Ler a matriz de adjacência, construir o grafo e criar os transportes
    for (int i = 0; i < numero_armazens; ++i) {
        for (int j = 0; j < numero_armazens; ++j) {
            int conexao;
            arquivo_entrada >> conexao;
            
            if (conexao == 1) {
                // Adiciona a conexão para a topologia do grafo (usado no cálculo de rotas)
                escalonador.adicionarConexao(i, j);
                Transporte novo_transporte(i, j, capacidade, latencia);
                escalonador.adicionarTransporte(novo_transporte);
            }
        }
    }

    // 6. Ler a lista de pacotes
    int numero_pacotes;
    arquivo_entrada >> numero_pacotes;

    int proximo_id_sequencial = 0; 
    for (int i = 0; i < numero_pacotes; ++i) {
        int tempo_postagem, id_original_do_arquivo, origem, destino;
        std::string str_pac, str_org, str_dst;

        arquivo_entrada >> tempo_postagem >> str_pac >> id_original_do_arquivo >> str_org >> origem >> str_dst >> destino;

        Pacote novo_pacote(proximo_id_sequencial, tempo_postagem, origem, destino, custo_remocao);
        escalonador.adicionarPacote(novo_pacote);
        proximo_id_sequencial++;
    }

    arquivo_entrada.close();

    // 7. Preparar e executar a simulação 
    escalonador.prepararSimulacao();
    escalonador.executar();

    return 0; // Sucesso
}