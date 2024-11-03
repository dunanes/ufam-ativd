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

# Funções de processamento
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

# Preparação do processamento
video = cv2.VideoCapture(nome_video)
fps = video.get(cv2.CAP_PROP_FPS)
salto_frames = int(fps)
cortes_hist = []
frame_count = 0
ret, frame_ant = video.read()

# Nome do diretório de saída específico para o vídeo
nome_base = os.path.splitext(os.path.basename(nome_video))[0]
output_dir = os.path.join("cenas_histograma", nome_base)
os.makedirs(output_dir, exist_ok=True)

print("Processamento iniciado...")

# Loop de processamento do vídeo
while True:
    ret, frame = video.read()
    if not ret:
        break

    if frame_count % salto_frames == 0:
        frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        frame_ant_gray = cv2.cvtColor(frame_ant, cv2.COLOR_BGR2GRAY)

        hist_local1, hist_local2 = particiona_imagem(frame_ant_gray), particiona_imagem(frame_gray)
        similaridade_hist = min(compara_histogramas(hist1, hist2) for hist1, hist2 in zip(hist_local1, hist_local2))

        if similaridade_hist < limiar:
            cortes_hist.append(frame_count / fps)
            # Salva o quadro-chave
            cv2.imwrite(f"{output_dir}/quadro_chave_{int(frame_count / fps)}.jpg", frame)
        
        frame_ant = frame.copy()
        
        # Exibe o progresso
        tempo_atual = frame_count / fps
        minutos = int(tempo_atual / 60)
        segundos = int(tempo_atual % 60)
        print(f"Processando: {minutos}m {segundos}s")

    frame_count += 1

video.release()

# Salva resultados em arquivo de texto
with open(f"{output_dir}/resultados_histograma.txt", "w") as f:
    for corte in cortes_hist:
        minutos = int(corte / 60)
        segundos = corte % 60
        f.write(f"{minutos}:{segundos:.1f}\n")

print(f"Processamento concluído. Resultados salvos em '{output_dir}'")
