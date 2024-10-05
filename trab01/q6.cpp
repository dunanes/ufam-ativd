/*
Quatro filtros espaciais: (1) filtro da média, (2) filtro dos k vizinhos mais próximos, (3)
filtro da mediana e (4) filtro da moda. Comparar os resultados entre os 4 filtros
implementados. Deve-se gerar imagens com ruído (tipicamente 10%) do tipo sal e pimenta
para testes e apresentar as imagens resultantes.
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

using namespace cv;
using namespace std;

// Funções para os filtros
Mat filtroMedia(const Mat& imagem, int ksize);
Mat filtroKProximos(const Mat& imagem, int k);
Mat filtroMediana(const Mat& imagem, int ksize);
Mat filtroModa(const Mat& imagem, int ksize);

// Função para adicionar ruído sal e pimenta
Mat adicionarRuidoSalEPimenta(const Mat& imagem, float quantidade);

int main() {
    // Carregar a imagem original
    Mat imagem = imread("input/lenna.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Adicionar ruído sal e pimenta (10%)
    Mat imagem_ruidosa = adicionarRuidoSalEPimenta(imagem, 0.1);

    // Aplicar os filtros
    Mat img_media = filtroMedia(imagem_ruidosa, 3);          // Filtro da média
    Mat img_kproximos = filtroKProximos(imagem_ruidosa, 3);  // Filtro dos k vizinhos mais próximos
    Mat img_mediana = filtroMediana(imagem_ruidosa, 3);      // Filtro da mediana
    Mat img_moda = filtroModa(imagem_ruidosa, 3);            // Filtro da moda

    // Salvar as imagens filtradas
    imwrite("output/q6/imagem_ruidosa.jpg", imagem_ruidosa);
    imwrite("output/q6/filtro_media.jpg", img_media);
    imwrite("output/q6/filtro_kproximos.jpg", img_kproximos);
    imwrite("output/q6/filtro_mediana.jpg", img_mediana);
    imwrite("output/q6/filtro_moda.jpg", img_moda);

    return 0;
}

// Função para adicionar ruído sal e pimenta
Mat adicionarRuidoSalEPimenta(const Mat& imagem, float quantidade) {
    Mat imagem_ruidosa = imagem.clone();
    int total_pixels = imagem.rows * imagem.cols;
    int num_ruido = total_pixels * quantidade;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib_linha(0, imagem.rows - 1);
    uniform_int_distribution<int> distrib_coluna(0, imagem.cols - 1);

    // Adicionar sal (branco)
    for (int i = 0; i < num_ruido / 2; ++i) {
        int y = distrib_linha(gen);
        int x = distrib_coluna(gen);
        imagem_ruidosa.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
    }

    // Adicionar pimenta (preto)
    for (int i = 0; i < num_ruido / 2; ++i) {
        int y = distrib_linha(gen);
        int x = distrib_coluna(gen);
        imagem_ruidosa.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
    }

    return imagem_ruidosa;
}

// Filtro da média
Mat filtroMedia(const Mat& imagem, int ksize) {
    Mat resultado;
    blur(imagem, resultado, Size(ksize, ksize));  // Aplicar média
    return resultado;
}

// Filtro dos k vizinhos mais próximos
Mat filtroKProximos(const Mat& imagem, int k) {
    Mat resultado = imagem.clone();
    Mat imagem_cinza;
    cvtColor(imagem, imagem_cinza, COLOR_BGR2GRAY);

    for (int y = 1; y < imagem.rows - 1; y++) {
        for (int x = 1; x < imagem.cols - 1; x++) {
            vector<int> vizinhos;

            // Obter vizinhos 3x3
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    vizinhos.push_back(imagem_cinza.at<uchar>(y + j, x + i));
                }
            }

            // Ordenar e calcular média dos k menores valores
            sort(vizinhos.begin(), vizinhos.end());
            int soma = 0;
            for (int i = 0; i < k; i++) {
                soma += vizinhos[i];
            }
            resultado.at<Vec3b>(y, x) = Vec3b(soma / k, soma / k, soma / k);
        }
    }

    return resultado;
}

// Filtro da mediana
Mat filtroMediana(const Mat& imagem, int ksize) {
    Mat resultado;
    medianBlur(imagem, resultado, ksize);  // Aplicar mediana
    return resultado;
}

// Filtro da moda
Mat filtroModa(const Mat& imagem, int ksize) {
    Mat resultado = imagem.clone();

    for (int y = 1; y < imagem.rows - 1; y++) {
        for (int x = 1; x < imagem.cols - 1; x++) {
            vector<int> vizinhos;

            // Obter vizinhos 3x3
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    Vec3b pixel = imagem.at<Vec3b>(y + j, x + i);
                    vizinhos.push_back(pixel[0]);  // Usar apenas canal B
                }
            }

            // Calcular moda (valor mais frequente)
            int moda = vizinhos[0];
            int max_count = 1;
            for (int i = 0; i < vizinhos.size(); i++) {
                int count = 1;
                for (int j = i + 1; j < vizinhos.size(); j++) {
                    if (vizinhos[i] == vizinhos[j]) count++;
                }
                if (count > max_count) {
                    max_count = count;
                    moda = vizinhos[i];
                }
            }

            resultado.at<Vec3b>(y, x) = Vec3b(moda, moda, moda);  // Aplicar moda
        }
    }

    return resultado;
}
