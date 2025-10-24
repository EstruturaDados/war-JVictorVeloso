/*
 * WAR Estruturado - Nível Aventureiro
 * Sistema de Batalha entre Territórios
 * 
 * Este programa implementa um sistema completo de War com funcionalidades de:
 * - Cadastro dinâmico de territórios
 * - Simulação de ataques entre territórios
 * - Gerenciamento de memória dinâmica
 * - Uso de ponteiros para manipulação de dados
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da struct Territorio para armazenar informações dos territórios
typedef struct {
    char nome[30];      // Nome do território (máximo 29 caracteres + \0)
    char cor[10];       // Cor do exército (máximo 9 caracteres + \0)
    int tropas;         // Quantidade de tropas no território
} Territorio;

// Função para alocar memória dinamicamente para os territórios
Territorio* alocarTeritorios(int quantidade) {
    Territorio* mapa = (Territorio*)calloc(quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro: Não foi possível alocar memória para os territórios!\n");
        exit(1);
    }
    printf("Memória alocada com sucesso para %d territórios.\n", quantidade);
    return mapa;
}

// Função para liberar a memória alocada dinamicamente
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("Memória liberada com sucesso.\n");
    }
}

// Função para cadastrar um território usando ponteiros
void cadastrarTerritorio(Territorio* territorio, int indice) {
    printf("\n=== CADASTRO DO TERRITÓRIO %d ===\n", indice + 1);
    
    // Entrada do nome do território
    printf("Digite o nome do território: ");
    scanf(" %29s", territorio->nome);  // Uso de ponteiro para acessar o campo
    
    // Entrada da cor do exército
    printf("Digite a cor do exército: ");
    scanf(" %9s", territorio->cor);    // Uso de ponteiro para acessar o campo
    
    // Entrada da quantidade de tropas (mínimo 1 para poder atacar)
    do {
        printf("Digite o número de tropas (mínimo 1): ");
        scanf("%d", &territorio->tropas);
        if (territorio->tropas < 1) {
            printf("Erro: Um território deve ter pelo menos 1 tropa!\n");
        }
    } while (territorio->tropas < 1);
    
    printf("Território '%s' cadastrado com sucesso!\n", territorio->nome);
}

// Função para exibir os dados de um território usando ponteiros
void exibirTerritorio(Territorio* territorio, int indice) {
    printf("Território %d:\n", indice + 1);
    printf("  Nome: %s\n", territorio->nome);
    printf("  Cor do Exército: %s\n", territorio->cor);
    printf("  Tropas: %d\n", territorio->tropas);
    printf("  ---------------------------\n");
}

// Função para exibir todos os territórios cadastrados
void exibirTodosTeritorios(Territorio* territorios, int quantidade) {
    printf("\n=== MAPA DE TERRITÓRIOS ===\n");
    printf("Total de territórios: %d\n\n", quantidade);
    
    for (int i = 0; i < quantidade; i++) {
        exibirTerritorio(&territorios[i], i);
    }
}

// Função para simular rolagem de dados (1 a 6)
int rolarDado() {
    return (rand() % 6) + 1;
}

// Função principal de ataque entre territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n=== SIMULAÇÃO DE BATALHA ===\n");
    printf("Atacante: %s (%s) com %d tropas\n", 
           atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s (%s) com %d tropas\n", 
           defensor->nome, defensor->cor, defensor->tropas);
    
    // Simular rolagem de dados
    int dadoAtacante = rolarDado();
    int dadoDefensor = rolarDado();
    
    printf("\nRolagem de dados:\n");
    printf("Atacante (%s): %d\n", atacante->cor, dadoAtacante);
    printf("Defensor (%s): %d\n", defensor->cor, dadoDefensor);
    
    // Determinar resultado da batalha
    if (dadoAtacante > dadoDefensor) {
        printf("\n🎯 VITÓRIA DO ATACANTE!\n");
        printf("O território '%s' foi conquistado por %s!\n", 
               defensor->nome, atacante->cor);
        
        // Transferir controle do território
        strcpy(defensor->cor, atacante->cor);
        
        // Transferir metade das tropas do atacante (mínimo 1)
        int tropasTransferidas = (atacante->tropas / 2) > 0 ? (atacante->tropas / 2) : 1;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
        
        printf("Tropas transferidas: %d\n", tropasTransferidas);
    } else {
        printf("\n🛡️ DEFESA BEM-SUCEDIDA!\n");
        printf("O território '%s' resistiu ao ataque!\n", defensor->nome);
        
        // Atacante perde uma tropa
        if (atacante->tropas > 1) {
            atacante->tropas--;
            printf("O atacante perdeu 1 tropa na batalha.\n");
        } else {
            printf("O atacante não pode perder mais tropas (mínimo 1).\n");
        }
    }
    
    printf("\nResultado pós-batalha:\n");
    printf("- %s (%s): %d tropas\n", 
           atacante->nome, atacante->cor, atacante->tropas);
    printf("- %s (%s): %d tropas\n", 
           defensor->nome, defensor->cor, defensor->tropas);
}

// Função para validar se um ataque é válido
int validarAtaque(Territorio* atacante, Territorio* defensor) {
    // Verificar se são territórios diferentes
    if (strcmp(atacante->nome, defensor->nome) == 0) {
        printf("Erro: Um território não pode atacar a si mesmo!\n");
        return 0;
    }
    
    // Verificar se o atacante tem tropas suficientes
    if (atacante->tropas < 2) {
        printf("Erro: O território atacante deve ter pelo menos 2 tropas para atacar!\n");
        return 0;
    }
    
    // Verificar se não é um ataque da mesma cor (aliados)
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: Territórios da mesma cor não podem se atacar!\n");
        return 0;
    }
    
    return 1; // Ataque válido
}

// Função para selecionar territórios para batalha
void selecionarEAtacar(Territorio* territorios, int quantidade) {
    int indiceAtacante, indiceDefensor;
    
    printf("\n=== SELEÇÃO DE TERRITÓRIOS PARA BATALHA ===\n");
    
    // Exibir territórios disponíveis
    exibirTodosTeritorios(territorios, quantidade);
    
    // Selecionar território atacante
    do {
        printf("\nEscolha o território ATACANTE (1 a %d): ", quantidade);
        scanf("%d", &indiceAtacante);
        indiceAtacante--; // Converter para índice base 0
        
        if (indiceAtacante < 0 || indiceAtacante >= quantidade) {
            printf("Erro: Índice inválido! Escolha entre 1 e %d.\n", quantidade);
        } else if (territorios[indiceAtacante].tropas < 2) {
            printf("Erro: O território '%s' não tem tropas suficientes para atacar!\n", 
                   territorios[indiceAtacante].nome);
            indiceAtacante = -1; // Forçar nova seleção
        }
    } while (indiceAtacante < 0 || indiceAtacante >= quantidade);
    
    // Selecionar território defensor
    do {
        printf("Escolha o território DEFENSOR (1 a %d): ", quantidade);
        scanf("%d", &indiceDefensor);
        indiceDefensor--; // Converter para índice base 0
        
        if (indiceDefensor < 0 || indiceDefensor >= quantidade) {
            printf("Erro: Índice inválido! Escolha entre 1 e %d.\n", quantidade);
        }
    } while (indiceDefensor < 0 || indiceDefensor >= quantidade);
    
    // Validar e executar o ataque
    if (validarAtaque(&territorios[indiceAtacante], &territorios[indiceDefensor])) {
        atacar(&territorios[indiceAtacante], &territorios[indiceDefensor]);
    }
}

int main() {
    // Inicializar gerador de números aleatórios
    srand(time(NULL));
    
    int quantidadeTeritorios;
    Territorio* mapa = NULL;
    int opcao;
    
    printf("=== WAR ESTRUTURADO - NÍVEL AVENTUREIRO ===\n");
    printf("Sistema de Batalha entre Territórios\n\n");
    
    // Solicitar quantidade de territórios
    do {
        printf("Quantos territórios deseja cadastrar? (mínimo 2): ");
        scanf("%d", &quantidadeTeritorios);
        if (quantidadeTeritorios < 2) {
            printf("Erro: É necessário pelo menos 2 territórios para jogar!\n");
        }
    } while (quantidadeTeritorios < 2);
    
    // Alocar memória dinamicamente
    mapa = alocarTeritorios(quantidadeTeritorios);
    
    printf("\nVocê irá cadastrar %d territórios.\n", quantidadeTeritorios);
    printf("Para cada território, informe: nome, cor do exército e número de tropas.\n");
    
    // Cadastrar territórios
    for (int i = 0; i < quantidadeTeritorios; i++) {
        cadastrarTerritorio(&mapa[i], i);
    }
    
    // Menu principal do jogo
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Exibir mapa de territórios\n");
        printf("2. Realizar ataque\n");
        printf("3. Sair do jogo\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                exibirTodosTeritorios(mapa, quantidadeTeritorios);
                break;
                
            case 2:
                selecionarEAtacar(mapa, quantidadeTeritorios);
                break;
                
            case 3:
                printf("\nEncerrando o jogo...\n");
                break;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        
        if (opcao != 3) {
            printf("\nPressione Enter para continuar...");
            getchar(); // Limpar buffer
            getchar(); // Aguardar Enter
        }
        
    } while (opcao != 3);
    
    // Exibir mapa final
    printf("\n=== MAPA FINAL DOS TERRITÓRIOS ===\n");
    exibirTodosTeritorios(mapa, quantidadeTeritorios);
    
    // Liberar memória alocada
    liberarMemoria(mapa);
    
    printf("\nObrigado por jogar WAR Estruturado!\n");
    return 0;
}
