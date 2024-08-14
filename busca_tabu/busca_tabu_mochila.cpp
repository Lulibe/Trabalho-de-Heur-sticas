#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <deque>

using namespace std;

struct Item {
    int valor;
    int peso;
    int index;
};

// Função para ler os itens do arquivo
pair<int, vector<Item>> lerItens(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    string linha;
    int capacidade, numeroItens;
    vector<Item> itens;

    if (arquivo.is_open()) {
        if (getline(arquivo, linha)) {
            istringstream ss(linha);
            ss >> numeroItens >> capacidade;
        }

        int index = 0;
        while (getline(arquivo, linha)) {
            istringstream ss(linha);
            Item item;
            ss >> item.valor >> item.peso;
            item.index = index++;
            itens.push_back(item);
        }
        arquivo.close();
    } else {
        cerr << "Não foi possível abrir o arquivo." << endl;
    }

    return {capacidade, itens};
}

// Função para calcular o valor total e o peso total de uma solução
pair<int, int> calcularValorEPeso(const vector<Item>& itens, const vector<int>& selecionados) {
    int valorTotal = 0;
    int pesoTotal = 0;

    for (size_t i = 0; i < itens.size(); ++i) {
        if (selecionados[i] == 1) {
            valorTotal += itens[i].valor;
            pesoTotal += itens[i].peso;
        }
    }

    return {valorTotal, pesoTotal};
}

// Função para gerar uma solução inicial utilizando um método guloso
vector<int> gerarSolucaoInicialGulosa(int capacidade, const vector<Item>& itens) {
    vector<int> solucao(itens.size(), 0);
    int pesoAtual = 0;

    // Criar um vetor de índices dos itens ordenados pela relação valor/peso
    vector<int> indices(itens.size());
    for (size_t i = 0; i < itens.size(); ++i) {
        indices[i] = i;
    }

    // Ordenar os itens com base na relação valor/peso de forma decrescente
    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return (double)itens[a].valor / itens[a].peso > (double)itens[b].valor / itens[b].peso;
    });

    // Adicionar itens à solução até que a capacidade seja atingida
    for (int i : indices) {
        if (pesoAtual + itens[i].peso <= capacidade) {
            solucao[i] = 1;
            pesoAtual += itens[i].peso;
        }
    }

    return solucao;
}

// Função para realizar a busca tabu
pair<int, vector<int>> buscaTabu(int capacidade, const vector<Item>& itens, const vector<int>& solucaoInicial, int tamanhoTabu, int iteracoesMax, int iteracoesSemMelhoraMax) {
    auto [valorTotal, pesoAtual] = calcularValorEPeso(itens, solucaoInicial);
    vector<int> selecionados = solucaoInicial;
    vector<int> melhorSolucao = solucaoInicial;
    int melhorValor = valorTotal;

    deque<vector<int>> listaTabu;
    listaTabu.push_back(selecionados);

    int iteracoesSemMelhora = 0;

    for (int iteracao = 0; iteracao < iteracoesMax; ++iteracao) {
        bool encontrouMelhor = false;
        vector<int> melhorVizinho = selecionados;
        int melhorValorVizinho = 0;
        bool todosTabu = true;

        for (size_t i = 0; i < itens.size(); ++i) {
            vector<int> novaSolucao = selecionados;
            novaSolucao[i] = 1 - novaSolucao[i];

            int novoPeso = pesoAtual + (novaSolucao[i] ? itens[i].peso : -itens[i].peso);
            int novoValor = valorTotal + (novaSolucao[i] ? itens[i].valor : -itens[i].valor);

            if (novoPeso <= capacidade) {
                bool isTabu = find(listaTabu.begin(), listaTabu.end(), novaSolucao) != listaTabu.end();
                if (!isTabu && novoValor > melhorValorVizinho) {
                    melhorVizinho = novaSolucao;
                    melhorValorVizinho = novoValor;
                    encontrouMelhor = true;
                    todosTabu = false;
                }
            }
        }

        if (todosTabu) {
            melhorVizinho = listaTabu.front();
            listaTabu.pop_front();
            encontrouMelhor = true;
        }

        if (encontrouMelhor) {
            selecionados = melhorVizinho;
            valorTotal = melhorValorVizinho;
            pesoAtual = calcularValorEPeso(itens, selecionados).second;

            if (valorTotal > melhorValor) {
                melhorValor = valorTotal;
                melhorSolucao = selecionados;
                iteracoesSemMelhora = 0;
            } else {
                ++iteracoesSemMelhora;
            }

            listaTabu.push_back(selecionados);
            if (listaTabu.size() > tamanhoTabu) {
                listaTabu.pop_front();
            }
        } else {
            ++iteracoesSemMelhora;
        }

        if (iteracoesSemMelhora >= iteracoesSemMelhoraMax) {
            break;
        }
    }

    return {melhorValor, melhorSolucao};
}

// Função principal
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <tamanhoTabu> <iteracoesMax> <iteracoesSemMelhoraMax>" << endl;
        return 1;
    }

    // Local do arquivo de entrada
    string nomeArquivo = "./Aula2_Complementares/mochila_100_1000_1";

    auto [capacidade, itens] = lerItens(nomeArquivo);

    if (itens.empty()) {
        cerr << "Nenhum item foi lido do arquivo." << endl;
        return 1;
    }

    // Gera uma solução inicial utilizando o método guloso
    vector<int> solucaoInicial = gerarSolucaoInicialGulosa(capacidade, itens);

    int tamanhoTabu = stoi(argv[1]);           // tamanho da lista tabu
    int iteracoesMax = stoi(argv[2]);          // número máximo de iterações
    int iteracoesSemMelhoraMax = stoi(argv[3]); // número máximo de iterações sem melhora

    auto solucaoFinal = buscaTabu(capacidade, itens, solucaoInicial, tamanhoTabu, iteracoesMax, iteracoesSemMelhoraMax);

    auto [melhorValor, melhorPeso] = calcularValorEPeso(itens, solucaoFinal.second);

    cout << "Valor máximo obtido após busca tabu: " << melhorValor << endl;
    cout << "Peso correspondente ao valor máximo: " << melhorPeso << endl;

    cout << "Itens selecionados: ";
    for (int sel : solucaoFinal.second) {
        cout << sel << " ";
    }
    cout << endl;

    return 0;
}
