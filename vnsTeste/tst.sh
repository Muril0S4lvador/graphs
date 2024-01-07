#!/bin/bash

# Especifica o diretório raiz onde as pastas de entrada estão localizadas
root_folder="entradas"

# Nome do programa a ser executado
program="./main"

output_file="Results.csv"

# Cria um arquivo vazio para as saídas consolidadas
> "$output_file"
echo "Nome;Cost Before VNS;Possibility;Cost After VNS;Possibility;Optimal cost;Optimal distance Before VNS;Optimal distance After VNS" >> "$output_file"
echo "Diretório atual: $(pwd)"


# Use o comando "find" para percorrer todos os arquivos em profundidade nas pastas "a," "b," e "e"


# Loop para percorrer as pastas "a," "b," e "e"
for folder in "$root_folder"/{a,b,e,f}; do
    # Verifica se a pasta existe
    if [ "$folder/" ]; then
        echo "Achado $folder/"
         for file in $folder/*.vrp; do
            echo "$file"
            if [ -f "$file" ]; then
                echo "$file"
                echo "Executando $program com $file"
                $program "$file" >> "$output_file"
            fi
        done
    fi
done
