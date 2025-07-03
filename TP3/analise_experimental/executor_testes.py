# executor_testes.py
import subprocess
import time
import os
import csv
import re # Importa a biblioteca de expressões regulares

# --- CONFIGURAÇÕES DOS TESTES ---
BASE_CLIENTES = 50
BASE_PACOTES = 100
BASE_EVENTOS_P_PACOTE = 8
BASE_CONSULTAS = 100

CONFIGS_TESTE = []

# --- Cenário 1: Variando o Número de Pacotes ---
# Mantém clientes, complexidade e consultas fixas.
# Variação: 50 a 1000 pacotes.
for num_pacotes in [5, 25, 50, 100, 250, 450, 500, 750, 1000]:
    CONFIGS_TESTE.append((BASE_CLIENTES, num_pacotes, BASE_EVENTOS_P_PACOTE, BASE_CONSULTAS))

# --- Cenário 2: Variando a Complexidade (Eventos por Pacote) ---
# Mantém clientes, pacotes e consultas fixas.
# Variação: 2 a 16 eventos por pacote (crescimento geométrico).
for eventos_pp in [2, 4, 8, 16, 32, 64]:
    CONFIGS_TESTE.append((BASE_CLIENTES, BASE_PACOTES, eventos_pp, BASE_CONSULTAS))

# --- Cenário 3: Variando o Número de Clientes ---
# Mantém pacotes, complexidade e consultas fixas.
# Variação: 10 a 200 clientes.
for num_clientes in [5, 10, 25, 50, 75, 100, 150, 200]:
    num_pacotes_ajustado = max(num_clientes * 5, 100) # Garante pacotes suficientes
    CONFIGS_TESTE.append((num_clientes, num_pacotes_ajustado, BASE_EVENTOS_P_PACOTE, BASE_CONSULTAS))

# --- Cenário 4: Variando o Número de Consultas ---
# Mantém a base de dados (clientes, pacotes, eventos) fixa.
# Variação: 50 a 400 consultas.
for num_consultas in [25, 50, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400]:
    CONFIGS_TESTE.append((BASE_CLIENTES, BASE_PACOTES, BASE_EVENTOS_P_PACOTE, num_consultas))


EXECUTAVEL_CPP = os.path.join('..', 'bin', 'tp3.out')
ARQUIVO_RESULTADOS = 'resultados.csv'

def main():
    if not os.path.exists(EXECUTAVEL_CPP):
        print(f"ERRO: Executável '{EXECUTAVEL_CPP}' não encontrado.")
        return

    # =========== MUDANÇA AQUI: Novas colunas no CSV ===========
    with open(ARQUIVO_RESULTADOS, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['num_clientes', 'num_pacotes', 'eventos_por_pacote', 'num_consultas', 'total_linhas', 'tempo_leitura_us', 'tempo_processamento_us'])

    for i, config in enumerate(CONFIGS_TESTE):
        # ... (lógica de geração de arquivo, sem alterações) ...
        num_c, num_p, ev_p_p, num_q = config
        print(f"\n--- Teste {i+1}/{len(CONFIGS_TESTE)}: Clientes={num_c}, Pacotes={num_p}, Eventos/Pacote={ev_p_p} ---")
        nome_arquivo_entrada = f'entrada_{num_c}c_{num_p}p_{ev_p_p}e.txt'
        print(f"Gerando arquivo '{nome_arquivo_entrada}'...")
        subprocess.run(['python', 'gerador_entradas.py', str(num_c), str(num_p), str(ev_p_p), str(num_q), nome_arquivo_entrada], check=True)
        with open(nome_arquivo_entrada) as f:
            total_linhas = sum(1 for _ in f)
        print(f"Executando {EXECUTAVEL_CPP}...")

        with open(nome_arquivo_entrada, 'r') as f_in:
            resultado = subprocess.run([EXECUTAVEL_CPP], stdin=f_in, capture_output=True, text=True, check=True)
            
        # =========== MUDANÇA AQUI: Expressão regular para buscar "US" ===========
        tempos = re.search(r"TEMPO_LEITURA_US:(\d+);TEMPO_PROCESSAMENTO_US:(\d+)", resultado.stderr)
        if tempos:
            tempo_leitura = int(tempos.group(1))
            tempo_processamento = int(tempos.group(2))
            print(f"Execução concluída. Leitura: {tempo_leitura} µs, Processamento: {tempo_processamento} µs.")
        else:
            tempo_leitura, tempo_processamento = -1, -1
            print("AVISO: Não foi possível extrair os tempos do stderr.")

        # Salva os novos dados
        with open(ARQUIVO_RESULTADOS, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([num_c, num_p, ev_p_p, num_q, total_linhas, tempo_leitura, tempo_processamento])
            
        os.remove(nome_arquivo_entrada)

    print(f"\nAnálise experimental concluída! Resultados salvos em '{ARQUIVO_RESULTADOS}'.")

if __name__ == '__main__':
    main()