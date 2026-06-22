#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

/* ===== STRUCT ===== */

typedef struct {
    int id;
    char titulo[80];
    char artista[60];
    char duracao[10]; /* formato MM:SS */
    char genero[30];
} Musica;

typedef struct {
    Musica dados[MAX];
    int tamanho;
} Playlist;

/* ===== INICIALIZAR ===== */

void inicializar(Playlist *p) {
    p->tamanho = 0;
}

/* ===== VAZIA / CHEIA ===== */

int listaVazia(Playlist *p) {
    return p->tamanho == 0;
}

int listaCheia(Playlist *p) {
    return p->tamanho == MAX;
}

/* ===== VERIFICAR ID DUPLICADO ===== */

int idExiste(Playlist *p, int id) {
    for (int i = 0; i < p->tamanho; i++) {
        if (p->dados[i].id == id)
            return 1;
    }
    return 0;
}

/* ===== BUSCAR POSICAO POR ID ===== */

int buscarPosicao(Playlist *p, int id) {
    for (int i = 0; i < p->tamanho; i++) {
        if (p->dados[i].id == id)
            return i;
    }
    return -1;
}

/* ===== INSERIR (no fim do vetor) ===== */

int inserir(Playlist *p, Musica m) {
    if (listaCheia(p)) {
        printf("Playlist cheia. Capacidade maxima: %d musicas.\n", MAX);
        return 0;
    }
    p->dados[p->tamanho] = m;
    p->tamanho++;
    return 1;
}

/* ===== LISTAR ===== */

void listar(Playlist *p) {
    if (listaVazia(p)) {
        printf("Nenhuma musica cadastrada.\n");
        return;
    }
    printf("\n%-4s %-6s %-35s %-25s %-8s %-15s\n",
           "#", "ID", "Titulo", "Artista", "Duracao", "Genero");
    printf("------------------------------------------------------------------------------------\n");
    for (int i = 0; i < p->tamanho; i++) {
        printf("%-4d %-6d %-35s %-25s %-8s %-15s\n",
               i + 1,
               p->dados[i].id,
               p->dados[i].titulo,
               p->dados[i].artista,
               p->dados[i].duracao,
               p->dados[i].genero);
    }
    printf("------------------------------------------------------------------------------------\n");
    printf("Total: %d musica(s)\n", p->tamanho);
}

/* ===== EDITAR ===== */

int editar(Playlist *p, int id) {
    int pos = buscarPosicao(p, id);
    if (pos == -1) {
        printf("Musica com ID %d nao encontrada.\n", id);
        return 0;
    }

    char buffer[80];

    printf("Novo titulo [%s]: ", p->dados[pos].titulo);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strncpy(p->dados[pos].titulo, buffer, sizeof(p->dados[pos].titulo) - 1);

    printf("Novo artista [%s]: ", p->dados[pos].artista);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strncpy(p->dados[pos].artista, buffer, sizeof(p->dados[pos].artista) - 1);

    printf("Nova duracao (MM:SS) [%s]: ", p->dados[pos].duracao);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strncpy(p->dados[pos].duracao, buffer, sizeof(p->dados[pos].duracao) - 1);

    printf("Novo genero [%s]: ", p->dados[pos].genero);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strncpy(p->dados[pos].genero, buffer, sizeof(p->dados[pos].genero) - 1);

    printf("Musica %d atualizada com sucesso.\n", id);
    return 1;
}

/* ===== REMOVER (desloca elementos) ===== */

int remover(Playlist *p, int id) {
    if (listaVazia(p)) {
        printf("Lista vazia. Nada para remover.\n");
        return 0;
    }

    int pos = buscarPosicao(p, id);
    if (pos == -1) {
        printf("Musica com ID %d nao encontrada.\n", id);
        return 0;
    }

    for (int i = pos; i < p->tamanho - 1; i++) {
        p->dados[i] = p->dados[i + 1];
    }
    p->tamanho--;
    printf("Musica ID %d removida com sucesso.\n", id);
    return 1;
}

/* ===== SALVAR CSV ===== */

void salvarCSV(Playlist *p) {
    if (listaVazia(p)) {
        printf("Lista vazia. Nada para salvar.\n");
        return;
    }

    FILE *arquivo = fopen("dados_a.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar.\n");
        return;
    }

    fprintf(arquivo, "id;titulo;artista;duracao;genero\n");
    for (int i = 0; i < p->tamanho; i++) {
        fprintf(arquivo, "%d;%s;%s;%s;%s\n",
                p->dados[i].id,
                p->dados[i].titulo,
                p->dados[i].artista,
                p->dados[i].duracao,
                p->dados[i].genero);
    }

    fclose(arquivo);
    printf("Dados salvos em dados_a.csv com sucesso.\n");
}

/* ===== CARREGAR CSV ===== */

void carregarCSV(Playlist *p) {
    FILE *arquivo = fopen("dados_a.csv", "r");
    if (arquivo == NULL) {
        printf("Arquivo dados_a.csv nao encontrado. Iniciando vazio.\n");
        return;
    }

    char linha[200];
    fgets(linha, sizeof(linha), arquivo); /* pula cabecalho */

    int carregados = 0;
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (listaCheia(p)) {
            printf("Playlist atingiu capacidade maxima durante o carregamento.\n");
            break;
        }

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

        inserir(p, m);
        carregados++;
    }

    fclose(arquivo);
    if (carregados > 0)
        printf("%d musica(s) carregada(s) do arquivo.\n", carregados);
    else
        printf("Arquivo vazio ou sem dados validos.\n");
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
    printf("    PLAYLIST DE MUSICAS (VETOR)         \n");
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
    Playlist playlist;
    inicializar(&playlist);

    carregarCSV(&playlist);

    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {

            case 1: {
                if (listaCheia(&playlist)) {
                    printf("Playlist cheia. Capacidade maxima: %d musicas.\n", MAX);
                    break;
                }

                Musica nova;
                printf("\n--- Cadastrar Musica ---\n");

                printf("ID: ");
                scanf("%d", &nova.id);
                getchar();

                if (nova.id <= 0) {
                    printf("ID invalido. Deve ser um numero positivo.\n");
                    break;
                }
                if (idExiste(&playlist, nova.id)) {
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

                if (inserir(&playlist, nova))
                    printf("Musica '%s' cadastrada com sucesso.\n", nova.titulo);
                break;
            }

            case 2: {
                int id;
                printf("\n--- Buscar por ID ---\n");
                printf("ID: ");
                scanf("%d", &id);
                getchar();

                int pos = buscarPosicao(&playlist, id);
                if (pos == -1) {
                    printf("Musica com ID %d nao encontrada.\n", id);
                } else {
                    printf("\nMusica encontrada:\n");
                    printf("  ID      : %d\n", playlist.dados[pos].id);
                    printf("  Titulo  : %s\n", playlist.dados[pos].titulo);
                    printf("  Artista : %s\n", playlist.dados[pos].artista);
                    printf("  Duracao : %s\n", playlist.dados[pos].duracao);
                    printf("  Genero  : %s\n", playlist.dados[pos].genero);
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
                editar(&playlist, id);
                break;
            }

            case 4: {
                int id;
                printf("\n--- Remover Musica ---\n");
                printf("ID da musica a remover: ");
                scanf("%d", &id);
                getchar();
                remover(&playlist, id);
                break;
            }

            case 5: {
                printf("\n--- Lista de Musicas ---\n");
                listar(&playlist);
                break;
            }

            case 6:
                salvarCSV(&playlist);
                break;

            case 7:
                inicializar(&playlist);
                carregarCSV(&playlist);
                break;

            case 0:
                salvarCSV(&playlist);
                printf("Saindo... Ate logo!\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
