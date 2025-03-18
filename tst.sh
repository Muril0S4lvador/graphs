#!/bin/bash

# Executa o programa compilado "main" em todos os arquivos dos diretórios passados

# Especifica o diretório raiz onde as pastas de entrada estão localizadas
root_folder="entradas"

# Nome do programa a ser executado
program="./main"

output_file="tabela.txt"

# Cria um arquivo vazio para as saídas consolidadas
> "$output_file"

# Deleta pastas txt e pasta out com informações antigas
delete="rm *.txt"
$delete

# Use o comando "find" para percorrer todos os arquivos em profundidade nas pastas "a," "b," e "e"

# Loop para percorrer as pastas "a," "b," e "e"

for folder in "$root_folder"/{a,p,b,e,f,m}; do
# for folder in "$root_folder"/vix; do
    # Verifica se a pasta existe
    if [ "$folder/" ]; then
        echo "Achado $folder/"
         for file in $folder/*.txt; do
        #  for file in $folder/*.vrp; do
            # echo "$file"
            if [ -f "$file" ]; then
               # echo "$file"
               echo "Executando $program com $file"
                $program "$file" >> "$output_file"
            fi
        done
    fi
done
