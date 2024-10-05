/*
Extração de propriedades de cor usando o descritor BIC. Aproveita a redução da
quantização da questão anterior. O resultado inclui histogramas de pixels de borda e interior,
e imagens separadas de borda e interior.
*/

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para aplicar quantização de cores (K-means)
Mat quantizacaoCores(const Mat& imagem, int k);

// Função para classificar pixels como borda ou interior e extrair os histogramas
void classificarPixels(const Mat& imagem, Mat& imagem_borda, Mat& imagem_interior, vector<int>& hist_borda, vector<int>& hist_interior);

int main() {
    // Carregar a imagem original
    Mat imagem = imread("input/lenna.jpg");

    if (imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Quantizar cores (ex: 64 cores)
    Mat imagem_quantizada = quantizacaoCores(imagem, 64);

    // Criar as imagens de borda e interior, e seus histogramas
    Mat imagem_borda = Mat::zeros(imagem.size(), imagem.type());
    Mat imagem_interior = Mat::zeros(imagem.size(), imagem.type());
    vector<int> hist_borda(64, 0);     // Histograma dos pixels de borda
    vector<int> hist_interior(64, 0);  // Histograma dos pixels de interior

    // Classificar os pixels entre borda e interior e preencher os histogramas
    classificarPixels(imagem_quantizada, imagem_borda, imagem_interior, hist_borda, hist_interior);

    // Salvar as imagens de borda e interior
    imwrite("output/q8/imagem_borda.jpg", imagem_borda);
    imwrite("output/q8/imagem_interior.jpg", imagem_interior);

    // Salvar os histogramas
    ofstream arquivo_hist_borda("output/q8/histograma_borda.txt");
    ofstream arquivo_hist_interior("output/q8/histograma_interior.txt");

    for (int i = 0; i < 64; i++) {
        arquivo_hist_borda << hist_borda[i] << endl;
        arquivo_hist_interior << hist_interior[i] << endl;
    }

    arquivo_hist_borda.close();
    arquivo_hist_interior.close();

    cout << "Imagens e histogramas salvos com sucesso!" << endl;

    return 0;
}

// Quantização de cores usando K-means
Mat quantizacaoCores(const Mat& imagem, int k) {
    Mat amostras = imagem.reshape(1, imagem.rows * imagem.cols);  // Reshape para uma linha por pixel
    amostras.convertTo(amostras, CV_32F);                         // Converter para float para K-means

    // Aplicar K-means
    TermCriteria criterio(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0);
    Mat labels, centros;
    kmeans(amostras, k, labels, criterio, 3, KMEANS_PP_CENTERS, centros);

    // Reconstruir a imagem quantizada
    Mat nova_imagem(imagem.size(), imagem.type());
    for (int i = 0; i < nova_imagem.rows * nova_imagem.cols; i++) {
        nova_imagem.at<Vec3b>(i / nova_imagem.cols, i % nova_imagem.cols) = Vec3b(
            centros.at<float>(labels.at<int>(i), 0),
            centros.at<float>(labels.at<int>(i), 1),
            centros.at<float>(labels.at<int>(i), 2));
    }

    return nova_imagem;
}

// Classificar os pixels como borda ou interior
void classificarPixels(const Mat& imagem, Mat& imagem_borda, Mat& imagem_interior, vector<int>& hist_borda, vector<int>& hist_interior) {
    int num_clusters = 64;

    // Classificar os pixels com base na diferença com os vizinhos (janela 3x3)
    for (int y = 1; y < imagem.rows - 1; y++) {
        for (int x = 1; x < imagem.cols - 1; x++) {
            Vec3b pixel = imagem.at<Vec3b>(y, x);
            bool is_borda = false;

            // Comparar com os vizinhos (janela 3x3)
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    if (j == 0 && i == 0) continue;  // Ignorar o próprio pixel
                    Vec3b vizinho = imagem.at<Vec3b>(y + j, x + i);
                    if (norm(pixel - vizinho) > 40) {  // Se houver uma diferença significativa (limite = 40)
                        is_borda = true;
                        break;
                    }
                }
                if (is_borda) break;
            }

            // Adicionar ao histograma de borda ou de interior
            int cluster_index = (int)(pixel[0] / 64) * 16 + (int)(pixel[1] / 64) * 4 + (int)(pixel[2] / 64);  // Índice no histograma

            if (is_borda) {
                imagem_borda.at<Vec3b>(y, x) = pixel;                    // Manter a cor original nas bordas
                imagem_interior.at<Vec3b>(y, x) = Vec3b(255, 255, 255);  // Pixels de borda são brancos na imagem de interior
                hist_borda[cluster_index]++;
            } else {
                imagem_interior.at<Vec3b>(y, x) = pixel;              // Manter a cor original no interior
                imagem_borda.at<Vec3b>(y, x) = Vec3b(255, 255, 255);  // Pixels de interior são brancos na imagem de borda
                hist_interior[cluster_index]++;
            }
        }
    }
}
