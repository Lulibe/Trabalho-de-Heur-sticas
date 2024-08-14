import subprocess
import pandas as pd
import time

# Configurações dos parâmetros a serem testados
tamanhosTabu = [5, 10, 20]
iteracoesMaxList = [50, 100, 200]
iteracoesSemMelhoraMaxList = [10, 50, 100]

# Lista para armazenar os resultados
resultados = []

# Caminho do executável
executavel = "./busca_tabu_tsp"

# Função para executar o programa e capturar a saída
def executar_programa(tamanhoTabu, iteracoesMax, iteracoesSemMelhoraMax):
    comando = [executavel, str(tamanhoTabu), str(iteracoesMax), str(iteracoesSemMelhoraMax)]
    tempos_execucao = []
    distancias_minimas = []

    for _ in range(10):
        inicio = time.time()
        resultado = subprocess.run(comando, capture_output=True, text=True)
        fim = time.time()
        tempo_execucao = fim - inicio
        
        # Processa a saída do programa
        linhas_saida = resultado.stdout.splitlines()
        distancia_minima = float(linhas_saida[0].split(":")[1].strip())
        
        tempos_execucao.append(tempo_execucao)
        distancias_minimas.append(distancia_minima)
    
    return {
        "TamanhoTabu": tamanhoTabu,
        "IteracoesMax": iteracoesMax,
        "IteracoesSemMelhoraMax": iteracoesSemMelhoraMax,
        "MelhorResultado": min(distancias_minimas),
        "PiorResultado": max(distancias_minimas),
        "MediaResultado": sum(distancias_minimas) / len(distancias_minimas),
        "TempoMedioExecucao": sum(tempos_execucao) / len(tempos_execucao)
    }

# Loop através de todas as combinações de parâmetros
for tamanhoTabu in tamanhosTabu:
    for iteracoesMax in iteracoesMaxList:
        for iteracoesSemMelhoraMax in iteracoesSemMelhoraMaxList:
            resultado = executar_programa(tamanhoTabu, iteracoesMax, iteracoesSemMelhoraMax)
            resultados.append(resultado)

# Cria um DataFrame a partir dos resultados
df = pd.DataFrame(resultados)

# Exibe a tabela de resultados
print(df)

# Salva os resultados em um arquivo CSV, se necessário
df.to_csv("resultados_busca_tabu_TSP.csv", index=False)
