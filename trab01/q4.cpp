/*
Histograma local. Defina um particionamento da imagem com no mínimo 3 partições.
Concatenar os histogramas em um único vetor. Armazenar o resultado em um arquivo para
facilitar a visualização do resultado.
*/

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para calcular o histograma de uma região
void calcularHistograma(const Mat& regiao, vector<int>& histograma);

void salvarHistograma(const vector<int>& histograma, const string& nome_arquivo);

int main() {
    // Carregar a imagem
    Mat imagem = imread("input/apple.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Definir partições (3 blocos verticais)
    int largura = imagem.cols / 3;

    // Definir as regiões
    Mat regiao_esquerda = imagem(Rect(0, 0, largura, imagem.rows));
    Mat regiao_centro = imagem(Rect(largura, 0, largura, imagem.rows));
    Mat regiao_direita = imagem(Rect(2 * largura, 0, largura, imagem.rows));

    // Vetor para armazenar os histogramas
    vector<int> histograma_concatenado;

    // Calcular os histogramas das três regiões
    calcularHistograma(regiao_esquerda, histograma_concatenado);
    calcularHistograma(regiao_centro, histograma_concatenado);
    calcularHistograma(regiao_direita, histograma_concatenado);

    // Salvar o histograma concatenado
    salvarHistograma(histograma_concatenado, "output/q4/histograma_local.txt");

    return 0;
}

// Calcular o histograma de uma região
void calcularHistograma(const Mat& regiao, vector<int>& histograma) {
    int num_bins = 256;
    vector<int> hist_r(num_bins, 0), hist_g(num_bins, 0), hist_b(num_bins, 0);

    // Iterar sobre os pixels da região
    for (int y = 0; y < regiao.rows; y++) {
        for (int x = 0; x < regiao.cols; x++) {
            Vec3b pixel = regiao.at<Vec3b>(y, x);
            hist_r[pixel[2]]++;
            hist_g[pixel[1]]++;
            hist_b[pixel[0]]++;
        }
    }

    // Concatenar os histogramas de R, G, B
    for (int i = 0; i < num_bins; ++i) {
        histograma.push_back(hist_r[i]);
        histograma.push_back(hist_g[i]);
        histograma.push_back(hist_b[i]);
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
