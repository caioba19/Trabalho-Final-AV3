#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== STRUCTS ===== */

typedef struct Trabalho {
    int id;
    char arquivo[60];
    int paginas;
    char tipo; /* 'N' = normal, 'P' = prioritario */
} Trabalho;

/* --- No para fila normal (simplesmente encadeada) --- */
typedef struct NoNormal {
    Trabalho dados;
    struct NoNormal *proximo;
} NoNormal;

/* --- No para fila circular encadeada (prioritaria) --- */
typedef struct NoCircular {
    Trabalho dados;
    struct NoCircular *proximo;
} NoCircular;

/* --- Fila Normal --- */
typedef struct {
    NoNormal *frente;
    NoNormal *fundo;
    int tamanho;
} FilaNormal;

/* --- Fila Prioritaria Circular --- */
typedef struct {
    NoCircular *ultimo; /* ponteiro para o ultimo; ultimo->proximo = primeiro */
    int tamanho;
} FilaCircular;

/* ===== INICIALIZAR ===== */

void inicializarNormal(FilaNormal *f) {
    f->frente = NULL;
    f->fundo = NULL;
    f->tamanho = 0;
}

void inicializarCircular(FilaCircular *f) {
    f->ultimo = NULL;
    f->tamanho = 0;
}

/* ===== VERIFICAR ID DUPLICADO ===== */

int idExisteNormal(FilaNormal *f, int id) {
    NoNormal *atual = f->frente;
    while (atual != NULL) {
        if (atual->dados.id == id) return 1;
        atual = atual->proximo;
    }
    return 0;
}

int idExisteCircular(FilaCircular *f, int id) {
    if (f->ultimo == NULL) return 0;
    NoCircular *atual = f->ultimo->proximo; /* primeiro */
    do {
        if (atual->dados.id == id) return 1;
        atual = atual->proximo;
    } while (atual != f->ultimo->proximo);
    return 0;
}

/* ===== ENFILEIRAR NORMAL ===== */

int enfileirarNormal(FilaNormal *f, Trabalho t) {
    NoNormal *novo = (NoNormal *)malloc(sizeof(NoNormal));
    if (novo == NULL) { printf("Erro: memoria insuficiente.\n"); return 0; }
    novo->dados = t;
    novo->proximo = NULL;
    if (f->fundo == NULL) {
        f->frente = novo;
        f->fundo = novo;
    } else {
        f->fundo->proximo = novo;
        f->fundo = novo;
    }
    f->tamanho++;
    return 1;
}

/* ===== ENFILEIRAR CIRCULAR ===== */

int enfileirarCircular(FilaCircular *f, Trabalho t) {
    NoCircular *novo = (NoCircular *)malloc(sizeof(NoCircular));
    if (novo == NULL) { printf("Erro: memoria insuficiente.\n"); return 0; }
    novo->dados = t;
    if (f->ultimo == NULL) {
        novo->proximo = novo;
        f->ultimo = novo;
    } else {
        novo->proximo = f->ultimo->proximo; /* novo aponta para o primeiro */
        f->ultimo->proximo = novo;
        f->ultimo = novo;
    }
    f->tamanho++;
    return 1;
}

/* ===== DESENFILEIRAR NORMAL ===== */

int desenfileirarNormal(FilaNormal *f, Trabalho *saida) {
    if (f->frente == NULL) { printf("Fila normal vazia.\n"); return 0; }
    NoNormal *remover = f->frente;
    *saida = remover->dados;
    f->frente = remover->proximo;
    if (f->frente == NULL) f->fundo = NULL;
    free(remover);
    f->tamanho--;
    return 1;
}

/* ===== DESENFILEIRAR CIRCULAR ===== */

int desenfileirarCircular(FilaCircular *f, Trabalho *saida) {
    if (f->ultimo == NULL) { printf("Fila prioritaria vazia.\n"); return 0; }
    NoCircular *primeiro = f->ultimo->proximo;
    *saida = primeiro->dados;
    if (primeiro == f->ultimo) {
        /* unico elemento */
        f->ultimo = NULL;
    } else {
        f->ultimo->proximo = primeiro->proximo;
    }
    free(primeiro);
    f->tamanho--;
    return 1;
}

/* ===== LISTAR NORMAL ===== */

void listarNormal(FilaNormal *f) {
    if (f->frente == NULL) {
        printf("Fila normal vazia.\n");
        return;
    }
    NoNormal *atual = f->frente;
    int pos = 1;
    printf("\n  [FILA NORMAL]\n");
    printf("  %-4s %-6s %-35s %-8s\n", "#", "ID", "Arquivo", "Paginas");
    printf("  -----------------------------------------------\n");
    while (atual != NULL) {
        printf("  %-4d %-6d %-35s %-8d\n", pos, atual->dados.id, atual->dados.arquivo, atual->dados.paginas);
        atual = atual->proximo;
        pos++;
    }
    printf("  Total: %d trabalho(s)\n", f->tamanho);
}

/* ===== LISTAR CIRCULAR ===== */

void listarCircular(FilaCircular *f) {
    if (f->ultimo == NULL) {
        printf("Fila prioritaria vazia.\n");
        return;
    }
    NoCircular *atual = f->ultimo->proximo; /* primeiro */
    int pos = 1;
    printf("\n  [FILA PRIORITARIA CIRCULAR]\n");
    printf("  %-4s %-6s %-35s %-8s\n", "#", "ID", "Arquivo", "Paginas");
    printf("  -----------------------------------------------\n");
    do {
        printf("  %-4d %-6d %-35s %-8d\n", pos, atual->dados.id, atual->dados.arquivo, atual->dados.paginas);
        atual = atual->proximo;
        pos++;
    } while (atual != f->ultimo->proximo);
    printf("  Total: %d trabalho(s)\n", f->tamanho);
}

/* ===== BUSCAR ===== */

void buscar(FilaNormal *fn, FilaCircular *fc, int id) {
    /* busca na fila normal */
    NoNormal *atual = fn->frente;
    while (atual != NULL) {
        if (atual->dados.id == id) {
            printf("\nTrabalho encontrado na FILA NORMAL:\n");
            printf("  ID      : %d\n", atual->dados.id);
            printf("  Arquivo : %s\n", atual->dados.arquivo);
            printf("  Paginas : %d\n", atual->dados.paginas);
            printf("  Tipo    : Normal\n");
            return;
        }
        atual = atual->proximo;
    }
    /* busca na circular */
    if (fc->ultimo != NULL) {
        NoCircular *c = fc->ultimo->proximo;
        do {
            if (c->dados.id == id) {
                printf("\nTrabalho encontrado na FILA PRIORITARIA:\n");
                printf("  ID      : %d\n", c->dados.id);
                printf("  Arquivo : %s\n", c->dados.arquivo);
                printf("  Paginas : %d\n", c->dados.paginas);
                printf("  Tipo    : Prioritario\n");
                return;
            }
            c = c->proximo;
        } while (c != fc->ultimo->proximo);
    }
    printf("Trabalho com ID %d nao encontrado.\n", id);
}

/* ===== CANCELAR (REMOVER) ===== */

int cancelarNormal(FilaNormal *f, int id) {
    if (f->frente == NULL) return 0;
    NoNormal *anterior = NULL;
    NoNormal *atual = f->frente;
    while (atual != NULL && atual->dados.id != id) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL) return 0;
    if (anterior == NULL) f->frente = atual->proximo;
    else anterior->proximo = atual->proximo;
    if (atual == f->fundo) f->fundo = anterior;
    free(atual);
    f->tamanho--;
    printf("Trabalho ID %d cancelado da fila normal.\n", id);
    return 1;
}

int cancelarCircular(FilaCircular *f, int id) {
    if (f->ultimo == NULL) return 0;
    NoCircular *anterior = f->ultimo;
    NoCircular *atual = f->ultimo->proximo; /* primeiro */
    int tamanho = f->tamanho;
    for (int i = 0; i < tamanho; i++) {
        if (atual->dados.id == id) {
            if (f->tamanho == 1) {
                f->ultimo = NULL;
            } else {
                anterior->proximo = atual->proximo;
                if (atual == f->ultimo) f->ultimo = anterior;
            }
            free(atual);
            f->tamanho--;
            printf("Trabalho ID %d cancelado da fila prioritaria.\n", id);
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return 0;
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

    NoNormal *n = fn->frente;
    while (n != NULL) {
        fprintf(arquivo, "%d;%s;%d;N\n", n->dados.id, n->dados.arquivo, n->dados.paginas);
        n = n->proximo;
    }

    if (fc->ultimo != NULL) {
        NoCircular *c = fc->ultimo->proximo;
        do {
            fprintf(arquivo, "%d;%s;%d;P\n", c->dados.id, c->dados.arquivo, c->dados.paginas);
            c = c->proximo;
        } while (c != fc->ultimo->proximo);
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

/* ===== LIBERAR MEMORIA ===== */

void liberarNormal(FilaNormal *f) {
    NoNormal *atual = f->frente;
    while (atual != NULL) {
        NoNormal *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    f->frente = NULL;
    f->fundo = NULL;
    f->tamanho = 0;
}

void liberarCircular(FilaCircular *f) {
    if (f->ultimo == NULL) return;
    NoCircular *primeiro = f->ultimo->proximo;
    NoCircular *atual = primeiro;
    do {
        NoCircular *prox = atual->proximo;
        free(atual);
        atual = prox;
    } while (atual != primeiro);
    f->ultimo = NULL;
    f->tamanho = 0;
}

/* ===== MENU ===== */

void exibirMenu() {
    printf("\n========================================\n");
    printf("        FILA DE IMPRESSAO               \n");
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
                liberarNormal(&fn);
                liberarCircular(&fc);
                inicializarNormal(&fn);
                inicializarCircular(&fc);
                carregarCSV(&fn, &fc);
                break;

            case 0:
                salvarCSV(&fn, &fc);
                liberarNormal(&fn);
                liberarCircular(&fc);
                printf("Saindo... Ate logo!\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}
