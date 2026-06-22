#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== STRUCTS ===== */

typedef struct Musica {
    int id;
    char titulo[80];
    char artista[60];
    char duracao[10]; /* formato MM:SS */
    char genero[30];
} Musica;

typedef struct No {
    Musica dados;
    struct No *proximo;
} No;

typedef struct {
    No *inicio;
    int tamanho;
} Lista;

/* ===== INICIALIZAR ===== */

void inicializar(Lista *l) {
    l->inicio = NULL;
    l->tamanho = 0;
}

/* ===== LISTA VAZIA ===== */

int listaVazia(Lista *l) {
    return l->inicio == NULL;
}

/* ===== VERIFICAR ID DUPLICADO ===== */

int idExiste(Lista *l, int id) {
    No *atual = l->inicio;
    while (atual != NULL) {
        if (atual->dados.id == id)
            return 1;
        atual = atual->proximo;
    }
    return 0;
}

/* ===== INSERIR NO FIM ===== */

int inserir(Lista *l, Musica m) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return 0;
    }
    novo->dados = m;
    novo->proximo = NULL;

    if (listaVazia(l)) {
        l->inicio = novo;
    } else {
        No *atual = l->inicio;
        while (atual->proximo != NULL)
            atual = atual->proximo;
        atual->proximo = novo;
    }
    l->tamanho++;
    return 1;
}

/* ===== BUSCAR POR ID ===== */

No *buscarPorId(Lista *l, int id) {
    No *atual = l->inicio;
    while (atual != NULL) {
        if (atual->dados.id == id)
            return atual;
        atual = atual->proximo;
    }
    return NULL;
}

/* ===== LISTAR ===== */

void listar(Lista *l) {
    if (listaVazia(l)) {
        printf("Nenhuma musica cadastrada.\n");
        return;
    }
    No *atual = l->inicio;
    int pos = 1;
    printf("\n%-4s %-6s %-35s %-25s %-8s %-15s\n",
           "#", "ID", "Titulo", "Artista", "Duracao", "Genero");
    printf("------------------------------------------------------------------------------------\n");
    while (atual != NULL) {
        printf("%-4d %-6d %-35s %-25s %-8s %-15s\n",
               pos,
               atual->dados.id,
               atual->dados.titulo,
               atual->dados.artista,
               atual->dados.duracao,
               atual->dados.genero);
        atual = atual->proximo;
        pos++;
    }
    printf("------------------------------------------------------------------------------------\n");
    printf("Total: %d musica(s)\n", l->tamanho);
}

/* ===== EDITAR ===== */

int editar(Lista *l, int id) {
    No *no = buscarPorId(l, id);
    if (no == NULL) {
        printf("Musica com ID %d nao encontrada.\n", id);
        return 0;
    }

    char buffer[80];

    printf("Novo titulo [%s]: ", no->dados.titulo);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strncpy(no->dados.titulo, buffer, sizeof(no->dados.titulo) - 1);

    printf("Novo artista [%s]: ", no->dados.artista);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strncpy(no->dados.artista, buffer, sizeof(no->dados.artista) - 1);

    printf("Nova duracao (MM:SS) [%s]: ", no->dados.duracao);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strncpy(no->dados.duracao, buffer, sizeof(no->dados.duracao) - 1);

    printf("Novo genero [%s]: ", no->dados.genero);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strncpy(no->dados.genero, buffer, sizeof(no->dados.genero) - 1);

    printf("Musica %d atualizada com sucesso.\n", id);
    return 1;
}

/* ===== REMOVER ===== */

int remover(Lista *l, int id) {
    if (listaVazia(l)) {
        printf("Lista vazia. Nada para remover.\n");
        return 0;
    }

    No *anterior = NULL;
    No *atual = l->inicio;

    while (atual != NULL && atual->dados.id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Musica com ID %d nao encontrada.\n", id);
        return 0;
    }

    if (anterior == NULL)
        l->inicio = atual->proximo;
    else
        anterior->proximo = atual->proximo;

    free(atual);
    l->tamanho--;
    printf("Musica ID %d removida com sucesso.\n", id);
    return 1;
}

/* ===== SALVAR CSV ===== */

void salvarCSV(Lista *l) {
    if (listaVazia(l)) {
        printf("Lista vazia. Nada para salvar.\n");
        return;
    }

    FILE *arquivo = fopen("dados_a.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar.\n");
        return;
    }

    fprintf(arquivo, "id;titulo;artista;duracao;genero\n");
    No *atual = l->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%d;%s;%s;%s;%s\n",
                atual->dados.id,
                atual->dados.titulo,
                atual->dados.artista,
                atual->dados.duracao,
                atual->dados.genero);
        atual = atual->proximo;
    }

    fclose(arquivo);
    printf("Dados salvos em dados_a.csv com sucesso.\n");
}

/* ===== CARREGAR CSV ===== */

void carregarCSV(Lista *l) {
    FILE *arquivo = fopen("dados_a.csv", "r");
    if (arquivo == NULL) {
        printf("Arquivo dados_a.csv nao encontrado. Iniciando vazio.\n");
        return;
    }

    char linha[200];
    fgets(linha, sizeof(linha), arquivo); /* pula cabecalho */

    int carregados = 0;
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0';

        Musica m;
        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        m.id = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(m.titulo, token, sizeof(m.titulo) - 1);
        m.titulo[sizeof(m.titulo) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(m.artista, token, sizeof(m.artista) - 1);
        m.artista[sizeof(m.artista) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(m.duracao, token, sizeof(m.duracao) - 1);
        m.duracao[sizeof(m.duracao) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(m.genero, token, sizeof(m.genero) - 1);
        m.genero[sizeof(m.genero) - 1] = '\0';

        inserir(l, m);
        carregados++;
    }

    fclose(arquivo);
    if (carregados > 0)
        printf("%d musica(s) carregada(s) do arquivo.\n", carregados);
    else
        printf("Arquivo vazio ou sem dados validos.\n");
}

/* ===== LIBERAR MEMORIA ===== */

void liberarLista(Lista *l) {
    No *atual = l->inicio;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    l->inicio = NULL;
    l->tamanho = 0;
}

/* ===== LER STRING ===== */

void lerString(char *destino, int tamanho, const char *mensagem) {
    printf("%s", mensagem);
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';
}

/* ===== MENU ===== */

void exibirMenu() {
    printf("\n========================================\n");
    printf("         PLAYLIST DE MUSICAS            \n");
    printf("========================================\n");
    printf("  1. Cadastrar musica\n");
    printf("  2. Buscar por ID\n");
    printf("  3. Editar musica\n");
    printf("  4. Remover musica\n");
    printf("  5. Listar todas\n");
    printf("  6. Salvar CSV\n");
    printf("  7. Carregar CSV\n");
    printf("  0. Sair\n");
    printf("========================================\n");
    printf("Opcao: ");
}

/* ===== MAIN ===== */

int main() {
    Lista lista;
    inicializar(&lista);

    carregarCSV(&lista);

    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {

            case 1: {
                Musica nova;
                printf("\n--- Cadastrar Musica ---\n");

                printf("ID: ");
                scanf("%d", &nova.id);
                getchar();

                if (nova.id <= 0) {
                    printf("ID invalido. Deve ser um numero positivo.\n");
                    break;
                }
                if (idExiste(&lista, nova.id)) {
                    printf("Erro: ID %d ja existe na playlist.\n", nova.id);
                    break;
                }

                lerString(nova.titulo, sizeof(nova.titulo), "Titulo: ");
                if (strlen(nova.titulo) == 0) { printf("Titulo nao pode ser vazio.\n"); break; }

                lerString(nova.artista, sizeof(nova.artista), "Artista: ");
                if (strlen(nova.artista) == 0) { printf("Artista nao pode ser vazio.\n"); break; }

                lerString(nova.duracao, sizeof(nova.duracao), "Duracao (MM:SS): ");
                if (strlen(nova.duracao) == 0) { printf("Duracao nao pode ser vazia.\n"); break; }

                lerString(nova.genero, sizeof(nova.genero), "Genero: ");
                if (strlen(nova.genero) == 0) { printf("Genero nao pode ser vazio.\n"); break; }

                if (inserir(&lista, nova))
                    printf("Musica '%s' cadastrada com sucesso.\n", nova.titulo);
                break;
            }

            case 2: {
                int id;
                printf("\n--- Buscar por ID ---\n");
                printf("ID: ");
                scanf("%d", &id);
                getchar();

                No *encontrado = buscarPorId(&lista, id);
                if (encontrado == NULL) {
                    printf("Musica com ID %d nao encontrada.\n", id);
                } else {
                    printf("\nMusica encontrada:\n");
                    printf("  ID      : %d\n", encontrado->dados.id);
                    printf("  Titulo  : %s\n", encontrado->dados.titulo);
                    printf("  Artista : %s\n", encontrado->dados.artista);
                    printf("  Duracao : %s\n", encontrado->dados.duracao);
                    printf("  Genero  : %s\n", encontrado->dados.genero);
                }
                break;
            }

            case 3: {
                int id;
                printf("\n--- Editar Musica ---\n");
                printf("ID da musica a editar: ");
                scanf("%d", &id);
                getchar();
                printf("Deixe em branco para manter o valor atual.\n");
                editar(&lista, id);
                break;
            }

            case 4: {
                int id;
                printf("\n--- Remover Musica ---\n");
                printf("ID da musica a remover: ");
                scanf("%d", &id);
                getchar();
                remover(&lista, id);
                break;
            }

            case 5: {
                printf("\n--- Lista de Musicas ---\n");
                listar(&lista);
                break;
            }

            case 6:
                salvarCSV(&lista);
                break;

            case 7:
                liberarLista(&lista);
                inicializar(&lista);
                carregarCSV(&lista);
                break;

            case 0:
                salvarCSV(&lista);
                liberarLista(&lista);
                printf("Saindo... Ate logo!\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
