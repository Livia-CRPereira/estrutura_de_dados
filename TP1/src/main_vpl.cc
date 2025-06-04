#include <iostream>
#include <fstream>
#include "ordenador.h"

int main(int argc, char* argv[]) {

    // Verifica se o arquivo foi passado como argumento
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo.txt>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[1];
    std::ifstream entrada(nomeArquivo);
    if (!entrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo entrada.txt" << std::endl;
        return 1;
    }

    // 1. Lê os parâmetros
    double a, b, c;
    double limiarCusto;
    int tam;
    int seed = 1;

    entrada >> seed;
    entrada >> limiarCusto;
    entrada >> a >> b >> c;
    entrada >> tam;

    // 2. Lê o vetor do arquivo
    int* vetorOriginal = new int[tam];
    for (int i = 0; i < tam; ++i) {
        if (!(entrada >> vetorOriginal[i])) {
            std::cerr << "Erro ao ler o elemento " << i << " do vetor." << std::endl;
            delete[] vetorOriginal;
            return 1;
        }
    }

    entrada.close();

    // 3. Inicializa o Ordenador
    Ordenador ord;
    ord.setPesos(a, b, c);
    std::cout << "size " << tam << " seed " << seed << " breaks " << ord.contaQuebras(vetorOriginal, tam) << std::endl;

    // 4. Calibração do limiar de partição (MPS)
    //std::cout << "Calibração da Partição" << std::endl;
    int limiarParticao = ord.determinaLimiarParticao(vetorOriginal, tam, limiarCusto);
    //std::cout << std::endl;

    // 5. Calibração do limiar de quebras (LQ)
    //std::cout << "Qualibração das Quebras" << std::endl;
    ord.determinaLimiarQuebras(seed, vetorOriginal, limiarParticao, tam, limiarCusto);
    //std::cout << std::endl;

    // std::cout << "Resultado final" << std::endl
       //       << "Limiar de Partição: " << limiarParticao << std::endl;
    //std::cout << "Limiar de Quebras: " << limiarQuebras << std::endl;

    delete[] vetorOriginal;
    return 0;
}
