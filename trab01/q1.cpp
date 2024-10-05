/*
Alteração de brilho. Um valor deve ser lido e passado por parâmetro para o procedimento
de alteração de brilho da imagem.
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para alterar o brilho manualmente
Mat alterarBrilho(const Mat& imagem, int valor_brilho);

int main() {
    Mat imagem = imread("input/rainbow.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    int valor_brilho;

    cout << "Digite o valor de brilho para alterar (-255 a 255): ";
    cin >> valor_brilho;

    // Alterar o brilho da imagem
    Mat imagem_resultado = alterarBrilho(imagem, valor_brilho);

    // Salvar a imagem
    imwrite("output/q1/imagem-brilho-200.jpg", imagem_resultado);

    return 0;
}

// Alterar o brilho manualmente
Mat alterarBrilho(const Mat& imagem, int valor_brilho) {
    Mat imagem_resultado = imagem.clone();  // Copiar a imagem original

    // Iterar sobre os pixels e ajustar o brilho
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b& pixel = imagem_resultado.at<Vec3b>(y, x);
            for (int c = 0; c < imagem.channels(); c++) {
                pixel[c] = saturate_cast<uchar>(pixel[c] + valor_brilho);
            }
        }
    }

    return imagem_resultado;  // Retornar a imagem com o brilho alterado
}