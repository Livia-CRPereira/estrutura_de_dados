#include "graph.hpp"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Uso: ./programa -d|-n|-k" << std::endl;
        return 1;
    }

    std::string opcao = argv[1];
    Grafo grafo;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        grafo.InsereVertice();
    }

    for (int i = 0; i < n; ++i) {

        int qtd, vizinho;
        std::cin >> qtd;

        for (int j = 0; j < qtd; ++j) {
            std::cin >> vizinho;
            grafo.InsereAresta(i, vizinho);
        }

    }

    if (opcao == "-d") {

        std::cout << grafo.QuantidadeVertices() << std::endl;
        std::cout << grafo.QuantidadeArestas() << std::endl;
        std::cout << grafo.GrauMinimo() << std::endl;
        std::cout << grafo.GrauMaximo() << std::endl;

    } else if (opcao == "-n") {

        int n = grafo.QuantidadeVertices();

        for (int i = 0; i < n; ++i) {
            grafo.ImprimeVizinhos(i);
        }

    } else if (opcao == "-k") {

        int n = grafo.QuantidadeVertices();

        int arestasEsperadas = n * (n - 1) / 2;

        std::cout << (grafo.QuantidadeArestas() == arestasEsperadas ? 1 : 0) << std::endl;

    } else {

        std::cerr << "Opção inválida." << std::endl;
        return 1;

    }

    return 0;
}

