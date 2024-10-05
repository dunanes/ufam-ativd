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

## Outras informações

- Trocar a imagem padrão de todos os códigos para entrada.jpg
- Checar se as transformadas radiométricas ficam legais
- Adicionar seção dizendo sobre as imagens de entrada e saída em cada código e explicar o que cada um faz
- Adicionar o enunciado dentro de cada código e talvez no readme
- Terminar o relatório
