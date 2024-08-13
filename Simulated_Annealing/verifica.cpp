#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Item {
    int valor;
    int peso;
};

vector<Item> lerItensDoArquivo(const string &nomeArquivo) {
    vector<Item> itens;
    ifstream arquivo(nomeArquivo);
    if (arquivo.is_open()) {
        int valor, peso;
        while (arquivo >> valor >> peso) {
            itens.push_back({valor, peso});
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir o arquivo." << endl;
        exit(1);
    }
    return itens;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <sequencia_binaria>" << endl;
        return 1;
    }

    // Ler os itens do arquivo
      vector<Item> itens = lerItensDoArquivo("./Aula2_Complementares/mochila_100_1000_1");

    // Ler a sequência binária da linha de comando
    string sequenciaBinaria = argv[1];
    if (sequenciaBinaria.length() != itens.size()) {
        cerr << "Erro: A sequência binária deve ter o mesmo tamanho que o número de itens." << endl;
        return 1;
    }

    int valorTotal = 0;
    int pesoTotal = 0;

    // Calcular o valor total e o peso total dos itens na mochila
    for (size_t i = 0; i < sequenciaBinaria.length(); ++i) {
        if (sequenciaBinaria[i] == '1') {
            valorTotal += itens[i].valor;
            pesoTotal += itens[i].peso;
            cout << "Item " << i + 1 << ": Valor = " << itens[i].valor << ", Peso = " << itens[i].peso << endl;
        }
    }

    cout << "Valor total na mochila: " << valorTotal << endl;
    cout << "Peso total na mochila: " << pesoTotal << endl;

    return 0;
}
