# Algoritmo de simulação do trajeto de partículas

O algoritmo faz parte da modelagem conduzida no projeto de pesquisa.

Para **compilar** o código:
```
g++ main.cpp implementacao/*.cpp -O2 -o exe
```
Explicação:
* *O2* é uma flag para o compilador fazer eventuais otimizações no código.
* *exe* é o nome do arquivo binário gerado no processo de compilação.

Para **executar** o arquivo binário:
```
./exe
```
Portanto, para **compilar e executar**:
```
g++ main.cpp implementacao/*.cpp -O2 -o exe && ./exe
```

# Entendendo os arquivos
* A pasta *headers* contém os arquivos com todas as definições deste código.
* A pasta *implementacao* contém os arquivos com todas as implementações deste código (de tudo o que foi definido em *headers*).
    * Valores de constantes.
    * Comportamentos de funções.
    * Etc.
* O arquivo *.gitignore* contém os arquivos que o git deve ignorar.
* O arquivo *main.cpp* é o arquivo que faz a chamada principal da ```rotina``` do código.
* O arquivo *README.m* é este que você está lendo.

### Headers
Nesta pasta estão todos os arquivos de cabeçalho (todos os *.h*).
* *constantes.h*: Define todas as constantes utilizadas no algoritmo de simulação.
* *debug.h*: Define se o código está em modo *debug*.
* *funcsimu.h*: Definição das funções utilizadas no algoritmo de simulação.
* *nucleo.h*: Definição das funções responsáveis por executar a rotina e o algoritmo de simulação.
* *uteis.h*: Definição de funções úteis para a experiência do usuário e detalhes do código como criação de pastas.

Note que nesta pasta estão **apenas as definições** para que o compilador saiba que tais elementos existem.

### Implementacao
Nesta pasta estão os arquivos com as implementações das funções e valores das variáveis que foram definidos nos respectivos arquivos de cabeçalho na pasta *headers*. 

# Como modificar a simulação?
#### Modo debug
Para colocar o código em modo debug basta descomentar a linha indicada no arquivo de cabeçalho *debug.h*.

#### Constantes
Para modificar alguma constante, por exemplo, resistência vascular de alguma rota, ou coeficiente de difusão, altere o valor desejado no arquivo *constantes.cpp* na pasta *implementacao*.

#### Forma de cálculo de alguma função
Você encontrará no arquivo *funcsimu.cpp* na pasta *implementacao*.

#### Geral
Se quiser alterar algum comportamento, deve fazer em algum arquivo *.cpp*.
Se quiser definir o modo do código, será em algum arquivo *.h*

#### Adicionar uma função
1. Defina-a no arquivo apropriado em *headers*.
2. Implemente seu comportamento no arquivo respectivo em *implementacao*.
3. Chame a função no ponto do código que desejar, *main.cpp*, *nucleo.cpp*, etc.

#### Alterar o valor da *seed* do modo debug
Modifique o número inteiro em ```#define RANDOM_SEED``` no arquivo *debug.h*. Substitua-o por outro número inteiro.

#### Alterar o nome da pasta que contém os resultados
Modifique a variável ```nomePastaResultados``` no arquivo *constantes.cpp* em *implementacao*.

#### Alterar o nome dos subdiretórios dentro da pasta que contém os resultados
Modifique a função ```sprintf()``` dentro da função ```criaPastaEstagioSemanaAtual``` em *uteis.cpp*.

#### Alterar o nome dos arquivos de resultado gerados
Modifique as funções ```sprintf()``` em *main.cpp*. Basta observar o padrão com o nome de cada arquivo de saída.

#### Não filtrar os resultados após execução da simulação
Comente a linha com a função ```filtrarArquivosPorSufixo``` em *main.cpp*.

**Lembre-se de salvar cada alteração realizada e compilar e executar o código novamente.**
# O que é o modo debug?
O algoritmo possui pseudo-aleatoriedade, porém, as vezes é necessário comparar alguns resultados ao modificar um parâmetro ou outro.
Para isso, o modo *debug* fixa a *seed* para a geração de números aleatórios e faz com que o resultado seja sempre o mesmo para uma dada *seed*.

Para rodar a simulação em modo *debug*, descomente a linha de código que contém ```#define DEBUG_MODE``` em *debug.h*.
O valor atribuido para o *seed* será o definido na variável ```RANDOM_SEED```.