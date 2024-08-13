#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>

using namespace std;

struct Item {
    int valor;
    int peso;
};

struct Mochila {
    int capacidadeMaxima;
    vector<Item> itens;
};

Mochila lerArquivo(const string &nomeArquivo) {
    Mochila mochila;
    ifstream arquivo(nomeArquivo);
    if (arquivo.is_open()) {
        int numItens;
        arquivo >> numItens >> mochila.capacidadeMaxima;
        mochila.itens.resize(numItens);

        for (int i = 0; i < numItens; ++i) {
            arquivo >> mochila.itens[i].valor >> mochila.itens[i].peso;
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir o arquivo." << endl;
        exit(1);
    }
    return mochila;
}

int calcularValor(const vector<int> &solucao, const Mochila &mochila) {
    int valorTotal = 0;
    for (size_t i = 0; i < solucao.size(); ++i) {
        if (solucao[i] == 1) {
            valorTotal += mochila.itens[i].valor;
        }
    }
    return valorTotal;
}

int calcularPeso(const vector<int> &solucao, const Mochila &mochila) {
    int pesoTotal = 0;
    for (size_t i = 0; i < solucao.size(); ++i) {
        if (solucao[i] == 1) {
            pesoTotal += mochila.itens[i].peso;
        }
    }
    return pesoTotal;
}

vector<int> gerarVizinha(const vector<int> &solucao) {
    vector<int> novaSolucao = solucao;
    int i = rand() % solucao.size();
    novaSolucao[i] = 1 - novaSolucao[i]; // Inverte a inclusão/exclusão do item
    return novaSolucao;
}

int main() {
    srand(time(nullptr));

    // Ler os dados do arquivo
    Mochila mochila = lerArquivo("./Aula2_Complementares/mochila_100_1000_1");

    // Parâmetros do Simulated Annealing
    double temperaturaInicial = 10000;
    double temperaturaMinima = 1;
    double fatorResfriamento = 0.995;
    int iteracoesPorTemperatura = 100;

    // Inicialização da solução
    vector<int> solucaoAtual(mochila.itens.size(), 0); // Solução inicial (vazia)
    double valorAtual = calcularValor(solucaoAtual, mochila);
    double pesoAtual = calcularPeso(solucaoAtual, mochila);

    double temperatura = temperaturaInicial;

    while (temperatura > temperaturaMinima) {
        for (int i = 0; i < iteracoesPorTemperatura; ++i) {
            vector<int> novaSolucao = gerarVizinha(solucaoAtual);
            double novoPeso = calcularPeso(novaSolucao, mochila);
            
            if (novoPeso <= mochila.capacidadeMaxima) { // Verifica se a solução é viável
                double novoValor = calcularValor(novaSolucao, mochila);
                double deltaValor = novoValor - valorAtual;

                if (deltaValor > 0 || ((double) rand() / RAND_MAX) < exp(deltaValor / temperatura)) {
                    solucaoAtual = novaSolucao;
                    valorAtual = novoValor;
                    pesoAtual = novoPeso;
                }
            }
        }
        temperatura *= fatorResfriamento;
    }

    cout << "Melhor solução encontrada: ";
    for (size_t i = 0; i < solucaoAtual.size(); ++i) {
        if (solucaoAtual[i] == 1) {
            cout << "Item " << i+1 << " (valor: " << mochila.itens[i].valor << ", peso: " << mochila.itens[i].peso << "), ";
        }
    }
    cout << endl;
    cout << "Valor total da solução: " << valorAtual << endl;
    cout << "Peso total da solução: " << pesoAtual << endl;

    return 0;
}
