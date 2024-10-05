#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para alterar o brilho manualmente
Mat alterarBrilho(const Mat& imagem, int valor_brilho);

int main() {
    Mat imagem = imread("input/pokemon-trainer.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    int valor_brilho;

    cout << "Digite o valor de brilho para alterar (-255 a 255): ";
    cin >> valor_brilho;

    // Alterar o brilho da imagem
    Mat imagem_resultado = alterarBrilho(imagem, valor_brilho);

    // Salvar a imagem resultante
    imwrite("output/imagem-brilho.jpg", imagem_resultado);

    return 0;
}

// Implementação manual para alterar o brilho
Mat alterarBrilho(const Mat& imagem, int valor_brilho) {
    Mat imagem_resultado = imagem.clone();  // Criar uma cópia da imagem original

    // * Não podia utilizar .convertTo(), porque é função pronta

    // imagem.convertTo(imagem_resultado, -1, 1, valor_brilho);

    // * Nova versão

    // Iterar sobre cada pixel da imagem
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            Vec3b& pixel = imagem_resultado.at<Vec3b>(y, x);  // Referência ao pixel atual
            for (int c = 0; c < imagem.channels(); c++) {
                pixel[c] = saturate_cast<uchar>(pixel[c] + valor_brilho);  // Ajustar o brilho do canal atual
            }
        }
    }

    return imagem_resultado;  // Retornar a imagem com o brilho alterado
}
