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
//void consultar_vendas(vendas *ptr_venda);
//void consultar_vendas_por_pagamento(vendas *ptr_venda);
//void consultar_vendas_por_cpf(vendas *ptr_venda);


int main() {
    produtos *pto = NULL;
    vendas *ptr_venda = NULL;
    int op;
    aloca_produtos(&pto, 1);
    do {
        system("cls");
        printf("\n[1] Cadastro\n[2] Consulta produto\n[3] Deleta produto\n[4] Altera produto\n[5] vendas\n[6] Fim\n Opcao: ");
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
        }
    } while (op != 6);

    free(pto);  // Liberando a memoria alocada
    return 0;
}

void aloca(produtos **pto, int tam) {
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
        produtos p;
        while (fread(&p, sizeof(produtos), 1, fptr) == 1) {
            if (p.reg_prod > ultimo_reg) {
                ultimo_reg = p.reg_prod;
            }
        }
        fclose(fptr);
    }

    // Procurar por um produto com preco -1
    for (int i = 0; i < qreg; i++) {
        if ((fptr = fopen("produtos.bin", "r+b")) == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            exit(1);
        }
        fseek(fptr, i * sizeof(produtos), 0);
        fread(pto, sizeof(produtos), 1, fptr);
        if (pto->preco == -1) {  // Produto com preco -1 encontrado
            pto->reg_prod = i + 1;  // Sobrescrever o produto existente

            printf("\nRegistro: %i", pto->reg_prod);
            printf("\nProduto: ");
            fflush(stdin);
            fgets(pto->produto, 20, stdin);
            pto->produto[strcspn(pto->produto, "\n")] = '\0';  // removendo o caractere '\n' do final
            printf("\nDescricao: ");
            fgets(pto->descricao, 80, stdin);
            pto->descricao[strcspn(pto->descricao, "\n")] = '\0';  // removendo o caractere '\n' do final
            printf("\nPreco: ");
            scanf("%f", &(pto->preco));
            fflush(stdin);

            // Escrever as alteracoes no arquivo
            fseek(fptr, i * sizeof(produtos), 0);
            fwrite(pto, sizeof(produtos), 1, fptr);
            fclose(fptr);
            return;
        }
        fclose(fptr);
        
    }

    // Se nao houver produto com preco -1, adicionar um novo produto
    pto->reg_prod = ultimo_reg + 1;

    printf("\nRegistro: %i", pto->reg_prod);
    printf("\nProduto: ");
    fflush(stdin);
    fgets(pto->produto, 20, stdin);
    pto->produto[strcspn(pto->produto, "\n")] = '\0';  // removendo o caractere '\n' do final
    printf("\nDescricao: ");
    fgets(pto->descricao, 80, stdin);
    pto->descricao[strcspn(pto->descricao, "\n")] = '\0';  // removendo o caractere '\n' do final
    printf("\nPreco: ");
    scanf("%f", &(pto->preco));
    fflush(stdin);

    if ((fptr = fopen("produtos.bin", "ab")) == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }
    fwrite(pto, sizeof(produtos), 1, fptr);
    fclose(fptr);
}


void consulta_produto(produtos *pto) {
    int op;
    printf("\n[1] Consulta Total\n[2] Consulta Parcial\nOpcao: ");
    scanf("%d", &op);
    switch (op) {
        case 1:
            consulta_produto_total(pto);
            break;
        case 2:
            consulta_produto_parcial(pto);
            break;
        default:
            printf("\nOpcao invalida\n");
            break;
    }
    system("pause");
}

void consulta_produto_total(produtos *pto) {
    int i, qreg;
    FILE *fptr = NULL;
    qreg = verifica();   //qtde de registros
    system("cls");
    if ((fptr = fopen("produtos.bin", "rb")) == NULL)
        printf("\nErro ao abrir o arquivo");
    else {
        for (i = 0; i < qreg; i++) {
            fseek(fptr, i * sizeof(produtos), 0);
            fread(pto, sizeof(produtos), 1, fptr);
            if (pto->preco >= 0) {  // Verificar se o produto nao foi excluido
                printf("\nRegistro: %i\nProduto: %s\nDescricao: %s\nPreco: %.2f\n\n", pto->reg_prod, pto->produto, pto->descricao, pto->preco);
            }
        }
        fclose(fptr);
    }
}

void consulta_produto_parcial(produtos *pto) {
    int i, qreg;
    char tipo[20];
    FILE *fptr = NULL;
    qreg = verifica();   //qtde de registros
    system("cls");
    printf("\nDigite o tipo a ser consultado: ");
    fflush(stdin);
    fgets(tipo, 20, stdin);
    tipo[strcspn(tipo, "\n")] = '\0';  // removendo o caractere '\n' do final
    if ((fptr = fopen("produtos.bin", "rb")) == NULL)
        printf("\nErro ao abrir o arquivo");
    else {
        for (i = 0; i < qreg; i++) {
            fseek(fptr, i * sizeof(produtos), 0);
            fread(pto, sizeof(produtos), 1, fptr);
            if (pto->preco >= 0 && strcmp(pto->produto, tipo) == 0) {  // Verificar se o produto nao foi excluado e se o tipo coincide
                printf("\nRegistro: %i\nDescricao: %s\nPreco: %.2f\n\n", pto->reg_prod, pto->descricao, pto->preco);
            }
        }
        fclose(fptr);
    }
}

void deleta_produto(produtos *pto) {
    int reg_excluir, qreg, i;
    FILE *fptr;

    qreg = verifica();  // numero de registros
    printf("Digite o numero do registro do produto a ser excluído: ");
    scanf("%d", &reg_excluir);

    if ((fptr = fopen("produtos.bin", "r+b")) == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    for (i = 0; i < qreg; i++) {
        fseek(fptr, i * sizeof(produtos), 0);
        fread(pto, sizeof(produtos), 1, fptr);
        if (pto->reg_prod == reg_excluir && pto->preco >= 0) {  // Verificar se o produto nao foi excluido
            // Movendo o ponteiro para a posicao do registro a ser alterado
            fseek(fptr, i * sizeof(produtos), 0);

            // Alterando o preco para -1 para marcar o produto como excluido
            pto->preco = -1;

            // Escrevendo as alteracoees no arquivo
            fwrite(pto, sizeof(produtos), 1, fptr);
            break;
        }
    }
    fclose(fptr);
}


void altera_produto(produtos *pto) {
    int reg_alterar, qreg, i;
    FILE *fptr;

    qreg = verifica();  // numero de registros
    printf("Digite o numero do registro do produto a ser alterado: ");
    scanf("%d", &reg_alterar);

    if ((fptr = fopen("produtos.bin", "r+b")) == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    for (i = 0; i < qreg; i++) {
        fseek(fptr, i * sizeof(produtos), 0);
        fread(pto, sizeof(produtos), 1, fptr);
        if (pto->reg_prod == reg_alterar && pto->preco >= 0) {  // Verificar se o produto nao foi excluido
            // Movendo o ponteiro para a posicao do registro a ser alterado
            fseek(fptr, i * sizeof(produtos), 0);

            printf("\nRegistro: %i", pto->reg_prod);
            printf("\nPreco: ");
            scanf("%f", &(pto->preco));
            fflush(stdin);

            // Escrevendo as alteracoes no arquivo
            fwrite(pto, sizeof(produtos), 1, fptr);
            break;
        }
    }
    fclose(fptr);
}


void aloca_vendas(vendas **ptr_venda, int tam) {
    if ((*ptr_venda = (vendas*) realloc(*ptr_venda, tam * sizeof(vendas))) == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
}


void cadastro_vendas(vendas *ptr_venda) {
    int qtd_registros_estoque = verifica();
    int qtd_registros_vendas = 0;

    FILE *fptr_estoque, *fptr_vendas;

    // Abre o arquivo de estoque
    if ((fptr_estoque = fopen("produtos.bin", "rb")) == NULL) {
        printf("Erro ao abrir arquivo de estoque\n");
        exit(1);
    }

    // Verifica a quantidade de registros no arquivo de vendas
    if ((fptr_vendas = fopen("vendas.bin", "rb")) != NULL) {
        fseek(fptr_vendas, 0, SEEK_END);
        qtd_registros_vendas = ftell(fptr_vendas) / sizeof(vendas);
        fclose(fptr_vendas);
    }

    // Define o número de registro da nova venda
   // ptr_venda = aloca_venda(ptr_venda);
    ptr_venda->reg_venda = qtd_registros_vendas + 1;

    printf("\nRegistro da venda: %d", ptr_venda->reg_venda);

    printf("\nNome do cliente: ");
    fflush(stdin);
    fgets(ptr_venda->nome, sizeof(ptr_venda->nome), stdin);
    ptr_venda->nome[strcspn(ptr_venda->nome, "\n")] = '\0';

    printf("CPF do cliente: ");
    fgets(ptr_venda->cpf, sizeof(ptr_venda->cpf), stdin);
    ptr_venda->cpf[strcspn(ptr_venda->cpf, "\n")] = '\0';

    printf("Registro do produto: ");
    scanf("%d", &ptr_venda->reg_prod);
    fflush(stdin);

    if (ptr_venda->reg_prod <= 0 || ptr_venda->reg_prod > qtd_registros_estoque) {
        printf("Registro do produto inválido!\n");
        return;
    }

    printf("Quantidade: ");
    scanf("%d", &ptr_venda->quantidade);
    fflush(stdin);

    // Lê o produto correspondente ao registro informado
	produtos produto;
	fseek(fptr_estoque, (ptr_venda->reg_prod - 1) * sizeof(produtos), SEEK_SET);
	fread(&produto, sizeof(produtos), 1, fptr_estoque);

    // Preenche as informações da venda com base no produto
	ptr_venda->preco = produto.preco;
	ptr_venda->total = ptr_venda->quantidade * ptr_venda->preco;
	ptr_venda->pago = 'N';

    // Abre o arquivo de vendas para adicionar a nova venda
    if ((fptr_vendas = fopen("vendas.bin", "ab")) == NULL) {
        printf("Erro ao abrir arquivo de vendas\n");
        return;
    }

    // Escreve a nova venda no arquivo de vendas
    fwrite(ptr_venda, sizeof(vendas), 1, fptr_vendas);

    fclose(fptr_vendas);
}

