import subprocess
import pandas as pd

# Configurações dos parâmetros a serem testados
tamanhosTabu = [5, 10, 20]
iteracoesMaxList = [50, 100, 200]
iteracoesSemMelhoraMaxList = [10, 50, 100]

# Lista para armazenar os resultados
resultados = []

# Caminho do executável
executavel = "./busca_tabu_tsp"

# Loop através de todas as combinações de parâmetros
for tamanhoTabu in tamanhosTabu:
    for iteracoesMax in iteracoesMaxList:
        for iteracoesSemMelhoraMax in iteracoesSemMelhoraMaxList:
            # Executa o programa C++ com os parâmetros atuais
            comando = [executavel, str(tamanhoTabu), str(iteracoesMax), str(iteracoesSemMelhoraMax)]
            resultado = subprocess.run(comando, capture_output=True, text=True)
            
            # Processa a saída do programa
            linhas_saida = resultado.stdout.splitlines()
            distancia_minima = float(linhas_saida[0].split(":")[1].strip())

            # Armazena os resultados
            resultados.append({
                "TamanhoTabu": tamanhoTabu,
                "IteracoesMax": iteracoesMax,
                "IteracoesSemMelhoraMax": iteracoesSemMelhoraMax,
                "DistanciaMinima": distancia_minima
            })

# Cria um DataFrame a partir dos resultados
df = pd.DataFrame(resultados)

# Exibe a tabela de resultados
print(df)

# Salva os resultados em um arquivo CSV, se necessário
df.to_csv("resultados_tabu_search.csv", index=False)
