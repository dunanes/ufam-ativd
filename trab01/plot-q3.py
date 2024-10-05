# Usei o Google Colab pra fazer os prints, só salvando aqui
# Upei o histograma.txt

import matplotlib.pyplot as plt
import numpy as np

# Função para ler o arquivo de histograma
def ler_histograma(arquivo_histograma):
    with open(arquivo_histograma, 'r') as arquivo:
        histograma = [int(linha.strip()) for linha in arquivo.readlines()]
    return histograma

# Função para plotar os histogramas
def plotar_histogramas(histograma, num_bins=256):
    # Separar os histogramas de R, G, B
    hist_r = histograma[0:num_bins]
    hist_g = histograma[num_bins:2*num_bins]
    hist_b = histograma[2*num_bins:3*num_bins]

    # Criação de subplots para cada canal
    plt.figure(figsize=(12, 6))

    # Histograma do canal R
    plt.subplot(1, 3, 1)
    plt.bar(range(num_bins), hist_r, color='red')
    plt.title('Histograma - Canal R (Vermelho)')
    plt.xlabel('Intensidade')
    plt.ylabel('Número de Pixels')

    # Histograma do canal G
    plt.subplot(1, 3, 2)
    plt.bar(range(num_bins), hist_g, color='green')
    plt.title('Histograma - Canal G (Verde)')
    plt.xlabel('Intensidade')
    plt.ylabel('Número de Pixels')

    # Histograma do canal B
    plt.subplot(1, 3, 3)
    plt.bar(range(num_bins), hist_b, color='blue')
    plt.title('Histograma - Canal B (Azul)')
    plt.xlabel('Intensidade')
    plt.ylabel('Número de Pixels')

    plt.tight_layout()
    plt.show()

# Caminho para o arquivo gerado em C++
arquivo_histograma = 'histograma.txt'

# Ler o histograma do arquivo
histograma = ler_histograma(arquivo_histograma)

# Plotar os histogramas de R, G, B
plotar_histogramas(histograma)
