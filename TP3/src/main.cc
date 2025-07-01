#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <istream>

#include "organizador_eventos.h"
#include "consulta.h"
#include "estruturas.h"

int main(int argc, char* argv[]) {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::ifstream file_stream;
    std::istream* input_stream;

    // Se um argumento de linha de comando for fornecido, usá-lo como arquivo.
    if (argc > 1) {
        file_stream.open(argv[1]);
        if (!file_stream.is_open()) {
            std::cerr << "ERRO: Nao foi possivel abrir o arquivo de entrada '" << argv[1] << "'" << std::endl;
            return 1; // Encerra com código de erro
        }
        input_stream = &file_stream;
    }

    // --- FASE 1: LEITURA COMPLETA DO ARQUIVO ---
    Vetor<std::string> linhas_lidas;
    std::string linha;
    while (std::getline(*input_stream, linha)) {
        linhas_lidas.adicionar(linha);
    }
    // Fecha o arquivo se ele foi aberto.
    if (file_stream.is_open()) {
        file_stream.close();
    }

    // --- INICIALIZAÇÃO DOS OBJETOS PRINCIPAIS ---
    OrganizadorEventos organizador;
    Consulta consulta(&organizador);

    // --- FASE 2: PROCESSAMENTO DAS LINHAS ARMAZENADAS ---
    for (size_t i = 0; i < linhas_lidas.tamanho(); ++i) {
        const std::string& linha_atual = linhas_lidas[i];
        if (linha_atual.empty()) {
            continue;
        }

        // Tokeniza a linha atual
        std::stringstream ss(linha_atual);
        Vetor<std::string> tokens;
        std::string token;
        while (ss >> token) {
            tokens.adicionar(token);
        }

        if (tokens.tamanho() < 2) {
            continue; // Linha inválida
        }

        const std::string& tipoCmd = tokens[1];

        // Despacha o comando para o objeto correto
        if (tipoCmd == "EV") { // Evento de pacote [cite: 180]
            organizador.processarEvento(tokens);
        } else if (tipoCmd == "PC") { // Consulta de histórico de pacote [cite: 180]
            consulta.processarConsultaPacote(tokens);
        } else if (tipoCmd == "CL") { // Consulta de histórico de cliente [cite: 179]
            consulta.processarConsultaCliente(tokens);
        }
    }

    return 0; // Sucesso!
}