# ATIVD

Análise e Tratamento de Imagens e Vídeos Digitais Trabalho Prático 1

## Alunos

- Girlana Souza
- Daniel Nunes

## Execução do Código

Para compilar o código em C++, é necessário a biblioteca [OpenCV](https://opencv.org/) instalada:

```bash
sudo apt install libopencv-dev
```

> A instalação e execução dos códigos foi feita no sistema Ubuntu 22.04

Para executar um código, é necessário usar o `g++` e apontar para a biblioteca OpenCV:

```bash
g++ <nome-do-código>.cpp -o <nome-da-saida>.out `pkg-config --cflags --libs opencv4`

# Exemplo:

g++ q1.cpp -o a.out \`pkg-config --cflags --libs opencv4`
```

Logo em seguida, executar o programa usando `./<nome-da-saida>.out`.

### Execução alternativa

Como alternativa, existe o script `executar.sh` que compila e executa um código que é passado como argumento:

```bash
chmod +x executar.sh

./executar.sh <nome-do-codigo>.cpp

# Exemplo

./executar.sh q1.cpp
```

> O conteúdo do script pode ser revisado, caso seja necessário.
> Também existe o compilar.sh que faz a mesma coisa, mas não executa

## Questões

### Informações gerais

Todas as operações pedidas nas questões devem ser implementadas pela equipe, ou seja, não é para utilizar procedimentos/funções disponíveis na biblioteca utilizada. Procedimentos para abertura de arquivos de imagens (jpeg, png, etc), gravar arquivos com as imagens resultantes, redução da quantização, estes procedimentos/funções podem e devem ser usados da biblioteca. Em caso de dúvida, pergunte ao professor. As imagens usadas no trabalho devem ser todas coloridas, no padrão RGB.

As operações dos itens 1, 2, 5, 6 e 7 devem gerar uma nova imagem como resultado (ou mais de uma imagem, conforme o caso). As operações 3 e 4 devem gerar um arquivo com o vetor de característica resultante. A questão 8 vai gerar novas imagens e arquivos com os vetores de características das imagens.

### Questão 01

Alteração de brilho. Um valor deve ser lido e passado por parâmetro para o procedimento de alteração de brilho da imagem.

### Questão 02

Imagem negativa (inversão de cores).

### Questão 03

Histograma global. Gerar um vetor para representar o histograma da imagem. Concatenar o histograma de cada banda RGB em um único vetor. Armazenar o resultado em um arquivo texto para facilitar a visualização do resultado.

### Questão 04

Histograma local. Defina um particionamento da imagem com no mínimo 3 partições. Concatenar os histogramas em um único vetor. Armazenar o resultado em um arquivo para facilitar a visualização do resultado.

### Questão 05

Quatro transformadas radiométricas:

1. Expansão de contraste linear
2. Compressão e expansão
3. Dente de serra
4. Transformada do logaritmo

### Questão 06

Quatro filtros espaciais:

1. Filtro da média
2. Filtro dos k vizinhos mais próximos
3. Filtro da mediana
4. Filtro da moda.

Comparar os resultados entre os 4 filtros implementados. Deve-se gerar imagens com ruído (tipicamente 10%) do tipo sal e pimenta para testes e apresentar as imagens resultantes.

### Questão 07

Um algoritmo de Detecção de bordas, qualquer técnica. Pode ser necessário reduzir a quantidade de cores (quantização). A quantização escolhida deve ficar entre 32 a 256 cores. A técnica de redução da quantização fica a sua escolha, podendo nesse caso, utilizar operações disponíveis na biblioteca OpenCV (ou a biblioteca da escolha da equipe).

### Questão 08

Extração de propriedades de cor usando o descritor BIC. Aproveita a redução da quantização da questão anterior. O resultado principal é um arquivo com os histogramas de pixels de borda e interior. Deve-se também gerar duas imagens, uma somente com os pixels de borda (com a cor original para os pixels de borda e os pixels de interior em branco) e outra com os pixels de interior (com a cor original para os pixels de interior e pixels de borda em branco).
