import matplotlib.pyplot as plt
import os
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

configs = ["ordenado", "reverso", "aleatorio"]
titles = {
    "ordenado": "Ordenado",
    "reverso": "Reverso",
    "aleatorio": "Aleatório"
}

fig, axes = plt.subplots(2, 3, figsize=(18, 10))
fig.suptitle("Limiar de Partição e Quebras vs Tamanho do Vetor (key=4, payload=4)", fontsize=16)

for i, config in enumerate(configs):
    df_config = df[(df["config"] == config) & (df["keysize"] == 4) & (df["payloadsize"] == 4)]

    # Limiar de Partição
    ax = axes[0, i]
    for key in sorted(df_config["keysize"].unique()):
        subset = df_config[df_config["keysize"] == key]
        means = subset.groupby("vetor_size")["limiar_particao"].mean()
        ax.plot(means.index, means.values, label=f"Keysize = {key}")
    ax.set_title(f"{titles[config]} - Limiar de Partição")
    ax.set_xlabel("Tamanho do Vetor")
    ax.set_ylabel("Limiar de Partição")
    ax.legend()

    # Limiar de Quebras
    ax = axes[1, i]
    for key in sorted(df_config["keysize"].unique()):
        subset = df_config[df_config["keysize"] == key]
        means = subset.groupby("vetor_size")["limiar_quebras"].mean()
        ax.plot(means.index, means.values, label=f"Keysize = {key}")
    ax.set_title(f"{titles[config]} - Limiar de Quebras")
    ax.set_xlabel("Tamanho do Vetor")
    ax.set_ylabel("Limiar de Quebras")
    ax.legend()

plt.tight_layout(rect=[0, 0, 1, 0.95])
plt.savefig(os.path.join(output_dir, "limiares_vs_tamanho_por_config.png"))
plt.close()
