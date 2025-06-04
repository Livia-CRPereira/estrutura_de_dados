import random
import numpy as np
from pathlib import Path

def generate_vector(size, vector_type, seed=1, num_breaks=0):
    """Gera vetores de diferentes tipos com mais padrões"""
    random.seed(seed)
    np.random.seed(seed)
    
    if vector_type == 'sorted':
        return list(range(size))
    elif vector_type == 'reverse':
        return list(range(size-1, -1, -1))
    elif vector_type == 'random':
        return random.sample(range(size*2), size) # Amplia o range para mais dispersão
    elif vector_type == 'nearly_sorted':
        vec = list(range(size))
        for _ in range(num_breaks):
            i, j = random.sample(range(size), 2)
            vec[i], vec[j] = vec[j], vec[i]
        return vec
    elif vector_type == 'repeated':
        return [random.randint(1, 5) for _ in range(size)]
    elif vector_type == 'sawtooth':
        return [i % 10 for i in range(size)]
    else:
        raise ValueError("Tipo de vetor desconhecido")

def generate_input_file(filename, vector, params):
    """Gera arquivo de entrada formatado"""
    with open(filename, 'w') as f:
        f.write(f"{params['seed']} {params['limiarCusto']} ")
        f.write(f"{params['a']} {params['b']} {params['c']} {params['tam']}\n")
        f.write(' '.join(map(str, vector)))

def main():
    # Configurações avançadas de teste
    sizes = [50, 100, 500, 1000, 5000, 10000, 20000] # Mais faixas de tamanho
    types = ['sorted', 'reverse', 'random', 'nearly_sorted', 'repeated', 'sawtooth']
    num_tests = 10 # 10 testes por combinação
    break_levels = [0.1, 0.3, 0.5] # Porcentagens de quebra para nearly_sorted
    
    # Parâmetros variados
    base_params = {
        'limiarCusto': 10000.0,
        'a': 0.012156,
        'b': -0.00637855,
        'c': 0.0172897
    }
    
    input_dir = Path("massive_test_inputs")
    input_dir.mkdir(exist_ok=True)
    
    test_count = 0
    for size in sizes:
        for vector_type in types:
            for test_num in range(1, num_tests + 1):
                params = base_params.copy()
                params['tam'] = size
                params['seed'] = test_num * size # Semente única
                
                if vector_type == 'nearly_sorted':
                    for break_pct in break_levels:
                        num_breaks = max(1, int(size * break_pct))
                        vec = generate_vector(size, vector_type, test_num, num_breaks)
                        filename = input_dir / f"test_{vector_type}_{size}_breaks{break_pct}_{test_num}.txt"
                        generate_input_file(filename, vec, params)
                        test_count += 1
                else:
                    vec = generate_vector(size, vector_type, test_num)
                    filename = input_dir / f"test_{vector_type}_{size}_{test_num}.txt"
                    generate_input_file(filename, vec, params)
                    test_count += 1
    
    print(f"\n⚡ Gerados {test_count} arquivos de teste em '{input_dir}'")
    print("Padrões incluídos:")
    print(f"- Tamanhos: {sizes}")
    print(f"- Tipos: {types}")
    print(f"- Níveis de quebra: {break_levels}")

if __name__ == "__main__":
    main()
