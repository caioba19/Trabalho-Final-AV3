# Trabalho Final – Estruturas de Dados 2026.1

Repositório do grupo para entrega do trabalho final da disciplina **Estruturas de Dados**.  
Curso: Sistemas de Análise e Desenvolvimento – UniJorge  
Professor: Nirton Afonso  
Referência: [Repositório do Professor](https://github.com/NirtonAfonso/Trabalho-Estruturas-de-Dados-2026)

---

## Integrantes

| Nome | Turma |
|------|-------|
| Caio Vinicius | ADS 2026.1 |
| Daniel Oliveira | ADS 2026.1 |
| João Pedro Gonzaga | ADS 2026.1 |
| Maicon Dias | ADS 2026.1 |
| Yuri Sales | ADS 2026.1 |
| Kauan Marinho| ADS 2026.1 |

---

## Temas Escolhidos

| Trabalho | Tema |
|----------|------|
| A – Lista Simples | **A1 – Playlist de Músicas** |
| B – Pilha Simples | **B2 – Chamadas de Emergência** |
| C – Fila Simples e Fila Circular Simples | **C2 – Fila de Impressão** |

---

## Sobre a Versão Entregue

Conforme a correção enviada pelo professor em 18/06/2026 e reforçada posteriormente, **a entrega obrigatória deste trabalho usa vetores de struct** (lista simples, pilha simples e filas simples/circular simples), sem alocação dinâmica.

O grupo também manteve, como material extra, a implementação original com estruturas dinâmicas encadeadas (`malloc`/`free`), disponível na pasta `Extra_Versao_Dinamica/`, válida como pontuação extra conforme indicado pelo professor.

---

## Organização do Repositório

```
Trabalho_Final_Estruturas_Dados/
├── README.md
├── relatorio.md
├── Trabalho_A/
│   ├── trabalho_a.c        (versão obrigatória – vetor)
│   └── dados_a.csv
├── Trabalho_B/
│   ├── trabalho_b.c        (versão obrigatória – vetor)
│   └── dados_b.csv
├── Trabalho_C/
│   ├── trabalho_c.c        (versão obrigatória – vetor)
│   └── dados_c.csv

```

---

## Trabalho A – Playlist de Músicas (Lista Simples em Vetor)

### Descrição

Sistema de gerenciamento de uma playlist musical usando **vetor de struct** como lista simples. Cada música ocupa uma posição do vetor, respeitando a ordem de cadastro. Capacidade máxima: 100 músicas.

Campos de cada música: `ID`, `título`, `artista`, `duração (MM:SS)`, `gênero`.

### Funcionalidades

| Opção | Descrição |
|-------|-----------|
| 1 | Cadastrar música |
| 2 | Buscar por ID |
| 3 | Editar música (ID não pode ser alterado) |
| 4 | Remover música (desloca os elementos seguintes) |
| 5 | Listar todas |
| 6 | Salvar CSV (`dados_a.csv`) |
| 7 | Carregar CSV |
| 0 | Sair (salva automaticamente) |

### Como compilar e executar

```bash
cd Trabalho_A
gcc -o playlist trabalho_a.c -std=c99
./playlist
```

> No Windows: `playlist.exe`

---

## Trabalho B – Chamadas de Emergência (Pilha Simples em Vetor)

### Descrição

Sistema de registro e atendimento de chamadas de emergência usando **vetor de struct** como pilha simples (LIFO), controlada por um índice `topo`. Capacidade máxima: 100 chamadas.

Campos de cada chamada: `protocolo`, `local`, `tipo da ocorrência`, `horário`.

### Funcionalidades

| Opção | Descrição |
|-------|-----------|
| 1 | Registrar chamada (Push) |
| 2 | Atender chamada (Pop) |
| 3 | Consultar próxima chamada (Peek) |
| 4 | Listar todas (topo → base) |
| 5 | Salvar CSV (`dados_b.csv`) |
| 6 | Carregar CSV |
| 0 | Sair (salva automaticamente) |

### Como compilar e executar

```bash
cd Trabalho_B
gcc -o emergencia trabalho_b.c -std=c99
./emergencia
```

> No Windows: `emergencia.exe`

---

## Trabalho C – Fila de Impressão (Fila Simples + Fila Circular Simples em Vetor)

### Descrição

Sistema de gerenciamento de fila de impressão com dois tipos de trabalho, ambos implementados com **vetores de struct**:

- **Fila Normal** – fila simples em vetor, com índices `frente` e `fim` (FIFO)
- **Fila Prioritária** – fila circular simples em vetor, com aritmética modular para reaproveitar posições liberadas

Se houver trabalho prioritário, ele é processado primeiro. Caso contrário, o próximo trabalho normal é processado. Capacidade máxima: 100 trabalhos por fila.

Campos de cada trabalho: `ID`, `nome do arquivo`, `quantidade de páginas`, `tipo (N/P)`.

### Funcionalidades

| Opção | Descrição |
|-------|-----------|
| 1 | Adicionar trabalho (N ou P) |
| 2 | Processar próximo |
| 3 | Buscar por ID |
| 4 | Listar todos |
| 5 | Cancelar trabalho |
| 6 | Salvar CSV (`dados_c.csv`) |
| 7 | Carregar CSV |
| 0 | Sair (salva automaticamente) |

### Como compilar e executar

```bash
cd Trabalho_C
gcc -o impressao trabalho_c.c -std=c99
./impressao
```

> No Windows: `impressao.exe`

---

## Requisitos Técnicos Atendidos

- [x] Código em linguagem C
- [x] Uso de `struct`
- [x] Vetor de struct como estrutura de dados principal
- [x] Menu interativo com opções numéricas
- [x] Cadastro/inserção de registros
- [x] Busca/consulta
- [x] Listagem dos dados
- [x] Remoção de registros
- [x] Validação de entradas
- [x] Tratamento de estrutura vazia (e cheia)
- [x] Salvamento em arquivo CSV
- [x] Carregamento de arquivo CSV

---

## Observações sobre o Funcionamento

- Todos os programas carregam o CSV automaticamente ao iniciar
- Ao sair (opção 0), os dados são salvos automaticamente
- IDs/protocolos duplicados são rejeitados com mensagem de erro
- Campos vazios são validados antes de inserir
- A capacidade máxima de cada estrutura é 100 registros (`#define MAX 100`); o programa avisa quando o limite é atingido

---

## Dificuldades Encontradas

- Implementação da fila circular simples em vetor (Trabalho C) exigiu uso de aritmética modular (`% MAX`) para que os índices de frente e fim reaproveitassem posições liberadas, em vez de "andar" indefinidamente pelo vetor.
- Adaptar o cancelamento de um trabalho no meio da fila exigiu deslocar os elementos seguintes para preencher a posição removida, recalculando o índice de fim corretamente.

---

## Material Extra – Versão com Estruturas Dinâmicas

Na pasta `Extra_Versao_Dinamica/` estão as implementações originais dos três trabalhos usando **listas, pilhas e filas encadeadas dinamicamente** (nós, ponteiros, `malloc` e `free`), conforme a proposta inicial do trabalho. Essa versão é apresentada como pontuação extra, conforme indicado pelo professor, e segue a mesma lógica funcional da versão em vetor, porém com alocação dinâmica de memória.
