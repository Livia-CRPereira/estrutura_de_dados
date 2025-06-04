#include "ordenador.h"

Ordenador::Ordenador() {
    // Coeficientes de custo para cálculo do custo total (a*comp + b*mov + c*chamadas)
    a = 0.0;
    b = 0.0;
    c = 0.0;

    // Contadores de operações para análise de desempenho
    cmp = move = calls = 0;

    // Limiares e diferenças para adaptação do algoritmo
    limiarMPS = limiarLQ = 0;
    diffMPS = diffLQ = 0.0f;
}


// Define os pesos para cálculo do custo de ordenação
void Ordenador::setPesos(double a_, double b_, double c_){
    
    a = a_;
    b = b_;
    c = c_;
}

// Auxiliares ----------------------------------------------------------------

// Cria uma cópia independente do vetor original para preservar dados durante testes
int* Ordenador::copiaVetor(const int* original, int tam) const {
    int* copia = new int[tam];
    for (int i = 0; i < tam; i++) copia[i] = original[i];
    return copia;
}

// Reinicia contadores de performance para nova medição
void Ordenador::resetCounters() {
    cmp = move = calls = 0;
}

// Conta o número de transições decrescentes no vetor (indicadores de desordem)
int Ordenador::contaQuebras(int* v, int tam){
    int quebras = 0;
    for (int i = 1; i < tam; i++) {
        if (v[i] < v[i - 1]) {
            quebras++;
        }
    }
    return quebras;
}

// Troca dois elementos de posição e atualiza contador de movimentações
void Ordenador::swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Floor customizado para ajuste de tamanhos de partição (trata negativos corretamente)
double Ordenador::customFloor(double num){
    if (num >= 0) return static_cast<int>(num);
    else return static_cast<int>(num) - 1;
}

// Introduz quebras controladas no vetor para simulação de diferentes níveis de desordem
void Ordenador::shuffle(int* vetor, int tam, int limiarQuebras){
    int p1 = 0, p2 = 0, temp;
    for (int t = 0; t < limiarQuebras; t++) {
        // Gera dois índices distintos no intervalo [0..size-1]
        while (p1 == p2) {
            p1 = (int)(drand48() * tam);
            p2 = (int)(drand48() * tam);
        }
        // Realiza a troca para introduzir uma quebra
        temp = vetor[p1];
        vetor[p1] = vetor[p2];
        vetor[p2] = temp;
        p1 = p2 = 0;
    }

}

// Algoritmos de Ordenação ----------------------------------------------------

// Insertion Sort para pequenos vetores ou vetores quase ordenados
void Ordenador::insercao(int *v, int ini, int end) {
    calls++;
    
    for (int i = ini + 1; i <= end; ++i) {
        int chave = v[i];
        move++;
        
        int j = i - 1;
        
        // Move elementos maiores que a chave para frente
        while (j >= ini && v[j] > chave) {
            cmp++;
            v[j + 1] = v[j];
            move++;
            j--;
        }
        cmp++;
        
        v[j + 1] = chave;
        move++;
    }
}


// Mediana de três para seleção de pivô
int Ordenador::median(int a, int b, int c){
    if (((a <= c) && (c <= b)) || ((b <= c) && (c <= a))) return c;
    if (((b <= a) && (a <= c)) || ((c <= a) && (a <= b))) return a;
    return b;  
}

// Particionamento triplo para QuickSort (divide em elementos <, =, > que o pivô)
void Ordenador::partition3(int *v, int l, int r, int *i, int *j) {
    calls++;
    int m = l + (r - l) / 2;
    int pivot = median(v[l], v[r], v[m]); // Particionamento triplo para QuickSort (divide em elementos <, =, > que o pivô)

    *i = l;
    *j = r;

    // Particiona o vetor em três seções
    while (*i <= *j) {
        while (cmp++, v[*i] < pivot) (*i)++;

        while (cmp++, v[*j] > pivot) (*j)--;

        if (*i <= *j) {
            swap(&v[*i], &v[*j]);
            move += 3; // Contabiliza 3 movimentos por swap
            (*i)++;
            (*j)--;
        }
    }
    // Ajuste para casos extremos com todos elementos iguais
    if (*i == l && *j == r && v[*i] == pivot && v[*j] == pivot) {
        (*i)++;
        (*j)--;
    }
}

// QuickSort recursivo com fallback para Insertion Sort em partições pequenas
void Ordenador::quickSortRec(int *v, int l, int r, int minTamParticao){
    if (l >= r) return;

    // Usa Insertion Sort se a partição for menor que o limiar
    else if (r - l < minTamParticao){
        insercao(v, l, r);
        return; 
    }else{
        calls++;
        int i, j;
        partition3(v, l, r, &i, &j);

        // Chamadas recursivas para partições esquerda e direita
        if (l < j) quickSortRec(v, l, j, minTamParticao);
        if (i < r) quickSortRec(v, i, r, minTamParticao);
    }
}


// Estratégia de ordenação adaptativa que escolhe entre Insertion e QuickSort
void Ordenador::OrdenadorUniversal(int* copia, int tam, int minTamParticao, double limiarQuebras) {
    // Escolhe o algoritmo com base no número de quebras (medida de desordem)
    if (contaQuebras(copia, tam) < limiarQuebras) {
        insercao(copia, 0, tam - 1);
    } else {
        // Decide entre QuickSort e Insertion baseado no tamanho mínimo da partição
        if (tam > minTamParticao) {
            quickSortRec(copia, 0, tam - 1, minTamParticao);
        } else {
            insercao(copia, 0, tam - 1);
        }
        
    }
}

// Métodos para Determinação de Parâmetros Ótimos -----------------------------

// Calcula a diferença de custo para a Partição (MPS)
float Ordenador::calculaDiffMPS(const Vetor<EstatisticaMPS>& estatisticas, int minIdx, int maxIdx) {
    double menor = estatisticas[minIdx].getCusto();
    double maior = estatisticas[maxIdx].getCusto();
    return (maior - menor) > 0 ? (maior - menor) : (-1)*(maior - menor);
}

// Encontra o melhor tamanho mínimo de partição (MPS) via busca iterativa
int Ordenador::determinaLimiarParticao(int* v, int tam, double limiarCusto) {
    // Parâmetros iniciais da busca
    int minMPS = 2;
    int maxMPS = tam;
    int passoMPS = customFloor((maxMPS - minMPS) / 5);

    int numMPS = 6;
    diffMPS = limiarCusto + 1;
    int iter = 0;

    // Loop principal de refinamento do MPS
    while (diffMPS > limiarCusto && numMPS >= 5){
        Vetor<EstatisticaMPS> estatisticas;
        numMPS = 0;

        // Testa diferentes valores de MPS
        for (int mps = minMPS; mps <= maxMPS; mps += passoMPS) {

            resetCounters();

            // Testa o MPS atual e coleta estatísticas
            int* copia = copiaVetor(v, tam);
            OrdenadorUniversal(copia, tam, mps, 0);
            delete[] copia;

            EstatisticaMPS e;
            e.setValor(mps);
            e.setCmp(cmp);
            e.setMove(move);
            e.setCall(calls);
            e.setCusto(a, b, c);

            estatisticas.adicionar(e);
            numMPS++;
        }

        // Impressão dos resultados da iteração
        /*
        std::cout << std::fixed << std::setprecision(9);
        std::cout << std::endl;
        std::cout << "iter " << iter << std::endl;
        for (int i = 0; i < numMPS; ++i) {
            std::cout << "mps " << estatisticas[i].getValor()
                      << " cost " << estatisticas[i].getCusto()
                      << " cmp " << estatisticas[i].getCmp()
                      << " move " << estatisticas[i].getMove()
                      << " calls " << estatisticas[i].getCall() << std::endl;
        }
        */

        // Encontrar melhor MPS (menor custo)
        int melhorIdx = 0;
        for (int i = 1; i < numMPS; ++i) {
            if (estatisticas[i].getCusto() < estatisticas[melhorIdx].getCusto()) {
                melhorIdx = i;
            }
        }

        limiarMPS = estatisticas[melhorIdx].getValor();

        /*
        std::cout << "nummps " << numMPS
                  << " limParticao " << limiarMPS;
        */
        

        // Ajusta a faixa de busca para a próxima iteração
        int minIdx, maxIdx;
        calculaNovaFaixaParticao(estatisticas, melhorIdx, numMPS, minMPS, maxMPS, passoMPS,minIdx, maxIdx);
        
        diffMPS = calculaDiffMPS(estatisticas, minIdx, maxIdx);

        /*
        std::cout << std::fixed << std::setprecision(6);
        std::cout << " mpsdiff " << diffMPS << std::endl;
        */
        
        iter++;

    }

    return limiarMPS;
}

// Ajusta os limites da busca pelo MPS com base no melhor resultado atual
void Ordenador::calculaNovaFaixaParticao(const Vetor<EstatisticaMPS>& est, int melhorIdx, int numMPS, 
                                            int& minMPS, int& maxMPS, int& passoMPS, int& minIdx, int& maxIdx) {
    // Define nova janela de busca ao redor do melhor MPS
    int newMinIdx, newMaxIdx;

    if (melhorIdx == 0) {
        newMinIdx = 0;
        newMaxIdx = 2;
    } else if (melhorIdx == numMPS - 1) {
        newMinIdx = numMPS - 3;
        newMaxIdx = numMPS - 1;
    } else {
        newMinIdx = melhorIdx - 1;
        newMaxIdx = melhorIdx + 1;
    }

    // Atualiza limites e passo da busca
    minMPS = est[newMinIdx].getValor();
    maxMPS = est[newMaxIdx].getValor();
    passoMPS = customFloor((maxMPS - minMPS) / 5);
    if (passoMPS <= 0) passoMPS = 1;

    minIdx = newMinIdx;
    maxIdx = newMaxIdx;
}



// Calcula diferença de custo para Limiar de Quebras (LQ)
float Ordenador::calculaDiffLQ(const Vetor<EstatisticaLQ>& estatisticas, int minIdx, int maxIdx){
    double menor = estatisticas[minIdx + 1].getCusto();
    double maior = estatisticas[maxIdx].getCusto();
    return (maior - menor) > 0 ? (maior - menor) : (-1)*(maior - menor);
}

// Determina o número ideal de quebras para trocar de algoritmo
int Ordenador::determinaLimiarQuebras(int seed, int* v, int limiarParticao, int tam, double limiarCusto) {

    // Parâmetros iniciais da busca
    int minLQ = 1;
    int maxLQ = tam;
    int passoLQ = customFloor((maxLQ - minLQ) / 10);

    int numLQ = 6;
    diffLQ = limiarCusto + 1;
    int iter = 0;

    // Pré-ordena o vetor para criar cenários controlados
    OrdenadorUniversal(v, tam, limiarParticao, 0);

    // Loop principal de refinamento do LQ
    while (diffLQ > limiarCusto && numLQ >= 5){
        Vetor<EstatisticaLQ> estatisticas;
        numLQ = 0;

        int limite = ((tam/2) < maxLQ) ? (tam/2) : maxLQ;

        // Testa diferentes quantidades de quebras
        for (int lq = minLQ; lq <= limite; lq += passoLQ) {
            // Criar cópia do vetor
            int* vetor = copiaVetor(v, tam);

            // Defino vetor com o numero de quebras que quero
            srand48(seed);
            if (lq > 0) shuffle(vetor, tam, lq);

            // QuickSort
            resetCounters();
            int* copiaQ = copiaVetor(vetor, tam);
            quickSortRec(copiaQ, 0, tam - 1, limiarParticao);
            delete[] copiaQ;

            EstatisticaLQ eQ;
            eQ.setAlgoritmo(qs);
            eQ.setValor(lq);
            eQ.setCmp(cmp);
            eQ.setMove(move);
            eQ.setCall(calls);
            eQ.setCusto(a, b, c);
            estatisticas.adicionar(eQ);

            // Insertion
            resetCounters();
            int* copiaI = copiaVetor(vetor, tam);
            insercao(copiaI, 0, tam - 1);
            delete[] copiaI;

            EstatisticaLQ eI;
            eI.setAlgoritmo(in);
            eI.setValor(lq);
            eI.setCmp(cmp);
            eI.setMove(move);
            eI.setCall(calls);
            eI.setCusto(a, b, c);
            estatisticas.adicionar(eI);

            numLQ++;

            delete[] vetor;
        }

        // Impressão de resultados de cada iteração
        /*
        std::cout << std::fixed << std::setprecision(9);
        std::cout << std::endl;
        std::cout << "iter " << iter << std::endl;
        for (int i = 0; i < estatisticas.getTamanho(); i++) {
            std::cout << ((estatisticas[i].getAlgoritmo() == qs) ? "qs" : "in")
                      << " lq " << estatisticas[i].getValor()
                      << " cost " << estatisticas[i].getCusto()
                      << " cmp " << estatisticas[i].getCmp()
                      << " move " << estatisticas[i].getMove()
                      << " calls " << estatisticas[i].getCall() << std::endl;
        }
        */
        

        // Encontra o ponto onde os custos dos algoritmos se aproximam
        double melhorCusto = (estatisticas[0].getCusto() - estatisticas[1].getCusto());
        if (melhorCusto < 0) melhorCusto = melhorCusto*(-1);

        int melhorIdx = 0;
        limiarLQ = estatisticas[0].getValor();

        for (int i = 0; i < estatisticas.getTamanho(); i += 2) {
            double custoQS = estatisticas[i].getCusto();
            double custoIN = estatisticas[i+1].getCusto();

            double custo = custoQS - custoIN;
            double absCusto = custo > 0 ? custo : (-1)*custo;
            
            if (absCusto < melhorCusto) {
                limiarLQ = estatisticas[i].getValor();
                melhorCusto = absCusto;
                melhorIdx = i;
            }
        }

        /*
        std::cout << "numlq " << numLQ
                  << " limQuebras " << limiarLQ;
        */
        

        // Ajusta a faixa de busca para próxima iteração
        int minIdx, maxIdx;
        calculaNovaFaixaQuebras(estatisticas, melhorIdx, numLQ, minLQ, maxLQ, passoLQ, minIdx, maxIdx);

        diffLQ = calculaDiffLQ(estatisticas, minIdx, maxIdx);

        /*
        std::cout << std::fixed << std::setprecision(6);
        std::cout << " lqdiff " << diffLQ << std::endl;
        */
        
        iter++;

    }

    return limiarLQ;
}    

// Ajusta os limites da busca pelo LQ com base nos melhores resultados
void Ordenador::calculaNovaFaixaQuebras(const Vetor<EstatisticaLQ>& est, int melhorIdx, int numLQ, 
                                       int& minLQ, int& maxLQ, int& passoLQ, int& minIdx, int& maxIdx) {
    // Define nova janela de busca ao redor do melhor LQ encontrado
    int newMinIdx, newMaxIdx;

    if (melhorIdx == 0) {
        newMinIdx = 0;
        newMaxIdx = 5;
    } else if (melhorIdx == (est.getTamanho() - 2)) {
        newMinIdx = (est.getTamanho() - 6);
        newMaxIdx = (est.getTamanho() - 1);
    }
    else {
        newMinIdx = melhorIdx - 2;
        newMaxIdx = melhorIdx + 3;
    }

    // Atualiza limites e passo da busca
    minLQ = est[newMinIdx].getValor();
    maxLQ = est[newMaxIdx].getValor();
    passoLQ = customFloor((maxLQ - minLQ) / 5);
    if (passoLQ <= 0) passoLQ = 1;

    minIdx = newMinIdx;
    maxIdx = newMaxIdx;
}