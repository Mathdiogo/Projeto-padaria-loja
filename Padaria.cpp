#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct produtos {
    int reg_prod;
    char produto[20];
    char descricao[80];
    float preco;
} produtos;

void aloca(produtos **pto, int tam);
int verifica();
void cadastro(produtos *pto);
void consulta_produto(produtos *pto);

int main() {
    produtos *pto = NULL;
    int op;
    aloca(&pto, 1);
    do {
        system("cls");
        printf("\n[1] Cadastro\n[2] Altera\n[3] Deleta\n[4] Mostra\n[5] Fim\nOpcao: ");
        scanf("%i", &op);
        fflush(stdin);
        switch (op) {
            case 1:
                cadastro(pto);
                printf("\nRegistro cadastrado com sucesso\n\n");
                system("pause");
                break;
            case 2:
            case 3:
                
                break;
            case 4:
        
                break;
        }
    } while (op != 5);

    free(pto);  // Liberando a memória alocada
    return 0;
}

void aloca(produtos **pto, int tam) {
    if ((*pto = (produtos*) realloc(*pto, tam * sizeof(produtos))) == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
}

int verifica() {
    FILE *fptr = fopen("estoque.bin", "r");
    if (fptr != NULL) {
        // O arquivo existe, retornar o número de registros
        fseek(fptr, 0, 2);  // Posiciona no fim do arquivo
        long int tamanho = ftell(fptr);
        fclose(fptr);
        return tamanho / sizeof(produtos);
    } else {
        // O arquivo não existe, criar o arquivo e retornar 0
        fptr = fopen("estoque.bin", "wb");
        if (fptr == NULL) {
            printf("Erro ao criar o arquivo.\n");
            exit(1);
        }
        fclose(fptr);
        return 0;
    }
}

void cadastro(produtos *pto) {
    int qreg, ultimo_reg;
    qreg = verifica();  // número de registros
    ultimo_reg = 0;
    
    FILE *fptr;
    if ((fptr = fopen("estoque.bin", "rb")) != NULL) {
        produtos p;
        while (fread(&p, sizeof(produtos), 1, fptr) == 1) {
            if (p.reg_prod > ultimo_reg) {
                ultimo_reg = p.reg_prod;
            }
        }
        fclose(fptr);
    }
    
    if (qreg == -1) {  // não há vago
        pto->reg_prod = qreg + 1;
    } else {  // achou vago
        pto->reg_prod = ultimo_reg + 1;
    }
    
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

    if ((fptr = fopen("estoque.bin", "ab")) == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }
    fwrite(pto, sizeof(produtos), 1, fptr);
    fclose(fptr);
}

void consulta_produto(produtos *pto) {
    int i, qreg;
    FILE *fptr = NULL;
    qreg = verifica();   //qtde de registros
    system("cls");
    if ((fptr = fopen("estoque.bin", "rb")) == NULL)
        printf("\nErro ao abrir o arquivo");
    else {
        for (i = 0; i < qreg; i++) {
            fseek(fptr, i * sizeof(produtos), 0);
            fread(pto, sizeof(produtos), 1, fptr);
            printf("\nRegistro: %i\nProduto: %s\nDescricao: %s\nPreco: %.2f\n\n", pto->reg_prod, pto->produto, pto->descricao, pto->preco);
        }
        fclose(fptr);
    }
    system("pause");
}

