/*
 * WAR Estruturado - Nível Mestre
 * Sistema Completo com Missões Estratégicas
 * 
 * Este programa implementa um sistema completo de War com:
 * - Cadastro dinâmico de territórios
 * - Sistema de jogadores com missões individuais
 * - Simulação de ataques entre territórios
 * - Verificação automática de condições de vitória
 * - Gerenciamento completo de memória dinâmica
 * - Modularização avançada com ponteiros
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

// Definição da struct Jogador para armazenar informações dos jogadores
typedef struct {
    char nome[20];      // Nome do jogador
    char cor[10];       // Cor do exército do jogador
    char* missao;       // Missão estratégica (alocada dinamicamente)
    int venceu;         // Flag para indicar se cumpriu a missão
} Jogador;

// Vetor global de missões estratégicas pré-definidas
char* missoesPredefinidas[] = {
    "Conquistar 3 territorios consecutivos da mesma cor",
    "Eliminar todas as tropas de cor vermelha do mapa",
    "Controlar pelo menos 5 territorios simultaneamente",
    "Acumular mais de 15 tropas em um unico territorio",
    "Conquistar territorios de pelo menos 3 cores diferentes",
    "Ser o jogador com maior numero total de tropas",
    "Conquistar o territorio chamado 'Capital' se existir"
};

const int TOTAL_MISSOES = 7;

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

// Função para alocar memória dinamicamente para os jogadores
Jogador* alocarJogadores(int quantidade) {
    Jogador* jogadores = (Jogador*)calloc(quantidade, sizeof(Jogador));
    if (jogadores == NULL) {
        printf("Erro: Não foi possível alocar memória para os jogadores!\n");
        exit(1);
    }
    printf("Memória alocada com sucesso para %d jogadores.\n", quantidade);
    return jogadores;
}

// Função para atribuir missão estratégica a um jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceMissao = rand() % totalMissoes;
    
    // Alocar memória para a missão
    destino = (char*)malloc(strlen(missoes[indiceMissao]) + 1);
    if (destino == NULL) {
        printf("Erro: Não foi possível alocar memória para a missão!\n");
        exit(1);
    }
    
    // Copiar a missão sorteada
    strcpy(destino, missoes[indiceMissao]);
}

// Função corrigida para atribuir missão (passagem por referência)
void atribuirMissaoCorreta(Jogador* jogador, char* missoes[], int totalMissoes) {
    int indiceMissao = rand() % totalMissoes;
    
    // Alocar memória para a missão
    jogador->missao = (char*)malloc(strlen(missoes[indiceMissao]) + 1);
    if (jogador->missao == NULL) {
        printf("Erro: Não foi possível alocar memória para a missão!\n");
        exit(1);
    }
    
    // Copiar a missão sorteada
    strcpy(jogador->missao, missoes[indiceMissao]);
    jogador->venceu = 0; // Inicializar como não venceu
}

// Função para exibir a missão de um jogador (passagem por valor)
void exibirMissao(char* missao, char* nomeJogador) {
    printf("\n🎯 MISSÃO ESTRATÉGICA DE %s:\n", nomeJogador);
    printf("   \"%s\"\n", missao);
    printf("   Complete esta missão para vencer o jogo!\n");
}

// Função para verificar se uma missão foi cumprida
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    // Missão 1: Conquistar 3 territórios consecutivos da mesma cor
    if (strstr(missao, "3 territorios consecutivos") != NULL) {
        int consecutivos = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                consecutivos++;
                if (consecutivos >= 3) return 1;
            } else {
                consecutivos = 0;
            }
        }
    }
    
    // Missão 2: Eliminar todas as tropas de cor vermelha
    else if (strstr(missao, "cor vermelha") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0 || strcmp(mapa[i].cor, "vermelho") == 0) {
                return 0; // Ainda existem territórios vermelhos
            }
        }
        return 1; // Não há mais territórios vermelhos
    }
    
    // Missão 3: Controlar pelo menos 5 territórios
    else if (strstr(missao, "5 territorios") != NULL) {
        int territoriosControlados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                territoriosControlados++;
            }
        }
        return territoriosControlados >= 5;
    }
    
    // Missão 4: Acumular mais de 15 tropas em um único território
    else if (strstr(missao, "15 tropas") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas > 15) {
                return 1;
            }
        }
    }
    
    // Missão 5: Conquistar territórios de pelo menos 3 cores diferentes
    else if (strstr(missao, "3 cores diferentes") != NULL) {
        char coresConquistadas[10][10];
        int totalCores = 0;
        
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                // Verificar se já conquistou território dessa cor original
                int jaContada = 0;
                for (int j = 0; j < totalCores; j++) {
                    if (strcmp(coresConquistadas[j], mapa[i].cor) == 0) {
                        jaContada = 1;
                        break;
                    }
                }
                if (!jaContada && totalCores < 10) {
                    strcpy(coresConquistadas[totalCores], mapa[i].cor);
                    totalCores++;
                }
            }
        }
        return totalCores >= 3;
    }
    
    // Missão 6: Ser o jogador com maior número total de tropas
    else if (strstr(missao, "maior numero total") != NULL) {
        int tropasJogador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                tropasJogador += mapa[i].tropas;
            }
        }
        // Implementação simplificada - retorna verdadeiro se tem mais de 20 tropas
        return tropasJogador > 20;
    }
    
    // Missão 7: Conquistar território chamado 'Capital'
    else if (strstr(missao, "Capital") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].nome, "Capital") == 0 && strcmp(mapa[i].cor, corJogador) == 0) {
                return 1;
            }
        }
    }
    
    return 0; // Missão não cumprida
}

// Função para verificar se algum jogador venceu
int verificarVencedor(Jogador* jogadores, int numJogadores, Territorio* mapa, int tamanhoMapa) {
    for (int i = 0; i < numJogadores; i++) {
        if (!jogadores[i].venceu) {
            if (verificarMissao(jogadores[i].missao, mapa, tamanhoMapa, jogadores[i].cor)) {
                jogadores[i].venceu = 1;
                printf("\n🏆 TEMOS UM VENCEDOR! 🏆\n");
                printf("Jogador %s (%s) completou sua missão!\n", 
                       jogadores[i].nome, jogadores[i].cor);
                printf("Missão: \"%s\"\n", jogadores[i].missao);
                return i; // Retorna índice do vencedor
            }
        }
    }
    return -1; // Nenhum vencedor ainda
}

// Função para liberar a memória alocada dinamicamente
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores) {
    // Liberar memória dos territórios
    if (mapa != NULL) {
        free(mapa);
        printf("Memória dos territórios liberada com sucesso.\n");
    }
    
    // Liberar memória das missões dos jogadores
    if (jogadores != NULL) {
        for (int i = 0; i < numJogadores; i++) {
            if (jogadores[i].missao != NULL) {
                free(jogadores[i].missao);
            }
        }
        free(jogadores);
    }
}

// Função para cadastrar um jogador
void cadastrarJogador(Jogador* jogador, int indice) {
    printf("\n=== CADASTRO DO JOGADOR %d ===\n", indice + 1);
    
    printf("Digite o nome do jogador: ");
    scanf(" %19s", jogador->nome);
    
    printf("Digite a cor do exército: ");
    scanf(" %9s", jogador->cor);
    
    // Atribuir missão estratégica
    atribuirMissaoCorreta(jogador, missoesPredefinidas, TOTAL_MISSOES);
    
    printf("Jogador '%s' cadastrado com sucesso!\n", jogador->nome);
    exibirMissao(jogador->missao, jogador->nome);
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
    
    int quantidadeTeritorios, numJogadores;
    Territorio* mapa = NULL;
    Jogador* jogadores = NULL;
    int opcao, vencedor = -1;
    
    printf("=== WAR ESTRUTURADO - NÍVEL MESTRE ===\n");
    printf("Sistema Completo com Missões Estratégicas\n\n");
    
    // Solicitar número de jogadores
    do {
        printf("Quantos jogadores irão participar? (2-4): ");
        scanf("%d", &numJogadores);
        if (numJogadores < 2 || numJogadores > 4) {
            printf("Erro: O jogo deve ter entre 2 e 4 jogadores!\n");
        }
    } while (numJogadores < 2 || numJogadores > 4);
    
    // Alocar memória para jogadores
    jogadores = alocarJogadores(numJogadores);
    
    // Cadastrar jogadores
    printf("\n=== FASE DE CADASTRO DOS JOGADORES ===\n");
    for (int i = 0; i < numJogadores; i++) {
        cadastrarJogador(&jogadores[i], i);
    }
    
    // Solicitar quantidade de territórios
    do {
        printf("\nQuantos territórios deseja cadastrar? (mínimo %d): ", numJogadores);
        scanf("%d", &quantidadeTeritorios);
        if (quantidadeTeritorios < numJogadores) {
            printf("Erro: Deve haver pelo menos 1 território por jogador!\n");
        }
    } while (quantidadeTeritorios < numJogadores);
    
    // Alocar memória dinamicamente para territórios
    mapa = alocarTeritorios(quantidadeTeritorios);
    
    printf("\n=== FASE DE CADASTRO DOS TERRITÓRIOS ===\n");
    printf("Você irá cadastrar %d territórios.\n", quantidadeTeritorios);
    
    // Cadastrar territórios
    for (int i = 0; i < quantidadeTeritorios; i++) {
        cadastrarTerritorio(&mapa[i], i);
    }
    
    printf("\n=== INÍCIO DO JOGO ===\n");
    printf("Todos os jogadores e territórios foram cadastrados!\n");
    printf("Consulte suas missões estratégicas para vencer!\n");
    
    // Menu principal do jogo
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Exibir mapa de territórios\n");
        printf("2. Exibir missões dos jogadores\n");
        printf("3. Realizar ataque\n");
        printf("4. Verificar condições de vitória\n");
        printf("5. Sair do jogo\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                exibirTodosTeritorios(mapa, quantidadeTeritorios);
                break;
                
            case 2:
                printf("\n=== MISSÕES ESTRATÉGICAS ===\n");
                for (int i = 0; i < numJogadores; i++) {
                    exibirMissao(jogadores[i].missao, jogadores[i].nome);
                }
                break;
                
            case 3:
                selecionarEAtacar(mapa, quantidadeTeritorios);
                // Verificar se alguém venceu após o ataque
                vencedor = verificarVencedor(jogadores, numJogadores, mapa, quantidadeTeritorios);
                if (vencedor >= 0) {
                    opcao = 5; // Encerrar jogo
                }
                break;
                
            case 4:
                printf("\n=== VERIFICAÇÃO DE MISSÕES ===\n");
                vencedor = verificarVencedor(jogadores, numJogadores, mapa, quantidadeTeritorios);
                if (vencedor < 0) {
                    printf("Nenhum jogador completou sua missão ainda.\n");
                    printf("Continue jogando para alcançar seus objetivos!\n");
                } else {
                    opcao = 5; // Encerrar jogo
                }
                break;
                
            case 5:
                printf("\nEncerrando o jogo...\n");
                break;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        
        if (opcao != 5 && vencedor < 0) {
            printf("\nPressione Enter para continuar...");
            getchar(); // Limpar buffer
            getchar(); // Aguardar Enter
        }
        
    } while (opcao != 5 && vencedor < 0);
    
    // Exibir resultado final
    if (vencedor >= 0) {
        printf("\n=== RESULTADO FINAL ===\n");
        printf("🏆 PARABÉNS! O vencedor é: %s (%s)\n", 
               jogadores[vencedor].nome, jogadores[vencedor].cor);
        printf("Missão cumprida: \"%s\"\n", jogadores[vencedor].missao);
    }
    
    // Exibir mapa final
    printf("\n=== MAPA FINAL DOS TERRITÓRIOS ===\n");
    exibirTodosTeritorios(mapa, quantidadeTeritorios);
    
    // Liberar memória alocada
    liberarMemoria(mapa, jogadores, numJogadores);
    
    printf("\nObrigado por jogar WAR Estruturado - Nível Mestre!\n");
    return 0;
}
