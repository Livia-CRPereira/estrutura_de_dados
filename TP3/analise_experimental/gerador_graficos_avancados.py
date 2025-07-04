# gerador_graficos_avancados.py
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from mpl_toolkits.mplot3d import Axes3D # Importa o necessário para 3D

ARQUIVO_RESULTADOS = 'resultados_avancados.csv'

def plotar_graficos(df):
    """Lê o DataFrame avançado e gera os gráficos de heatmap e um gráfico 3D."""
    sns.set_theme(style="whitegrid", palette="magma")
    print("Iniciando a geração de gráficos avançados...")

    # --- FIGURA 1: Dashboard com 2 Heatmaps (sem alteração) ---
    print("Gerando Figura 1: Dashboard de Heatmaps...")
    fig, axes = plt.subplots(1, 2, figsize=(22, 9))
    fig.suptitle('Análise de Performance por Interação de Parâmetros', fontsize=20, weight='bold')
    
    # Heatmap 1.1
    df_pivot1 = df.pivot_table(index='num_pacotes', columns='eventos_por_pacote', values='tempo_processamento_us', aggfunc='mean')
    sns.heatmap(ax=axes[0], data=df_pivot1, annot=True, fmt=".0f", cmap="magma", linewidths=.5)
    axes[0].set_title('Tempo de Proc. (µs) - Pacotes vs. Complexidade/Evento', fontsize=16)
    axes[0].set_xlabel('Eventos por Pacote (Complexidade)', fontsize=12)
    axes[0].set_ylabel('Número de Pacotes', fontsize=12)

    # Heatmap 1.2
    df_pivot2 = df.pivot_table(index='num_clientes', columns='num_consultas', values='tempo_processamento_us', aggfunc='mean')
    sns.heatmap(ax=axes[1], data=df_pivot2, annot=True, fmt=".0f", cmap="viridis", linewidths=.5)
    axes[1].set_title('Tempo de Proc. (µs) - Clientes vs. Consultas', fontsize=16)
    axes[1].set_xlabel('Número de Consultas', fontsize=12)
    axes[1].set_ylabel('Número de Clientes', fontsize=12)

    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.savefig('grafico_dashboard_heatmaps.png', dpi=300)
    plt.close()
    print("-> 'grafico_dashboard_heatmaps.png' salvo.")

    # --- FIGURA 2: Gráfico de Dispersão 3D (NOVO GRÁFICO) ---
    print("Gerando Figura 2: Gráfico de Dispersão 3D...")
    fig = plt.figure(figsize=(15, 12))
    ax = fig.add_subplot(111, projection='3d')

    # Mapeia os valores para as dimensões do gráfico
    x = df['num_pacotes']
    y = df['num_clientes']
    z = df['tempo_processamento_us']
    # Cor baseada na complexidade
    c = df['eventos_por_pacote']
    # Tamanho baseado no número de consultas (com um fator de escala para ficar visível)
    s = df['num_consultas'] * 1.5

    scatter = ax.scatter(x, y, z, c=c, s=s, cmap='magma', alpha=0.7)

    ax.set_title('Análise Multidimensional do Tempo de Processamento', fontsize=18, weight='bold')
    ax.set_xlabel('Número de Pacotes', fontsize=12, labelpad=10)
    ax.set_ylabel('Número de Clientes', fontsize=12, labelpad=10)
    ax.set_zlabel('Tempo de Processamento (µs)', fontsize=12, labelpad=10)

    # Adiciona legendas
    legend1 = ax.legend(*scatter.legend_elements(prop="colors", num=5), title="Eventos por Pacote")
    ax.add_artist(legend1)
    
    # Cria uma legenda para o tamanho dos pontos
    handles = [plt.scatter([],[], s=q*1.5, color='gray') for q in [25, 100, 200]]
    labels = [str(q) for q in [25, 100, 200]]
    ax.legend(handles, labels, title='No. de Consultas', bbox_to_anchor=(1.1, 0.7))

    plt.savefig('grafico_3d_performance.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("-> 'grafico_3d_performance.png' salvo.")


if __name__ == '__main__':
    try:
        df_resultados = pd.read_csv('resultados_avancados.csv')
        plotar_graficos(df_resultados)
        print("\nTodos os gráficos avançados foram gerados com sucesso!")
    except FileNotFoundError:
        print(f"ERRO: Arquivo 'resultados_avancados.csv' não encontrado. Execute 'executor_testes_avancados.py' primeiro.")