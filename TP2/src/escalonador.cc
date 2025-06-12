#include "escalonador.h"

Escalonador::Escalonador() : tempo_sistema(0), pacotes_entregues(0), total_pacotes_na_simulacao(0) {}

Escalonador::~Escalonador() {
    while (!fila_eventos.vazia()) {
        delete fila_eventos.removerMinimo();
    }
}

void Escalonador::carregarConfiguracao(int interv_transp, int custo_rem, int num_armazens) {
    this->intervalo_transportes = interv_transp;
    this->custo_remocao = custo_rem;

    for (int i = 0; i < num_armazens; ++i) {
        Armazem a;
        a.setId(i);
        armazens.adicionar(a);
        rede_armazens.adicionarVertice(i);
    }
}

void Escalonador::adicionarConexao(int u, int v) {
    rede_armazens.adicionarAresta(u, v, 1);
    // O grafo já adiciona a aresta de volta
    getArmazemPorId(u)->adicionarVizinhoESecao(v);
    getArmazemPorId(v)->adicionarVizinhoESecao(u);
}

void Escalonador::adicionarPacote(const Pacote& p) {
    pacotes.adicionar(p);
}

void Escalonador::adicionarTransporte(const Transporte& t) {
    transportes.adicionar(t);
}

void Escalonador::prepararSimulacao() {
    this->total_pacotes_na_simulacao = pacotes.tamanho();
    this->pacotes_entregues = 0;

    if (total_pacotes_na_simulacao == 0) return; 

    // Encontrar o tempo do primeiro evento da simulação (a postagem mais cedo)
    int tempo_inicio_simulacao = pacotes[0].getTempoPostagem();
    for (size_t i = 1; i < pacotes.tamanho(); ++i) {
        if (pacotes[i].getTempoPostagem() < tempo_inicio_simulacao) {
            tempo_inicio_simulacao = pacotes[i].getTempoPostagem();
        }
    }

    for (size_t i = 0; i < pacotes.tamanho(); ++i) {
        calcularRotaParaPacote(pacotes[i]);
        fila_eventos.inserir(new EventoChegadaPacote(pacotes[i].getTempoPostagem(), pacotes[i].getId()));
    }

    // Agenda o primeiro transporte para cada rota definida
    for (size_t i = 0; i < transportes.tamanho(); ++i) {
        int origem = transportes[i].getOrigemId();
        int destino = transportes[i].getDestinoId();
        
        // O primeiro transporte acontece 'intervalo' após o início da atividade
        int tempo_primeiro_transporte = tempo_inicio_simulacao + intervalo_transportes;
        
        fila_eventos.inserir(new EventoTransporte(tempo_primeiro_transporte, origem, destino));
    }
}

void Escalonador::executar() {
    while (pacotes_entregues < total_pacotes_na_simulacao) {
        if (fila_eventos.vazia()) break;

        Evento* proximo_evento = fila_eventos.removerMinimo();
        this->tempo_sistema = proximo_evento->tempo;

        if (proximo_evento->tipo == EVENTO_CHEGADA_PACOTE) {
            processarEventoChegada(static_cast<EventoChegadaPacote*>(proximo_evento));
        } else {
            processarEventoTransporte(static_cast<EventoTransporte*>(proximo_evento));
        }
        delete proximo_evento;
    }
}

void Escalonador::processarEventoChegada(EventoChegadaPacote* evento) {
    Pacote* pacote = getPacotePorId(evento->id_pacote);
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
        
        imprimirEvento(evento->tempo, pacote->getId(), "entregue em " + formatarNumero(armazem_atual_id, 3));
    } else {
        armazem_atual_id = pacote->getArmazemAtual(); // A rota já avançou
        int proximo_armazem_id = pacote->getProximoArmazem();
        getArmazemPorId(armazem_atual_id)->armazenaPacote(*pacote, proximo_armazem_id);
        imprimirEvento(evento->tempo, pacote->getId(), "armazenado em " + formatarNumero(armazem_atual_id, 3) + " na secao " + formatarNumero(proximo_armazem_id, 3));
    }
}

void Escalonador::processarEventoTransporte(EventoTransporte* evento) {
    int origem = evento->id_origem;
    int destino = evento->id_destino;

    fila_eventos.inserir(new EventoTransporte(evento->tempo + intervalo_transportes, origem, destino));

    Armazem* armazem_origem = getArmazemPorId(origem);
    if (armazem_origem->isSecaoVazia(destino)) return;

    // Busca o transporte específico para esta rota para pegar capacidade e latência
    Transporte* rota_transporte = getTransportePorRota(origem, destino);
    if (!rota_transporte) return; // Rota não definida, não faz nada

    Pilha<Pacote>& secao = armazem_origem->getSecaoRef(destino);
    Vetor<Pacote> pacotes_removidos_lifo;
    int tempo_operacao = evento->tempo;

    while (!secao.vazia()) {
        Pacote p = secao.desempilhar();
        tempo_operacao += this->custo_remocao;

        Pacote* p_original_ptr = getPacotePorId(p.getId());
        int tempo_espera_desta_etapa = tempo_operacao - p_original_ptr->getTempoChegadaAtual();
        p_original_ptr->addTempoEspera(tempo_espera_desta_etapa);

        imprimirEvento(tempo_operacao, p.getId(), "removido de " + formatarNumero(origem, 3) + " na secao " + formatarNumero(destino, 3));
        pacotes_removidos_lifo.adicionar(p);
    }
    int tempo_final_op = tempo_operacao;

    Vetor<Pacote*> pacotes_para_ordenar;
    for (size_t i = 0; i < pacotes_removidos_lifo.tamanho(); ++i) {
        pacotes_para_ordenar.adicionar(getPacotePorId(pacotes_removidos_lifo[i].getId()));
    }
    
    int evento_tempo = evento->tempo;
    quickSort(pacotes_para_ordenar, [evento_tempo](const Pacote* a, const Pacote* b) {
        int espera_a = evento_tempo - a->getTempoChegadaAtual();
        int espera_b = evento_tempo - b->getTempoChegadaAtual();
        if (espera_a != espera_b) return espera_a > espera_b;
        return a->getTempoPostagem() < b->getTempoPostagem();
    });

    Vetor<int> ids_para_transportar;
    for (size_t i = 0; i < pacotes_para_ordenar.tamanho() && i < (size_t)rota_transporte->getCapacidadeMaxPacotes(); ++i) {
        ids_para_transportar.adicionar(pacotes_para_ordenar[i]->getId());
    }

    for (int i = pacotes_removidos_lifo.tamanho() - 1; i >= 0; --i) {
        Pacote* p_original = getPacotePorId(pacotes_removidos_lifo[i].getId());
        bool vai_viajar = false;
        for (size_t k = 0; k < ids_para_transportar.tamanho(); ++k) {
            if (ids_para_transportar[k] == p_original->getId()) {
                vai_viajar = true;
                break;
            }
        }

        if (vai_viajar) {
            int latencia_da_rota = rota_transporte->getTempoTransportePorSegmento();
            p_original->addTempoTransporte(latencia_da_rota);

            imprimirEvento(tempo_final_op, p_original->getId(), "em transito de " + formatarNumero(origem, 3) + " para " + formatarNumero(destino, 3));
            int tempo_chegada = tempo_final_op + rota_transporte->getTempoTransportePorSegmento();
            fila_eventos.inserir(new EventoChegadaPacote(tempo_chegada, p_original->getId()));
        } else {
            imprimirEvento(tempo_final_op, p_original->getId(), "rearmazenado em " + formatarNumero(origem, 3) + " na secao " + formatarNumero(destino, 3));
            secao.empilhar(*p_original);
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
    for (size_t i = 0; i < pacotes.tamanho(); ++i) {
        if (pacotes[i].getId() == id) return &pacotes[i];
    }
    return nullptr;
}

Armazem* Escalonador::getArmazemPorId(int id) {
    if (id >= 0 && (size_t)id < armazens.tamanho()) {
        return &armazens[(size_t)id];
    }
    return nullptr;
}

Transporte* Escalonador::getTransportePorRota(int id_origem, int id_destino) {
    for (size_t i = 0; i < transportes.tamanho(); ++i) {
        if (transportes[i].getOrigemId() == id_origem && transportes[i].getDestinoId() == id_destino) {
            return &transportes[i];
        }
    }
    return nullptr; // Rota não encontrada
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