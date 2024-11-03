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
cortes_bic = []
frame_count = 0
ret, frame_ant = video.read()

# Nome do diretório de saída específico para o vídeo
nome_base = os.path.splitext(os.path.basename(nome_video))[0]
output_dir = os.path.join("cenas_bic", nome_base)
os.makedirs(output_dir, exist_ok=True)

print("Processamento iniciado...")

# Loop de processamento do vídeo
while True:
    ret, frame = video.read()
    if not ret:
        break

    if frame_count % salto_frames == 0:
        bic1_borda, bic1_int = bic(frame_ant)
        bic2_borda, bic2_int = bic(frame)
        similaridade_bic = compara_bic(bic1_borda, bic1_int, bic2_borda, bic2_int)

        if similaridade_bic < limiar:
            cortes_bic.append(frame_count / fps)
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
with open(f"{output_dir}/resultados_bic.txt", "w") as f:
    for corte in cortes_bic:
        minutos = int(corte / 60)
        segundos = corte % 60
        f.write(f"{minutos}:{segundos:.1f}\n")

print(f"Processamento concluído. Resultados salvos em '{output_dir}'")
