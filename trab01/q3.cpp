/*
Histograma global. Gerar um vetor para representar o histograma da imagem. Concatenar
o histograma de cada banda RGB em um único vetor. Armazenar o resultado em um arquivo
texto para facilitar a visualização do resultado.
*/

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para calcular o histograma de cada canal manualmente (R, G, B)
void calcularHistograma(const Mat& imagem, vector<int>& histograma_concatenado);

void salvarHistograma(const vector<int>& histograma, const string& nome_arquivo);

int main() {
    // Carregar a imagem
    Mat imagem = imread("input/apple.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Vetor para armazenar o histograma concatenado
    vector<int> histograma_concatenado;

    // Calcular o histograma global da imagem
    calcularHistograma(imagem, histograma_concatenado);

    // Salvar o histograma em um arquivo texto
    salvarHistograma(histograma_concatenado, "output/q3/histograma.txt");

    return 0;
}

// Cálculo do histograma
void calcularHistograma(const Mat& imagem, vector<int>& histograma_concatenado) {
    int num_bins = 256;  // Intensidades de 0 a 255
    vector<int> hist_r(num_bins, 0), hist_g(num_bins, 0), hist_b(num_bins, 0);

    // Iterar sobre cada pixel
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b pixel = imagem.at<Vec3b>(y, x);
            hist_r[pixel[2]]++;  // Canal R
            hist_g[pixel[1]]++;  // Canal G
            hist_b[pixel[0]]++;  // Canal B
        }
    }

    // Concatenar os histogramas
    for (int i = 0; i < num_bins; ++i) {
        histograma_concatenado.push_back(hist_r[i]);
        histograma_concatenado.push_back(hist_g[i]);
        histograma_concatenado.push_back(hist_b[i]);
    }
}

// Salvar o histograma em um arquivo
void salvarHistograma(const vector<int>& histograma, const string& nome_arquivo) {
    ofstream arquivo(nome_arquivo);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    // Gravar os valores no arquivo
    for (const auto& valor : histograma) {
        arquivo << valor << endl;
    }

    arquivo.close();
    cout << "Histograma salvo em " << nome_arquivo << endl;
}
