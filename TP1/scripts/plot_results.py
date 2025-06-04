import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import StandardScaler

# Carregar os dados do CSV
df = pd.read_csv('resultados.csv')

# Separar variáveis explicativas (X) e variável dependente (y)
X = df[['comparacoes', 'movimentacoes', 'chamadas']]
y = df['tempo']

# Padronizar os dados (escala Z-score)
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# Criar e treinar o modelo de regressão linear COM intercepto
modelo = LinearRegression(fit_intercept=True)
modelo.fit(X_scaled, y)  # Usando X_scaled

# Extrair coeficientes e calcular métricas
a, b, c = modelo.coef_
intercepto = modelo.intercept_
r2 = modelo.score(X_scaled, y)

# Calcular os valores previstos (usando X_scaled!)
y_pred = modelo.predict(X_scaled)

# Exibir resultados
print("Resultados da Regressão Linear Múltipla (com dados padronizados)")
print("------------------------------------------------------------")
print(f"Coeficiente a (comparações): {a:.6f}")
print(f"Coeficiente b (movimentações): {b:.6f}")
print(f"Coeficiente c (chamadas): {c:.6f}")
print(f"Intercepto (termo constante): {intercepto:.6f}")
print(f"Coeficiente de Determinação (R²): {r2:.4f}")
# Exemplo de cálculo manual (lembrando que os dados foram escalonados)
print("\nFórmula do custo padronizado:")
print(f"custo_padronizado = ({a:.6f}*comparações_padronizadas) + ({b:.6f}*movimentações_padronizadas) + ({c:.6f}*chamadas_padronizadas) + {intercepto:.6f}")


# Ajuste para coeficientes na escala original (opcional)
a_original = a / scaler.scale_[0]
b_original = b / scaler.scale_[1]
c_original = c / scaler.scale_[2]
intercepto_original = intercepto - (a*scaler.mean_[0]/scaler.scale_[0] + b*scaler.mean_[1]/scaler.scale_[1] + c*scaler.mean_[2]/scaler.scale_[2])

print("\nCoeficientes na escala original:")
print(f"a (comparações): {a_original:.6f}")
print(f"b (movimentações): {b_original:.6f}")
print(f"c (chamadas): {c_original:.6f}")
print(f"Intercepto: {intercepto_original:.6f}")

# Criar gráfico de comparação
plt.figure(figsize=(10, 6))
plt.scatter(y, y_pred, alpha=0.5, label='Amostras')
plt.plot([y.min(), y.max()], [y.min(), y.max()], 'k--', lw=2, label='Linha de Referência (Ideal)')
plt.xlabel('Tempo Real')
plt.ylabel('Tempo Previsto pelo Modelo')
plt.title(f'Comparação: Valores Reais vs Previstos\nR² = {r2:.4f}')
plt.legend()
plt.grid(True)

# Salvar o gráfico
plt.savefig('regressao_comparacao.png', dpi=300, bbox_inches='tight')
print("\nGráfico salvo como 'regressao_comparacao.png'")




