# gerador_entradas.py
import random
import sys

def gerar_nomes_clientes(num_clientes):
    """Gera uma lista de nomes de clientes únicos."""
    return [f"Cliente_{i:04d}" for i in range(num_clientes)]

def gerar_entradas(num_clientes, num_pacotes, eventos_por_pacote, num_consultas):
    """Gera uma lista de linhas de evento e consulta."""
    clientes = gerar_nomes_clientes(num_clientes)
    num_armazens = max(10, num_clientes // 100) # Escala o número de armazéns
    
    timestamp = 0
    eventos = []
    
    # Passo 1: Gerar os pacotes (eventos RG)
    pacotes_criados = []
    for i in range(num_pacotes):
        timestamp += random.randint(1, 5)
        remetente, destinatario = random.sample(clientes, 2)
        armazem_orig = random.randint(0, num_armazens)
        armazem_dest = random.randint(0, num_armazens)
        while armazem_dest == armazem_orig:
            armazem_dest = random.randint(0, num_armazens)
            
        pacote_id = i
        pacotes_criados.append(pacote_id)
        eventos.append(f"{timestamp:07d} EV RG {pacote_id:03d} {remetente} {destinatario} {armazem_orig:03d} {armazem_dest:03d}")

    # Passo 2: Gerar eventos intermediários para cada pacote
    tipos_evento_meio = ["AR", "RM", "TR", "UR"]
    for pacote_id in pacotes_criados:
        armazem_atual = random.randint(0, num_armazens)
        for _ in range(eventos_por_pacote):
            timestamp += random.randint(10, 50)
            tipo = random.choice(tipos_evento_meio)
            secao = random.randint(0, 9)
            
            if tipo == "TR":
                armazem_novo = random.randint(0, num_armazens)
                while armazem_novo == armazem_atual:
                    armazem_novo = random.randint(0, num_armazens)
                eventos.append(f"{timestamp:07d} EV {tipo} {pacote_id:03d} {armazem_atual:03d} {armazem_novo:03d}")
                armazem_atual = armazem_novo
            else:
                eventos.append(f"{timestamp:07d} EV {tipo} {pacote_id:03d} {armazem_atual:03d} {secao:03d}")

        # Gera um evento final de entrega
        timestamp += random.randint(10, 50)
        eventos.append(f"{timestamp:07d} EV EN {pacote_id:03d} {armazem_atual:03d}")

    # Passo 3: Inserir consultas aleatórias
    for _ in range(num_consultas):
        timestamp += random.randint(1, 10)
        if random.random() > 0.5 and pacotes_criados:
            # Consulta PC
            pacote_consulta = random.choice(pacotes_criados)
            eventos.append(f"{timestamp:07d} PC {pacote_consulta:03d}")
        elif clientes:
            # Consulta CL
            cliente_consulta = random.choice(clientes)
            eventos.append(f"{timestamp:07d} CL {cliente_consulta}")
            
    # Passo 4: Ordenar tudo por timestamp e retornar
    # O primeiro token de cada linha é o timestamp
    eventos.sort(key=lambda x: int(x.split()[0]))
    return eventos

if __name__ == "__main__":
    if len(sys.argv) != 6:
        print("Uso: python gerador_entradas.py <num_clientes> <num_pacotes> <eventos_por_pacote> <num_consultas> <arquivo_saida>")
        sys.exit(1)
        
    num_c = int(sys.argv[1])
    num_p = int(sys.argv[2])
    ev_p_p = int(sys.argv[3])
    num_q = int(sys.argv[4])
    output_file = sys.argv[5]
    
    linhas = gerar_entradas(num_c, num_p, ev_p_p, num_q)
    
    with open(output_file, 'w') as f:
        for linha in linhas:
            f.write(linha + "\n")
            
    print(f"Arquivo de entrada '{output_file}' gerado com {len(linhas)} linhas.")