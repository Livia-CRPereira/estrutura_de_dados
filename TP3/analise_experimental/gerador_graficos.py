# gerador_graficos_filtrado.py
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

ARQUIVO_RESULTADOS = 'resultados.csv'

# !!! IMPORTANTE !!!
# Certifique-se que estes valores base são os mesmos usados no seu `executor_testes.py`
BASE_CLIENTES = 50
BASE_PACOTES = 100
BASE_EVENTOS_P_PACOTE = 8
BASE_CONSULTAS = 100

def plotar_graficos_filtrados(df):
    """
    Gera gráficos de análise, filtrando os dados para cada cenário específico
    para garantir que as tendências observadas sejam precisas e não poluídas.
    """
    sns.set_theme(style="whitegrid", palette="viridis")
    print("Iniciando a geração de gráficos com dados filtrados...")

    # --- FIGURA 1: Dashboard de Performance (com gráficos filtrados) ---
    print("Gerando Figura 1: Dashboard de Performance...")
    fig, axes = plt.subplots(2, 2, figsize=(18, 14))
    fig.suptitle('Análise de Performance com Dados Isolados', fontsize=22, weight='bold')

    # Gráfico 1.1: Composição do Tempo (Pode usar todos os dados para uma visão geral)
    df_tempos = df.melt(id_vars=['total_linhas'], value_vars=['tempo_leitura_us', 'tempo_processamento_us'],
                        var_name='Tipo de Tempo', value_name='Tempo (µs)')
    sns.barplot(ax=axes[0, 0], data=df_tempos, x='total_linhas', y='Tempo (µs)', hue='Tipo de Tempo', dodge=False)
    axes[0, 0].set_title('Composição Geral do Tempo de Execução', fontsize=16)
    axes[0, 0].set_xlabel('Número Total de Linhas (Todos os Testes)', fontsize=12)
    axes[0, 0].set_ylabel('Tempo (microssegundos)', fontsize=12)
    axes[0, 0].legend(['Leitura', 'Processamento'])
    axes[0, 0].tick_params(axis='x', rotation=45)

    # Gráfico 1.2: Escalabilidade Geral (Pode usar todos os dados)
    sns.lineplot(ax=axes[0, 1], data=df, x='total_linhas', y='tempo_leitura_us', label='Tempo de Leitura', marker='o', lw=2)
    sns.lineplot(ax=axes[0, 1], data=df, x='total_linhas', y='tempo_processamento_us', label='Tempo de Processamento', marker='o', lw=2)
    axes[0, 1].set_title('Escalabilidade Geral: Leitura vs. Processamento', fontsize=16)
    axes[0, 1].set_xlabel('Número Total de Linhas (Todos os Testes)', fontsize=12)
    axes[0, 1].set_ylabel('Tempo (microssegundos)', fontsize=12)
    axes[0, 1].set_yscale('log')
    axes[0, 1].legend()

    # Gráfico 1.3: Impacto do No. de Pacotes (COM FILTRO)
    # Filtra apenas os testes onde SÓ o número de pacotes variava.
    df_filtrado_pacotes = df[
        (df['num_clientes'] == BASE_CLIENTES) &
        (df['eventos_por_pacote'] == BASE_EVENTOS_P_PACOTE) &
        (df['num_consultas'] == BASE_CONSULTAS)
    ]
    sns.lineplot(ax=axes[1, 0], data=df_filtrado_pacotes, x='num_pacotes', y='tempo_processamento_us', marker='o', lw=2.5, color=sns.color_palette("plasma")[2])
    axes[1, 0].set_title('Impacto do No. de Pacotes (Cenário Isolado)', fontsize=16)
    axes[1, 0].set_xlabel('Número de Pacotes Únicos', fontsize=12)
    axes[1, 0].set_ylabel('Tempo de Processamento (µs)', fontsize=12)

    # Gráfico 1.4: Impacto do No. de Clientes (COM FILTRO)
    # Filtra apenas os testes onde SÓ o número de clientes variava.
    # Nota: a lógica de 'num_pacotes_ajustado' no executor é tratada aqui.
    df_filtrado_clientes = df[
        (df['eventos_por_pacote'] == BASE_EVENTOS_P_PACOTE) &
        (df['num_consultas'] == BASE_CONSULTAS) &
        (df['num_pacotes'] > BASE_PACOTES - 150) # Heurística para pegar os pacotes ajustados
    ]
    sns.barplot(ax=axes[1, 1], data=df_filtrado_clientes, x='num_clientes', y='tempo_processamento_us', palette="plasma")
    axes[1, 1].set_title('Impacto do No. de Clientes (Cenário Isolado)', fontsize=16)
    axes[1, 1].set_xlabel('Número de Clientes Únicos', fontsize=12)
    axes[1, 1].set_ylabel('Tempo de Processamento (µs)', fontsize=12)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig('grafico_dashboard_filtrado.png', dpi=300)
    plt.close()
    print("-> 'grafico_dashboard_filtrado.png' salvo.")

    # --- FIGURA 2: Análise de Vazão por Complexidade (COM FILTRO) ---
    print("Gerando Figura 2: Análise de Vazão Filtrada...")
    # Filtra apenas os testes onde SÓ a complexidade (eventos_pp) variava.
    df_filtrado_vazao = df[
        (df['num_clientes'] == BASE_CLIENTES) &
        (df['num_pacotes'] == BASE_PACOTES) &
        (df['num_consultas'] == BASE_CONSULTAS)
    ]
    df_filtrado_vazao['vazao_proc'] = df_filtrado_vazao['total_linhas'] / (df_filtrado_vazao['tempo_processamento_us'] / 1000000.0)
    
    plt.figure(figsize=(12, 7))
    sns.lineplot(data=df_filtrado_vazao, x='eventos_por_pacote', y='vazao_proc', marker='o', lw=2.5)
    plt.title('Vazão do Sistema vs. Complexidade por Pacote (Cenário Isolado)', fontsize=16)
    plt.xlabel('Eventos por Pacote (Complexidade)', fontsize=12)
    plt.ylabel('Linhas Processadas por Segundo', fontsize=12)
    plt.grid(True, which="both", ls="--")
    plt.tight_layout()
    plt.savefig('grafico_eficiencia_filtrado.png', dpi=300)
    plt.close()
    print("-> 'grafico_eficiencia_filtrado.png' salvo.")


if __name__ == '__main__':
    try:
        # É importante que o CSV usado seja o gerado pelo executor que varia um parâmetro por vez.
        df_resultados = pd.read_csv(ARQUIVO_RESULTADOS)
        plotar_graficos_filtrados(df_resultados)
        print("\nTodos os gráficos filtrados foram gerados com sucesso!")
    except FileNotFoundError:
        print(f"ERRO: Arquivo '{ARQUIVO_RESULTADOS}' não encontrado. Execute o 'executor_testes.py' correspondente primeiro.")
    except Exception as e:
        print(f"Ocorreu um erro: {e}")