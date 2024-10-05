#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Funções para as transformações radiométricas
Mat expansaoContrasteLinear(const Mat& imagem);
Mat compressaoExpansao(const Mat& imagem, float gamma);
Mat denteDeSerra(const Mat& imagem);
Mat transformadaLogaritmica(const Mat& imagem);

int main() {
    // Carregar a imagem de entrada
    Mat imagem = imread("input/apple.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Aplicar as transformações
    Mat img_contraste = expansaoContrasteLinear(imagem);
    Mat img_compressao_expansao = compressaoExpansao(imagem, 0.5);  // Compressão (gamma < 1)
    Mat img_dente_serra = denteDeSerra(imagem);
    Mat img_logaritmica = transformadaLogaritmica(imagem);

    // Salvar as imagens resultantes
    imwrite("output/q5/expansao_contraste.jpg", img_contraste);
    imwrite("output/q5/compressao_expansao.jpg", img_compressao_expansao);
    imwrite("output/q5/dente_de_serra.jpg", img_dente_serra);
    imwrite("output/q5/transformada_logaritmica.jpg", img_logaritmica);

    return 0;
}

// 1. Expansão de Contraste Linear
Mat expansaoContrasteLinear(const Mat& imagem) {
    Mat resultado = imagem.clone();
    int minVal = 255, maxVal = 0;

    // Encontrar os valores mínimo e máximo de intensidade na imagem
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b pixel = imagem.at<Vec3b>(y, x);
            for (int c = 0; c < 3; c++) {
                if (pixel[c] < minVal) minVal = pixel[c];
                if (pixel[c] > maxVal) maxVal = pixel[c];
            }
        }
    }

    // Aplicar a transformação linear
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b& pixel = resultado.at<Vec3b>(y, x);
            for (int c = 0; c < 3; c++) {
                pixel[c] = saturate_cast<uchar>(255 * (pixel[c] - minVal) / (float)(maxVal - minVal));
            }
        }
    }

    return resultado;
}

// 2. Compressão e Expansão com a função de Gamma
Mat compressaoExpansao(const Mat& imagem, float gamma) {
    Mat resultado = imagem.clone();

    // Aplicar a compressão ou expansão usando a fórmula I' = I^gamma
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b& pixel = resultado.at<Vec3b>(y, x);
            for (int c = 0; c < 3; c++) {
                pixel[c] = saturate_cast<uchar>(pow(pixel[c] / 255.0, gamma) * 255.0);
            }
        }
    }

    return resultado;
}

// 3. Transformada Dente de Serra
Mat denteDeSerra(const Mat& imagem) {
    Mat resultado = imagem.clone();
    int periodo = 50;  // Define o período da função dente de serra

    // Aplicar a função de dente de serra
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b& pixel = resultado.at<Vec3b>(y, x);
            for (int c = 0; c < 3; c++) {
                pixel[c] = (pixel[c] % periodo) * (255 / periodo);  // Ajusta o valor com base no período
            }
        }
    }

    return resultado;
}

// 4. Transformada Logarítmica
Mat transformadaLogaritmica(const Mat& imagem) {
    Mat resultado = imagem.clone();
    float constante = 255 / log(1 + 255);  // Constante de normalização para ajustar a intensidade

    // Aplicar a transformação logarítmica
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b& pixel = resultado.at<Vec3b>(y, x);
            for (int c = 0; c < 3; c++) {
                pixel[c] = saturate_cast<uchar>(constante * log(1 + pixel[c]));
            }
        }
    }

    return resultado;
}
