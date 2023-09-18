# 🛠️ Grafos 🛠️ (DESATUALIZADO)

Este repositório foi criado com a finalidade de estudar grafos e implementar suas representações.

## Representações Implementadas

- Lista de Adjacências
- Matriz de Adjacências

## Lista de Adjacências

A implementação dessa estrutura é projetada para ser aproveitada por grafos direcionados ou não. Abaixo estão os principais pontos sobre a minha representação de Lista de Adjacências:

- É considerado um tamanho fixo de vértices.
- A manipulação dos dados de cada vértice não está considerada no código, mas ele foi desenvolvido de forma a facilitar a adição dessa funcionalidade no futuro.

## Matriz de Adjacências

A implementação dessa estrutura é projetada para ser aproveitada por grafos direcionados ou não. Abaixo estão os principais pontos sobre a minha representação de Matriz de Adjacências:

- É considerado um tamanho fixo de vértices.
- A manipulação dos dados de cada vértice não está considerada no código, mas recomendo a adição de um vector para salvar os dados e retorná-los em tempo constante.

## 🛠️ Como Abrir e Executar o Projeto

Dentro de cada pasta, você encontrará um arquivo `main` pronto para ser utilizado. Para testar o código, siga os passos abaixo:

1. Abra o terminal e copie o diretorio.

   ```bash
   git clone https://github.com/Muril0S4lvador/graphs.git

2. Ande até uma das pastas do projeto

3. Compile o projeto executando o seguinte comando:

   ```bash
   make

4. Em seguida, execute o programa com o comando:

   ```bash
   ./main


# DESATUALIZADO!!

Antes eu tinha 2 mains p cada implementação. Consegui reduzir pra uma, mas pra selecionar a implementação precisa mexer no graph.h nos defines.

Adiciona x, y e damanda do vertice em um vetor. Ainda nao resolvi onde guardar esses dados na struct, então coloquei uma estrutura nova para guardar todos os vertices
Acho que ate daria para colocar na struct de cada implementação mas n sei se ia ficar extremamente mais custoso doq usando um vector.
Arquivo de texto é lido por passagem (lê o nome e a capacidade do caminhão, mas não salva em nenhum lugar).

```bash
./main < file.txt
