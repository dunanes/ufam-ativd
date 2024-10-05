#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para calcular o histograma de cada canal manualmente (R, G, B)
void calcularHistograma(const Mat& imagem, vector<int>& histograma_concatenado);

void salvarHistograma(const vector<int>& histograma, const string& nome_arquivo);

int main() {
    // Carregar a imagem (substitua o caminho pelo correto)
    Mat imagem = imread("input/pokemon-trainer.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Vetor para armazenar o histograma concatenado (R, G, B)
    vector<int> histograma_concatenado;

    // Calcular o histograma global da imagem
    calcularHistograma(imagem, histograma_concatenado);

    // Salvar o histograma em um arquivo texto
    salvarHistograma(histograma_concatenado, "output/histograma.txt");

    return 0;
}

// Implementação manual do cálculo do histograma (sem normalização)
void calcularHistograma(const Mat& imagem, vector<int>& histograma_concatenado) {
    int num_bins = 256;                                                         // Intensidades variando de 0 a 255
    vector<int> hist_r(num_bins, 0), hist_g(num_bins, 0), hist_b(num_bins, 0);  // Histogramas para os três canais

    // * Modifiquei pra não usar a função da biblioteca e tirei a normalização
    // * acho que a visualização fica melhor com inteiros em vez de float (posso estar errado)

    // Iterar sobre cada pixel da imagem
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b pixel = imagem.at<Vec3b>(y, x);  // Acessa o pixel (com três canais)
            hist_r[pixel[2]]++;                    // Contabiliza no histograma do canal R (vermelho)
            hist_g[pixel[1]]++;                    // Contabiliza no histograma do canal G (verde)
            hist_b[pixel[0]]++;                    // Contabiliza no histograma do canal B (azul)
        }
    }

    // Concatenar os histogramas de R, G, B em um único vetor (sem normalizar)
    for (int i = 0; i < num_bins; ++i) {
        histograma_concatenado.push_back(hist_r[i]);  // Contagem de pixels no canal R
        histograma_concatenado.push_back(hist_g[i]);  // Contagem de pixels no canal G
        histograma_concatenado.push_back(hist_b[i]);  // Contagem de pixels no canal B
    }
}

// Função para salvar o histograma em um arquivo texto
void salvarHistograma(const vector<int>& histograma, const string& nome_arquivo) {
    ofstream arquivo(nome_arquivo);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo para salvar o histograma!" << endl;
        return;
    }

    // Grava os valores do histograma no arquivo, uma linha para cada valor
    for (const auto& valor : histograma) {
        arquivo << valor << endl;
    }

    arquivo.close();
    cout << "Histograma salvo em " << nome_arquivo << endl;
}
