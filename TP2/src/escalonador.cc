#include "escalonador.h"
#include <fstream> // Para leitura de arquivos
#include <sstream> // Para parsing de strings
#include <limits>  // Para std::numeric_limits


// Construtor
Escalonador::Escalonador()
    : relogio_simulacao(), 
      condicao_termino(false),
      proximo_id_pacote(0)
{
    // Construção dos objetos membros. Nada mais necessário aqui.
}

// Destrutor
Escalonador::~Escalonador() {
    // A maioria dos membros (Vetor, minHeap) gerencia sua própria memória.
    // Nada especial para desalocar aqui.
}

// Inicia e executa a simulação principal
void Escalonador::simuladorArmazens() {
    inicializa(); // Inicializa condição de término, relógio, variáveis de estado e eventos iniciais [cite: 51]

    // Enquanto a condição de término for FALSA [cite: 51]
    while (!condicao_termino) {
        // Verifica se há eventos pendentes ou pacotes nos armazéns para continuar a simulação
        if (fila_de_prioridade.vazia() && todasSecoesArmazensVazias()) {
            condicao_termino = true; // Se não há mais eventos e armazéns vazios, a simulação termina [cite: 54]
            break;
        }

        EventoSimulacao proximo_evento = retiraProximoEvento(); // Remove o próximo evento do escalonador [cite: 51]

        // Avança o relógio para o instante do próximo evento [cite: 51]
        relogio_simulacao = proximo_evento.tempo;

        // Executa o próximo evento e, se for o caso, agenda novos eventos [cite: 51]
        switch (proximo_evento.tipo) {
            case EVENTO_CHEGADA_PACOTE:
                processaChegadaPacote(proximo_evento);
                break;
            case EVENTO_INICIO_TRANSPORTE:
                processaInicioTransporte(proximo_evento);
                break;
            case EVENTO_FIM_SIMULACAO:
                condicao_termino = true;
                break;
            default:
                std::cerr << "Erro: Tipo de evento desconhecido ou inválido." << std::endl;
                break;
        }

        atualizarEstatisticas(); // Atualiza as estatísticas [cite: 51]
    }

    finaliza(); // Finaliza a simulação e gera relatórios [cite: 51]
}

// 1. Inicializa o estado do sistema e agenda eventos iniciais
void Escalonador::inicializa() {
    condicao_termino = false; // Garante que a condição de término é FALSA [cite: 51]
    relogio_simulacao = Time(0.0); // Inicializa o Relógio (usualmente zero) [cite: 51]

    // Inicializa as variáveis de estado do sistema
    carregaTopologiaArmazens(); // Carrega a estrutura do grafo de armazéns
    // Assume que a `carregaTopologiaArmazens` também popula o `armazens` Vetor
    // Ex: for (int i = 0; i < topologia_armazens.quantidadeVertices(); ++i) {
    //         armazens.adicionar(Armazem(i, topologia_armazens.getNome(i), topologia_armazens.quantidadeVertices()));
    //     }

    carregaPacotesIniciais(); // Lê o arquivo de entrada e agenda as chegadas iniciais dos pacotes
    agendarTransportesPeriodicos(); // Agenda os primeiros eventos de transporte entre armazéns
}

// 2. Insere um evento na fila de prioridade
void Escalonador::insereEvento(const EventoSimulacao& evento) {
    fila_de_prioridade.inserir(evento); // [cite: 66]
}

// 3. Retira o próximo evento da fila de prioridade
EventoSimulacao Escalonador::retiraProximoEvento() {
    if (fila_de_prioridade.vazia()) {
        // Isso não deve acontecer se a condição de término for bem verificada,
        // mas é uma salvaguarda.
        return EventoSimulacao(relogio_simulacao, EVENTO_FIM_SIMULACAO); // Retorna um evento de fim para parar o loop
    }
    return fila_de_prioridade.removerMinimo(); // [cite: 66]
}

// 4. Finaliza a simulação e gera relatórios
void Escalonador::finaliza() {
    gerarRelatorios(); // [cite: 51]
    std::cout << "Simulação finalizada no tempo: " << relogio_simulacao.toHours() << " horas." << std::endl;
}

// --- Métodos de Lógica da Simulação (esqueletos) ---

// Processa o evento de chegada de um pacote em um armazém
void Escalonador::processaChegadaPacote(const EventoSimulacao& evento) {
    // Recupera o pacote envolvido (precisará de um método para buscar pacote por ID)
    // Pacote& pacote_atual = pacotes_no_sistema[evento.id_pacote];

    // Se pacote chegou ao destino final [cite: 78]
    // if (pacote_atual.chegouAoDestinoFinal()) {
    //     registraEntregaPacote(pacote_atual); [cite: 78]
    // } else { // Senão chegou ao destino final
        // Armazena o pacote na respectiva seção [cite: 77]
        // Armazem& armazem_alvo = armazens[evento.id_armazem_destino]; // Onde o pacote acabou de chegar
        // int proximo_destino_id = pacote_atual.getProximoArmazemRota(); // Próximo armazém na rota
        // armazem_alvo.armazenaPacote(pacote_atual, proximo_destino_id, relogio_simulacao);
        // pacote_atual.setEstado(ARMAZENADO); [cite: 58]
    // }
    std::cout << "DEBUG: Pacote " << evento.id_pacote << " chegou ao armazém " << evento.id_armazem_destino << " no tempo " << relogio_simulacao.toHours() << std::endl;
}

// Processa o evento de início de transporte entre armazéns
void Escalonador::processaInicioTransporte(const EventoSimulacao& evento) {
    int origem_id = evento.id_armazem_origem;
    int destino_id = evento.id_armazem_destino;

    // Armazem& armazem_origem = armazens[origem_id];

    // Lógica para remover os pacotes mais antigos até a capacidade do transporte [cite: 72]
    // (Isso é o ponto crucial e mais complexo do TP: LIFO + prioridade de tempo)
    // Vetor<Pacote> pacotes_transportados;
    // int capacidade = info_transporte_global.getCapacidadeMaxPacotes();
    // double tempo_manipulacao_por_pacote = 0.1; // Exemplo: 0.1 horas por pacote desempilhado/reempilhado

    // Simulação da remoção LIFO com prioridade:
    // Você teria que iterar pela pilha (desempilhando e reempilhando),
    // selecionar os N pacotes mais antigos (até a capacidade) e contabilizar o tempo de manipulação
    // para cada pacote "bloqueador" que teve que ser movido.

    // Exemplo simplificado (sem LIFO complexo ou prioridade):
    /*
    for (int i = 0; i < capacidade && !armazem_origem.isSecaoVazia(destino_id); ++i) {
        // Isso só pegaria o topo LIFO. A complexidade LIFO precisa ser implementada dentro de Armazem::recuperaPacoteParaEnvio
        Pacote pacote_a_transportar = armazem_origem.recuperaPacote(); // Errado se for por seções
        pacotes_transportados.adicionar(pacote_a_transportar);
    }
    */

    // Escalona a chegada dos pacotes removidos no próximo armazém [cite: 73]
    // double tempo_transporte_segmento = info_transporte_global.getTempoTransportePorSegmento().toHours();
    /*
    for (size_t i = 0; i < pacotes_transportados.tamanho(); ++i) {
        Pacote& p = pacotes_transportados[i];
        p.setEstado(SENDO_TRANSPORTADO); [cite: 58]
        p.acumularTempoEmTransito(info_transporte_global.getTempoTransportePorSegmento());
        // p.avancaNaRota(); // Move o pacote para o próximo segmento da rota
        // int proximo_armazem_na_rota = p.getProximoArmazemRota(); // Onde ele vai chegar

        insereEvento(EventoSimulacao(relogio_simulacao + info_transporte_global.getTempoTransportePorSegmento(),
                                     EVENTO_CHEGADA_PACOTE, p.getId(), origem_id, destino_id)); // destino_id é o próximo na rota
    }
    */

    // Escalona o próximo evento de transporte para essa ligação (diário) [cite: 73]
    // agendarTransportesPeriodicos(); // Ou agendar um transporte específico para essa rota
    std::cout << "DEBUG: Inicio de transporte do armazém " << origem_id << " para " << destino_id << " no tempo " << relogio_simulacao.toHours() << std::endl;
}

// --- Métodos Auxiliares de Configuração e Verificação ---

void Escalonador::carregaTopologiaArmazens() {
    // Implementar a leitura da topologia do grafo a partir de um arquivo (se houver)
    // Ou criar um grafo de exemplo estaticamente.
    // Exemplo estático simples:
    int id_a = topologia_armazens.adicionarVertice("Armazem_A");
    int id_b = topologia_armazens.adicionarVertice("Armazem_B");
    int id_c = topologia_armazens.adicionarVertice("Armazem_C");

    // Adiciona armazéns ao vetor de armazéns gerenciados pelo escalonador
    armazens.adicionar(Armazem(id_a, "Armazem_A", topologia_armazens.quantidadeVertices()));
    armazens.adicionar(Armazem(id_b, "Armazem_B", topologia_armazens.quantidadeVertices()));
    armazens.adicionar(Armazem(id_c, "Armazem_C", topologia_armazens.quantidadeVertices()));


    // Adiciona arestas com peso (tempo de transporte)
    // O problema diz que o tempo de transporte é idêntico.
    // Devo conseguir armazenar o tempo aqui para calculos futuros

    std::cout << "DEBUG: Topologia de armazéns carregada." << std::endl;
}

void Escalonador::carregaPacotesIniciais() {
    // Lê os dados de entrada de pacotes de um arquivo [cite: 18]
    // Para cada linha do arquivo:
    // Data hora da postagem, Nome Remetente, Nome Destinatário, Tipo, Armazém de origem, Armazém de destino [cite: 18]
    // Exemplo de como ler e criar um pacote:
    /*
    std::ifstream arquivo_entradas("entradas.txt");
    std::string linha;
    while (std::getline(arquivo_entradas, linha)) {
        std::stringstream ss(linha);
        std::string s_data_hora, s_remetente, s_destinatario, s_tipo, s_ar_origem, s_ar_destino;
        // Assume que os campos são separados por vírgula ou espaço e lidos adequadamente
        // (Isso exigiria parsing mais robusto, mas para o esqueleto, é conceitual)
        // ss >> s_data_hora >> s_remetente >> s_destinatario >> s_tipo >> s_ar_origem >> s_ar_destino;

        // Exemplo direto de criação de pacote para teste:
        Time tempo_postagem = Time::fromString("2025-01-01 08:00:00");
        int ar_origem_id = topologia_armazens.getIndice("Armazem_A");
        int ar_destino_id = topologia_armazens.getIndice("Armazem_C");

        if (ar_origem_id != -1 && ar_destino_id != -1) {
            Pacote novo_pacote(proximo_id_pacote++, tempo_postagem, "Remetente", "Destinatario", "Padrao", ar_origem_id, ar_destino_id);
            // Calcular e armazenar a rota do pacote [cite: 5]
            // Isso exigiria um algoritmo de busca (BFS/DFS) no Grafo
            // ListaEncadeada<int> rota_calculada = encontrarRotaBFS(ar_origem_id, ar_destino_id, topologia_armazens);
            // novo_pacote.setRota(rota_calculada);

            pacotes_no_sistema.adicionar(novo_pacote);

            // Escalona a chegada de pacotes nos armazéns de postagem [cite: 74, 75]
            insereEvento(EventoSimulacao(tempo_postagem, EVENTO_CHEGADA_PACOTE, novo_pacote.getId(), ar_origem_id, ar_origem_id)); // Pacote chega no armazém de origem
            std::cout << "DEBUG: Pacote " << novo_pacote.getId() << " agendado para postagem." << std::endl;
        } else {
            std::cerr << "Erro: Armazém de origem ou destino não encontrado na topologia para pacote." << std::endl;
        }
    }
    arquivo_entradas.close();
    */
    // Para fins de esqueleto, vamos criar um pacote de exemplo:
    Time tempo_postagem_exemplo = Time::fromString("2025-01-01 08:00:00");
    int ar_origem_exemplo_id = topologia_armazens.getIndice("Armazem_A");
    int ar_destino_exemplo_id = topologia_armazens.getIndice("Armazem_C");

    if (ar_origem_exemplo_id != -1 && ar_destino_exemplo_id != -1) {
        Pacote pacote_exemplo(proximo_id_pacote++, tempo_postagem_exemplo, "Alice", "Bob", "Standard", ar_origem_exemplo_id, ar_destino_exemplo_id);

        // TODO: Implementar algoritmo BFS para calcular a rota
        // Por enquanto, uma rota simplificada: A -> B -> C
        ListaEncadeada<int> rota_exemplo;
        rota_exemplo.adicionar(topologia_armazens.getIndice("Armazem_B"));
        rota_exemplo.adicionar(topologia_armazens.getIndice("Armazem_C"));
        pacote_exemplo.setRota(rota_exemplo);


        pacotes_no_sistema.adicionar(pacote_exemplo);
        // Pacotes devem ser armazenados de forma que possam ser acessados por ID facilmente.
        // O vetor pacotes_no_sistema pode servir a isso, mas considere um mapeamento se IDs não forem sequenciais.

        insereEvento(EventoSimulacao(tempo_postagem_exemplo, EVENTO_CHEGADA_PACOTE, pacote_exemplo.getId(), ar_origem_exemplo_id, ar_origem_exemplo_id));
        std::cout << "DEBUG: Pacote de exemplo " << pacote_exemplo.getId() << " agendado para postagem em " << tempo_postagem_exemplo.toHours() << "h." << std::endl;
    }
}

void Escalonador::agendarTransportesPeriodicos() {
    // Uma vez por dia pacotes são transportados [cite: 12]
    // Definir o intervalo de "um dia" em horas (ex: 24 horas)
    Time intervalo_transporte_diario = Time(24.0);

    // Agendar o primeiro evento de transporte para cada ligação existente no grafo
    // Assumimos que o transporte ocorrerá em um horário fixo diário, por exemplo, às 00:00 de cada dia,
    // ou 24 horas após o início da simulação, e depois a cada 24 horas.
    // Para simplificar, vamos agendar o transporte 24h após o tempo de postagem do primeiro pacote.
    // No entanto, o pseudocódigo da Figura 2 sugere "Para cada ligação entre dois armazéns Escalona evento de transporte".
    // Isso deve ser feito para cada *ligação*, ou seja, aresta do grafo.

    // Isso exige que o Grafo tenha um jeito de iterar sobre as arestas, ou iteramos sobre vértices e seus vizinhos
    for (int u = 0; u < topologia_armazens.quantidadeVertices(); ++u) {
        const Vetor<Aresta<double>>& vizinhos = topologia_armazens.vizinhos(u);
        for (size_t i = 0; i < vizinhos.tamanho(); ++i) {
            int v = vizinhos[i].idx_destino;
            // Para evitar agendar o mesmo transporte duas vezes (A->B e B->A)
            if (u < v) { // Garante que cada aresta é considerada uma vez
                // Agenda o primeiro transporte para esta ligação.
                // O tempo inicial pode ser o tempo atual + intervalo, ou o próximo "meio-dia"
                Time tempo_primeiro_transporte = relogio_simulacao + intervalo_transporte_diario;
                insereEvento(EventoSimulacao(tempo_primeiro_transporte, EVENTO_INICIO_TRANSPORTE, -1, u, v));
                std::cout << "DEBUG: Transporte de " << topologia_armazens.getNome(u) << " para " << topologia_armazens.getNome(v) << " agendado para " << tempo_primeiro_transporte.toHours() << "h." << std::endl;
            }
        }
    }
}

// Verifica se todas as seções de todos os armazéns estão vazias
bool Escalonador::todasSecoesArmazensVazias() const {
    for (size_t i = 0; i < armazens.tamanho(); ++i) {
        if (!armazens[i].todasSecoesVazias()) {
            return false;
        }
    }
    return true;
}

void Escalonador::atualizarEstatisticas() {
    // Aqui você coletaria e acumularia dados para as estatísticas gerais do sistema
    // Ex: tempo médio de espera, utilização dos armazéns, identificar gargalos [cite: 14, 47]
    // Isso seria feito iterando sobre os pacotes_no_sistema e os armazens.
}

void Escalonador::gerarRelatorios() {
    // Imprimir as estatísticas finais para cada pacote:
    // • Tempo esperado de estadia [cite: 19]
    // • Tempo armazenado [cite: 19]
    // • Tempo em trânsito [cite: 19]
    // E estatísticas gerais do sistema.
    /*
    std::cout << "\n--- Relatório Final da Simulação ---" << std::endl;
    for (size_t i = 0; i < pacotes_no_sistema.tamanho(); ++i) {
        const Pacote& p = pacotes_no_sistema[i];
        std::cout << "Pacote ID: " << p.getId()
                  << ", Postagem: " << p.getTempoPostagem().toHours() << "h"
                  << ", Destino: " << topologia_armazens.getNome(p.getArmazemDestino())
                  << ", Estado Final: " << p.getEstado() // Você pode mapear o enum para string
                  << ", Tempo Total no Sistema: " << p.calcularTempoTotalNoSistema(relogio_simulacao).toHours() << "h"
                  << ", Tempo Armazenado: " << p.calcularTempoArmazenado().toHours() << "h"
                  << ", Tempo em Trânsito: " << p.calcularTempoEmTransito().toHours() << "h"
                  << ", Tempo Espera Manipulação: " << p.getTempoEspera().toHours() << "h"
                  << ", Tempo Esperado Estadia (Mínimo): " << p.getTempoEsperadoEstadia().toHours() << "h" // Adicione o cálculo do esperado quando tiver
                  << std::endl;
    }
    */

    // TODO: Adicionar estatísticas gerais do sistema aqui
}


