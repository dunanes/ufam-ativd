import cv2
import numpy as np
import os
import sys

# Configurações iniciais: nome do vídeo e limiar
if len(sys.argv) == 3:
    nome_video = sys.argv[1]
    limiar = float(sys.argv[2])
else:
    print("Erro: Passe o nome do vídeo como primeiro parâmetro.")
    print("Erro: Passe o limiar como segundo parâmetro.")
    sys.exit(1)

# Funções de processamento para BIC
def quantizacao(img):
    img_quantizada = np.zeros((len(img), len(img[0])), int)
    for i in range(len(img)):
        for j in range(len(img[0])):
            for x in range(3):
                img_quantizada[i][j] += (4**x) * (round(int(img[i][j][x]) * 3 / 255))
    return img_quantizada

def bic(img):
    quantizada = quantizacao(img)
    hist_borda = np.zeros(64)
    hist_interior = np.zeros(64)
    for i in range(1, len(quantizada)-1):
        for j in range(1, len(quantizada[0])-1):
            v1 = int(quantizada[i][j])
            if any(v1 != int(quantizada[i+di][j+dj]) for di, dj in [(-1, 0), (1, 0), (0, -1), (0, 1)]):
                hist_borda[v1] += 1
            else:
                hist_interior[v1] += 1
    return hist_borda, hist_interior

def compara_bic(hist1_borda, hist1_int, hist2_borda, hist2_int):
    return (compara_histogramas(hist1_borda, hist2_borda) + 
            compara_histogramas(hist1_int, hist2_int)) / 2

# Funções de processamento para histograma local
def histograma(img):
    hist = np.zeros(64)
    for i in range(len(img)):
        for j in range(len(img[0])):
            valor = img[i][j] // 4  # Reduz a escala de 256 para 64 níveis
            hist[valor] += 1
    return hist

def particiona_imagem(img, num_particoes=5):
    div_i, div_j = len(img) // num_particoes, len(img[0]) // num_particoes
    particoes = [img[i*div_i:(i+1)*div_i, j*div_j:(j+1)*div_j] 
                 for i in range(num_particoes) for j in range(num_particoes)]
    return [histograma(part) for part in particoes]

def compara_histogramas(hist1, hist2):
    soma1, soma2 = np.sum(hist1), np.sum(hist2)
    hist1_normalizado = hist1 / soma1 if soma1 != 0 else np.zeros_like(hist1)
    hist2_normalizado = hist2 / soma2 if soma2 != 0 else np.zeros_like(hist2)
    distance = np.sqrt(np.sum(hist1_normalizado * hist2_normalizado))
    return 1 - distance

# Função para verificar se o quadro é uma imagem de transição (preto ou branco)
def verifica_quadro_chave(frame, threshold=15):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    media_intensidade = np.mean(gray)
    return threshold < media_intensidade < (255 - threshold)

# Preparação do processamento
video = cv2.VideoCapture(nome_video)
fps = video.get(cv2.CAP_PROP_FPS)
salto_frames = int(fps)
cortes_bic = []
cortes_hist = []
frame_count = 0
ret, frame_ant = video.read()

# Diretórios de saída para BIC e histograma local
nome_base = os.path.splitext(os.path.basename(nome_video))[0]
output_dir_bic = os.path.join("cenas_bic", nome_base)
output_dir_hist = os.path.join("cenas_histograma", nome_base)
os.makedirs(output_dir_bic, exist_ok=True)
os.makedirs(output_dir_hist, exist_ok=True)

print("Processamento iniciado...")

# Loop de processamento do vídeo
while True:
    ret, frame = video.read()
    if not ret:
        break

    if frame_count % salto_frames == 0:
        frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        frame_ant_gray = cv2.cvtColor(frame_ant, cv2.COLOR_BGR2GRAY)

        # Processamento para BIC
        bic1_borda, bic1_int = bic(frame_ant)
        bic2_borda, bic2_int = bic(frame)
        similaridade_bic = compara_bic(bic1_borda, bic1_int, bic2_borda, bic2_int)

        if similaridade_bic < limiar and verifica_quadro_chave(frame):
            cortes_bic.append(frame_count / fps)
            cv2.imwrite(f"{output_dir_bic}/quadro_chave_{int(frame_count / fps)}.jpg", frame)

        # Processamento para Histograma Local
        hist_local1, hist_local2 = particiona_imagem(frame_ant_gray), particiona_imagem(frame_gray)
        similaridade_hist = min(compara_histogramas(hist1, hist2) for hist1, hist2 in zip(hist_local1, hist_local2))

        if similaridade_hist < limiar and verifica_quadro_chave(frame):
            cortes_hist.append(frame_count / fps)
            cv2.imwrite(f"{output_dir_hist}/quadro_chave_{int(frame_count / fps)}.jpg", frame)

        # Atualiza o quadro anterior
        frame_ant = frame.copy()
        
        # Exibe o progresso
        tempo_atual = frame_count / fps
        minutos = int(tempo_atual / 60)
        segundos = int(tempo_atual % 60)
        print(f"Processando: {minutos}m {segundos}s")

    frame_count += 1

video.release()

# Salva resultados em arquivos de texto
with open(f"{output_dir_bic}/resultados_bic.txt", "w") as f_bic:
    for corte in cortes_bic:
        minutos = int(corte / 60)
        segundos = corte % 60
        f_bic.write(f"{minutos}:{segundos:.1f}\n")

with open(f"{output_dir_hist}/resultados_histograma.txt", "w") as f_hist:
    for corte in cortes_hist:
        minutos = int(corte / 60)
        segundos = corte % 60
        f_hist.write(f"{minutos}:{segundos:.1f}\n")

print(f"Processamento concluído. Resultados salvos em '{output_dir_bic}' e '{output_dir_hist}'")
