import subprocess
import time
import statistics
import csv

# Definindo os parâmetros a serem testados
temperatura_inicial_values = [1000, 500, 100]
temperatura_minima_values = [1, 0.1, 0.01]
fator_resfriamento_values = [0.995, 0.99, 0.9]
iteracoes_por_temperatura_values = [100, 500, 1000]

# Caminho para o executável do programa C++
executavel = "./TSP_Simulated_Annealing"  # Substitua pelo nome correto do executável

# Lista para armazenar os resultados
resultados = []

# Executando o algoritmo com as combinações de parâmetros
for temp_inicial in temperatura_inicial_values:
    for temp_minima in temperatura_minima_values:
        for fator_resfriamento in fator_resfriamento_values:
            for iteracoes_por_temp in iteracoes_por_temperatura_values:
                resultados_execucoes = []
                tempos_execucoes = []

                for _ in range(10):  # Executa 10 vezes para cada combinação de parâmetros
                    # Comando a ser executado
                    comando = [
                        executavel,
                        str(temp_inicial),
                        str(temp_minima),
                        str(fator_resfriamento),
                        str(iteracoes_por_temp)
                    ]

                    # Medindo o tempo de execução
                    inicio = time.time()
                    resultado = subprocess.run(comando, capture_output=True, text=True)
                    fim = time.time()

                    # Coletando a saída
                    saida = resultado.stdout.strip().splitlines()
                    custo = float(saida[-1].split(":")[-1].strip())  # Obtém o custo da última linha
                    tempo_execucao = fim - inicio

                    resultados_execucoes.append(custo)
                    tempos_execucoes.append(tempo_execucao)

                # Calcula o melhor, pior e média dos resultados e tempo médio
                melhor_resultado = max(resultados_execucoes)
                pior_resultado = min(resultados_execucoes)
                media_resultado = statistics.mean(resultados_execucoes)
                tempo_medio_execucao = statistics.mean(tempos_execucoes)

                # Armazena os resultados na lista
                resultados.append([
                    temp_inicial,
                    temp_minima,
                    fator_resfriamento,
                    iteracoes_por_temp,
                    melhor_resultado,
                    pior_resultado,
                    media_resultado,
                    tempo_medio_execucao
                ])

# Salvando os resultados em um arquivo CSV
with open("resultados_simulated_annealing_tsp.csv", "w", newline='') as csvfile:
    escritor_csv = csv.writer(csvfile)
    # Escrevendo o cabeçalho
    escritor_csv.writerow([
        "Temp Inicial", "Temp Minima", "Fator Resfriamento", "Iteracoes/Temp",
        "Melhor Resultado", "Pior Resultado", "Media Resultado", "Tempo Médio (s)"
    ])
    # Escrevendo os dados
    for resultado in resultados:
        escritor_csv.writerow(resultado)

print("Resultados salvos em 'resultados_simulated_annealing_tsp.csv'")
