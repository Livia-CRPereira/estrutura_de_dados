import os
import random
import networkx as nx

def gerar_grafo_conectado(num_armazens, num_rotas):
    """Gera um grafo aleatório e garante que ele seja conectado."""
    while True:
        # Garante R >= A-1 para conectividade
        rotas_a_gerar = max(num_rotas, num_armazens - 1)
        g = nx.gnm_random_graph(num_armazens, rotas_a_gerar)
        if nx.is_connected(g):
            return list(g.edges())

def criar_caso_de_teste(caminho_arquivo, A, R, P):
    """Cria um único arquivo de caso de teste."""
    print(f"Gerando: {os.path.basename(caminho_arquivo)} (A={A}, P={P})")
    
    # Parâmetros fixos da simulação
    intervalo_transportes = 100
    custo_remocao = 1
    capacidade_base = 5

    # Gera a topologia da rede
    arestas = gerar_grafo_conectado(A, R)
    num_rotas_real = len(arestas)

    with open(caminho_arquivo, 'w') as f:
        # Escreve cabeçalho da simulação
        f.write(f"{intervalo_transportes} {custo_remocao}\n")
        f.write(f"{A} {num_rotas_real * 2}\n") # Rotas são bidirecionais

        # Escreve as rotas com latência variável
        for u, v in arestas:
            # Latência aleatória entre 10 e 50
            latencia_uv = random.randint(10, 50)
            latencia_vu = random.randint(10, 50)
            f.write(f"{u} {v} {capacidade_base} {latencia_uv}\n")
            f.write(f"{v} {u} {capacidade_base} {latencia_vu}\n")

        # Escreve os pacotes
        f.write(f"{P}\n")
        for i in range(P):
            tempo_postagem = random.randint(1, P * 5)
            origem = random.randint(0, A - 1)
            # Garante que o destino seja diferente da origem
            destino = random.choice([j for j in range(A) if j != origem])
            f.write(f"{tempo_postagem} pac {i+1} org {origem} dst {destino}\n")

if __name__ == '__main__':
    PASTA_TESTES = 'casos_de_teste'
    if not os.path.exists(PASTA_TESTES):
        os.makedirs(PASTA_TESTES)
    
    # =================================================================
    # CAMPANHA 1: Teste Fatorial para Heatmap (A vs P) - MANTENHA COMO ESTÁ
    # =================================================================
    print("--- Gerando Casos de Teste Fatoriais (A vs P) ---")
    valores_P_fatorial = [100, 500, 1000, 2500, 5000]
    valores_A_fatorial = [10, 25, 50, 100]

    for A in valores_A_fatorial:
        for P in valores_P_fatorial:
            R = 2 * A # Mantém a densidade da rede constante em 2
            nome_arquivo = f"teste_fatorial_A{A}_R{R}_P{P}.txt"
            caminho_completo = os.path.join(PASTA_TESTES, nome_arquivo)
            criar_caso_de_teste(caminho_completo, A, R, P)
            
    
    print("\n[SUCESSO] Geração de todos os casos de teste concluída!")