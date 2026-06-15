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

---

## Temas Escolhidos

| Trabalho | Tema |
|----------|------|
| A – Listas Encadeadas Dinâmicas | **A1 – Playlist de Músicas** |
| B – Pilhas Dinâmicas (LIFO) | **B2 – Chamadas de Emergência** |
| C – Filas Dinâmicas e Circulares | **C2 – Fila de Impressão** |

---

## Organização do Repositório

```
/
├── README.md
├── Trabalho_A/
│   ├── trabalho_a.c
│   └── dados_a.csv
├── Trabalho_B/
│   ├── trabalho_b.c
│   └── dados_b.csv
└── Trabalho_C/
    ├── trabalho_c.c
    └── dados_c.csv
```

---

## Trabalho A – Playlist de Músicas (Lista Encadeada)

### Descrição

Sistema de gerenciamento de uma playlist musical usando **lista simplesmente encadeada dinâmica**. Cada música é um nó criado com `malloc`. A lista respeita a ordem de cadastro.

Campos de cada música: `ID`, `título`, `artista`, `duração (MM:SS)`, `gênero`.

### Funcionalidades

| Opção | Descrição |
|-------|-----------|
| 1 | Cadastrar música |
| 2 | Buscar por ID |
| 3 | Editar música (ID não pode ser alterado) |
| 4 | Remover música |
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

## Trabalho B – Chamadas de Emergência (Pilha LIFO)

### Descrição

Sistema de registro e atendimento de chamadas de emergência usando **pilha dinâmica encadeada (LIFO)**. A última chamada registrada é a primeira a ser atendida.

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

## Trabalho C – Fila de Impressão (Fila Normal + Circular)

### Descrição

Sistema de gerenciamento de fila de impressão com dois tipos de trabalho:

- **Fila Normal** – lista encadeada simples (FIFO)
- **Fila Prioritária** – fila circular encadeada (FIFO, mas sempre processada antes da normal)

Se houver trabalho prioritário, ele é processado primeiro. Caso contrário, o próximo trabalho normal é processado.

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
- [x] Uso de ponteiros
- [x] Alocação dinâmica com `malloc`
- [x] Liberação de memória com `free`
- [x] Menu interativo com opções numéricas
- [x] Cadastro/inserção de registros
- [x] Busca/consulta
- [x] Listagem dos dados
- [x] Remoção de registros
- [x] Validação de entradas
- [x] Tratamento de estrutura vazia
- [x] Salvamento em arquivo CSV
- [x] Carregamento de arquivo CSV
- [x] Nenhum vetor substituindo estruturas dinâmicas

---

## Observações sobre o Funcionamento

- Todos os programas carregam o CSV automaticamente ao iniciar
- Ao sair (opção 0), os dados são salvos automaticamente
- IDs/protocolos duplicados são rejeitados com mensagem de erro
- Campos vazios são validados antes de inserir

---

## Dificuldades Encontradas

- Implementação da fila circular encadeada no Trabalho C exigiu atenção especial com o ponteiro do último nó apontando para o primeiro, especialmente nos casos de inserção e remoção com apenas um elemento na fila.
- Garantir a liberação correta de toda a memória alocada ao encerrar os programas.