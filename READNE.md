# BCC6001 — Algoritmos em Grafos
## Prática 1 — Análise de Grafos

**Professora:** Profª. Drª. Aretha Barbosa Alencar  
**Instituição:** Universidade Tecnológica Federal do Paraná (UTFPR)  
**Campus:** Campo Mourão

## Objetivo

Dado um grafo não dirigido de entrada, calcular estatísticas utilizando algoritmos apresentados em aula.

## 1. Dados de Entrada — Formato GEXF

O formato **GEXF (Graph Exchange XML Format)** é um formato baseado em XML utilizado para representar grafos.

Referências sobre o formato:
- [GEXF — Basic Format](https://gexf.net/basic.html)
- [Site oficial do GEXF](https://gexf.net/)

### Requisitos

- Implementar a leitura de um grafo armazenado em um arquivo no formato `.gexf`.
- Armazenar o grafo em uma estrutura de dados que represente um grafo não dirigido.
- Utilizar o grafo de teste disponibilizado no Moodle da disciplina.
- Considerar que o grafo de entrada é conexo.

## 2. Eccentricity (Excentricidade)

Pesquisar e implementar um algoritmo que calcule a **excentricidade de cada vértice** do grafo.

### Requisitos

- Calcular a excentricidade de cada vértice.
- Documentar, nos comentários do código, o significado dessa estatística.
- Imprimir a excentricidade de cada vértice como saída de texto.

## 3. Closeness Centrality (Centralidade de Proximidade)

Pesquisar e implementar um algoritmo que calcule a **versão normalizada da closeness centrality de cada vértice**.

### Requisitos

- Calcular a closeness centrality normalizada de cada vértice.
- Documentar, nos comentários do código, o significado dessa estatística.
- Imprimir a closeness centrality normalizada de cada vértice como saída de texto.

## 4. Observações

- Podem ser utilizados algoritmos apresentados em aula.
- O código pode ser implementado em qualquer linguagem de programação.
- O uso de **orientação a objetos é obrigatório**.

## 5. Avaliação

| Critério | Pontuação |
|---|---:|
| Leitura dos dados de entrada no formato GEXF | 3,0 pontos |
| Implementação da excentricidade (*Eccentricity*) | 3,5 pontos |
| Implementação da *Closeness Centrality* normalizada | 3,5 pontos |
| **Total** | **10,0 pontos** |

## 6. Referencial Teórico

- [Parsing XML using Java DOM Parser](https://coderanch.com/t/755601/languages/Parsing-XML-Java-DOM-Parser)

## Tecnologias e conceitos

Os seguintes conceitos são abordados nesta atividade:

- Teoria dos grafos.
- Grafos não dirigidos e conexos.
- Leitura e processamento de arquivos XML.
- Formato GEXF.
- Excentricidade de vértices.
- Closeness centrality normalizada.
- Programação orientada a objetos.

## Execução

As instruções de compilação e execução devem ser adicionadas conforme a linguagem de programação e a estrutura utilizadas no projeto.
