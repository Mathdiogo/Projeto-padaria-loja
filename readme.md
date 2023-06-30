README.

Este é um programa em C para gerenciar um estoque de produtos e registrar vendas. O programa permite cadastrar produtos, consultar informações sobre os produtos, realizar vendas e consultar as vendas realizadas.

Funcionalidades:

O programa oferece as seguintes funcionalidades:

Cadastro: permite cadastrar novos produtos no estoque.

Consulta produto: oferece opções para consultar informações sobre os produtos no estoque.

Consulta Total: exibe todas as informações dos produtos cadastrados no estoque.

Consulta Parcial: permite consultar produtos específicos com base em um tipo de produto.

Deleta produto: permite excluir um produto do estoque.

Altera produto: permite alterar o preço de um produto existente no estoque.

Nova venda: permite registrar uma nova venda, associando o cliente, o produto vendido e a quantidade.

Consulta Vendas: exibe todas as vendas realizadas, incluindo informações sobre o cliente, produto, quantidade e valor total.

Pagamento: permite marcar uma venda como paga.

Fim: encerra o programa.

Compilação e Execução

Para compilar o programa, utilize um compilador de C compatível (por exemplo, GCC) com o seguinte comando:

gcc nome_do_programa.c -o nome_do_programa
Em seguida, execute o programa digitando:
./nome_do_programa

Observações
O programa utiliza arquivos binários para armazenar os dados de produtos e vendas. Os arquivos utilizados são "produtos.bin" e "vendas.bin". Certifique-se de que esses arquivos existam no mesmo diretório do programa antes de executá-lo.

Durante a execução do programa, siga as instruções exibidas no console para interagir com as diferentes funcionalidades disponíveis.

Certifique-se de que o formato de entrada de dados seja seguido corretamente para evitar erros. Por exemplo, ao cadastrar um produto, forneça os valores solicitados nas ordens corretas e evite exceder o tamanho máximo dos campos definidos.
