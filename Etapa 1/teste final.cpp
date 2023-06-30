#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct produtos {
    int reg_prod;				//Numero de registros do produto
    char produto[20];			//Nome do produto
    char descricao[80];			//descricao do produto
    float preco;				//preco do produto
} produtos;

typedef struct vendas {
    int reg_venda;				//Numero de registro da venda
    char nome[80];				//Nome do cliente
    char cpf[15];				//CPF do cliente
    int reg_prod;				//Nuemro de registro do produto vendido
    int quantidade;				//Quantidade do produto vendido
    float preco;				//Preco do produto
    float total;				//Valor total da venda (quantidade*preco)
    char pago;					//Indica se o pagamento foi realizado
} vendas;

typedef struct cliente{
int reg_cli;
char nome[80];
char CPF[15];
float gasto;
}cliente; 

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
void consulta_vendas(vendas *ptr_venda);
void mostra_produto();
void consulta_vendas_pagamento(vendas *ptr_venda);
void consulta_vendas_cpf(vendas *ptr_venda);
void pagamento(cliente *pcli, vendas *ptr_venda);
void mostra_historico_clientes(vendas *ptr_venda);


int main() {
    produtos *pto = NULL;
    vendas *ptr_venda = NULL;
    cliente *pcli = NULL;
    int op;
    aloca_produtos(&pto, 1);				// Aloca memoria para a estrutura de produtos
    aloca_vendas(&ptr_venda, 1);			// Aloca memória para a estrutura de vendas
    do {
        system("cls");
        printf("\n[1] Cadastro\n[2] Consulta produto\n[3] Deleta produto\n[4] Altera produto\n[5] Nova venda\n[6] Consulta Vendas\n[7] Pagamento\n[8] Historico cliente\n[9] Fim\n Opcao: ");
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
            	consulta_vendas(ptr_venda);
            	break;
            case 7:
            	pagamento(pcli, ptr_venda);
            	break;
            case 8:
            	mostra_historico_clientes(ptr_venda);
            	break;
        }
    } while (op != 8);

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
    fgets(ptr_venda->nome, sizeof(ptr_venda->nome), stdin);
    fflush(stdin);
    ptr_venda->nome[strcspn(ptr_venda->nome, "\n")] = '\0';

    printf("CPF do cliente: ");
    fgets(ptr_venda->cpf, sizeof(ptr_venda->cpf), stdin);
    ptr_venda->cpf[strcspn(ptr_venda->cpf, "\n")] = '\0';
    
    system("cls");
    mostra_produto();

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

void mostra_produto() {
    FILE *fptr_estoque;

    // Abre o arquivo de estoque
    if ((fptr_estoque = fopen("produtos.bin", "rb")) == NULL) {
        printf("Erro ao abrir arquivo de estoque\n");
        return;
    }

    produtos produto;

    // Le todos os registros do arquivo de estoque e exibe as informações de cada produto
    while (fread(&produto, sizeof(produtos), 1, fptr_estoque) == 1) {
        produtos *pto = &produto;

        printf("\nRegistro do produto: %d", pto->reg_prod);
        printf("\nNome do produto: %s", pto->produto);
        printf("\nDescricao: %s", pto->descricao);
        printf("\nPreco: %.2f\n\n", pto->preco);
    }

    fclose(fptr_estoque);
}

void consulta_vendas(vendas *ptr_venda) {
    int op;
    printf("\n[1] Consulta por pagamento\n[2] Consulta por cpf\nOpcao: ");
    scanf("%d", &op);
    switch (op) {
        case 1:
            consulta_vendas_pagamento(ptr_venda);
            break;
        case 2:
            consulta_vendas_cpf(ptr_venda);
            break;
        default:
            printf("\nOpcao invalida\n");
            break;
    }
    system("pause");
}

void consulta_vendas_pagamento(vendas *ptr_venda) {
    FILE *fptr_vendas;
    if ((fptr_vendas = fopen("vendas.bin", "rb")) == NULL) {
        printf("Erro ao abrir arquivo de vendas\n");
        return;
    }

    char resposta;
    printf("Deseja consultar vendas pagas [S/N]? ");
    scanf(" %c", &resposta);
    fflush(stdin);

    char pago;
    if (resposta == 'S' || resposta == 's') {
        pago = 'S';
    } else if (resposta == 'N' || resposta == 'n') {
        pago = 'N';
    } else {
        printf("Opcao invalida.\n");
        fclose(fptr_vendas);
        return;
    }

    vendas venda;

    while (fread(&venda, sizeof(vendas), 1, fptr_vendas) == 1) {
        vendas *ptr_venda = &venda;

        if (ptr_venda->pago == pago) {
            printf("\nRegistro da venda: %d", ptr_venda->reg_venda);
            printf("\nNome do cliente: %s", ptr_venda->nome);
            printf("\nCPF do cliente: %s", ptr_venda->cpf);
            printf("\nRegistro do produto: %d", ptr_venda->reg_prod);
            printf("\nQuantidade: %d", ptr_venda->quantidade);
            printf("\nPreco: %.2f", ptr_venda->preco);
            printf("\nTotal: %.2f\n", ptr_venda->total);
        }
    }

    fclose(fptr_vendas);
}


void consulta_vendas_cpf(vendas *ptr_venda) {
    FILE *fptr_vendas;
    if ((fptr_vendas = fopen("vendas.bin", "rb")) == NULL) {
        printf("Erro ao abrir arquivo de vendas\n");
        return;
    }

    char cpf[15];
    printf("Digite o CPF do cliente: ");
    fflush(stdin);
    fgets(cpf, sizeof(cpf), stdin);
    fflush(stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    vendas venda;
    int cpf_cadastrado = 0;

    while (fread(&venda, sizeof(vendas), 1, fptr_vendas) == 1) {
        vendas *ptr_venda = &venda;

        if (strcmp(ptr_venda->cpf, cpf) == 0) {
            cpf_cadastrado = 1;

            printf("\nRegistro da venda: %d", ptr_venda->reg_venda);
            printf("\nNome do cliente: %s", ptr_venda->nome);
            printf("\nRegistro do produto: %d", ptr_venda->reg_prod);
            printf("\nQuantidade: %d", ptr_venda->quantidade);
            printf("\nPreco: %.2f", ptr_venda->preco);
            printf("\nTotal: %.2f", ptr_venda->total);
            printf("\nPagamento: %c\n", ptr_venda->pago);
        }
    }

    fclose(fptr_vendas);

    if (!cpf_cadastrado) {
        printf("\nCPF nao cadastrado.\n");
    }
}

void pagamento(cliente *pcli, vendas *ptr_venda) {
    FILE *fptr_vendas;
    FILE *fptr_clientes;
    
    if ((fptr_vendas = fopen("vendas.bin", "rb")) == NULL) {
        printf("Erro ao abrir arquivo de vendas\n");
        return;
    }
    
    if ((fptr_clientes = fopen("cliente.bin", "a")) == NULL) {
        printf("Erro ao abrir arquivo dos clientes\n");
        return;
    }
    
char cpf[15];
printf("Digite o CPF do cliente: ");
scanf("%s", cpf);
fflush(stdin);

vendas venda;
cliente cliente_atual;
int cliente_encontrado = 0;

while (fread(&venda, sizeof(vendas), 1, fptr_vendas) == 1) {
    vendas *ptr_venda = &venda;

    if (strcmp(ptr_venda->cpf, cpf) == 0 && ptr_venda->pago == 'N') {
        printf("\nRegistro da venda: %d", ptr_venda->reg_venda);
        printf("\nNome do cliente: %s", ptr_venda->nome);
        printf("\nRegistro do produto: %d", ptr_venda->reg_prod);
        printf("\nQuantidade: %d", ptr_venda->quantidade);
        printf("\nPreco: %.2f", ptr_venda->preco);
        printf("\nTotal: %.2f\n", ptr_venda->total);

        char resposta;
        printf("\nDeseja pagar essa venda? [S/N]: ");
        scanf(" %c", &resposta);
        fflush(stdin);

        if (resposta == 'S' || resposta == 's') {
            ptr_venda->pago = 'S';
            fseek(fptr_vendas, sizeof(vendas), SEEK_CUR);
            fwrite(ptr_venda, sizeof(vendas), 1, fptr_vendas);

            if (!cliente_encontrado) {
                cliente *pcli = &cliente_atual;

                while (fread(pcli, sizeof(cliente), 1, fptr_clientes) == 1) {
                    if (strcmp(pcli->CPF, ptr_venda->cpf) == 0) {
                        cliente_encontrado = 1;
                        pcli->gasto += ptr_venda->total;
                        fseek(fptr_clientes, sizeof(cliente), SEEK_CUR);
                        fwrite(pcli, sizeof(cliente), 1, fptr_clientes);
                        
                        
                    }
                }
            }

            if (!cliente_encontrado) {
                fseek(fptr_clientes, 0, SEEK_END);
                cliente novo_cliente;
                novo_cliente.reg_cli = 0;  // Preencha com o valor adequado
                strcpy(novo_cliente.nome, ptr_venda->nome);
                strcpy(novo_cliente.CPF, ptr_venda->cpf);
                novo_cliente.gasto = ptr_venda->total;
                fwrite(&novo_cliente, sizeof(cliente), 1, fptr_clientes);
            }

            printf("\nPagamento realizado com sucesso.\n");
            break;
        } else if (resposta == 'N' || resposta == 'n') {
            printf("\nPagamento cancelado.\n");
            break;
        } else {
            printf("\nOpcao invalida.\n");
            fclose(fptr_vendas);
            fclose(fptr_clientes);
            return;
        }
    }
}
}

void mostra_historico_clientes(vendas *ptr_venda) {
    int qtd_registros_vendas = 0;

    FILE *fptr_vendas;

    // Verifica a quantidade de registros no arquivo de vendas
    if ((fptr_vendas = fopen("vendas.bin", "rb")) != NULL) {
        fseek(fptr_vendas, 0, SEEK_END);
        qtd_registros_vendas = ftell(fptr_vendas) / sizeof(vendas);
        fclose(fptr_vendas);
    }

    printf("\n--- Historico de Clientes ---\n");

    for (int i = 0; i < qtd_registros_vendas; i++) {
        if ((fptr_vendas = fopen("vendas.bin", "rb")) == NULL) {
            printf("Erro ao abrir o arquivo de vendas.\n");
            exit(1);
        }

        fseek(fptr_vendas, i * sizeof(vendas), 0);
        fread(ptr_venda, sizeof(vendas), 1, fptr_vendas);
        fclose(fptr_vendas);

        printf("\nRegistro: %i\nNome: %s\nCPF: %s\nGasto: %.2f\nPagamento: %s\n",
               ptr_venda->reg_venda, ptr_venda->nome, ptr_venda->cpf, ptr_venda->total,
               (ptr_venda->pago == 'S' || ptr_venda->pago == 's') ? "Pago" : "Nao pago");
    }

    printf("\n--- Fim do Historico ---\n");
}

