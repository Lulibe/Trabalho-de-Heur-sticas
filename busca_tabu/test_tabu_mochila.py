import os
import subprocess
import pandas as pd

# Caminho para o executável do programa
executavel = "./busca_tabu_mochila"  # Substitua pelo caminho do seu executável

# Valores das variáveis que você deseja testar
tamanhoTabu_vals = [10, 25, 50]
iteracoesMax_vals = [500, 1000, 1500]
iteracoesSemMelhoraMax_vals = [50, 100, 200]

# Lista para armazenar os resultados
resultados = []

# Executar o programa para cada combinação de valores
for tamanhoTabu in tamanhoTabu_vals:
    for iteracoesMax in iteracoesMax_vals:
        for iteracoesSemMelhoraMax in iteracoesSemMelhoraMax_vals:
            # Construir o comando de linha de comando
            comando = [executavel, str(tamanhoTabu), str(iteracoesMax), str(iteracoesSemMelhoraMax)]
            
            # Executar o comando e capturar a saída
            resultado = subprocess.run(comando, capture_output=True, text=True)
            saida = resultado.stdout
            
            # Processar a saída para extrair o valor máximo obtido
            for linha in saida.splitlines():
                if "Valor máximo obtido após busca tabu" in linha:
                    valor_maximo = int(linha.split(":")[1].strip())
                    break
            
            # Armazenar os resultados
            resultados.append({
                "tamanhoTabu": tamanhoTabu,
                "iteracoesMax": iteracoesMax,
                "iteracoesSemMelhoraMax": iteracoesSemMelhoraMax,
                "valorMaximo": valor_maximo
            })

# Criar um DataFrame pandas para exibir os resultados em formato de tabela
df_resultados = pd.DataFrame(resultados)
print(df_resultados)

# Salvar a tabela em um arquivo CSV
df_resultados.to_csv("resultados_busca_tabu_mochila.csv", index=False)
