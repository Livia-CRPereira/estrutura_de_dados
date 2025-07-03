#include "escalonador.h"

Escalonador::Escalonador() : contador_rearmazenamentos(0), tempo_sistema(0), pacotes_entregues(0), total_pacotes_na_simulacao(0),
                            mapa_pacotes(1009), mapa_armazens(251), mapa_transportes(1009){}

Escalonador::~Escalonador() {
    // Libera os eventos da fila
    while (!fila_eventos.vazia()) {
        delete fila_eventos.removerMinimo();
    }

    // Libera a memória de todos os objetos que criamos com 'new'
    for (size_t i = 0; i < pacotes.tamanho(); ++i) {
        delete pacotes[i];
    }
    for (size_t i = 0; i < armazens.tamanho(); ++i) {
        delete armazens[i];
    }
    for (size_t i = 0; i < transportes.tamanho(); ++i) {
        delete transportes[i];
    }
}

void Escalonador::carregarConfiguracao(int interv_transp, int custo_rem, int num_armazens) {
    this->intervalo_transportes = interv_transp;
    this->custo_remocao = custo_rem;

    for (int i = 0; i < num_armazens; ++i) {
        // 1. Criar objeto no HEAP com 'new'
        Armazem* a = new Armazem();
        a->setId(i);

        // 2. Adicionar o ponteiro ao vetor
        armazens.adicionar(a);
        rede_armazens.adicionarVertice(i);
        
        // 3. Adicionar o mesmo ponteiro ao mapa
        mapa_armazens.inserir(i, a);
    }
}

void Escalonador::adicionarConexao(int u, int v) {
    rede_armazens.adicionarAresta(u, v, 1);
    // O grafo já adiciona a aresta de volta
    getArmazemPorId(u)->adicionarVizinhoESecao(v);
    getArmazemPorId(v)->adicionarVizinhoESecao(u);
}

void Escalonador::adicionarPacote(const Pacote& p) {
    // 1. Criar uma cópia no HEAP com 'new'
    Pacote* novo_pacote = new Pacote(p);

    // 2. Adicionar o ponteiro ao vetor
    pacotes.adicionar(novo_pacote);

    // 3. Adicionar o mesmo ponteiro ao mapa
    mapa_pacotes.inserir(novo_pacote->getId(), novo_pacote);
}

void Escalonador::adicionarTransporte(const Transporte& t) {
    // 1. Criar uma cópia no HEAP com 'new'
    Transporte* novo_transporte = new Transporte(t);
    
    // 2. Adicionar o ponteiro ao vetor
    transportes.adicionar(novo_transporte);

    // 3. Adicionar o mesmo ponteiro ao mapa
    long long chave_rota = (long long)novo_transporte->getOrigemId() * 10000 + novo_transporte->getDestinoId();
    mapa_transportes.inserir(chave_rota, novo_transporte);
}

void Escalonador::prepararSimulacao() {
    this->total_pacotes_na_simulacao = pacotes.tamanho();
    this->pacotes_entregues = 0;

    if (total_pacotes_na_simulacao == 0) return;

    // Encontrar o tempo do primeiro evento da simulação (a postagem mais cedo)
    int tempo_inicio_simulacao = pacotes[0]->getTempoPostagem();
    for (size_t i = 1; i < pacotes.tamanho(); ++i) {
        if (pacotes[i]->getTempoPostagem() < tempo_inicio_simulacao) {
            tempo_inicio_simulacao = pacotes[i]->getTempoPostagem();
        }
    }

    // Agenda a chegada inicial de cada pacote
    for (size_t i = 0; i < pacotes.tamanho(); ++i) {
        calcularRotaParaPacote(*pacotes[i]);
        Pacote* p = pacotes[i];
        
        fila_eventos.inserir(new Evento(p->getTempoPostagem(), p->getId(), p->getArmazemOrigem(), p->getArmazemOrigem(), PACOTE, CHEGADA_PACOTE));
    }

    // Agenda o primeiro transporte para cada rota definida
    for (size_t i = 0; i < transportes.tamanho(); ++i) {
        int origem = transportes[i]->getOrigemId();
        int destino = transportes[i]->getDestinoId();
        
        int tempo_primeiro_transporte = tempo_inicio_simulacao + intervalo_transportes;

        fila_eventos.inserir(new Evento(tempo_primeiro_transporte, i, origem, destino, TRANSPORTE, SAIDA_TRANSPORTE));
    }
}

void Escalonador::executar() {
    while (pacotes_entregues < total_pacotes_na_simulacao) {
        if (fila_eventos.vazia()) break;

        Evento* proximo_evento = fila_eventos.removerMinimo();
        this->tempo_sistema = proximo_evento->tempo;

        if (proximo_evento->tipo_evento == CHEGADA_PACOTE) {
            processarEventoChegada(proximo_evento);
        } else { // O tipo só pode ser SAIDA_TRANSPORTE
            processarEventoTransporte(proximo_evento);
        }
        delete proximo_evento;
    }
}

void Escalonador::processarEventoChegada(Evento* evento) {

    Pacote* pacote = getPacotePorId(evento->id);
    
    if (!pacote || pacote->getEstado() == ENTREGUE) return;

    
    if (pacote->getEstado() != NAO_FOI_POSTADO) {
        pacote->avancarNaRota();
    }
    pacote->setEstado(ARMAZENADO_EM_ARMAZEM);
    pacote->setTempoChegadaAtual(evento->tempo);

    int armazem_atual_id = pacote->getArmazemAtual();

    if (armazem_atual_id == pacote->getArmazemDestino()) {
        pacote->setEstado(ENTREGUE);
        pacotes_entregues++;

        int tempo_total = evento->tempo - pacote->getTempoPostagem();
        pacote->addTempoTotal(tempo_total);

        // imprimirEvento(evento->tempo, pacote->getId(), "entregue em " + formatarNumero(armazem_atual_id, 3));
    } else {
        int proximo_armazem_id = pacote->getProximoArmazem();
        getArmazemPorId(armazem_atual_id)->armazenaPacote(pacote, proximo_armazem_id); 
        // imprimirEvento(evento->tempo, pacote->getId(), "armazenado em " + formatarNumero(armazem_atual_id, 3) + " na secao " + formatarNumero(proximo_armazem_id, 3));
    }
}


void Escalonador::processarEventoTransporte(Evento* evento) {

    int origem = evento->origem;
    int destino = evento->destino;

    
    fila_eventos.inserir(new Evento(evento->tempo + intervalo_transportes, evento->id, origem, destino, TRANSPORTE, SAIDA_TRANSPORTE));

    Armazem* armazem_origem = getArmazemPorId(origem);
    Transporte* rota_transporte = getTransportePorRota(origem, destino);

    if (!rota_transporte || armazem_origem->isSecaoVazia(destino)) {
        return;
    }
    
    Pilha<Pacote*> pilha_auxiliar;
    int tempo_operacao = evento->tempo;

    Pilha<Pacote*>& secao = armazem_origem->getSecaoRef(destino);
    while (!secao.vazia()) {
        Pacote* p = secao.desempilhar();
        tempo_operacao += this->custo_remocao;
        
        int tempo_espera_etapa = tempo_operacao - p->getTempoChegadaAtual();
        p->addTempoEspera(tempo_espera_etapa);

        // imprimirEvento(tempo_operacao, p->getId(), "removido de " + formatarNumero(origem, 3) + " na secao " + formatarNumero(destino, 3));
        pilha_auxiliar.empilhar(p);
    }
    int tempo_final_remocao = tempo_operacao;

    int pacotes_carregados = 0;
    while (!pilha_auxiliar.vazia()) {
        Pacote* p = pilha_auxiliar.desempilhar();

        if (pacotes_carregados < rota_transporte->getCapacidadeMaxPacotes()) {
            pacotes_carregados++;
            
            p->addTempoTransporte(rota_transporte->getTempoTransportePorSegmento());
            // imprimirEvento(tempo_final_remocao, p->getId(), "em transito de " + formatarNumero(origem, 3) + " para " + formatarNumero(destino, 3));

            int tempo_chegada_destino = tempo_final_remocao + rota_transporte->getTempoTransportePorSegmento();
            
            
            fila_eventos.inserir(new Evento(tempo_chegada_destino, p->getId(), origem, destino, PACOTE, CHEGADA_PACOTE));

        } else {
            this->contador_rearmazenamentos++;
            // imprimirEvento(tempo_final_remocao, p->getId(), "rearmazenado em " + formatarNumero(origem, 3) + " na secao " + formatarNumero(destino, 3));
            armazem_origem->armazenaPacote(p, destino);
            p->setTempoChegadaAtual(tempo_final_remocao);
        }
    }
}

// --- Funções Auxiliares ---

std::string Escalonador::formatarNumero(int numero, int largura) {
    std::stringstream ss;
    ss << std::setw(largura) << std::setfill('0') << numero;
    return ss.str();
}

void Escalonador::imprimirEvento(int tempo, int id_pacote, const std::string& msg) {
    std::cout << formatarNumero(tempo, 7) << " pacote " << formatarNumero(id_pacote, 3) << " " << msg << std::endl;
}

Pacote* Escalonador::getPacotePorId(int id) {
    // buscar() retorna V**, que é Pacote**. O primeiro * desreferencia para Pacote*
    Pacote** resultado_ptr = mapa_pacotes.buscar(id);
    return (resultado_ptr == nullptr) ? nullptr : *resultado_ptr;
}

Armazem* Escalonador::getArmazemPorId(int id) {
    Armazem** resultado_ptr = mapa_armazens.buscar(id);
    return (resultado_ptr == nullptr) ? nullptr : *resultado_ptr;
}

Transporte* Escalonador::getTransportePorRota(int id_origem, int id_destino) {
    long long chave_rota = (long long)id_origem * 10000 + id_destino;
    Transporte** resultado_ptr = mapa_transportes.buscar(chave_rota);
    return (resultado_ptr == nullptr) ? nullptr : *resultado_ptr;
}

void Escalonador::calcularRotaParaPacote(Pacote& pacote) {
    int origem = pacote.getArmazemOrigem();
    int destino = pacote.getArmazemDestino();
    ListaEncadeada<int> rota_final;

    if (origem == destino) {
        rota_final.adicionar(origem);
        pacote.setRota(rota_final);
        return;
    }

    ListaEncadeada<int> fila_bfs;
    Vetor<bool> visitado(rede_armazens.quantidadeVertices(), false);
    Vetor<int> pred(rede_armazens.quantidadeVertices(), -1);

    fila_bfs.adicionar(origem);
    visitado[origem] = true;
    bool achou = false;

    while (!fila_bfs.vazia()) {
        int u = fila_bfs.removerPrimeiro();
        if (u == destino) {
            achou = true;
            break;
        }
        const Vetor<Aresta<int>>& vizinhos = rede_armazens.vizinhos(u);
        for (size_t i = 0; i < vizinhos.tamanho(); ++i) {
            int v = vizinhos[i].idx_destino;
            if (!visitado[v]) {
                visitado[v] = true;
                pred[v] = u;
                fila_bfs.adicionar(v);
            }
        }
    }

    if (achou) {
        for (int at = destino; at != -1; at = pred[at]) {
            rota_final.adicionarNoInicio(at);
        }
    }
    pacote.setRota(rota_final);
}