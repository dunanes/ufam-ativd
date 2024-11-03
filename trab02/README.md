# Trabalho Prático 2 - Análise e Tratamento de Imagens e Vídeos Digitais

**Disciplina**: Análise e Tratamento de Imagens e Vídeos Digitais  
**Instituto de Computação - Universidade Federal do Amazonas**

## Alunos

- Girlana Souza
- Daniel Nunes

## Objetivo

Este trabalho tem como objetivo a implementação de dois métodos para detecção de corte de cenas em vídeos digitais, utilizando os descritores:

1. **Histograma Local** com particionamento da imagem.
2. **BIC (Border/Interior Classification)**, aplicando o mesmo particionamento.

Cada técnica identifica os quadros onde ocorrem mudanças de cena e seleciona um quadro-chave para representar a nova cena. A acurácia dos resultados é comparada com a detecção manual.

## Estrutura do Projeto

O projeto inclui os seguintes scripts:

- `cenas_bic.py`: Detecta cenas usando o descritor BIC e salva quadros-chave e tempos dos cortes identificados.
- `cenas_histograma.py`: Detecta cenas usando o descritor de histograma local e salva quadros-chave e tempos dos cortes identificados.

Os resultados são organizados em pastas separadas para cada técnica (`cenas_bic` e `cenas_histograma`), com subpastas para cada vídeo processado.

## Pré-requisitos

O código utiliza a biblioteca [OpenCV](https://opencv.org/) para manipulação de vídeo e processamento de imagem.

Foi usado um ambiente Python para a execução:

```bash
# Para instalar os requirements (opencv e numpy)
pip install -r requirements.txt
```

## Execução dos Códigos

### Detectando Cenas com BIC

O código `cenas_bic.py` identifica mudanças de cena com o descritor BIC. Para executar:

```bash
python bic.py <nome-do-video>.mp4 <limiar>
```

Exemplo:

```bash
python bic.py gato.mp4 0.5
```

Esse comando processa o vídeo `gato.mp4` com um limiar de similaridade de `0.5` e salva os resultados em `cenas_bic/gato/`.

### Detectando Cenas com Histograma Local

O código `cenas_histograma.py` usa o descritor de histograma local para detectar mudanças de cena. Para executar:

```bash
python cenas_histograma.py <nome-do-video>.mp4 <limiar>
```

Exemplo:

```bash
python cenas_histograma.py gato.mp4 0.5
```

Esse comando processa o vídeo `gato.mp4` com um limiar de similaridade de `0.5` e salva os resultados em `cenas_histograma/gato/`.

> **Observação**: Ajuste o valor do limiar empiricamente para obter melhores resultados. Testes mostraram que valores entre `0.4` e `0.6` funcionam bem, dependendo do conteúdo do vídeo.

## Estrutura de Saída

Cada execução cria uma pasta para o vídeo processado, organizada da seguinte forma:

```txt
cenas_bic/
└── <nome-do-video>/
    ├── quadro_chave_0.jpg
    ├── quadro_chave_1.jpg
    └── resultados_bic.txt

cenas_histograma/
└── <nome-do-video>/
    ├── quadro_chave_0.jpg
    ├── quadro_chave_1.jpg
    └── resultados_histograma.txt
```

### Conteúdo dos Arquivos

- **quadros*chave*\*.jpg**: Imagens dos quadros-chave identificados após cada mudança de cena.
- **resultados_bic.txt** e **resultados_histograma.txt**: Arquivos de texto que registram os tempos (minutos e segundos) dos cortes identificados.

## Informações Adicionais

Os códigos foram testados em vídeos de diferentes categorias, como filmes, comerciais e desenhos animados, para avaliar a eficácia dos descritores em diferentes tipos de conteúdo.
