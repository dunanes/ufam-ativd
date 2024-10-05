# Usei o Google Colab pra fazer os prints, só salvando aqui
# Upei o histograma.txt

import matplotlib.pyplot as plt
import numpy as np

# Função para ler o arquivo de histograma
def ler_histograma(arquivo_histograma):
    with open(arquivo_histograma, 'r') as arquivo:
        histograma = [int(linha.strip()) for linha in arquivo.readlines()]
    return histograma

# Função para plotar os histogramas locais
def plotar_histogramas_locais(histograma, num_bins=256):
    # O histograma contém 3 regiões, cada uma com 3 canais (R, G, B)
    num_regioes = 3
    tamanho_histograma = num_bins * 3  # Tamanho de um histograma completo por região

    # Separar os histogramas de cada região
    for i in range(num_regioes):
        hist_r = histograma[i*tamanho_histograma : i*tamanho_histograma + num_bins]
        hist_g = histograma[i*tamanho_histograma + num_bins : i*tamanho_histograma + 2*num_bins]
        hist_b = histograma[i*tamanho_histograma + 2*num_bins : i*tamanho_histograma + 3*num_bins]

        plt.figure(figsize=(12, 6))

        # Histograma do canal R
        plt.subplot(1, 3, 1)
        plt.bar(range(num_bins), hist_r, color='red')
        plt.title(f'Histograma - Canal R (Vermelho) - Região {i+1}')
        plt.xlabel('Intensidade')
        plt.ylabel('Número de Pixels')

        # Histograma do canal G
        plt.subplot(1, 3, 2)
        plt.bar(range(num_bins), hist_g, color='green')
        plt.title(f'Histograma - Canal G (Verde) - Região {i+1}')
        plt.xlabel('Intensidade')
        plt.ylabel('Número de Pixels')

        # Histograma do canal B
        plt.subplot(1, 3, 3)
        plt.bar(range(num_bins), hist_b, color='blue')
        plt.title(f'Histograma - Canal B (Azul) - Região {i+1}')
        plt.xlabel('Intensidade')
        plt.ylabel('Número de Pixels')

        plt.tight_layout()
        plt.show()

# Caminho para o arquivo gerado em C++
arquivo_histograma = 'histograma_local.txt'

# Ler o histograma do arquivo
histograma = ler_histograma(arquivo_histograma)

# Plotar os histogramas locais de cada região
plotar_histogramas_locais(histograma)
