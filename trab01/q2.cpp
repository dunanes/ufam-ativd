/*
Imagem negativa (inversão de cores).
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para inverter as cores da imagem manualmente (imagem negativa)
Mat inverterCores(const Mat& imagem);

int main() {
    Mat imagem = imread("input/rainbow.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Chama a função para inverter as cores da imagem
    Mat imagem_negativa = inverterCores(imagem);

    // Salva a imagem
    imwrite("output/q2/imagem_negativa.jpg", imagem_negativa);

    return 0;
}

// Implementação manual para inverter as cores
Mat inverterCores(const Mat& imagem) {
    Mat imagem_negativa = imagem.clone();  // Cria uma cópia da imagem original

    // * Também não pode usar 😭
    // bitwise_not(imagem, imagem_negativa);

    // Itera sobre cada pixel da imagem
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b& pixel = imagem_negativa.at<Vec3b>(y, x);  // Referência ao pixel atual

            // Inverte cada canal de cor (R, G, B) subtraindo o valor de 255
            for (int c = 0; c < 3; c++) {
                pixel[c] = 255 - pixel[c];
            }
        }
    }

    return imagem_negativa;  // Retorna a imagem negativa
}
