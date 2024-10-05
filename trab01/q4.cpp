#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para calcular o histograma manualmente para uma imagem de uma região específica
void calcularHistograma(const Mat& regiao, vector<int>& histograma);

void salvarHistograma(const vector<int>& histograma, const string& nome_arquivo);

int main() {
    // Carregar a imagem de entrada (substitua pelo caminho correto)
    Mat imagem = imread("input/pokemon-trainer.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Definir partições (3 blocos verticais): esquerda, centro, direita
    int largura = imagem.cols / 3;  // Dividir a largura da imagem em 3 partes iguais

    // Definir as regiões (ROI - Region of Interest)
    Mat regiao_esquerda = imagem(Rect(0, 0, largura, imagem.rows));           // Região esquerda
    Mat regiao_centro = imagem(Rect(largura, 0, largura, imagem.rows));       // Região central
    Mat regiao_direita = imagem(Rect(2 * largura, 0, largura, imagem.rows));  // Região direita

    // Vetor para armazenar os histogramas concatenados das três regiões
    vector<int> histograma_concatenado;

    // Calcular e concatenar os histogramas das três regiões
    calcularHistograma(regiao_esquerda, histograma_concatenado);  // Histograma da região esquerda
    calcularHistograma(regiao_centro, histograma_concatenado);    // Histograma da região central
    calcularHistograma(regiao_direita, histograma_concatenado);   // Histograma da região direita

    // Salvar o histograma concatenado em um arquivo texto
    salvarHistograma(histograma_concatenado, "output/histograma_local.txt");

    return 0;
}

// Implementação manual para calcular o histograma de uma região específica (sem normalização)
void calcularHistograma(const Mat& regiao, vector<int>& histograma) {
    int num_bins = 256;                                                         // Intensidades variando de 0 a 255
    vector<int> hist_r(num_bins, 0), hist_g(num_bins, 0), hist_b(num_bins, 0);  // Histogramas para os três canais

    // Iterar sobre cada pixel da região
    for (int y = 0; y < regiao.rows; y++) {
        for (int x = 0; x < regiao.cols; x++) {
            Vec3b pixel = regiao.at<Vec3b>(y, x);  // Acessa o pixel (com três canais)
            hist_r[pixel[2]]++;                    // Contabiliza no histograma do canal R (vermelho)
            hist_g[pixel[1]]++;                    // Contabiliza no histograma do canal G (verde)
            hist_b[pixel[0]]++;                    // Contabiliza no histograma do canal B (azul)
        }
    }

    // Concatenar os histogramas de R, G, B em um único vetor
    for (int i = 0; i < num_bins; ++i) {
        histograma.push_back(hist_r[i]);  // Contagem de pixels no canal R
        histograma.push_back(hist_g[i]);  // Contagem de pixels no canal G
        histograma.push_back(hist_b[i]);  // Contagem de pixels no canal B
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
