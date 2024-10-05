#!/bin/bash

# Verifica se o nome do arquivo foi passado como argumento
if [ -z "$1" ]; then
    echo "Uso: $0 nome_do_arquivo.cpp"
    exit 1
fi

# Compilando o código com g++ e salvando a saída em a.out
g++ "$1" -o a.out `pkg-config --cflags --libs opencv4`

# Verifica se a compilação foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "Compilação concluída com sucesso. Executando ./a.out"
    ./a.out
else
    echo "Erro na compilação."
    exit 1
fi
