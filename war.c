/*
 * Programa de Cadastro de Territórios
 * Desafio: Estrutura de Dados Composta
 * 
 * Este programa implementa um sistema simples para cadastrar territórios
 * utilizando structs para organizar dados relacionados (nome, cor do exército, tropas).
 */

#include <stdio.h>
#include <string.h>

// Definição da struct Territorio para armazenar informações dos territórios
typedef struct {
    char nome[30];      // Nome do território (máximo 29 caracteres + \0)
    char cor[10];       // Cor do exército (máximo 9 caracteres + \0)
    int tropas;         // Quantidade de tropas no território
} Territorio;

// Função para cadastrar um território
void cadastrarTerritorio(Territorio *territorio, int indice) {
    printf("\n=== CADASTRO DO TERRITÓRIO %d ===\n", indice + 1);
    
    // Entrada do nome do território
    printf("Digite o nome do território: ");
    scanf(" %29s", territorio->nome);  // Espaço antes do %s para ignorar whitespace
    
    // Entrada da cor do exército
    printf("Digite a cor do exército: ");
    scanf(" %9s", territorio->cor);
    
    // Entrada da quantidade de tropas
    printf("Digite o número de tropas: ");
    scanf("%d", &territorio->tropas);
    
    printf("Território cadastrado com sucesso!\n");
}

// Função para exibir os dados de um território
void exibirTerritorio(Territorio territorio, int indice) {
    printf("Território %d:\n", indice + 1);
    printf("  Nome: %s\n", territorio.nome);
    printf("  Cor do Exército: %s\n", territorio.cor);
    printf("  Tropas: %d\n", territorio.tropas);
    printf("  ---------------------------\n");
}

// Função para exibir todos os territórios cadastrados
void exibirTodosTeritorios(Territorio territorios[], int quantidade) {
    printf("\n=== RELATÓRIO DE TERRITÓRIOS CADASTRADOS ===\n");
    printf("Total de territórios: %d\n\n", quantidade);
    
    for (int i = 0; i < quantidade; i++) {
        exibirTerritorio(territorios[i], i);
    }
}

int main() {
    // Declaração do vetor de structs para armazenar 5 territórios
    Territorio territorios[5];
    const int TOTAL_TERRITORIOS = 5;
    
    printf("=== SISTEMA DE CADASTRO DE TERRITÓRIOS ===\n");
    printf("Você irá cadastrar %d territórios.\n", TOTAL_TERRITORIOS);
    printf("Para cada território, informe: nome, cor do exército e número de tropas.\n");
    
    // Laço para cadastrar os 5 territórios
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        cadastrarTerritorio(&territorios[i], i);
    }
    
    // Exibição dos dados de todos os territórios cadastrados
    exibirTodosTeritorios(territorios, TOTAL_TERRITORIOS);
    
    printf("\nCadastro concluído com sucesso!\n");
    printf("Pressione Enter para finalizar...");
    getchar(); // Para capturar o Enter pendente
    getchar(); // Para aguardar o usuário pressionar Enter
    
    return 0;
}
