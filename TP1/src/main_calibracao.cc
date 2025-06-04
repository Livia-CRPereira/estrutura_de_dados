
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector> // Adicionei esta include
#include <string> // Adicionei esta include

#include "estatistica.h"
#include "ordenador.h"

void saveToCSV(const std::string& filename, 
               const std::string& algoritmo,
               int tamanho,
               double comparacoes,
               double movimentacoes,
               double chamadas,
               double tempo) {
    
    std::ofstream file(filename, std::ios::app); // Modo append
    
    // Se o arquivo estiver vazio, escreve o cabeçalho
    if (file.tellp() == 0) {
        file << "algoritmo,tamanho,comparacoes,movimentacoes,chamadas,tempo\n";
    }
    
    file << algoritmo << ","
         << tamanho << ","
         << comparacoes << ","
         << movimentacoes << ","
         << chamadas << ","
         << tempo << "\n";
}


int main(int argc, char* argv[]) {

    Ordenador ordenador;

    std::vector<std::string> csvData; // Adicionei esta declaração

    // Verifica se o número de argumentos está correto
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    // Abre o arquivo de entrada
    std::ifstream entrada(argv[1]);
    if (!entrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << argv[1] << std::endl;
        return 1;
    }
    
    int seed = 0, tam = 0;
    double limiarCusto = 0, a = 0, b = 0, c = 0;
    // 1. Lê os parâmetros
    if (!(entrada >> seed >> limiarCusto 
                 >> a >> b >> c >> tam)) {
        std::cerr << "Erro ao ler os parâmetros do arquivo." << std::endl;
        return 1;
    }

    // 2. Lê o vetor do arquivo
    int* arr = new int[tam];
    for (int i = 0; i < tam; ++i) {
        if (!(entrada >> arr[i])) {
            std::cerr << "Erro ao ler o elemento " << i << " do vetor." << std::endl;
            delete[] arr;
            return 1;
        }
    }

    entrada.close();

    // Cabeçalho do CSV
    csvData.push_back("algoritmo,tamanho,comparacoes,movimentacoes,chamadas,tempo");

    // Teste Insertion Sort
    {
        int* arr_copy = new int[tam];
        std::copy(arr, arr + tam, arr_copy);
        
        ordenador.resetCounters();
        auto start = std::chrono::high_resolution_clock::now();
        ordenador.insercao(arr_copy, 0, tam - 1);
        auto end = std::chrono::high_resolution_clock::now();
        double tempo = std::chrono::duration<double>(end - start).count();
        
        saveToCSV("resultados.csv", 
            "insertion", 
            tam, 
            ordenador.getCmp(), 
            ordenador.getMove(), 
            ordenador.getCalls(), 
            tempo);
        
        delete[] arr_copy;
    }

    // Teste QuickSort
    {
        int* arr_copy = new int[tam];
        std::copy(arr, arr + tam, arr_copy);
        
        ordenador.resetCounters();
        auto start = std::chrono::high_resolution_clock::now();
        ordenador.quickSortRec(arr_copy, 0, tam - 1, 1);
        auto end = std::chrono::high_resolution_clock::now();
        double tempo = std::chrono::duration<double>(end - start).count();
        
        saveToCSV("resultados.csv", 
            "quicksort", 
            tam, 
            ordenador.getCmp(), 
            ordenador.getMove(), 
            ordenador.getCalls(),  
            tempo);
        
        delete[] arr_copy;
    }
    
    // Limpeza
    delete[] arr;
    return 0;
}