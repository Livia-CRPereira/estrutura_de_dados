# gerador_graficos_avancados.py
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

ARQUIVO_RESULTADOS = 'resultados_avancados.csv'

def plotar_graficos(df):
    """Lê o DataFrame avançado e gera os gráficos de heatmap e relacionais."""
    sns.set_theme(style="whitegrid", palette="magma")
    print("Iniciando a geração de gráficos avançados...")

    # --- FIGURA 1: Dashboard com 2 Heatmaps ---
    print("Gerando Figura 1: Dashboard de Heatmaps...")
    fig, axes = plt.subplots(1, 2, figsize=(22, 9))
    fig.suptitle('Análise de Performance por Interação de Parâmetros', fontsize=20, weight='bold')

    # Heatmap 1.1 (Esquerda): Carga de Dados vs. Complexidade dos Eventos
    # Pivotar a tabela para criar a matriz, usando a média do tempo
    df_pivot1 = df.pivot_table(index='num_pacotes', columns='eventos_por_pacote', values='tempo_processamento_us', aggfunc='mean')
    sns.heatmap(ax=axes[0], data=df_pivot1, annot=True, fmt=".0f", cmap="magma", linewidths=.5)
    axes[0].set_title('Tempo de Proc. (µs) - Pacotes vs. Complexidade/Evento', fontsize=16)
    axes[0].set_xlabel('Eventos por Pacote (Complexidade)', fontsize=12)
    axes[0].set_ylabel('Número de Pacotes', fontsize=12)

    # Heatmap 1.2 (Direita): Carga de Usuários vs. Carga de Consultas
    df_pivot2 = df.pivot_table(index='num_clientes', columns='num_consultas', values='tempo_processamento_us', aggfunc='mean')
    sns.heatmap(ax=axes[1], data=df_pivot2, annot=True, fmt=".0f", cmap="viridis", linewidths=.5)
    axes[1].set_title('Tempo de Proc. (µs) - Clientes vs. Consultas', fontsize=16)
    axes[1].set_xlabel('Número de Consultas', fontsize=12)
    axes[1].set_ylabel('Número de Clientes', fontsize=12)

    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.savefig('grafico_dashboard_heatmaps.png', dpi=300)
    plt.close()
    print("-> 'grafico_dashboard_heatmaps.png' salvo.")

    # --- FIGURA 2: Gráfico Relacional Avançado (5 Dimensões) ---
    print("Gerando Figura 2: Gráfico Relacional...")
    # Este gráfico mostra a relação entre pacotes e tempo, mas usa
    # COR para a complexidade (eventos_pp), TAMANHO para o num de consultas,
    # e cria um GRID para cada número de clientes. É uma visualização muito rica.
    g = sns.relplot(
        data=df,
        x="num_pacotes", y="tempo_processamento_us",
        hue="eventos_por_pacote", size="num_consultas",
        col="num_clientes", col_wrap=3, # 3 gráficos por linha
        palette="rocket", sizes=(50, 300),
        height=5, aspect=1.2
    )
    g.fig.suptitle('Análise Multidimensional do Tempo de Processamento', fontsize=22, weight='bold', y=1.03)
    g.set_axis_labels("Número de Pacotes", "Tempo de Processamento (µs)")
    g.set_titles("Clientes = {col_name}", size=14)
    g.add_legend(title="Legenda", loc='upper left', bbox_to_anchor=(.02, .85))
    
    plt.savefig('grafico_relacional_avancado.png', dpi=300)
    plt.close()
    print("-> 'grafico_relacional_avancado.png' salvo.")


if __name__ == '__main__':
    try:
        df_resultados = pd.read_csv(ARQUIVO_RESULTADOS)
        plotar_graficos(df_resultados)
        print("\nTodos os gráficos avançados foram gerados com sucesso!")
    except FileNotFoundError:
        print(f"ERRO: Arquivo '{ARQUIVO_RESULTADOS}' não encontrado. Execute 'executor_testes_avancados.py' primeiro.")