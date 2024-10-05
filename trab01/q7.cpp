/*
Um algoritmo de Detecção de bordas, qualquer técnica. Pode ser necessário reduzir a
quantidade de cores (quantização). A quantização escolhida deve ficar entre 32 a 256
cores. A técnica de redução da quantização fica a sua escolha, podendo nesse caso, utilizar
operações disponíveis na biblioteca OpenCV (ou a biblioteca da escolha da equipe).
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para quantização de cores usando K-means
Mat quantizacaoCores(const Mat& imagem, int k);

// Função para detecção de bordas (usando Sobel)
Mat detectarBordas(const Mat& imagem);

int main() {
    // Carregar a imagem
    Mat imagem = imread("input/apple.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Quantizar cores (ex: 64 cores)
    Mat imagem_quantizada = quantizacaoCores(imagem, 64);

    // Detectar bordas
    Mat imagem_bordas = detectarBordas(imagem_quantizada);

    // Salvar resultado com bordas em rosa choque
    imwrite("output/q7/imagem_bordas_rosa.jpg", imagem_bordas);

    return 0;
}

// Quantização de cores usando K-means
Mat quantizacaoCores(const Mat& imagem, int k) {
    Mat imagem_quantizada;
    Mat amostras = imagem.reshape(1, imagem.rows * imagem.cols);
    amostras.convertTo(amostras, CV_32F);

    // K-means para agrupar cores
    TermCriteria criterio(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0);
    Mat labels, centros;
    kmeans(amostras, k, labels, criterio, 3, KMEANS_PP_CENTERS, centros);

    // Reconstruir imagem quantizada
    Mat nova_imagem(imagem.size(), imagem.type());
    for (int i = 0; i < nova_imagem.rows * nova_imagem.cols; i++) {
        nova_imagem.at<Vec3b>(i / nova_imagem.cols, i % nova_imagem.cols) = Vec3b(
            centros.at<float>(labels.at<int>(i), 0),
            centros.at<float>(labels.at<int>(i), 1),
            centros.at<float>(labels.at<int>(i), 2));
    }

    return nova_imagem;
}

// Detecção de bordas usando Sobel
Mat detectarBordas(const Mat& imagem) {
    Mat imagem_cinza, grad_x, grad_y, grad;

    // Converter para escala de cinza
    cvtColor(imagem, imagem_cinza, COLOR_BGR2GRAY);

    // Aplicar Sobel para gradientes
    Sobel(imagem_cinza, grad_x, CV_16S, 1, 0, 3);
    Sobel(imagem_cinza, grad_y, CV_16S, 0, 1, 3);

    // Converter para valor absoluto e somar gradientes
    Mat abs_grad_x, abs_grad_y;
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    // Criar imagem com bordas rosa choque (fundo preto)
    Mat imagem_bordas = Mat::zeros(imagem.size(), CV_8UC3);

    // Pintar as bordas
    for (int y = 0; y < grad.rows; y++) {
        for (int x = 0; x < grad.cols; x++) {
            if (grad.at<uchar>(y, x) > 50) {                         // Limiar de bordas
                imagem_bordas.at<Vec3b>(y, x) = Vec3b(255, 0, 255);  // Rosa choque
            }
        }
    }

    return imagem_bordas;
}
