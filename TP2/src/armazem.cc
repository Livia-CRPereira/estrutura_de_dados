#include "armazem.h"

Armazem::Armazem() : id(-1), nome(""), pilha_de_pacotes() {
}

Armazem(int armazem_id, std::string armazem_nome, int quant_pacotes){
    id = armazem_id;
    nome = armazem_nome;
    pilha_de_pacotes();
}

Armazem::~Armazem() {
}

int Armazem::getId() {
    return id;
}

std::string Armazem::getNome() {
    return nome;
}

void Armazem::armazenaPacote(Pacote pacote) {
    pilha_de_pacotes.empilhar(pacote);
}

Pacote Armazem::recuperaPacote() {
    if (pilha_de_pacotes.vazia()) {
        throw std::runtime_error("Pilha de pacotes vazia no armazém. Não foi possível recuperar.");
    }
    return pilha_de_pacotes.desempilhar();
}

bool Armazem::isSecaoVazia(int proximo_armazem_destino_id) const {
    throw std::runtime_error("isSecaoVazia nao implementado para uma unica pilha. Armazem precisa de secoes por destino.");
    return true;
}

bool Armazem::todasSecoesVazias() const {
    return pilha_de_pacotes.vazia();
}

int Armazem::getQuantidadePacotesNaSecao(int proximo_armazem_destino_id) const {
    throw std::runtime_error("getQuantidadePacotesNaSecao nao implementado para uma unica pilha. Armazem precisa de secoes por destino.");
    return 0; // Placeholder
}
