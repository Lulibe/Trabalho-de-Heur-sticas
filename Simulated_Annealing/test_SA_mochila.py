import subprocess
import time
import statistics
import csv
import os

# Parâmetros para testar
temperaturas_iniciais = [100, 50, 20]
temperaturas_minimas = [1, 0.1, 10]
fatores_resfriamento = [0.995, 0.999, 0.9]
iteracoes_por_temperatura = [10, 50, 100]

# Número de execuções por configuração
num_execucoes = 10

# Nome do executável
executavel = "./Mochila_Simulated_Annealing"

# Tabela de resultados
resultados = []

for temp_inicial in temperaturas_iniciais:
    for temp_minima in temperaturas_minimas:
        for fator_resfriamento in fatores_resfriamento:
            for iteracoes in iteracoes_por_temperatura:
                resultados_execucao = []
                tempos_execucao = []

                for _ in range(num_execucoes):
                    # Montar comando
                    comando = [
                        executavel,
                        str(temp_inicial),
                        str(temp_minima),
                        str(fator_resfriamento),
                        str(iteracoes)
                    ]
                    
                    # Capturar tempo de início
                    inicio = time.time()
                    
                    # Executar o comando
                    resultado = subprocess.run(comando, capture_output=True, text=True)
                    
                    # Capturar tempo de fim
                    fim = time.time()
                    
                    # Extrair o valor da função objetivo (valor total) da saída
                    for linha in resultado.stdout.splitlines():
                        if "Valor total da solução" in linha:
                            valor_objetivo = int(linha.split(": ")[1])
                            resultados_execucao.append(valor_objetivo)
                            break
                    
                    # Calcular o tempo de execução
                    tempo_execucao = fim - inicio
                    tempos_execucao.append(tempo_execucao)
                
                # Calcular melhor, pior e média dos resultados
                melhor_resultado = max(resultados_execucao)
                pior_resultado = min(resultados_execucao)
                media_resultado = statistics.mean(resultados_execucao)
                media_tempo = statistics.mean(tempos_execucao)
                
                # Adicionar resultados à tabela
                resultados.append({
                    "Temp Inicial": temp_inicial,
                    "Temp Minima": temp_minima,
                    "Fator Resfriamento": fator_resfriamento,
                    "Iterações": iteracoes,
                    "Melhor Resultado": melhor_resultado,
                    "Pior Resultado": pior_resultado,
                    "Média Resultado": media_resultado,
                    "Média Tempo Execução (s)": media_tempo
                })

# Nome do arquivo CSV de saída
arquivo_csv = "resultados_simulated_annealing_mochila.csv"

# Escrever os resultados no arquivo CSV
with open(arquivo_csv, mode='w', newline='') as file:
    writer = csv.DictWriter(file, fieldnames=["Temp Inicial", "Temp Minima", "Fator Resfriamento", "Iterações",
                                              "Melhor Resultado", "Pior Resultado", "Média Resultado",
                                              "Média Tempo Execução (s)"])
    writer.writeheader()
    for resultado in resultados:
        writer.writerow(resultado)

print(f"Resultados gravados em {arquivo_csv}")
