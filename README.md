# ♻️ Waste Collection Vehicle Routing Problem (WCVRP)

Este repositório contém a implementação e estudo do Problema de Roteamento de Veículos de Coleta de Lixo (WCVRP - _Waste Collection Vehicle Routing Problem_), abordando técnicas de otimização para a coleta seletiva de resíduos recicláveis na cidade de Vitória.

## 📌 Sobre o Projeto

O WCVRP é um problema clássico de otimização combinatória, onde o objetivo é criar rotas eficientes para que caminhões de lixo atendam diversos pontos de coleta com o menor custo possível. Neste estudo, utilizamos métodos consagrados na literatura para construção e refinamento das soluções, visando tornar a gestão de resíduos sólidos recicláveis mais eficiente.

Este trabalho foi desenvolvido no **LABOTIM (Laboratório de Otimização e Modelagem Matemática - UFES)** entre **setembro de 2023 e agosto de 2024**, sob orientação da **Prof.ª Maria Claudia Silva Boeres**. O relatório final pode ser acessado em: [Relatório Final](https://anaisjornadaic.sappg.ufes.br/desc.php?&id=22441).

### 🔍 Técnicas Utilizadas

Para resolver o problema, foram aplicadas as seguintes técnicas:

- **Algoritmo de Economias**: Método construtivo para gerar soluções iniciais eficientes.
- **Variable Neighborhood Search (VNS)**: Meta-heurística para aprimoramento das soluções.
- **Variable Neighborhood Descent (VND)**: Heurística de busca local para refinar as rotas geradas.

## 🏗️ Estrutura do Repositório

- **Vector/**: Estrutura de vetor (array) dinâmico
  - `vector.c`, `vector.h`
- **adjacency_matrix/**: Estrutura de matriz de adjacências (representação do grafo)
  - `matrix.c`, `matrix.h`
- **algorithms/**: Algoritmos de otimização utilizados
  - `algorithms.c`, `algorithms.h`
- **data/**: Dados que cada vértice armazena
  - `data.c`, `data.h`
- **graph/**: Implementação do grafo da instância
  - `graph.c`, `graph.h`
- **graphviz_print/**: Funções para visualização com Graphviz
  - `graphviz_print.c`, `graphviz_print.h`
- **info/**: Estatísticas de execução para métricas de desempenho
  - `info.c`, `info.h`
- **route/**: Manipulação das rotas criadas
  - `route.c`, `route.h`
- **union_find/**: Algoritmo para criação da Minimum Spanning Tree (MST)
  - `union_find.c`, `union_find.h`
- **tst.sh**: Script Bash que executa todas as instâncias automaticamente a partir de pastas predefinidas

## 🚀 Como Executar

### Pré-requisitos

- Compilador C (gcc, clang, etc.)
- Sistema operacional Linux ou Windows com WSL
- **Graphviz** (para visualização de grafos)

### Passos para Compilação e Execução

1. Clone o repositório:
   ```bash
   git clone https://github.com/Muril0S4lvador/graphs.git
   cd graphs
   ```
2. Instale o Graphviz (se ainda não estiver instalado):
   ```bash
   sudo apt-get install graphviz  # Para Debian/Ubuntu
   ```
3. Compile o código:
   ```bash
   make
   ```
4. Execute o programa:
   ```bash
   ./main entradas/a/A-n32-k5.vrp 1
   ```
5. Para rodar todas as instâncias automaticamente:
   ```bash
   ./tst.sh
   ```

## 📊 Resultados e Conclusões

Os experimentos mostraram que as técnicas aplicadas geraram soluções satisfatórias em um tempo de execução razoável. A otimização das rotas pode contribuir significativamente para a redução de custos operacionais e para a melhoria da eficiência da coleta seletiva.

## 📬 Contato

Caso tenha dúvidas ou sugestões, entre em contato:
📧 Email: murilo.salvador95@gmail.com
