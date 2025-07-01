#include "organizador_eventos.h"
#include <stdexcept>
#include <iostream> 

void deletarVetorPacotes(Vetor<int>* vetor) {
    if (vetor) {
        delete vetor;
    }
}

OrganizadorEventos::OrganizadorEventos() {}

OrganizadorEventos::~OrganizadorEventos() {
    indiceClientes.limparValores(deletarVetorPacotes);
}

// Getters
const Vetor<Evento>& OrganizadorEventos::getBaseEventos() const { return baseDeEventos; }
const HashMap<std::string, Vetor<int>*>& OrganizadorEventos::getIndiceClientes() const { return indiceClientes; }
const HashMap<int, int>& OrganizadorEventos::getIndiceEstadoAtual() const { return indiceEstadoAtual; }
const ArvoreAVL<long long, int>& OrganizadorEventos::getIndicePacoteTempo() const { return indicePacoteTempo; }

// Processamento Principal de Eventos 
void OrganizadorEventos::processarEvento(const Vetor<std::string>& tokens) {
    // std::cout << "\n--- Processando Evento: " << tokens[0] << " " << tokens[1] << " " << tokens[2] << " " << tokens[3] << " ---" << std::endl;
    
    Evento novoEvento;
    novoEvento.dataHora = extrairInt(tokens[0]);
    novoEvento.idPacote = extrairInt(tokens[3]);
    novoEvento.tipo = stringParaTipoEvento(tokens[2]);

    switch (novoEvento.tipo) {
        case RG:
            novoEvento.remetente = tokens[4];
            novoEvento.destinatario = tokens[5];
            novoEvento.armazemOrigem = extrairInt(tokens[6]);
            novoEvento.armazemDestino = extrairInt(tokens[7]);
            break;
        case AR: case RM: case UR:
            novoEvento.armazemDestino = extrairInt(tokens[4]);
            novoEvento.secaoDestino = extrairInt(tokens[5]);
            break;
        case TR:
            novoEvento.armazemOrigem = extrairInt(tokens[4]);
            novoEvento.armazemDestino = extrairInt(tokens[5]);
            break;
        case EN:
            novoEvento.armazemDestino = extrairInt(tokens[4]);
            break;
        case INDEFINIDO:
            std::cout << "[AVISO] Evento com tipo indefinido." << std::endl;
            return;
    }

    baseDeEventos.adicionar(novoEvento);
    int indice_na_base = baseDeEventos.tamanho() - 1;
    // std::cout << "[DEBUG Organizador] Evento adicionado na base. Indice: " << indice_na_base << std::endl;

    long long chave_composta = (long long)novoEvento.idPacote * 10000000000LL + ((long long)novoEvento.dataHora * 100000) + indice_na_base;
    indicePacoteTempo.inserir(chave_composta, indice_na_base);
    // std::cout << "[DEBUG Organizador] IndicePacoteTempo atualizado. Chave: " << chave_composta << std::endl;

    indiceEstadoAtual.inserir(novoEvento.idPacote, indice_na_base);
    // std::cout << "[DEBUG Organizador] IndiceEstadoAtual atualizado para pacote " << novoEvento.idPacote << ". Aponta para evento no indice " << indice_na_base << std::endl;

    if (novoEvento.tipo == RG) {
        // std::cout << "[DEBUG Organizador] Evento eh RG. Atualizando indice de clientes para '" 
        //          << novoEvento.remetente << "' e '" << novoEvento.destinatario << "'." << std::endl;
        
        Vetor<std::string> clientes_envolvidos;
        clientes_envolvidos.adicionar(novoEvento.remetente);
        clientes_envolvidos.adicionar(novoEvento.destinatario);

        for (size_t i = 0; i < clientes_envolvidos.tamanho(); ++i) {
            const std::string& nome_cliente = clientes_envolvidos[i];
            if (nome_cliente.empty()) continue;

            // std::cout << "  -> Processando para cliente: " << nome_cliente << std::endl;
            Vetor<int>** ptr_vetor_pacotes = indiceClientes.buscar(nome_cliente);

            if (ptr_vetor_pacotes == nullptr) {
                // std::cout << "  -> Cliente novo. Criando novo vetor de pacotes." << std::endl;
                Vetor<int>* novo_vetor = new Vetor<int>();
                novo_vetor->adicionar(novoEvento.idPacote);
                indiceClientes.inserir(nome_cliente, novo_vetor);
            } else {
                // std::cout << "  -> Cliente existente. Adicionando pacote ao vetor." << std::endl;
                (*ptr_vetor_pacotes)->adicionar(novoEvento.idPacote);
            }
        }
    }
}


// --- Funções Auxiliares ---
TipoEvento OrganizadorEventos::stringParaTipoEvento(const std::string& s) {
    if (s == "RG") return RG;
    if (s == "AR") return AR;
    if (s == "RM") return RM;
    if (s == "UR") return UR;
    if (s == "TR") return TR;
    if (s == "EN") return EN;
    return INDEFINIDO;
}

int OrganizadorEventos::extrairInt(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return -1;
    }
}