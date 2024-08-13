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

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Uso: " << argv[0] << " <sequencia de 0s e 1s>" << endl;
    return 1;
  }

  // Local do arquivo de entrada
  string nomeArquivo = "./Aula2_Complementares/mochila_100_1000_1";

  auto [capacidade, itens] = lerItens(nomeArquivo);

  if (itens.empty()) {
    cerr << "Nenhum item foi lido do arquivo." << endl;
    return 1;
  }

  string sequencia(argv[1]);
  if (sequencia.length() != itens.size()) {
    cerr << "A sequência de 0s e 1s deve ter o mesmo tamanho que o número de itens." << endl;
    return 1;
  }

  vector<int> solucaoInicial;
  for (char c : sequencia) {
    if (c == '0' || c == '1') {
      solucaoInicial.push_back(c - '0');
    } else {
      cerr << "A sequência deve conter apenas 0s e 1s." << endl;
      return 1;
    }
  }

  auto [valorInicial, pesoInicial] = calcularValorEPeso(itens, solucaoInicial);
  if (pesoInicial > capacidade) {
    cerr << "A solução inicial excede a capacidade da mochila." << endl;
    return 1;
  }

  int tamanhoTabu = 25; // tamanho da lista tabu
  int iteracoesMax = 1000; // número máximo de iterações
  int iteracoesSemMelhoraMax = 100; // número máximo de iterações sem melhora

  auto solucaoFinal = buscaTabu(capacidade, itens, solucaoInicial, tamanhoTabu, iteracoesMax, iteracoesSemMelhoraMax);

  cout << "Valor máximo obtido após busca tabu: " << solucaoFinal.first << endl;

  cout << "Itens selecionados: ";
  for (int sel : solucaoFinal.second) {
    cout << sel << " ";
  }
  cout << endl;

  return 0;
}
