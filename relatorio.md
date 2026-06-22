# Relatório Técnico – Trabalho Final de Estruturas de Dados

## 1. Introdução

Este relatório detalha as decisões técnicas, estruturas de dados e algoritmos utilizados na implementação dos três programas que compõem o trabalho final da disciplina de Estruturas de Dados.

A proposta inicial do trabalho previa o uso de listas, pilhas e filas **dinâmicas encadeadas**. Em 18/06/2026, o professor comunicou o ajuste do trabalho para que a entrega obrigatória passasse a usar **vetores de struct**, mantendo a versão dinâmica como pontuação extra para quem já a tivesse desenvolvido. O professor reforçou posteriormente que a versão em vetor é a entrega obrigatória, independentemente do grupo já ter implementado a versão dinâmica.

Por isso, este grupo entrega:

- **Versão obrigatória** (`Trabalho_A/`, `Trabalho_B/`, `Trabalho_C/`): implementada com vetores de struct.
- **Versão extra** (`Extra_Versao_Dinamica/`): implementada com listas/pilhas/filas encadeadas dinamicamente, válida como pontuação adicional.

---

## 2. Trabalho A – Playlist de Músicas

### 2.1 Versão obrigatória (Lista Simples em Vetor)

```c
typedef struct {
    int id;
    char titulo[80];
    char artista[60];
    char duracao[10];
    char genero[30];
} Musica;

typedef struct {
    Musica dados[MAX]; /* MAX = 100 */
    int tamanho;
} Playlist;
```

A playlist é um vetor de structs de tamanho fixo (`MAX = 100`), com um contador `tamanho` indicando quantas posições estão ocupadas. Novas músicas são sempre inseridas na posição `tamanho` (final do vetor), respeitando a ordem de cadastro.

| Operação | Complexidade | Observação |
|----------|--------------|------------|
| Inserir | O(1) | Insere diretamente na posição `tamanho` |
| Buscar por ID | O(n) | Busca sequencial no vetor |
| Editar | O(n) | Localiza a posição e atualiza os campos |
| Remover | O(n) | Desloca todos os elementos posteriores uma posição à esquerda |
| Listar | O(n) | Percorre o vetor de 0 até `tamanho - 1` |

O controle de capacidade (`listaCheia`) impede inserção além do limite do vetor, retornando mensagem ao usuário.

### 2.2 Versão extra (Lista Encadeada Dinâmica)

Implementada com nós alocados via `malloc`, sem limite fixo de capacidade. A inserção no fim exige percorrer a lista até o último nó (O(n)), enquanto a remoção reencadeia os ponteiros `anterior`/`atual`.

---

## 3. Trabalho B – Chamadas de Emergência

### 3.1 Versão obrigatória (Pilha Simples em Vetor)

```c
typedef struct {
    int protocolo;
    char local[50];
    char tipo[30];
    char horario[20];
} Chamada;

typedef struct {
    Chamada dados[MAX];
    int topo; /* -1 = pilha vazia */
} Pilha;
```

A pilha é representada por um vetor e um índice `topo`. O `push` incrementa `topo` e insere na nova posição; o `pop` lê o elemento em `topo` e decrementa o índice — sem necessidade de deslocar elementos, mantendo o comportamento LIFO em O(1).

| Operação | Complexidade |
|----------|--------------|
| Push | O(1) |
| Pop | O(1) |
| Peek | O(1) |
| Listar (topo → base) | O(n) |

O controle `pilhaCheia` (`topo == MAX - 1`) evita estouro do vetor.

### 3.2 Versão extra (Pilha Dinâmica Encadeada)

Implementada com nós encadeados, onde `push` insere um novo nó como topo e `pop` remove o nó do topo, liberando a memória com `free`. Sem limite de capacidade, exceto pela memória disponível.

---

## 4. Trabalho C – Fila de Impressão

### 4.1 Versão obrigatória (Fila Simples + Fila Circular Simples em Vetor)

```c
typedef struct {
    int id;
    char arquivo[60];
    int paginas;
    char tipo; /* 'N' ou 'P' */
} Trabalho;

typedef struct {
    Trabalho dados[MAX];
    int frente;
    int fim;
    int tamanho;
} FilaNormal;   /* fila simples */

typedef struct {
    Trabalho dados[MAX];
    int frente;
    int fim;
    int tamanho;
} FilaCircular; /* fila circular simples */
```

Ambas as filas usam vetor de tamanho fixo com índices `frente` e `fim`. A diferença está no uso de **aritmética modular** (`% MAX`): tanto a fila normal quanto a fila circular reaproveitam posições do vetor à medida que elementos são removidos, evitando que o índice `fim` "ande" indefinidamente e o vetor pareça cheio antes da hora.

A fila circular recebe esse nome por reaproveitar o início do vetor após o índice `fim` ultrapassar `MAX - 1`, retornando à posição 0 — diferentemente de uma fila linear simples que poderia descartar espaço já liberado.

### 4.2 Algoritmo de processamento

A regra de negócio prioriza trabalhos do tipo P sobre os do tipo N:

```
processarProximo():
    se fila circular (prioritaria) não está vazia:
        desenfileira da fila circular
    senão se fila normal não está vazia:
        desenfileira da fila normal
    senão:
        nenhum trabalho disponível
```

| Operação | Complexidade |
|----------|--------------|
| Enfileirar | O(1) |
| Desenfileirar | O(1) |
| Buscar | O(n) |
| Cancelar (meio da fila) | O(n) — desloca elementos para preencher a posição removida |
| Listar | O(n) |

### 4.3 Versão extra (Fila Normal Encadeada + Fila Circular Encadeada)

A fila normal é uma lista encadeada simples com ponteiros de `frente` e `fundo`. A fila prioritária é uma **fila circular encadeada**, em que o ponteiro `ultimo->proximo` aponta sempre para o primeiro nó, eliminando a necessidade de percorrer a lista para localizar o início.

---

## 5. Comparação entre as Versões

| Aspecto | Versão Vetor (obrigatória) | Versão Encadeada (extra) |
|---------|------------------------------|----------------------------|
| Alocação de memória | Estática, tamanho fixo (`MAX = 100`) | Dinâmica, via `malloc`/`free` |
| Limite de registros | Sim (100 por estrutura) | Não (limitado pela memória disponível) |
| Inserção/remoção | Pode exigir deslocamento de elementos (O(n) em alguns casos) | Apenas reencadeamento de ponteiros |
| Uso de ponteiros | Mínimo (apenas índices) | Extensivo (nós e referências) |
| Complexidade de implementação | Mais simples | Mais complexa, exige mais cuidado com ponteiros |

---

## 6. Testes Realizados

Ambas as versões (vetor e encadeada) foram testadas manualmente cobrindo os seguintes cenários, conforme exigido pelo professor:

- Inserção em estrutura vazia
- Inserção de múltiplos registros
- Busca por registro existente e inexistente
- Remoção no início, meio e fim (quando aplicável)
- Tentativa de chave/ID/protocolo duplicado (rejeitada com mensagem de erro)
- Salvamento e carregamento de CSV
- Encerramento do programa, com liberação de memória na versão dinâmica

---

## 7. Conclusão

A implementação obrigatória, usando vetores de struct, atende a todos os requisitos solicitados pelo professor após o ajuste no enunciado, mantendo a lógica funcional de lista, pilha e filas (simples e circular). A versão dinâmica encadeada foi preservada como material extra, demonstrando o domínio do grupo sobre estruturas de dados implementadas com ponteiros e alocação dinâmica de memória. Cada programa pode ser compilado e avaliado de forma independente.
