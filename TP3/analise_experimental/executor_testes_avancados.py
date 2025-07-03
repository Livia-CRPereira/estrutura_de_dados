# executor_testes_avancados.py
import subprocess
import os
import csv
import re
import itertools # Biblioteca para criar as combinações facilmente

# --- CONFIGURAÇÕES DA GRADE DE PARÂMETROS ---
pacotes_lista = [25, 50, 100, 150, 200]
clientes_lista = [10, 40, 70, 100, 130]
eventos_pp_lista = [1, 4, 9, 16, 25]
consultas_lista = [25, 50, 100, 150, 200]

# Gera todas as combinações possíveis
configs_completas = list(itertools.product(pacotes_lista, clientes_lista, eventos_pp_lista, consultas_lista))
total_testes = len(configs_completas)

EXECUTAVEL_CPP = os.path.join('..', 'bin', 'tp3.out')
ARQUIVO_RESULTADOS = 'resultados_avancados.csv'

def main():
    if not os.path.exists(EXECUTAVEL_CPP):
        print(f"ERRO: Executável '{EXECUTAVEL_CPP}' não encontrado.")
        return

    # Prepara o arquivo CSV para os resultados
    with open(ARQUIVO_RESULTADOS, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['num_pacotes', 'num_clientes', 'eventos_por_pacote', 'num_consultas', 'total_linhas', 'tempo_leitura_us', 'tempo_processamento_us'])

    print(f"Iniciando análise com {total_testes} combinações de parâmetros. Isso pode levar um tempo considerável...")
    
    # Executa os testes para cada configuração
    for i, config in enumerate(configs_completas):
        num_p, num_c, ev_p_p, num_q = config
        
        # Imprime o progresso
        print(f"\n--- Teste {i+1}/{total_testes}: P={num_p}, C={num_c}, E/P={ev_p_p}, Q={num_q} ---")
        
        nome_arquivo_entrada = f'entrada_temp.txt'
        
        subprocess.run([
            'python', 'gerador_entradas.py',
            str(num_c), str(num_p), str(ev_p_p), str(num_q),
            nome_arquivo_entrada
        ], check=True, capture_output=True, text=True)

        with open(nome_arquivo_entrada) as f:
            total_linhas = sum(1 for _ in f)

        with open(nome_arquivo_entrada, 'r') as f_in:
            resultado = subprocess.run([EXECUTAVEL_CPP], stdin=f_in, capture_output=True, text=True, check=True)
            
        tempos = re.search(r"TEMPO_LEITURA_US:(\d+);TEMPO_PROCESSAMENTO_US:(\d+)", resultado.stderr)
        if tempos:
            tempo_leitura = int(tempos.group(1))
            tempo_processamento = int(tempos.group(2))
        else:
            tempo_leitura, tempo_processamento = -1, -1

        with open(ARQUIVO_RESULTADOS, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([num_p, num_c, ev_p_p, num_q, total_linhas, tempo_leitura, tempo_processamento])
            
    os.remove(nome_arquivo_entrada)
    print(f"\nAnálise experimental avançada concluída! Resultados salvos em '{ARQUIVO_RESULTADOS}'.")

if __name__ == '__main__':
    main()