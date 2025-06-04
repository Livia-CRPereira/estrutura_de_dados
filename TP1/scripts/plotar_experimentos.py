import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Configurações de visualização
sns.set(style="whitegrid")
plt.rcParams["axes.titlesize"] = 12

# Criar pasta de saída
output_dir = "plots_experimentos"
os.makedirs(output_dir, exist_ok=True)

# Carregar o CSV
df = pd.read_csv("resultados_expe.csv")

# 1. Heatmap do tempo de leitura (apenas vetores ordenados)
df_ord = df[df["config"] == "ordenado"]
pivot = df_ord.pivot_table(index="keysize", columns="payloadsize", values="tempo_leitura", aggfunc="mean")

plt.figure(figsize=(8, 6))
sns.heatmap(pivot, annot=True, fmt=".6f", cmap="coolwarm")
plt.title("Tempo de Leitura (vetores ordenados)\nKeysize vs Payloadsize")
plt.xlabel("Payloadsize")
plt.ylabel("Keysize")
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "tempo_leitura_heatmap_ordenado.png"))
plt.close()

# 3. Limiar de partição vs tamanho do vetor (vetores ordenados)
df_aleatorio = df[(df["config"] == "ordenado") & (df["keysize"] == 4) & (df["payloadsize"] == 4)]
plt.figure(figsize=(10, 6))
for key in sorted(df_aleatorio["keysize"].unique()):
    subset = df_aleatorio[df_aleatorio["keysize"] == key]
    means = subset.groupby("vetor_size")["limiar_particao"].mean()
    plt.plot(means.index, means.values, label=f"Keysize = {key}")
plt.title("Limiar de Partição vs Tamanho do Vetor")
plt.xlabel("Tamanho do Vetor")
plt.ylabel("Limiar de Partição")
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "limiar_particao_vs_tamanho_aleatorio.png"))
plt.close()

# 4. Limiar de quebras vs tamanho do vetor
plt.figure(figsize=(10, 6))
for key in sorted(df_aleatorio["keysize"].unique()):
    subset = df_aleatorio[df_aleatorio["keysize"] == key]
    means = subset.groupby("vetor_size")["limiar_quebras"].mean()
    plt.plot(means.index, means.values, label=f"Keysize = {key}")
plt.title("Limiar de Quebras vs Tamanho do Vetor")
plt.xlabel("Tamanho do Vetor")
plt.ylabel("Limiar de Quebras")
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "limiar_quebras_vs_tamanho_aleatorio.png"))
plt.close()

import matplotlib.pyplot as plt
import os

# Filtro comum
key = 4
payload = 4

# Filtra os dados para cada configuração
df_ordenado = df[(df["config"] == "ordenado") & (df["keysize"] == key) & (df["payloadsize"] == payload)]
df_reverso = df[(df["config"] == "reverso") & (df["keysize"] == key) & (df["payloadsize"] == payload)]
df_aleatorio = df[(df["config"] == "aleatorio") & (df["keysize"] == key) & (df["payloadsize"] == payload)]

# Agrupa pela média de tempo
ordenado_sort = df_ordenado.groupby("vetor_size")["tempo_ordenacao"].mean()
reverso_sort = df_reverso.groupby("vetor_size")["tempo_ordenacao"].mean()
aleatorio_sort = df_aleatorio.groupby("vetor_size")["tempo_ordenacao"].mean()

# Cria o gráfico conjunto
plt.figure(figsize=(10, 6))
plt.plot(ordenado_sort.index, ordenado_sort.values, marker='o', label='Ordenado', color='green')
plt.plot(reverso_sort.index, reverso_sort.values, marker='o', label='Reverso', color='red')
plt.plot(aleatorio_sort.index, aleatorio_sort.values, marker='o', label='Aleatório', color='blue')

# Personalização
plt.title("Tempo de Ordenação vs Tamanho do Vetor\n(key=4, payload=4)")
plt.xlabel("Tamanho do Vetor")
plt.ylabel("Tempo de Ordenação (s)")
plt.grid(True)
plt.legend()
plt.tight_layout()

# Salva a imagem
plt.savefig(os.path.join(output_dir, "tempo_ordenacao_comparativo_key4_payload4.png"))
plt.close()
