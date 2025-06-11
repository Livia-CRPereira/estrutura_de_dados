#include "escalonador.h"

// Função auxiliar para formatar números
inline std::string formatarNumero(int numero, int largura) {
    std::stringstream ss;
    ss << std::setw(largura) << std::setfill('0') << numero;
    return ss.str();
}

// --- Implementações da Classe Escalonador ---

Escalonador::Escalonador() : tempo_sistema(0), pacotes_entregues(0) {}
Escalonador::~Escalonador() {
    while (!fila_eventos.vazia()) {
        delete fila_eventos.removerMinimo();
    }
}

void Escalonador::carregarConfiguracao(int interv, int custo, int num_a) {
    intervalo_transportes = interv;
    custo_remocao = custo;

    for (int i = 0; i < num_a; ++i) {
        Armazem a; a.setId(i);
        armazens.adicionar(a);
        rede_armazens.adicionarVertice(i);
    }
}

void Escalonador::adicionarConexao(int u, int v) { rede_armazens.adicionarAresta(u, v, 1); }
void Escalonador::adicionarPacote(const Pacote& p) { pacotes.adicionar(p); }
void Escalonador::adicionarTransporte(const Transporte& t) { transportes.adicionar(t); }

// -- Simulação --
void Escalonador::prepararSimulacao() {
    inicializarSecoesDosArmazens();
    for (size_t i = 0; i < pacotes.tamanho(); ++i) calcularRotaParaPacote(pacotes[i]);

    this->min_postagem_global = -1;
    if (pacotes.tamanho() > 0) {
        this->min_postagem_global = pacotes[0].getTempoPostagem();
        for (size_t i = 1; i < pacotes.tamanho(); ++i) {
            if (pacotes[i].getTempoPostagem() < this->min_postagem_global) {
                this->min_postagem_global = pacotes[i].getTempoPostagem();
            }
        }
    }

    for (size_t i = 0; i < pacotes.tamanho(); ++i) fila_eventos.inserir(new EventoEntregaPacote(pacotes[i].getTempoPostagem(), pacotes[i].getId()));
    for (int i = 0; i < rede_armazens.quantidadeVertices(); ++i) {
        const Vetor<Aresta<int>>& vizinhos = rede_armazens.vizinhos(i);
        for (size_t j = 0; j < vizinhos.tamanho(); ++j) {
            fila_eventos.inserir(new EventoTransporte(intervalo_transportes, i, vizinhos[j].idx_destino));
        }
    }
}

void Escalonador::executar() {
    while (pacotes_entregues < pacotes.tamanho()) {
        if (fila_eventos.vazia()){ // Verificação de segurança
            break; 
        }
        int tempo_atual_onda = fila_eventos.verMinimo()->tempo;
        Vetor<Evento*> onda_de_eventos;
        while (!fila_eventos.vazia() && fila_eventos.verMinimo()->tempo == tempo_atual_onda) {
            onda_de_eventos.adicionar(fila_eventos.removerMinimo());
        }

        Vetor<Evento*> onda_pacotes;
        Vetor<Evento*> onda_transportes;
        for (size_t i = 0; i < onda_de_eventos.tamanho(); ++i) {
            if (onda_de_eventos[i]->tipo == EVENTO_PACOTE) onda_pacotes.adicionar(onda_de_eventos[i]);
            else onda_transportes.adicionar(onda_de_eventos[i]);
        }
        
        std::sort(onda_pacotes.begin(), onda_pacotes.end(), [](Evento* a, Evento* b){
            return static_cast<EventoEntregaPacote*>(a)->id_pacote < static_cast<EventoEntregaPacote*>(b)->id_pacote;
        });

        for (size_t i = 0; i < onda_pacotes.tamanho(); ++i) {
            tempo_sistema = onda_pacotes[i]->tempo;
            processarEventoEntrega(static_cast<EventoEntregaPacote*>(onda_pacotes[i]));
            delete onda_pacotes[i];
        }

        if (onda_transportes.tamanho() > 0) {
            tempo_sistema = onda_transportes[0]->tempo;
            processarOndaDeTransporte(onda_transportes);
        }
    }
}

void Escalonador::processarOndaDeTransporte(Vetor<Evento*>& onda) {

    int tempo_base_operacao = onda[0]->tempo + custo_remocao + this->min_postagem_global;

    std::sort(onda.begin(), onda.end(), [](Evento* a, Evento* b) {
        EventoTransporte* et_a = static_cast<EventoTransporte*>(a);
        EventoTransporte* et_b = static_cast<EventoTransporte*>(b);
        if (et_a->id_origem != et_b->id_origem) return et_a->id_origem < et_b->id_origem;
        return et_a->id_destino < et_b->id_destino;
    });

    struct OperacaoTransporte {
        int origem, destino;
        Vetor<Pacote> pacotes_removidos;
    };
    Vetor<OperacaoTransporte> operacoes;

    for (size_t i = 0; i < onda.tamanho(); ++i) {
        EventoTransporte* et = static_cast<EventoTransporte*>(onda[i]);
        Armazem* armazem = getArmazemPorId(et->id_origem);
        if (armazem && !armazem->getSecaoRef(et->id_destino).vazia()) {
            OperacaoTransporte op = {et->id_origem, et->id_destino};
            Pilha<Pacote>& secao = armazem->getSecaoRef(et->id_destino);
            while(!secao.vazia()) op.pacotes_removidos.adicionar(secao.desempilhar());
            operacoes.adicionar(op);
        }
        fila_eventos.inserir(new EventoTransporte(onda[i]->tempo + intervalo_transportes, et->id_origem, et->id_destino));
        delete onda[i];
    }
    
    for (size_t i = 0; i < operacoes.tamanho(); ++i) {
        OperacaoTransporte& op = operacoes[i];
        if (op.pacotes_removidos.tamanho() == 0) continue;


        Transporte* rota_especifica = getTransportePorRota(op.origem, op.destino);
        if (!rota_especifica) {
            Armazem* armazem = getArmazemPorId(op.origem);
            for(int j = op.pacotes_removidos.tamanho() - 1; j >= 0; --j) {
                armazem->getSecaoRef(op.destino).empilhar(op.pacotes_removidos[j]);
            }
            continue; 
        }

        size_t capacidade_da_rota = rota_especifica->getCapacidadeMaxPacotes();
        int latencia_da_rota = rota_especifica->getTempoTransportePorSegmento();

        Vetor<Pacote*> ptrs_pacotes_originais;
        for(size_t k = 0; k < op.pacotes_removidos.tamanho(); ++k){
            ptrs_pacotes_originais.adicionar(getPacotePorId(op.pacotes_removidos[k].getId()));
        }

        std::sort(ptrs_pacotes_originais.begin(), ptrs_pacotes_originais.end(), 
            [this](const Pacote* a, const Pacote* b){ 
                // 1. Prioridade principal: maior tempo de espera na seção atual
                int espera_a = this->tempo_sistema - a->getTempoChegadaAtual();
                int espera_b = this->tempo_sistema - b->getTempoChegadaAtual();
                
                if (espera_a != espera_b) {
                    return espera_a > espera_b;
                }
                
                // 2. Desempate: pacote mais antigo por postagem
                return a->getTempoPostagem() < b->getTempoPostagem();
            });

        Vetor<int> ids_que_viajam;
        for (size_t k = 0; k < ptrs_pacotes_originais.tamanho() && k < capacidade_da_rota; ++k) {
            ids_que_viajam.adicionar(ptrs_pacotes_originais[k]->getId());
        }

        Vetor<Pacote> a_serem_transportados;
        Vetor<Pacote> a_serem_rearmazenados;
        int tempo_final_op = 0;

        for (size_t j = 0; j < op.pacotes_removidos.tamanho(); ++j) {
            Pacote* p_original = getPacotePorId(op.pacotes_removidos[j].getId());

            int tempo_operacao = tempo_base_operacao + (j * custo_remocao);
            imprimirEvento(tempo_operacao, p_original->getId(), "removido de " + formatarNumero(op.origem, 3) + " na secao " + formatarNumero(op.destino, 3));
   
            p_original->addTempoEspera(custo_remocao * (j + 1));
            p_original->addTempoTotal(custo_remocao * (j + 1));
            
            if (j == op.pacotes_removidos.tamanho() - 1) {
                tempo_final_op = tempo_operacao;
            }

            bool vai_viajar = false;
            for(size_t k = 0; k < ids_que_viajam.tamanho(); ++k) if(ids_que_viajam[k] == p_original->getId()) vai_viajar = true;
            
            if (vai_viajar) a_serem_transportados.adicionar(*p_original);
            else a_serem_rearmazenados.adicionar(*p_original);
        }
        

        if (op.pacotes_removidos.tamanho() <= capacidade_da_rota) {
             std::sort(a_serem_transportados.begin(), a_serem_transportados.end(), 
                [](const Pacote& a, const Pacote& b){ return a.getTempoPostagem() < b.getTempoPostagem(); });
        } else {
             std::sort(a_serem_transportados.begin(), a_serem_transportados.end(), 
                [](const Pacote& a, const Pacote& b){ return a.getTempoPostagem() > b.getTempoPostagem(); });
        }

        for(size_t j = 0; j < a_serem_transportados.tamanho(); ++j) {
            Pacote* p = getPacotePorId(a_serem_transportados[j].getId());


            p->addTempoTransporte(latencia_da_rota);
            p->addTempoTotal(latencia_da_rota);

            imprimirEvento(tempo_final_op, p->getId(), "em transito de " + formatarNumero(op.origem, 3) + " para " + formatarNumero(op.destino, 3));
            

            fila_eventos.inserir(new EventoEntregaPacote(tempo_final_op + latencia_da_rota, p->getId()));
        }
        
        for(int j = a_serem_rearmazenados.tamanho() - 1; j >= 0; --j) {
            Pacote* p = getPacotePorId(a_serem_rearmazenados[j].getId());
            imprimirEvento(tempo_final_op, p->getId(), "rearmazenado em " + formatarNumero(op.origem, 3) + " na secao " + formatarNumero(op.destino, 3));
            getArmazemPorId(op.origem)->getSecaoRef(op.destino).empilhar(*p);
        }
    }
}

void Escalonador::processarEventoEntrega(EventoEntregaPacote* evento) {
    Pacote* pacote = getPacotePorId(evento->id_pacote);
    if (!pacote) return;

    pacote->setTempoChegadaAtual(tempo_sistema);

    if (pacote->getEstado() == NAO_FOI_POSTADO) {
        pacote->setEstado(ARMAZENADO_EM_ARMAZEM);
    } else {
        pacote->avancarNaRota();
    }
    
    Armazem* armazem_atual = getArmazemPorId(pacote->getArmazemAtual());

    if (pacote->getArmazemAtual() == pacote->getArmazemDestino()) {
        if (pacote->getEstado() != ENTREGUE) {
            pacotes_entregues++;
        }
        pacote->setEstado(ENTREGUE);
        imprimirEvento(tempo_sistema, pacote->getId(), "entregue em " + formatarNumero(armazem_atual->getId(), 3));
    } else {
        int proximo_armazem_id = pacote->getProximoArmazem();
        armazem_atual->armazenaPacote(*pacote, proximo_armazem_id);
        imprimirEvento(tempo_sistema, pacote->getId(), "armazenado em " + formatarNumero(armazem_atual->getId(), 3) + " na secao " + formatarNumero(proximo_armazem_id, 3));
    }
}

// --- Funções Auxiliares (getters, etc.) ---
void Escalonador::imprimirEvento(int tempo, int id_pacote, const std::string& msg) {
    std::cout << formatarNumero(tempo, 7) << " pacote " << formatarNumero(id_pacote, 3) << " " << msg << std::endl;
}

void Escalonador::inicializarSecoesDosArmazens() {
    for (int i = 0; i < rede_armazens.quantidadeVertices(); ++i) {
        Armazem* armazem_atual = getArmazemPorId(i);
        const Vetor<Aresta<int>>& vizinhos = rede_armazens.vizinhos(i);
        for (size_t j = 0; j < vizinhos.tamanho(); ++j) {
            armazem_atual->adicionarVizinhoESecao(vizinhos[j].idx_destino);
        }
    }
}

Pacote* Escalonador::getPacotePorId(int id) {
    for (size_t i = 0; i < pacotes.tamanho(); ++i) {
        if (pacotes[i].getId() == id) return &pacotes[i];
    }
    return nullptr;
}

Armazem* Escalonador::getArmazemPorId(int id) {
    if (id >= 0 && static_cast<size_t>(id) < armazens.tamanho()) {
        return &armazens[static_cast<size_t>(id)];
    }
    return nullptr;
}

Transporte* Escalonador::getTransportePorRota(int id_origem, int id_destino) {
    for (size_t i = 0; i < transportes.tamanho(); ++i) {
        if (transportes[i].getOrigemId() == id_origem && transportes[i].getDestinoId() == id_destino) {
            return &transportes[i];
        }
    }
    return nullptr; // Retorna nulo se a rota de transporte não foi definida
}

void Escalonador::calcularRotaParaPacote(Pacote& pacote) {
    int origem = pacote.getArmazemOrigem();
    int destino = pacote.getArmazemDestino();
    int n = rede_armazens.quantidadeVertices();
    if (origem == destino) {
        ListaEncadeada<int> r; r.adicionar(origem); pacote.setRota(r); return;
    }
    ListaEncadeada<int> fila_bfs;
    Vetor<bool> visitado(n, false);
    Vetor<int> pred(n, -1);
    fila_bfs.adicionar(origem);
    visitado[origem] = true;
    bool achou = false;
    while (!fila_bfs.vazia()) {
        int u = fila_bfs.removerPrimeiro();
        if (u == destino) { achou = true; break; }
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
    ListaEncadeada<int> rota_final;
    if (achou) {
        for (int at = destino; at != -1; at = pred[at]) {
            rota_final.adicionarNoInicio(at);
        }
    }
    pacote.setRota(rota_final);
}