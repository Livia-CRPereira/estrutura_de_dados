# analisador_transportes.py
import os
import time
import subprocess
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# --- PARÂMETROS DOS EXPERIMENTOS ---
# As listas devem ser idênticas às do script de geração
CAPACIDADES = [5, 10, 25, 50, 100, 1000]
LATENCIAS = [10, 25, 50, 100, 200, 500]
INTERVALOS = [10, 25, 50, 100, 200, 500]

# --- CONFIGURAÇÕES DE EXECUÇÃO ---
TEST_DIR = "cenarios_de_teste"  # Deve ser o mesmo diretório da geração
EXECUTABLE = "../bin/tp2.out"  # Caminho para o seu executável C++

def analisar_experimento(params_dict, var1_name, var1_prefix, var2_name, var2_prefix):
    """
    Função genérica para rodar a simulação, coletar métricas e retornar um DataFrame.
    :param params_dict: Dicionário contendo as listas de valores para cada variável.
    :param var1_name: Nome da variável do eixo X (ex: "Capacidade").
    :param var1_prefix: Prefixo usado no nome do arquivo para a variável 1 (ex: "C").
    :param var2_name: Nome da variável do eixo Y (ex: "Intervalo").
    :param var2_prefix: Prefixo usado no nome do arquivo para a variável 2 (ex: "I").
    """
    results = []
    print(f"\n--- INICIANDO EXPERIMENTO: {var1_name} vs. {var2_name} ---")

    var1_values = params_dict[var1_name]
    var2_values = params_dict[var2_name]

    for val1 in var1_values:
        for val2 in var2_values:
            # Constrói o nome do arquivo de forma robusta usando os prefixos
            test_file = os.path.join(TEST_DIR, f"cenario_{var1_prefix}{val1}_{var2_prefix}{val2}.txt")
            
            if not os.path.exists(test_file):
                print(f"AVISO: Arquivo de teste não encontrado, pulando: {test_file}")
                continue

            try:
                # Medir tempo de execução do processo C++
                start_time = time.time()
                process = subprocess.run(
                    [EXECUTABLE, test_file], 
                    capture_output=True, 
                    text=True, 
                    check=True,
                    encoding='utf-8'
                )
                exec_time = time.time() - start_time
                
                # Coletar a contagem de rearmazenamentos do stdout
                rearmazenamentos = int(process.stdout.strip())
                
                results.append({
                    var1_name: val1,
                    var2_name: val2,
                    "TempoExecucao": exec_time,
                    "Rearmazenamentos": rearmazenamentos
                })
                print(f"  {var1_name}={val1}, {var2_name}={val2} -> Tempo: {exec_time:.4f}s, Rearmazenamentos: {rearmazenamentos}")

            except subprocess.CalledProcessError as e:
                print(f"ERRO ao executar o teste para {test_file}:")
                print(f"  Stderr: {e.stderr}")
            except (ValueError, IndexError):
                print(f"ERRO: Não foi possível ler a saída (rearmazenamentos) para o teste {test_file}.")
                print(f"  Stdout recebido: {process.stdout}")

    return pd.DataFrame(results)

def plotar_heatmaps_comparativos(df, exp_title, x_var, y_var, output_filename):
    """
    Plota e salva heatmaps para Tempo de Execução e Rearmazenamentos.
    """
    if df.empty:
        print(f"DataFrame vazio. Não é possível plotar o gráfico para '{exp_title}'.")
        return

    fig, axes = plt.subplots(1, 2, figsize=(22, 9))
    fig.suptitle(exp_title, fontsize=20, y=0.98)
    
    # Estilo do Gráfico
    sns.set_theme(style="white")
    plt.rcParams['font.family'] = 'sans-serif'

    # Heatmap 1: Tempo de Execução
    pivot_tempo = df.pivot_table(index=y_var, columns=x_var, values="TempoExecucao")
    sns.heatmap(pivot_tempo.iloc[::-1], annot=True, fmt=".3f", cmap="magma", linewidths=.5, ax=axes[0])
    axes[0].set_title("Tempo de Execução (segundos)", fontsize=16)
    axes[0].set_xlabel(f"Variação de '{x_var}'", fontsize=12)
    axes[0].set_ylabel(f"Variação de '{y_var}'", fontsize=12)
    
    # Heatmap 2: Rearmazenamentos (Gargalo)
    pivot_rearm = df.pivot_table(index=y_var, columns=x_var, values="Rearmazenamentos")
    # CORREÇÃO APLICADA AQUI: fmt=".0f" em vez de fmt="d"
    sns.heatmap(pivot_rearm.iloc[::-1], annot=True, fmt=".0f", cmap="viridis", linewidths=.5, ax=axes[1])
    axes[1].set_title("Contagem de Rearmazenamentos (Gargalo)", fontsize=16)
    axes[1].set_xlabel(f"Variação de '{x_var}'", fontsize=12)
    axes[1].set_ylabel("") # Label do eixo Y é compartilhado

    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    print(f"\nGráfico salvo com sucesso em: '{output_filename}'")


if __name__ == "__main__":
    params = {"Capacidade": CAPACIDADES, "Latencia": LATENCIAS, "Intervalo": INTERVALOS}

    # --- Roda e plota o primeiro experimento ---
    df_exp1 = analisar_experimento(
        params_dict=params,
        var1_name="Capacidade", var1_prefix="C",
        var2_name="Intervalo",  var2_prefix="I"
    )
    plotar_heatmaps_comparativos(
        df=df_exp1,
        exp_title="Análise de Desempenho: Efeito da Capacidade vs. Intervalo",
        x_var="Capacidade",
        y_var="Intervalo",
        output_filename="analise_capacidade_vs_intervalo.png"
    )

    print("\n" + "="*60 + "\n")

    # --- Roda e plota o segundo experimento ---
    df_exp2 = analisar_experimento(
        params_dict=params,
        var1_name="Latencia", var1_prefix="L",
        var2_name="Intervalo", var2_prefix="I"
    )
    plotar_heatmaps_comparativos(
        df=df_exp2,
        exp_title="Análise de Desempenho: Efeito da Latência vs. Intervalo",
        x_var="Latencia",
        y_var="Intervalo",
        output_filename="analise_latencia_vs_intervalo.png"
    )