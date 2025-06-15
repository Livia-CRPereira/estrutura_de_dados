import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

def criar_graficos_desempenho(df, pasta_saida):
    """Cria e salva os gráficos de desempenho computacional como imagens separadas."""
    print("Gerando gráficos de desempenho...")
    
    # Pré-processamento e cálculo de novas métricas
    df['throughput_pacotes_por_s'] = df['pacotes_P'] / df['tempo_medio_s']

    # --- Gráfico 2: Eficiência  vs. Número de Pacotes ---
    plt.figure(figsize=(12, 7))
    sns.lineplot(data=df, x='pacotes_P', y='throughput_pacotes_por_s', hue='armazens_A', marker='o', palette='plasma')
    plt.title('Desempenho: Eficiência (Throughput) vs. Carga de Pacotes', fontsize=16)
    plt.xlabel('Número de Pacotes (P)', fontsize=12)
    plt.ylabel('Pacotes Processados por Segundo', fontsize=12)
    plt.legend(title='Nº de Armazéns (A)')
    plt.grid(True, which="both", ls="--")
    caminho_grafico = os.path.join(pasta_saida, 'desempenho_throughput_vs_pacotes.png')
    plt.savefig(caminho_grafico)
    print(f"Gráfico salvo em: {caminho_grafico}")
    plt.close()

    # --- Gráfico 3: Gráfico de Calor (Heatmap) ---
    try:
        heatmap_data = df.pivot_table(index='armazens_A', columns='pacotes_P', values='tempo_medio_s')
        plt.figure(figsize=(14, 9))
        sns.heatmap(heatmap_data, annot=True, fmt=".3f", cmap='magma', annot_kws={"size": 10})
        plt.title('Heatmap do Tempo de Execução (s)', fontsize=16)
        plt.xlabel('Número de Pacotes (P)', fontsize=12)
        plt.ylabel('Número de Armazéns (A)', fontsize=12)
        caminho_grafico = os.path.join(pasta_saida, 'desempenho_heatmap_tempo.png')
        plt.savefig(caminho_grafico)
        print(f"Gráfico salvo em: {caminho_grafico}")
        plt.close()
    except Exception as e:
        print(f"Aviso: Não foi possível gerar o heatmap. Motivo: {e}")


def criar_dashboard_estatisticas(df_stats, pasta_saida):
    """Cria uma grade 2x2 com gráficos sobre as métricas da simulação."""
    print("\nGerando dashboard de estatísticas da simulação...")

    fig, axes = plt.subplots(2, 2, figsize=(20, 16))
    fig.suptitle('Dashboard de Análise das Métricas dos Pacotes', fontsize=20, y=0.98)
    
    # --- Gráfico 1 (Canto Superior Esquerdo): Distribuição do Tempo Total de Entrega ---
    sns.histplot(ax=axes[0, 0], data=df_stats, x='tempo_total', kde=True, bins=40, color='royalblue')
    axes[0, 0].set_title('Distribuição dos Tempos de Entrega')
    axes[0, 0].set_xlabel('Tempo Total no Sistema')
    axes[0, 0].set_ylabel('Contagem de Pacotes')

    # --- Gráfico 2 (Canto Superior Direito): Correlação Espera vs. Transporte ---
    sns.scatterplot(ax=axes[0, 1], data=df_stats, x='tempo_espera', y='tempo_transporte', alpha=0.5, color='darkgreen')
    axes[0, 1].set_title('Tempo de Espera vs. Tempo de Transporte por Pacote')
    axes[0, 1].set_xlabel('Tempo Total em Espera')
    axes[0, 1].set_ylabel('Tempo Total em Transporte')

    # --- Gráfico 3 (Canto Inferior Esquerdo): % do Tempo Gasto ---
    df_stats['perc_espera'] = (df_stats['tempo_espera'] / df_stats['tempo_total']) * 100
    df_stats['perc_transporte'] = (df_stats['tempo_transporte'] / df_stats['tempo_total']) * 100
    sns.boxplot(ax=axes[1, 0], data=df_stats[['perc_espera', 'perc_transporte']])
    axes[1, 0].set_title('Distribuição Percentual do Tempo Gasto')
    axes[1, 0].set_ylabel('% do Tempo Total no Sistema')
    
    # --- Gráfico 4 (Canto Inferior Direito): Tempo Total vs. Tempo de Espera ---
    sns.regplot(ax=axes[1, 1], data=df_stats, x='tempo_espera', y='tempo_total',
                scatter_kws={'alpha':0.2, 'color':'darkred'}, line_kws={'color':'black', 'ls':'--'})
    axes[1, 1].set_title('Relação entre Tempo de Espera e Entrega Final')
    axes[1, 1].set_xlabel('Tempo em Espera')
    axes[1, 1].set_ylabel('Tempo Total de Entrega')

    plt.tight_layout(rect=[0, 0.03, 1, 0.95]) # Ajusta o layout para o título principal
    caminho_grafico = os.path.join(pasta_saida, 'dashboard_estatisticas.png')
    plt.savefig(caminho_grafico)
    print(f"Dashboard de estatísticas salvo em: {caminho_grafico}")
    plt.close()


if __name__ == '__main__':
    sns.set_theme(style="whitegrid", palette="muted")
    
    CSV_DESEMPENHO = 'resultados/resultados_desempenho.csv'
    CSV_STATS = 'resultados/stats_pacotes.csv'
    PASTA_GRAFICOS = 'resultados'
    
    if not os.path.exists(PASTA_GRAFICOS):
        os.makedirs(PASTA_GRAFICOS)

    # Gera gráficos de desempenho (separados)
    try:
        df_desempenho = pd.read_csv(CSV_DESEMPENHO)
        criar_graficos_desempenho(df_desempenho, PASTA_GRAFICOS)
    except FileNotFoundError:
        print(f"Erro: Arquivo '{CSV_DESEMPENHO}' não encontrado.")
        print("Por favor, execute o 'executor_testes.py' primeiro.")
    except Exception as e:
        print(f"Ocorreu um erro ao gerar os gráficos de desempenho: {e}")

    # Gera dashboard de estatísticas (grade)
    try:
        df_stats = pd.read_csv(CSV_STATS)
        if df_stats.empty or df_stats['tempo_total'].sum() == 0:
             print("\nArquivo 'stats_pacotes.csv' está vazio ou não contém dados válidos. Pulando a análise de estatísticas.")
        else:
            # Filtra casos onde o tempo total é 0 para evitar divisão por zero
            df_stats = df_stats[df_stats['tempo_total'] > 0].copy()
            criar_dashboard_estatisticas(df_stats, PASTA_GRAFICOS)
    except FileNotFoundError:
        print(f"\nAviso: Arquivo '{CSV_STATS}' não encontrado.")
        print("Execute uma simulação para gerar este arquivo e obter a análise de estatísticas.")
    except Exception as e:
        print(f"Ocorreu um erro ao gerar o dashboard de estatísticas: {e}")