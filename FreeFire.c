// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

/*
  torre_avancada.c
  Módulo Avançado: Organização de Componentes da Torre de Resgate
  - Implementa Bubble (nome), Insertion (tipo) e Selection (prioridade)
  - Conta comparações, mede tempo com clock()
  - Busca binária por nome (aplicável apenas se estiver ordenado por nome)
  - Até 20 componentes
  - Interface interativa, código modular e comentado
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_COMP 20
#define MAX_NOME 30
#define MAX_TIPO 20

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; // 1..10
} Componente;

/* Estado de ordenação atual do vetor (para validar busca binária) */
enum OrdenacaoAtual { NAO_ORDENADO=0, ORDENADO_POR_NOME, ORDENADO_POR_TIPO, ORDENADO_POR_PRIORIDADE } estado_ord = NAO_ORDENADO;

/* --- Assinaturas de funções --- */
void cadastrarComponentes(Componente comps[], int *n);
void mostrarComponentes(Componente comps[], int n);
void bubbleSortNome(Componente arr[], int n, long *comparacoes);
void insertionSortTipo(Componente arr[], int n, long *comparacoes);
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes);
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes);
void medirTempoOrdenacao(void (*algoritmo)(Componente[], int, long*), Componente arr[], int n, long *comparacoes, const char *rotulo);
void copiarVetor(Componente src[], Componente dst[], int n);
void rodarBenchmark(Componente origem[], int n);
void limparString(char *s, size_t cap);
void toLowerStr(char *dst, const char *src);
int confirmar(const char *msg);

/* --- Main --- */
int main(void) {
    Componente comps[MAX_COMP];
    int n = 0;
    char linha[64];

    printf("=== Módulo Avançado: Torre de Resgate (Organização de Componentes) ===\n\n");
    cadastrarComponentes(comps, &n);

    if (n == 0) {
        printf("Nenhum componente cadastrado. Encerrando.\n");
        return 0;
    }

    while (1) {
        printf("\n--- Menu Principal ---\n");
        printf("1 - Mostrar componentes (ordem atual)\n");
        printf("2 - Ordenar por NOME (Bubble Sort)\n");
        printf("3 - Ordenar por TIPO (Insertion Sort)\n");
        printf("4 - Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("5 - Busca binária por NOME (só se ordenado por NOME)\n");
        printf("6 - Rodar benchmark (executa as 3 ordenações em cópias e compara)\n");
        printf("7 - Re-cadastrar componentes (apaga e cadastra novos)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (!fgets(linha, sizeof(linha), stdin)) break;
        int opc = atoi(linha);

        if (opc == 0) {
            printf("Encerrando. Boa sorte na fuga!\n");
            break;
        }

        switch (opc) {
            case 1:
                mostrarComponentes(comps, n);
                break;

            case 2: {
                long comps_count = 0;
                medirTempoOrdenacao(bubbleSortNome, comps, n, &comps_count, "Bubble Sort por NOME");
                printf("Comparações: %ld\n", comps_count);
                estado_ord = ORDENADO_POR_NOME;
                mostrarComponentes(comps, n);
                break;
            }

            case 3: {
                long comps_count = 0;
                medirTempoOrdenacao(insertionSortTipo, comps, n, &comps_count, "Insertion Sort por TIPO");
                printf("Comparações: %ld\n", comps_count);
                estado_ord = ORDENADO_POR_TIPO;
                mostrarComponentes(comps, n);
                break;
            }

            case 4: {
                long comps_count = 0;
                medirTempoOrdenacao(selectionSortPrioridade, comps, n, &comps_count, "Selection Sort por PRIORIDADE");
                printf("Comparações: %ld\n", comps_count);
                estado_ord = ORDENADO_POR_PRIORIDADE;
                mostrarComponentes(comps, n);
                break;
            }

            case 5: {
                if (estado_ord != ORDENADO_POR_NOME) {
                    printf("Busca binária só é segura se o vetor estiver ordenado por NOME.\n");
                    if (confirmar("Deseja ordenar agora por NOME (Bubble Sort) e prosseguir? (s/n): ")) {
                        long c = 0;
                        medirTempoOrdenacao(bubbleSortNome, comps, n, &c, "Bubble Sort por NOME (pré-busca)");
                        printf("Comparações: %ld\n", c);
                        estado_ord = ORDENADO_POR_NOME;
                    } else {
                        printf("Operação de busca cancelada. Ordene por NOME antes.\n");
                        break;
                    }
                }
                char chave[MAX_NOME];
                printf("Digite o NOME do componente-chave (busca exata): ");
                if (!fgets(chave, sizeof(chave), stdin)) { chave[0] = '\0'; }
                limparString(chave, sizeof(chave));
                if (chave[0] == '\0') {
                    printf("Busca cancelada (nome vazio).\n");
                    break;
                }
                long comps_busca = 0;
                int idx = buscaBinariaPorNome(comps, n, chave, &comps_busca);
                printf("Comparações na busca: %ld\n", comps_busca);
                if (idx >= 0) {
                    printf("\n>>> Componente-CHAVE encontrado! (índice %d)\n", idx);
                    printf("  Nome: %s\n  Tipo: %s\n  Prioridade: %d\n\n", comps[idx].nome, comps[idx].tipo, comps[idx].prioridade);
                    printf(">>> Confirmação visual: componente presente — torre poderá ser ativada.\n");
                } else {
                    printf("Componente '%s' não localizado.\n", chave);
                }
                break;
            }

            case 6:
                rodarBenchmark(comps, n);
                break;

            case 7:
                if (confirmar("Deseja apagar o cadastro atual e re-cadastrar? (s/n): ")) {
                    cadastrarComponentes(comps, &n);
                    estado_ord = NAO_ORDENADO;
                } else {
                    printf("Re-cadastro cancelado.\n");
                }
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }
    return 0;
}

/* -------------------------------------------------------------
   cadastrarComponentes
   - Lê até MAX_COMP componentes usando fgets
   - Uso seguro de strings (trim newline)
   ------------------------------------------------------------- */
void cadastrarComponentes(Componente comps[], int *n) {
    char buf[128];
    *n = 0;
    printf("Cadastro de componentes (até %d). Deixe o NOME vazio para parar.\n", MAX_COMP);
    for (int i = 0; i < MAX_COMP; ++i) {
        printf("\nComponente %d\n", i+1);
        printf("Nome: ");
        if (!fgets(comps[i].nome, sizeof(comps[i].nome), stdin)) { comps[i].nome[0] = '\0'; }
        limparString(comps[i].nome, sizeof(comps[i].nome));
        if (comps[i].nome[0] == '\0') {
            break; // parada precoce
        }
        printf("Tipo (ex: controle, suporte, propulsao): ");
        if (!fgets(comps[i].tipo, sizeof(comps[i].tipo), stdin)) comps[i].tipo[0] = '\0';
        limparString(comps[i].tipo, sizeof(comps[i].tipo));

        int p = 0;
        while (1) {
            printf("Prioridade (1-10): ");
            if (!fgets(buf, sizeof(buf), stdin)) buf[0] = '\0';
            p = atoi(buf);
            if (p >= 1 && p <= 10) break;
            printf("Prioridade inválida. Digite um número entre 1 e 10.\n");
        }
        comps[i].prioridade = p;
        (*n)++;
    }
    printf("\nCadastro finalizado. %d componentes cadastrados.\n", *n);
}

/* -------------------------------------------------------------
   mostrarComponentes
   - Exibe o vetor formatado com índices
   ------------------------------------------------------------- */
void mostrarComponentes(Componente comps[], int n) {
    if (n == 0) {
        printf("\nNenhum componente para exibir.\n");
        return;
    }
    printf("\n--- Componentes (total: %d) ---\n", n);
    printf("%-4s | %-30s | %-20s | %-9s\n", "Idx", "NOME", "TIPO", "PRIORIDADE");
    printf("----------------------------------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        printf("%-4d | %-30s | %-20s | %-9d\n", i, comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
    printf("----------------------------------------------------------------------------\n");
}

/* -------------------------------------------------------------
   bubbleSortNome
   - Ordena por nome (strcmp), conta comparações
   - Versão com "short-circuit" se já ordenado
   ------------------------------------------------------------- */
void bubbleSortNome(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n < 2) return;
    for (int i = 0; i < n - 1; ++i) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; ++j) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

/* -------------------------------------------------------------
   insertionSortTipo
   - Ordena por tipo (strcmp), conta comparações
   - Estável por construção (insertion)
   ------------------------------------------------------------- */
void insertionSortTipo(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; ++i) {
        Componente chave = arr[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = chave;
    }
}

/* -------------------------------------------------------------
   selectionSortPrioridade
   - Ordena por prioridade (int) crescente (1..10)
   - Conta comparações (entre prioridades)
   ------------------------------------------------------------- */
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; ++i) {
        int menor = i;
        for (int j = i + 1; j < n; ++j) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[menor].prioridade) menor = j;
        }
        if (menor != i) {
            Componente tmp = arr[i];
            arr[i] = arr[menor];
            arr[menor] = tmp;
        }
    }
}

/* -------------------------------------------------------------
   buscaBinariaPorNome
   - Requer vetor ordenado por nome (lexicográfico)
   - Retorna índice ou -1 se não encontrado
   - Conta comparações (cada strcmp com o meio)
   ------------------------------------------------------------- */
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes) {
    int l = 0, r = n - 1;
    *comparacoes = 0;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        (*comparacoes)++;
        int cmp = strcmp(arr[mid].nome, chave);
        if (cmp == 0) return mid;
        else if (cmp < 0) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}

/* -------------------------------------------------------------
   medirTempoOrdenacao
   - Executa o algoritmo (in-place) e imprime tempo em segundos
   - Retorna comparações via ponteiro
   ------------------------------------------------------------- */
void medirTempoOrdenacao(void (*algoritmo)(Componente[], int, long*), Componente arr[], int n, long *comparacoes, const char *rotulo) {
    clock_t t0 = clock();
    algoritmo(arr, n, comparacoes);
    clock_t t1 = clock();
    double tempo = (double)(t1 - t0) / CLOCKS_PER_SEC;
    printf("\n[%s] Tempo de execução: %.6f s\n", rotulo, tempo);
}

/* -------------------------------------------------------------
   copiarVetor
   - Cópia simples de structs (útil para testes/benchmark)
   ------------------------------------------------------------- */
void copiarVetor(Componente src[], Componente dst[], int n) {
    for (int i = 0; i < n; ++i) dst[i] = src[i];
}

/* -------------------------------------------------------------
   rodarBenchmark
   - Executa os 3 algoritmos em cópias do vetor original
   - Mostra comparações e tempo lado a lado (educacional)
   ------------------------------------------------------------- */
void rodarBenchmark(Componente origem[], int n) {
    if (n == 0) {
        printf("Nenhum componente para benchmark.\n");
        return;
    }
    Componente a[MAX_COMP], b[MAX_COMP], c[MAX_COMP];
    copiarVetor(origem, a, n);
    copiarVetor(origem, b, n);
    copiarVetor(origem, c, n);

    long compA=0, compB=0, compC=0;
    clock_t s, e;
    double tA, tB, tC;

    s = clock(); bubbleSortNome(a, n, &compA); e = clock(); tA = (double)(e - s) / CLOCKS_PER_SEC;
    s = clock(); insertionSortTipo(b, n, &compB); e = clock(); tB = (double)(e - s) / CLOCKS_PER_SEC;
    s = clock(); selectionSortPrioridade(c, n, &compC); e = clock(); tC = (double)(e - s) / CLOCKS_PER_SEC;

    printf("\n--- Benchmark educativo (ordena cópias do vetor) ---\n");
    printf("%-25s | %-12s | %-12s\n", "Algoritmo", "Comparações", "Tempo (s)");
    printf("-------------------------------------------------------------\n");
    printf("%-25s | %-12ld | %-12.6f\n", "Bubble Sort (nome)", compA, tA);
    printf("%-25s | %-12ld | %-12.6f\n", "Insertion Sort (tipo)", compB, tB);
    printf("%-25s | %-12ld | %-12.6f\n", "Selection Sort (prioridade)", compC, tC);
    printf("-------------------------------------------------------------\n");
    printf("Observação: os tempos e comparações dependem do estado inicial do vetor e do hardware.\n");
}

/* -------------------------------------------------------------
   limparString
   - Remove '\n' final e espaços extras nas bordas
   ------------------------------------------------------------- */
void limparString(char *s, size_t cap) {
    if (!s) return;
    size_t len = strnlen(s, cap);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' || s[len-1] == ' ' || s[len-1] == '\t')) {
        s[len-1] = '\0';
        len--;
    }
    // trim left
    size_t i = 0;
    while (s[i] && (s[i] == ' ' || s[i] == '\t')) i++;
    if (i > 0) memmove(s, s + i, strlen(s + i) + 1);
}

/* -------------------------------------------------------------
   toLowerStr (não usada diretamente, mas útil se desejar
   busca case-insensitive futuramente)
   ------------------------------------------------------------- */
void toLowerStr(char *dst, const char *src) {
    while (*src && (dst[0] = (char)tolower((unsigned char)*src))) { dst++; src++; }
    *dst = '\0';
}

/* -------------------------------------------------------------
   confirmar
   - Lê 's' ou 'n' do usuário (case-insensitive)
   ------------------------------------------------------------- */
int confirmar(const char *msg) {
    char buf[8];
    printf("%s", msg);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    limparString(buf, sizeof(buf));
    if (buf[0] == 's' || buf[0] == 'S') return 1;
    return 0;
}
    
    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
