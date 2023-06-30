#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct produtos {
    int reg_prod;
    char produto[20];
    char descricao[80];
    float preco;
} produtos;

typedef struct vendas {
    int reg_venda;
    char nome[80];
    char cpf[15];
    int reg_prod;
    int quantidade;
    float preco;
    float total;
    char pago;
} vendas;

void aloca_produtos(produtos **pto, int tam);
void aloca_vendas(vendas  **ptr_venda, int tam);
int verifica();
void cadastro(produtos *pto, vendas *ptr_venda);
void cadastro_produto(produtos *pto);
void consulta_produto(produtos *pto);
void consulta_produto_total(produtos *pto);
void consulta_produto_parcial(produtos *pto);
void deleta_produto(produtos *pto);
void altera_produto(produtos *pto);
void cadastro_vendas(vendas *ptr_venda);
void consultar_vendas(vendas *ptr_venda);
void consultar_vendas_por_pagamento(vendas *ptr_venda);
void consultar_vendas_por_cpf(vendas *ptr_venda);

int main() {
    produtos *pto = NULL;
    vendas *ptr_venda = NULL;
    int op;
    aloca_produtos(&pto, 1);
    do {
        system("cls");
        printf("\n[1] Cadastro\n[2] Consulta produto\n[3] Deleta produto\n[4] Altera produto\n[5] cadastro vendas [6] consultar vendas [7] consultar por pagamento [8] consultar por cpf [9]Fim\n Opcao: ");
        scanf("%i", &op);
        fflush(stdin);
        switch (op) {
            case 1:
                cadastro_produto(pto);
                printf("\nRegistro cadastrado com sucesso\n\n");
                system("pause");
                break;
            case 2:
                consulta_produto(pto);
                break;
            case 3:
                deleta_produto(pto);
                break;
            case 4:
                altera_produto(pto);
                break;
            case 5:
            	cadastro_vendas(ptr_venda);
            	break;
            case 6:
            	consultar_vendas(ptr_venda);
            	break;
            case 7:
            	consultar_vendas_por_pagamento(ptr_venda);
            	break;
            case 8:
            	consultar_vendas_por_cpf(ptr_venda);
            	break;
            		
            	
        }
    } while (op != 9);

    free(pto);  // Liberando a memoria alocada
    return 0;
}

void aloca_produtos(produtos **pto, int tam) {
    if ((*pto = (produtos*) realloc(*pto, tam * sizeof(produtos))) == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
}

int verifica() {
    FILE *fptr = fopen("produtos.bin", "rb");
    if (fptr != NULL) {
        // O arquivo existe, retornar o numero de registros
        fseek(fptr, 0, SEEK_END);  // Posiciona no fim do arquivo
        long int tamanho = ftell(fptr);
        fclose(fptr);
        return tamanho / sizeof(produtos);
    } else {
        // O arquivo nao existe, criar o arquivo e retornar 0
        fptr = fopen("produtos.bin", "ab");
        if (fptr == NULL) {
            printf("Erro ao criar o arquivo.\n");
            exit(1);
        }
        fclose(fptr);
        return 0;
    }
}

void cadastro_produto(produtos *pto) {
    int qreg, ultimo_reg;
    qreg = verifica();  // numero de registros
    ultimo_reg = 0;

    FILE *fptr;
    if ((fptr = fopen("produtos.bin", "rb")) != NULL) {
        produtos prod;
        fread(&prod, sizeof(produtos), 1, fptr);
        while (!feof(fptr)) {
            ultimo_reg = prod.reg_prod;
            fread(&prod, sizeof(produtos), 1, fptr);
        }
        fclose(fptr);
    }

    aloca_produtos(&pto, qreg + 1);

    pto[qreg].reg_prod = ultimo_reg + 1;
    printf("Informe o nome do produto: ");
    gets(pto[qreg].produto);
    fflush(stdin);
    printf("Informe a descricao do produto: ");
    gets(pto[qreg].descricao);
    fflush(stdin);
    printf("Informe o preco do produto: ");
    scanf("%f", &pto[qreg].preco);
    fflush(stdin);

    if ((fptr = fopen("produtos.bin", "ab")) == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    fwrite(&pto[qreg], sizeof(produtos), 1, fptr);
    fclose(fptr);
}

void consulta_produto(produtos *pto) {
    int op;
    do {
        system("cls");
        printf("[1] Consulta produto total\n");
        printf("[2] Consulta produto parcial\n");
        printf("[3] Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        fflush(stdin);
        switch (op) {
            case 1:
                consulta_produto_total(pto);
                break;
            case 2:
                consulta_produto_parcial(pto);
                break;
        }
    } while (op != 3);
}

void consulta_produto_total(produtos *pto) {
    FILE *fptr;
    if ((fptr = fopen("produtos.bin", "rb")) == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    produtos prod;
    fread(&prod, sizeof(produtos), 1, fptr);
    while (!feof(fptr)) {
        printf("\nRegistro: %d\n", prod.reg_prod);
        printf("Produto: %s\n", prod.produto);
        printf("Descricao: %s\n", prod.descricao);
        printf("Preco: %.2f\n", prod.preco);
        fread(&prod, sizeof(produtos), 1, fptr);
    }
    fclose(fptr);
    system("pause");
}

void consulta_produto_parcial(produtos *pto) {
    char busca[20];
    printf("Informe o nome do produto para busca: ");
    gets(busca);
    fflush(stdin);

    FILE *fptr;
    if ((fptr = fopen("produtos.bin", "rb")) == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    produtos prod;
    fread(&prod, sizeof(produtos), 1, fptr);
    while (!feof(fptr)) {
        if (strstr(prod.produto, busca) != NULL) {
            printf("\nRegistro: %d\n", prod.reg_prod);
            printf("Produto: %s\n", prod.produto);
            printf("Descricao: %s\n", prod.descricao);
            printf("Preco: %.2f\n", prod.preco);
        }
        fread(&prod, sizeof(produtos), 1, fptr);
    }
    fclose(fptr);
    system("pause");
}

void deleta_produto(produtos *pto) {
    int qreg, reg;
    qreg = verifica();  // numero de registros
    printf("Informe o registro do produto para exclusao: ");
    scanf("%d", &reg);
    fflush(stdin);

    FILE *fptr;
    if ((fptr = fopen("produtos.bin", "rb")) == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    produtos *prod = (produtos*) malloc(qreg * sizeof(produtos));
    fread(prod, sizeof(produtos), qreg, fptr);
    fclose(fptr);

    if ((fptr = fopen("produtos.bin", "wb")) == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    for (int i = 0; i < qreg; i++) {
        if (prod[i].reg_prod != reg) {
            fwrite(&prod[i], sizeof(produtos), 1, fptr);
        }
    }
    fclose(fptr);
    free(prod);
    printf("Produto excluido com sucesso.\n");
    system("pause");
}

void altera_produto(produtos *pto) {
    int qreg, reg;
    qreg = verifica();  // numero de registros
    printf("Informe o registro do produto para alteracao: ");
    scanf("%d", &reg);
    fflush(stdin);

    FILE *fptr;
    if ((fptr = fopen("produtos.bin", "rb")) == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    produtos *prod = (produtos*) malloc(qreg * sizeof(produtos));
    fread(prod, sizeof(produtos), qreg, fptr);
    fclose(fptr);

    if ((fptr = fopen("produtos.bin", "wb")) == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    for (int i = 0; i < qreg; i++) {
        if (prod[i].reg_prod == reg) {
            printf("Informe o nome do produto: ");
            gets(prod[i].produto);
            fflush(stdin);
            printf("Informe a descricao do produto: ");
            gets(prod[i].descricao);
            fflush(stdin);
            printf("Informe o preco do produto: ");
            scanf("%f", &prod[i].preco);
            fflush(stdin);
        }
        fwrite(&prod[i], sizeof(produtos), 1, fptr);
    }
    fclose(fptr);
    free(prod);
    printf("Produto alterado com sucesso.\n");
    system("pause");
}

