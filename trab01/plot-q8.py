import matplotlib.pyplot as plt
import numpy as np

# Função para ler o arquivo de histograma
def ler_histograma(arquivo_histograma):
    with open(arquivo_histograma, 'r') as arquivo:
        histograma = [int(linha.strip()) for linha in arquivo.readlines()]
    return histograma

# Função para plotar um histograma em separado
def plotar_histograma(histograma, titulo, cor, num_bins=64):
    plt.figure(figsize=(8, 6))
    plt.bar(range(num_bins), histograma, color=cor)
    plt.title(titulo)
    plt.xlabel('Intervalo de Cores (Quantização)')
    plt.ylabel('Número de Pixels')
    plt.tight_layout()
    plt.show()

# Caminhos para os arquivos de histograma de borda e interior
arquivo_histograma_borda = 'histograma_borda.txt'
arquivo_histograma_interior = 'histograma_interior.txt'

# Ler os histogramas dos arquivos
histograma_borda = ler_histograma(arquivo_histograma_borda)
histograma_interior = ler_histograma(arquivo_histograma_interior)

# Plotar o histograma de borda
plotar_histograma(histograma_borda, 'Histograma - Pixels de Borda', 'magenta')

# Plotar o histograma de interior
plotar_histograma(histograma_interior, 'Histograma - Pixels de Interior', 'cyan')
