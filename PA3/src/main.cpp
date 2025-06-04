#include "heap.hpp"

int main(){
    int n;
    std::cin >> n;

    Heap heap(n);

    for (int i = 0; i < n; ++i) {
        int num;
        std::cin >> num;
        heap.Inserir(num);
    }

    while (!heap.Vazio()){
        heap.Remover();
    }

    std::cout << std::endl;

    return 0;
}