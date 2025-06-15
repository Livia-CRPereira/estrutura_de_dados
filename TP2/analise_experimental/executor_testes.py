import os
import subprocess
import csv
import statistics
import re

def extrair_parametros_do_nome(nome_arquivo):
    """Extrai A e P do nome do arquivo, ex: 'teste_A10_P100.txt'"""
    match_A = re.search(r'A(\d+)', nome_arquivo)
    match_P = re.search(r'P(\d+)', nome_arquivo)
    A = int(match_A.group(1)) if match_A else 0
    P = int(match_P.group(1)) if match_P else 0
    return A, P

def executar_simulacao(executavel, arquivo_teste, num_repeticoes=3):
    """Executa o simulador C++ e retorna a média e desvio padrão do tempo."""
    tempos = []
    print(f"Executando {os.path.basename(arquivo_teste)}...")
    for i in range(num_repeticoes):
        # O tempo é impresso no stderr, o log da simulação no stdout
        resultado = subprocess.run([executavel, arquivo_teste], capture_output=True, text=True)
        
        if resultado.returncode != 0:
            print(f"  ERRO na execução! Código de saída: {resultado.returncode}")
            print(f"  Stderr: {resultado.stderr}")
            return None, None
            
        try:
            tempo = float(resultado.stderr.strip())
            tempos.append(tempo)
        except (ValueError, IndexError):
            print(f"  ERRO: Não foi possível extrair o tempo da saída.")
            print(f"  Stderr: {resultado.stderr}")
            return None, None

    media = statistics.mean(tempos) if tempos else 0
    desvio_padrao = statistics.stdev(tempos) if len(tempos) > 1 else 0
    
    return media, desvio_padrao

if __name__ == "__main__":
    PASTA_TESTES = 'casos_de_teste'
    EXECUTAVEL_CPP = '../bin/tp2.out' # Ajuste o caminho conforme sua estrutura
    CSV_SAIDA = 'resultados/resultados_desempenho.csv'

    if not os.path.exists(os.path.dirname(CSV_SAIDA)):
        os.makedirs(os.path.dirname(CSV_SAIDA))

    # Pega todos os arquivos de teste da pasta
    arquivos_de_teste = [os.path.join(PASTA_TESTES, f) for f in os.listdir(PASTA_TESTES) if f.endswith('.txt')]
    
    # Ordena para uma execução previsível
    arquivos_de_teste.sort()

    with open(CSV_SAIDA, 'w', newline='') as f_csv:
        writer = csv.writer(f_csv)
        writer.writerow(['armazens_A', 'pacotes_P', 'tempo_medio_s', 'desvio_padrao_s'])

        for arquivo in arquivos_de_teste:
            A, P = extrair_parametros_do_nome(os.path.basename(arquivo))
            tempo_medio, desvio_padrao = executar_simulacao(EXECUTAVEL_CPP, arquivo)

            if tempo_medio is not None:
                writer.writerow([A, P, tempo_medio, desvio_padrao])
                print(f"  -> Média: {tempo_medio:.4f}s, Desvio Padrão: {desvio_padrao:.4f}s")
    
    print(f"\nExecução concluída! Resultados salvos em '{CSV_SAIDA}'.")