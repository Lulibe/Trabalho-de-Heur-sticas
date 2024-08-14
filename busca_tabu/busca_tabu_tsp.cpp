#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <deque>

using namespace std;

struct Cidade {
    int id;
    double x, y;
};

double calcularDistancia(const Cidade &a, const Cidade &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

vector<Cidade> lerCidades(const string &nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    string linha;
    vector<Cidade> cidades;

    if (arquivo.is_open()) {
        while (getline(arquivo, linha)) {
            istringstream ss(linha);
            Cidade cidade;
            ss >> cidade.id >> cidade.x >> cidade.y;
            cidades.push_back(cidade);
        }
        arquivo.close();
    } else {
        cerr << "Não foi possível abrir o arquivo." << endl;
        exit(1);
    }

    return cidades;
}

double calcularDistanciaTotal(const vector<int> &rota, const vector<vector<double>> &matrizDistancias) {
    double distanciaTotal = 0.0;
    for (size_t i = 0; i < rota.size() - 1; ++i) {
        distanciaTotal += matrizDistancias[rota[i]][rota[i + 1]];
    }
    distanciaTotal += matrizDistancias[rota.back()][rota[0]]; // Voltar ao ponto inicial
    return distanciaTotal;
}

vector<int> gerarMelhorVizinho(const vector<int> &rota, const vector<vector<double>> &matrizDistancias, const deque<vector<int>> &listaTabu, double melhorDistancia) {
    vector<int> melhorVizinho = rota;
    double melhorDistanciaVizinho = numeric_limits<double>::max();
    bool encontrouNaoTabu = false;

    for (size_t i = 0; i < rota.size() - 1; ++i) {
        for (size_t j = i + 1; j < rota.size(); ++j) {
            vector<int> novaRota = rota;
            swap(novaRota[i], novaRota[j]);

            double novaDistancia = calcularDistanciaTotal(novaRota, matrizDistancias);

            bool isTabu = find(listaTabu.begin(), listaTabu.end(), novaRota) != listaTabu.end();
            
            // Critério de Aspiração: Permite movimentos tabu se melhorarem a melhor solução conhecida
            if (novaDistancia < melhorDistancia || !isTabu) {
                if (novaDistancia < melhorDistanciaVizinho) {
                    melhorVizinho = novaRota;
                    melhorDistanciaVizinho = novaDistancia;
                    encontrouNaoTabu = true;
                }
            }
        }
    }

    // Se todos os movimentos são tabu e não encontramos nenhum não-tabu, permitir o melhor tabu
    if (!encontrouNaoTabu) {
        melhorVizinho = listaTabu.front(); // Pega um vizinho tabu (opcionalmente, poderia ser aleatório)
    }

    return melhorVizinho;
}

pair<double, vector<int>> buscaTabuTSP(const vector<Cidade> &cidades, const vector<int> &solucaoInicial, int tamanhoTabu, int iteracoesMax, int iteracoesSemMelhoraMax) {
    size_t numCidades = cidades.size();
    vector<vector<double>> matrizDistancias(numCidades, vector<double>(numCidades, 0.0));

    for (size_t i = 0; i < numCidades; ++i) {
        for (size_t j = 0; j < numCidades; ++j) {
            matrizDistancias[i][j] = calcularDistancia(cidades[i], cidades[j]);
        }
    }

    double distanciaAtual = calcularDistanciaTotal(solucaoInicial, matrizDistancias);
    vector<int> rotaAtual = solucaoInicial;
    vector<int> melhorRota = solucaoInicial;
    double melhorDistancia = distanciaAtual;

    deque<vector<int>> listaTabu;
    listaTabu.push_back(rotaAtual);

    int iteracoesSemMelhora = 0;

    for (int iteracao = 0; iteracao < iteracoesMax; ++iteracao) {
        vector<int> melhorVizinho = gerarMelhorVizinho(rotaAtual, matrizDistancias, listaTabu, melhorDistancia);
        double distanciaVizinho = calcularDistanciaTotal(melhorVizinho, matrizDistancias);

        if (distanciaVizinho < melhorDistancia) {
            melhorDistancia = distanciaVizinho;
            melhorRota = melhorVizinho;
            iteracoesSemMelhora = 0;
        } else {
            ++iteracoesSemMelhora;
        }

        rotaAtual = melhorVizinho;
        listaTabu.push_back(rotaAtual);
        if (listaTabu.size() > tamanhoTabu) {
            listaTabu.pop_front();
        }

        if (iteracoesSemMelhora >= iteracoesSemMelhoraMax) {
            break;
        }
    }

    return {melhorDistancia, melhorRota};
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <tamanhoTabu> <iteracoesMax> <iteracoesSemMelhoraMax>" << endl;
        return 1;
    }

    // Lê os parâmetros da linha de comando
    int tamanhoTabu = stoi(argv[1]);
    int iteracoesMax = stoi(argv[2]);
    int iteracoesSemMelhoraMax = stoi(argv[3]);

    // Local fixo do arquivo de entrada
    string nomeArquivo = "./Aula2_Complementares/tsp_51";

    // Lê as cidades do arquivo
    vector<Cidade> cidades = lerCidades(nomeArquivo);

    // Gera uma solução inicial (permutação aleatória das cidades)
    vector<int> solucaoInicial(cidades.size());
    for (size_t i = 0; i < cidades.size(); ++i) {
        solucaoInicial[i] = i;
    }
    random_shuffle(solucaoInicial.begin(), solucaoInicial.end());

    // Aplica Busca Tabu para resolver o TSP
    auto resultado = buscaTabuTSP(cidades, solucaoInicial, tamanhoTabu, iteracoesMax, iteracoesSemMelhoraMax);

    // Exibe a melhor solução encontrada
    cout << "Distância mínima encontrada: " << resultado.first << endl;
    cout << "Rota ótima: ";
    for (int cidade : resultado.second) {
        cout << cidades[cidade].id << " ";
    }
    cout << endl;

    return 0;
}
