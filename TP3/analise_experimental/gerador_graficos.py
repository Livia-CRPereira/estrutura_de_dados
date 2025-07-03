import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

ARQUIVO_RESULTADOS = 'resultados.csv'

def plotar_graficos(df):
    sns.set_theme(style="whitegrid", palette="viridis")
    print("Iniciando a geração dos gráficos com precisão em microssegundos...")

    # =========== MUDANÇA AQUI: Usar as colunas com "_us" ===========
    fig, axes = plt.subplots(2, 2, figsize=(18, 14))
    fig.suptitle('Análise de Performance: Leitura vs. Processamento', fontsize=22, weight='bold')

    # Gráfico 1.1: Composição do Tempo (Barras Empilhadas)
    df_tempos = df.melt(id_vars=['total_linhas'], value_vars=['tempo_leitura_us', 'tempo_processamento_us'],
                        var_name='Tipo de Tempo', value_name='Tempo (µs)')
    
    sns.barplot(ax=axes[0, 0], data=df_tempos, x='total_linhas', y='Tempo (µs)', hue='Tipo de Tempo', dodge=False)
    axes[0, 0].set_title('Composição do Tempo de Execução por Carga', fontsize=16)
    axes[0, 0].set_xlabel('Número Total de Linhas na Entrada', fontsize=12)
    axes[0, 0].set_ylabel('Tempo (microssegundos)', fontsize=12) # Rótulo atualizado
    axes[0, 0].legend(['Leitura', 'Processamento'])
    axes[0, 0].tick_params(axis='x', rotation=45)

    # Gráfico 1.2: Escala dos Tempos (Gráfico de Linha)
    sns.lineplot(ax=axes[0, 1], data=df, x='total_linhas', y='tempo_leitura_us', label='Tempo de Leitura', marker='o', lw=2)
    sns.lineplot(ax=axes[0, 1], data=df, x='total_linhas', y='tempo_processamento_us', label='Tempo de Processamento', marker='o', lw=2)
    axes[0, 1].set_title('Escalabilidade do Tempo de Leitura vs. Processamento', fontsize=16)
    axes[0, 1].set_xlabel('Número Total de Linhas na Entrada', fontsize=12)
    axes[0, 1].set_ylabel('Tempo (microssegundos)', fontsize=12) # Rótulo atualizado
    axes[0, 1].set_yscale('log')
    axes[0, 1].legend()
    axes[0, 1].grid(True, which="both", ls="--")

    # Gráfico 1.3: Impacto do No. de Pacotes no Processamento
    df_pacotes = df.groupby('num_pacotes')['tempo_processamento_us'].mean().reset_index()
    sns.lineplot(ax=axes[1, 0], data=df_pacotes, x='num_pacotes', y='tempo_processamento_us', marker='o', lw=2.5, color=sns.color_palette("plasma")[2])
    axes[1, 0].set_title('Impacto do No. de Pacotes no Tempo de Processamento', fontsize=16)
    axes[1, 0].set_xlabel('Número de Pacotes Únicos', fontsize=12)
    axes[1, 0].set_ylabel('Tempo Médio de Processamento (µs)', fontsize=12) # Rótulo atualizado

    # Gráfico 1.4: Análise de Vazão (Throughput)
    # Vazão ainda faz mais sentido em segundos, então convertemos aqui
    df['vazao_proc'] = df['total_linhas'] / (df['tempo_processamento_us'] / 1000000.0)
    sns.lineplot(ax=axes[1, 1], data=df, x='total_linhas', y='vazao_proc', marker='o', color='purple', lw=2.5)
    axes[1, 1].set_title('Vazão do Processamento (Eficiência)', fontsize=16)
    axes[1, 1].set_xlabel('Número Total de Linhas na Entrada', fontsize=12)
    axes[1, 1].set_ylabel('Linhas Processadas por Segundo', fontsize=12)
    
    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig('grafico_dashboard_completo.png', dpi=300)
    plt.close()
    print("-> 'grafico_dashboard_completo.png' salvo.")
    
        # --- FIGURA 2: Heatmap de Custo de Processamento ---
    print("Gerando Figura 2: Mapa de Calor...")
    try:
        df_pivot = df.pivot_table(index='num_pacotes', columns='eventos_por_pacote', values='tempo_processamento_us')
        plt.figure(figsize=(12, 8))
        sns.heatmap(df_pivot, annot=True, fmt=".0f", cmap="magma", linewidths=.5)
        plt.title('Mapa de Calor: Tempo de Processamento (ms)\n(No. Pacotes vs. Complexidade por Pacote)', fontsize=16)
        plt.xlabel('Eventos por Pacote (Complexidade)', fontsize=12)
        plt.ylabel('Número de Pacotes', fontsize=12)
        plt.savefig('grafico_heatmap_processamento.png', dpi=300)
        plt.close()
        print("-> 'grafico_heatmap_processamento.png' salvo.")
    except Exception as e:
        print(f"Não foi possível gerar o heatmap. Verifique se os dados em 'resultados.csv' são suficientes.")

    # --- FIGURA 3: Análise de Eficiência (Vazão) ---
    print("Gerando Figura 3: Análise de Eficiência...")
    # Calcula a vazão usando apenas o tempo de processamento
    df['vazao_proc'] = df['total_linhas'] / (df['tempo_processamento_us'] / 1000.0)
    
    plt.figure(figsize=(12, 7))
    sns.lmplot(data=df, x='total_linhas', y='vazao_proc', hue='eventos_por_pacote', 
               height=7, aspect=1.5, legend=False)
    plt.title('Análise de Vazão (Eficiência) do Processamento', fontsize=16)
    plt.xlabel('Número Total de Linhas na Entrada', fontsize=12)
    plt.ylabel('Linhas Processadas por Segundo', fontsize=12)
    plt.grid(True, which="both", ls="--")
    plt.legend(title='Eventos por Pacote', loc='best')
    plt.tight_layout()
    plt.savefig('grafico_eficiencia_vazao.png', dpi=300)
    plt.close()
    print("-> 'grafico_eficiencia_vazao.png' salvo.")

if __name__ == '__main__':
    try:
        df_resultados = pd.read_csv(ARQUIVO_RESULTADOS)
        plotar_graficos(df_resultados)
        print("\nTodos os gráficos foram gerados com sucesso!")
    except FileNotFoundError:
        print(f"ERRO: Arquivo '{ARQUIVO_RESULTADOS}' não encontrado. Execute 'executor_testes.py' primeiro.")