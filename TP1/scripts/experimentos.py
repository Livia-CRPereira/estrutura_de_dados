import os
import subprocess
import random
import time
import csv

# Configurações para experimentar
KEYSIZES = [4, 8, 16, 32]
PLSIZES = [4, 8, 16, 32]
VET_SIZES = [50, 100, 1000, 1500, 3000, 5000, 7000, 10000]
CONFIGURACOES = ['ordenado', 'reverso', 'aleatorio']

# Caminhos
# Caminhos base
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # volta da pasta /scripts para a pasta do projeto

# Caminhos corretos
CALIBRACAO_C = os.path.join(BASE_DIR, 'src', 'main_experimental.cc')
BIN_DIR = os.path.join(BASE_DIR, 'bin')
ENTRADA_DIR = os.path.join(BASE_DIR, 'scripts/entradas_expe')
CSV_SAIDA = os.path.join(BASE_DIR, 'scripts/resultados_expe.csv')

os.makedirs(BIN_DIR, exist_ok=True)
os.makedirs(ENTRADA_DIR, exist_ok=True)

def gerar_vetor(tamanho, keysize, payloadsize, tipo):
    vetor = []
    for i in range(tamanho):
        if tipo == 'ordenado':
            key = f"{i:0{keysize-1}d}"
        elif tipo == 'reverso':
            key = f"{(tamanho-i):0{keysize-1}d}"
        else:  # aleatorio
            key = ''.join(str(random.randint(0, 9)) for _ in range(keysize - 1))
        key += '\0'

        payload = ''.join(str(j % 10) for j in range(payloadsize - 1)) + '\0'
        vetor.append((key, payload))
    return vetor

def salvar_vetor_em_txt(vetor, nome_arquivo):
    with open(nome_arquivo, 'w') as f:
        for key, payload in vetor:
            f.write(f"{key.strip()} {payload.strip()}\n")

def compilar_binario(keysz, plsz):
    nome_bin = f"calibracao_k{keysz}_p{plsz}"
    bin_path = os.path.join(BIN_DIR, nome_bin)

    # Caminhos completos
    main_path = os.path.join(BASE_DIR, 'src', 'main_experimental.cc')
    ordenador_path = os.path.join(BASE_DIR, 'src', 'ordenador.cc')
    include_path = os.path.join(BASE_DIR, 'include')

    # Comando de compilação com -I para incluir cabeçalhos do include/
    subprocess.run([
        "g++", "-O2",
        f"-DKEYSZ={keysz}", f"-DPLSZ={plsz}",
        main_path, ordenador_path,
        "-I", include_path,
        "-o", bin_path
    ])

    return bin_path


def executar_binario(binario, arquivo_entrada):
    # Executa e mede o tempo de execução total
    inicio = time.perf_counter()
    resultado = subprocess.run([binario, arquivo_entrada], capture_output=True, text=True)
    fim = time.perf_counter()

    # Espera-se que a saída do binário seja CSV-like com:
    # limiarParticao,limiarQuebras,custo,tempo_calibracao,tempo_ordenacao
    saida = resultado.stdout.strip()
    return saida, fim - inicio

def rodar_experimentos():
    with open(CSV_SAIDA, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow([
            'keysize', 'payloadsize', 'vetor_size', 'config', 
            'limiar_particao', 'limiar_quebras', 'custo', 'tempo_leitura', 
            'tempo_calibracao', 'tempo_ordenacao', 'tempo_total'
        ])

        for keysz in KEYSIZES:
            for plsz in PLSIZES:
                binario = compilar_binario(keysz, plsz)
                for vsize in VET_SIZES:
                    for config in CONFIGURACOES:
                        nome_entrada = f"entrada_k{keysz}_p{plsz}_n{vsize}_{config}.txt"
                        caminho_entrada = os.path.join(ENTRADA_DIR, nome_entrada)

                        vetor = gerar_vetor(vsize, keysz, plsz, config)
                        salvar_vetor_em_txt(vetor, caminho_entrada)

                        print(f"Executando: KS={keysz}, PS={plsz}, N={vsize}, tipo={config}")
                        saida, tempo_total = executar_binario(binario, caminho_entrada)

                        if saida:
                            dados = saida.strip().split(',')
                            if len(dados) == 7:
                                try:
                                    limiar_particao = int(dados[0])
                                    limiar_quebras = int(dados[1])
                                    custo = float(dados[2])
                                    tempo_leit = float(dados[3])
                                    tempo_calib = float(dados[4])
                                    tempo_sort = float(dados[5])
                                    tempo_total = float(dados[6])

                                    writer.writerow([
                                        keysz, plsz, vsize, config,
                                        limiar_particao, limiar_quebras, custo, tempo_leit,
                                        tempo_calib, tempo_sort, tempo_total
                                    ])
                                except ValueError:
                                    print(f"[ERRO] Falha ao converter os dados: {dados}")
                            else:
                                print(f"[ERRO] Saída inesperada: {saida}")
                        else:
                            print("[ERRO] Binário não produziu saída.")


if __name__ == '__main__':
    rodar_experimentos()
