# testes_transporte.py
import os
import random

# --- PARÂMETROS FIXOS DA RODADA EXPERIMENTAL ---
A = 50      # Número de Armazéns
P = 1000    # Número de Pacotes
CUSTO_REMOCAO = 1

# --- PARÂMETROS VARIÁVEIS ---
# Listas de valores que vamos testar para cada parâmetro
CAPACIDADES = [5, 10, 25, 50, 100, 1000]
INTERVALOS = [10, 25, 50, 100, 200, 500]
LATENCIAS = [10, 25, 50, 100, 200, 500]

# --- PARÂMETROS DE CONTROLE PARA OS EXPERIMENTOS ---
# Para o experimento de Capacidade vs. Intervalo, a latência será fixa.
LATENCIA_FIXA_EXP1 = 25
# Para o experimento de Latência vs. Intervalo, a capacidade será fixa.
CAPACIDADE_FIXA_EXP2 = 50

# Diretório para salvar os testes
TEST_DIR = "cenarios_de_teste"
os.makedirs(TEST_DIR, exist_ok=True)

def gerar_grafo_conectado(num_vertices, densidade=0.1):
    """Gera uma matriz de adjacência para um grafo conectado e uma lista de arestas."""
    matriz = [[0] * num_vertices for _ in range(num_vertices)]
    arestas = []
    vertices_conectados = {0}
    
    # Garante conectividade usando uma abordagem similar à de Prim/Kruskal
    while len(vertices_conectados) < num_vertices:
        u = random.choice(list(vertices_conectados))
        v = random.choice([i for i in range(num_vertices) if i not in vertices_conectados])
        matriz[u][v] = 1
        matriz[v][u] = 1
        arestas.append(tuple(sorted((u, v))))
        vertices_conectados.add(v)

    # Adiciona arestas aleatórias com base na densidade
    max_arestas_adicionais = int((num_vertices * (num_vertices - 1) / 2) * densidade)
    arestas_adicionadas = 0
    while arestas_adicionadas < max_arestas_adicionais:
        u, v = random.randint(0, num_vertices-1), random.randint(0, num_vertices-1)
        if u != v and matriz[u][v] == 0:
            matriz[u][v] = 1
            matriz[v][u] = 1
            arestas.append(tuple(sorted((u, v))))
            arestas_adicionadas += 1
            
    return matriz, sorted(list(set(arestas)))

def gerar_arquivo_cenario(filename, intervalo, capacidade, latencia, matriz_adj, pacotes, num_rotas, arestas):
    """
    Função auxiliar que escreve um único arquivo de cenário com os parâmetros fornecidos.
    """
    with open(filename, 'w') as f:
        # 1. Parâmetros Globais
        f.write(f"{intervalo}\n")
        f.write(f"{CUSTO_REMOCAO}\n")
        
        # 2. Rede
        f.write(f"{A}\n")
        for i in range(A):
            f.write(" ".join(map(str, matriz_adj[i])) + "\n")
        
        # 3. Pacotes
        f.write(f"{P}\n")
        for pacote_str in pacotes:
            f.write(pacote_str + "\n")
        
        # 4. Rotas de Transporte
        f.write(f"{num_rotas}\n")
        for u, v in arestas:
            # Adiciona rota de ida e de volta com os parâmetros específicos do cenário
            f.write(f"{u} {v} {capacidade} {latencia}\n")
            f.write(f"{v} {u} {capacidade} {latencia}\n")

def gerar_todos_testes():
    """
    Função principal que gera todos os arquivos de teste para os experimentos.
    """
    print("Iniciando geração de casos de teste...")
    
    # A topologia e os pacotes são gerados uma única vez para garantir consistência
    # entre todos os cenários de um mesmo lote de experimentos.
    matriz_adj, arestas = gerar_grafo_conectado(A)
    num_rotas = len(arestas) * 2  # Rotas de ida e volta
    
    pacotes = []
    for i in range(P):
        origem = random.randint(0, A - 1)
        destino = random.randint(0, A - 1)
        while origem == destino:
            destino = random.randint(0, A - 1)
        tempo_postagem = random.randint(1, 500)
        pacotes.append(f"{tempo_postagem} pac {i} org {origem} dst {destino}")

    # --- Experimento 1: Análise de Capacidade vs. Intervalo ---
    print(f"\nGerando cenários para Capacidade vs. Intervalo (Latência fixa em {LATENCIA_FIXA_EXP1})...")
    count_exp1 = 0
    for cap in CAPACIDADES:
        for interv in INTERVALOS:
            filename = os.path.join(TEST_DIR, f"cenario_C{cap}_I{interv}.txt")
            gerar_arquivo_cenario(filename, interv, cap, LATENCIA_FIXA_EXP1, matriz_adj, pacotes, num_rotas, arestas)
            count_exp1 += 1
    print(f"{count_exp1} arquivos gerados para o Experimento 1.")

    # --- Experimento 2: Análise de Latência vs. Intervalo ---
    print(f"\nGerando cenários para Latência vs. Intervalo (Capacidade fixa em {CAPACIDADE_FIXA_EXP2})...")
    count_exp2 = 0
    for lat in LATENCIAS:
        for interv in INTERVALOS:
            filename = os.path.join(TEST_DIR, f"cenario_L{lat}_I{interv}.txt")
            gerar_arquivo_cenario(filename, interv, CAPACIDADE_FIXA_EXP2, lat, matriz_adj, pacotes, num_rotas, arestas)
            count_exp2 += 1
    print(f"{count_exp2} arquivos gerados para o Experimento 2.")
    print(f"\nTotal de {count_exp1 + count_exp2} arquivos de teste gerados no diretório '{TEST_DIR}/'")

if __name__ == "__main__":
    gerar_todos_testes()