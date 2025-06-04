# grau da regressao temq ue ser do grau da complexidade que queremos opter
# mpo de execução variando o tempo de execução

#ele tem que rodar todos os arquivos e #
import subprocess
import os

BINARIO = "./bin/calibracao"
ARQUIVO_CSV = "resultados.csv"
PASTA_ENTRADAS = "massive_test_inputs"
def executar_todos():
    entradas = [f for f in os.listdir(PASTA_ENTRADAS) if f.endswith(".txt")]
    for entrada in entradas:
        caminho = os.path.join(PASTA_ENTRADAS, entrada)
        print(f"Executando {entrada}...")
        subprocess.run([BINARIO, caminho])

if __name__ == "__main__":
    executar_todos()
