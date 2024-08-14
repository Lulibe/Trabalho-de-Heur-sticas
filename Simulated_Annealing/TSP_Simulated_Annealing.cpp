#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;

struct Cidade {
    int id;
    double x, y;
};

double calcularDistancia(const Cidade &a, const Cidade &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double calcularDistanciaTotal(const vector<int> &rota, const vector<vector<double>> &matrizDistancias) {
    double distanciaTotal = 0.0;
    for (size_t i = 0; i < rota.size() - 1; ++i) {
        distanciaTotal += matrizDistancias[rota[i]][rota[i + 1]];
    }
    distanciaTotal += matrizDistancias[rota.back()][rota[0]]; 
    return distanciaTotal;
}

vector<int> gerarVizinha(const vector<int> &rota) {
    vector<int> novaRota = rota;
    int i = rand() % rota.size();
    int j = rand() % rota.size();
    swap(novaRota[i], novaRota[j]);
    return novaRota;
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));

    if (argc < 5) {
        cerr << "Uso: " << argv[0] << " <temperaturaInicial> <temperaturaMinima> <fatorResfriamento> <iteracoesPorTemperatura>" << endl;
        return 1;
    }

    // Parâmetros do Simulated Annealing recebidos como argumentos de linha de comando
    double temperaturaInicial = stod(argv[1]);
    double temperaturaMinima = stod(argv[2]);
    double fatorResfriamento = stod(argv[3]);
    int iteracoesPorTemperatura = stoi(argv[4]);

    vector<Cidade> cidades;
    ifstream arquivo("./Aula2_Complementares/tsp_51");
    if (arquivo.is_open()) {
        int id;
        double x, y;
        while (arquivo >> id >> x >> y) {
            cidades.push_back({id, x, y});
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    size_t numCidades = cidades.size();
    vector<vector<double>> matrizDistancias(numCidades, vector<double>(numCidades, 0.0));
    for (size_t i = 0; i < numCidades; ++i) {
        for (size_t j = 0; j < numCidades; ++j) {
            matrizDistancias[i][j] = calcularDistancia(cidades[i], cidades[j]);
        }
    }

    vector<int> rota(numCidades);
    for (size_t i = 0; i < numCidades; ++i) {
        rota[i] = i;
    }
    random_shuffle(rota.begin(), rota.end());
    double custoAtual = calcularDistanciaTotal(rota, matrizDistancias);

    double temperatura = temperaturaInicial;

    while (temperatura > temperaturaMinima) {
        for (int i = 0; i < iteracoesPorTemperatura; ++i) {
            vector<int> novaRota = gerarVizinha(rota);
            double novoCusto = calcularDistanciaTotal(novaRota, matrizDistancias);
            double deltaCusto = novoCusto - custoAtual;

            if (deltaCusto < 0 || ((double) rand() / RAND_MAX) < exp(-deltaCusto / temperatura)) {
                rota = novaRota;
                custoAtual = novoCusto;
            }
        }
        temperatura *= fatorResfriamento;
    }

    cout << "Rota ótima encontrada: ";
    for (int cidade : rota) {
        cout << cidades[cidade].id << " ";
    }
    cout << endl;
    cout << "Custo da rota ótima: " << custoAtual << endl;

    return 0;
}
