#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

/* ===== STRUCT ===== */

typedef struct {
    int id;
    char arquivo[60];
    int paginas;
    char tipo; /* 'N' = normal, 'P' = prioritario */
} Trabalho;

/* --- Fila Normal: fila simples em vetor (frente avanca, sem reaproveitar espaco) --- */
typedef struct {
    Trabalho dados[MAX];
    int frente;
    int fim;     /* proxima posicao livre */
    int tamanho;
} FilaNormal;

/* --- Fila Prioritaria: fila circular simples em vetor --- */
typedef struct {
    Trabalho dados[MAX];
    int frente;
    int fim;
    int tamanho;
} FilaCircular;

/* ===== INICIALIZAR ===== */

void inicializarNormal(FilaNormal *f) {
    f->frente = 0;
    f->fim = 0;
    f->tamanho = 0;
}

void inicializarCircular(FilaCircular *f) {
    f->frente = 0;
    f->fim = 0;
    f->tamanho = 0;
}

/* ===== VAZIA / CHEIA ===== */

int normalVazia(FilaNormal *f)  { return f->tamanho == 0; }
int normalCheia(FilaNormal *f)  { return f->tamanho == MAX; }
int circularVazia(FilaCircular *f) { return f->tamanho == 0; }
int circularCheia(FilaCircular *f) { return f->tamanho == MAX; }

/* ===== VERIFICAR ID DUPLICADO ===== */

int idExisteNormal(FilaNormal *f, int id) {
    for (int i = 0; i < f->tamanho; i++) {
        int pos = (f->frente + i) % MAX;
        if (f->dados[pos].id == id) return 1;
    }
    return 0;
}

int idExisteCircular(FilaCircular *f, int id) {
    for (int i = 0; i < f->tamanho; i++) {
        int pos = (f->frente + i) % MAX;
        if (f->dados[pos].id == id) return 1;
    }
    return 0;
}

/* ===== ENFILEIRAR NORMAL (fila simples) ===== */

int enfileirarNormal(FilaNormal *f, Trabalho t) {
    if (normalCheia(f)) {
        printf("Fila normal cheia. Capacidade maxima: %d trabalhos.\n", MAX);
        return 0;
    }
    f->dados[f->fim] = t;
    f->fim = (f->fim + 1) % MAX; /* indice circula apenas para reaproveitar o vetor */
    f->tamanho++;
    return 1;
}

/* ===== ENFILEIRAR CIRCULAR (fila circular simples) ===== */

int enfileirarCircular(FilaCircular *f, Trabalho t) {
    if (circularCheia(f)) {
        printf("Fila prioritaria cheia. Capacidade maxima: %d trabalhos.\n", MAX);
        return 0;
    }
    f->dados[f->fim] = t;
    f->fim = (f->fim + 1) % MAX;
    f->tamanho++;
    return 1;
}

/* ===== DESENFILEIRAR NORMAL ===== */

int desenfileirarNormal(FilaNormal *f, Trabalho *saida) {
    if (normalVazia(f)) {
        printf("Fila normal vazia.\n");
        return 0;
    }
    *saida = f->dados[f->frente];
    f->frente = (f->frente + 1) % MAX;
    f->tamanho--;
    return 1;
}

/* ===== DESENFILEIRAR CIRCULAR ===== */

int desenfileirarCircular(FilaCircular *f, Trabalho *saida) {
    if (circularVazia(f)) {
        printf("Fila prioritaria vazia.\n");
        return 0;
    }
    *saida = f->dados[f->frente];
    f->frente = (f->frente + 1) % MAX;
    f->tamanho--;
    return 1;
}

/* ===== LISTAR NORMAL ===== */

void listarNormal(FilaNormal *f) {
    if (normalVazia(f)) {
        printf("Fila normal vazia.\n");
        return;
    }
    printf("\n  [FILA NORMAL]\n");
    printf("  %-4s %-6s %-35s %-8s\n", "#", "ID", "Arquivo", "Paginas");
    printf("  -----------------------------------------------\n");
    for (int i = 0; i < f->tamanho; i++) {
        int pos = (f->frente + i) % MAX;
        printf("  %-4d %-6d %-35s %-8d\n", i + 1, f->dados[pos].id, f->dados[pos].arquivo, f->dados[pos].paginas);
    }
    printf("  Total: %d trabalho(s)\n", f->tamanho);
}

/* ===== LISTAR CIRCULAR ===== */

void listarCircular(FilaCircular *f) {
    if (circularVazia(f)) {
        printf("Fila prioritaria vazia.\n");
        return;
    }
    printf("\n  [FILA PRIORITARIA CIRCULAR]\n");
    printf("  %-4s %-6s %-35s %-8s\n", "#", "ID", "Arquivo", "Paginas");
    printf("  -----------------------------------------------\n");
    for (int i = 0; i < f->tamanho; i++) {
        int pos = (f->frente + i) % MAX;
        printf("  %-4d %-6d %-35s %-8d\n", i + 1, f->dados[pos].id, f->dados[pos].arquivo, f->dados[pos].paginas);
    }
    printf("  Total: %d trabalho(s)\n", f->tamanho);
}

/* ===== BUSCAR ===== */

void buscar(FilaNormal *fn, FilaCircular *fc, int id) {
    for (int i = 0; i < fn->tamanho; i++) {
        int pos = (fn->frente + i) % MAX;
        if (fn->dados[pos].id == id) {
            printf("\nTrabalho encontrado na FILA NORMAL:\n");
            printf("  ID      : %d\n", fn->dados[pos].id);
            printf("  Arquivo : %s\n", fn->dados[pos].arquivo);
            printf("  Paginas : %d\n", fn->dados[pos].paginas);
            printf("  Tipo    : Normal\n");
            return;
        }
    }
    for (int i = 0; i < fc->tamanho; i++) {
        int pos = (fc->frente + i) % MAX;
        if (fc->dados[pos].id == id) {
            printf("\nTrabalho encontrado na FILA PRIORITARIA:\n");
            printf("  ID      : %d\n", fc->dados[pos].id);
            printf("  Arquivo : %s\n", fc->dados[pos].arquivo);
            printf("  Paginas : %d\n", fc->dados[pos].paginas);
            printf("  Tipo    : Prioritario\n");
            return;
        }
    }
    printf("Trabalho com ID %d nao encontrado.\n", id);
}

/* ===== CANCELAR (remover do meio, deslocando elementos logicamente) ===== */

int cancelarNormal(FilaNormal *f, int id) {
    int achou = -1;
    for (int i = 0; i < f->tamanho; i++) {
        int pos = (f->frente + i) % MAX;
        if (f->dados[pos].id == id) { achou = i; break; }
    }
    if (achou == -1) return 0;

    for (int i = achou; i < f->tamanho - 1; i++) {
        int posAtual = (f->frente + i) % MAX;
        int posProximo = (f->frente + i + 1) % MAX;
        f->dados[posAtual] = f->dados[posProximo];
    }
    f->fim = (f->frente + f->tamanho - 1) % MAX;
    f->tamanho--;
    printf("Trabalho ID %d cancelado da fila normal.\n", id);
    return 1;
}

int cancelarCircular(FilaCircular *f, int id) {
    int achou = -1;
    for (int i = 0; i < f->tamanho; i++) {
        int pos = (f->frente + i) % MAX;
        if (f->dados[pos].id == id) { achou = i; break; }
    }
    if (achou == -1) return 0;

    for (int i = achou; i < f->tamanho - 1; i++) {
        int posAtual = (f->frente + i) % MAX;
        int posProximo = (f->frente + i + 1) % MAX;
        f->dados[posAtual] = f->dados[posProximo];
    }
    f->fim = (f->frente + f->tamanho - 1) % MAX;
    f->tamanho--;
    printf("Trabalho ID %d cancelado da fila prioritaria.\n", id);
    return 1;
}

/* ===== PROCESSAR PROXIMO ===== */

void processarProximo(FilaNormal *fn, FilaCircular *fc) {
    Trabalho t;
    if (fc->tamanho > 0) {
        desenfileirarCircular(fc, &t);
        printf("Imprimindo [PRIORITARIO] ID %d: %s (%d paginas)\n", t.id, t.arquivo, t.paginas);
    } else if (fn->tamanho > 0) {
        desenfileirarNormal(fn, &t);
        printf("Imprimindo [NORMAL] ID %d: %s (%d paginas)\n", t.id, t.arquivo, t.paginas);
    } else {
        printf("Nenhum trabalho na fila de impressao.\n");
    }
}

/* ===== SALVAR CSV ===== */

void salvarCSV(FilaNormal *fn, FilaCircular *fc) {
    FILE *arquivo = fopen("dados_c.csv", "w");
    if (arquivo == NULL) { printf("Erro ao salvar.\n"); return; }
    fprintf(arquivo, "id;arquivo;paginas;tipo\n");

    for (int i = 0; i < fn->tamanho; i++) {
        int pos = (fn->frente + i) % MAX;
        fprintf(arquivo, "%d;%s;%d;N\n", fn->dados[pos].id, fn->dados[pos].arquivo, fn->dados[pos].paginas);
    }
    for (int i = 0; i < fc->tamanho; i++) {
        int pos = (fc->frente + i) % MAX;
        fprintf(arquivo, "%d;%s;%d;P\n", fc->dados[pos].id, fc->dados[pos].arquivo, fc->dados[pos].paginas);
    }

    fclose(arquivo);
    printf("Dados salvos em dados_c.csv com sucesso.\n");
}

/* ===== CARREGAR CSV ===== */

void carregarCSV(FilaNormal *fn, FilaCircular *fc) {
    FILE *arquivo = fopen("dados_c.csv", "r");
    if (arquivo == NULL) { printf("Arquivo dados_c.csv nao encontrado. Iniciando vazio.\n"); return; }

    char linha[150];
    fgets(linha, sizeof(linha), arquivo);

    int carregados = 0;
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0';
        Trabalho t;
        char *token = strtok(linha, ";");
        if (!token) continue;
        t.id = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(t.arquivo, token, sizeof(t.arquivo) - 1);
        t.arquivo[sizeof(t.arquivo) - 1] = '\0';

        token = strtok(NULL, ";");
        if (!token) continue;
        t.paginas = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        t.tipo = token[0];

        if (t.tipo == 'P') enfileirarCircular(fc, t);
        else enfileirarNormal(fn, t);
        carregados++;
    }
    fclose(arquivo);
    if (carregados > 0) printf("%d trabalho(s) carregado(s) do arquivo.\n", carregados);
    else printf("Arquivo vazio ou sem dados validos.\n");
}

/* ===== MENU ===== */

void exibirMenu() {
    printf("\n========================================\n");
    printf("   FILA DE IMPRESSAO (VETOR)             \n");
    printf("========================================\n");
    printf("  1. Adicionar trabalho\n");
    printf("  2. Processar proximo\n");
    printf("  3. Buscar trabalho\n");
    printf("  4. Listar todos\n");
    printf("  5. Cancelar trabalho\n");
    printf("  6. Salvar CSV\n");
    printf("  7. Carregar CSV\n");
    printf("  0. Sair\n");
    printf("========================================\n");
    printf("Opcao: ");
}

/* ===== MAIN ===== */

int main() {
    FilaNormal fn;
    FilaCircular fc;
    inicializarNormal(&fn);
    inicializarCircular(&fc);

    carregarCSV(&fn, &fc);

    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {

            case 1: {
                Trabalho novo;
                printf("\n--- Adicionar Trabalho ---\n");

                printf("ID: ");
                scanf("%d", &novo.id);
                getchar();
                if (novo.id <= 0) { printf("ID invalido.\n"); break; }
                if (idExisteNormal(&fn, novo.id) || idExisteCircular(&fc, novo.id)) {
                    printf("Erro: ID %d ja existe.\n", novo.id);
                    break;
                }

                printf("Nome do arquivo: ");
                fgets(novo.arquivo, sizeof(novo.arquivo), stdin);
                novo.arquivo[strcspn(novo.arquivo, "\n")] = '\0';
                if (strlen(novo.arquivo) == 0) { printf("Nome nao pode ser vazio.\n"); break; }

                printf("Quantidade de paginas: ");
                scanf("%d", &novo.paginas);
                getchar();
                if (novo.paginas <= 0) { printf("Paginas deve ser positivo.\n"); break; }

                printf("Tipo (N = Normal / P = Prioritario): ");
                char tipo[5];
                fgets(tipo, sizeof(tipo), stdin);
                novo.tipo = tipo[0];
                if (novo.tipo != 'N' && novo.tipo != 'n' && novo.tipo != 'P' && novo.tipo != 'p') {
                    printf("Tipo invalido. Use N ou P.\n"); break;
                }
                novo.tipo = (novo.tipo == 'p') ? 'P' : (novo.tipo == 'n') ? 'N' : novo.tipo;

                if (novo.tipo == 'P') {
                    if (enfileirarCircular(&fc, novo))
                        printf("Trabalho prioritario ID %d adicionado.\n", novo.id);
                } else {
                    if (enfileirarNormal(&fn, novo))
                        printf("Trabalho normal ID %d adicionado.\n", novo.id);
                }
                break;
            }

            case 2:
                printf("\n--- Processar Proximo ---\n");
                processarProximo(&fn, &fc);
                break;

            case 3: {
                int id;
                printf("\n--- Buscar Trabalho ---\n");
                printf("ID: ");
                scanf("%d", &id);
                getchar();
                buscar(&fn, &fc, id);
                break;
            }

            case 4:
                printf("\n--- Listar Todos ---\n");
                listarCircular(&fc);
                listarNormal(&fn);
                printf("\n  Total geral: %d trabalho(s)\n", fn.tamanho + fc.tamanho);
                break;

            case 5: {
                int id;
                printf("\n--- Cancelar Trabalho ---\n");
                printf("ID: ");
                scanf("%d", &id);
                getchar();
                if (!cancelarCircular(&fc, id))
                    if (!cancelarNormal(&fn, id))
                        printf("Trabalho ID %d nao encontrado.\n", id);
                break;
            }

            case 6:
                salvarCSV(&fn, &fc);
                break;

            case 7:
                inicializarNormal(&fn);
                inicializarCircular(&fc);
                carregarCSV(&fn, &fc);
                break;

            case 0:
                salvarCSV(&fn, &fc);
                printf("Saindo... Ate logo!\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}
