# Algoritmo de simulação de trajeto de uma partícula

O algoritmo faz parte da modelagem conduzida no projeto de pesquisa.

Para compilar o código:
```
g++ zika.cpp modelagemzika.cpp -o exe -O2
```
* *O2* é uma flag para o compilador fazer eventuais otimizações no código.
* *exe* é o nome do arquivo binário gerado no processo de compilação.

Para executar o arquivo binário:
```
./exe
```
Em um comando apenas:
```
g++ zika.cpp modelagemZika.cpp -o exe -O2 && ./exe
```

# Arquivos necessários
* *modelagemZika.h* é onde estão declarações de funções e importações de bibliotecas utilizadas no código principal.
* *modelagemZika.cpp* é onde está descrito o comportamento da função.
* *zika.cpp*" é o arquivo de código principal.

# Personalizar
As principais constantes estão na parte inicial do arquivo *zika.cpp*, antes de qualquer função, e podem ser alteradas e afetam diretamente os resultados obtidos na simulação.
Pode-se alterar o nome da pasta de resultados, atualmente como "resultados", trocando o valor da variável *nomePastaResultados* na primeira linha da função  ```int main()``` no arquivo *zika.cpp*. Não foi implementada como constante pois facilita a implementação.
O nome dos subdiretórios com os resultados também pode ser alterado com alguma manipulação da função ```sprintf()```.

# Modo Debug
O algoritmo se baseia em alterações aleatórias, porém, as vezes é necessário comparar alguns resultados ao modificar um parâmetro ou outro.
Para isso, o modo debug fixa a *seed* para a geração de números aleatórios e faz com que os números gerados sejam sempre os mesmos para uma dada *seed*.
Para rodar a simulação em modo debug, descomente a linha de código que contém ```#define DEBUG_MODE``` logo no início do código principal, após os includes.
O valor atribuido para o seed será o definido na variavel ```RANDOM_SEED_CONSTANT```.