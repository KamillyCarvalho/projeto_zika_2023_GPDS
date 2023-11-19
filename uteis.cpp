#include "uteis.h"

void criaPasta(const char* folderName, bool printMsg){
    struct stat st;
    if (stat(folderName, &st) == 0)
    {
        // Se o diretório já existe, remove-o
        std::string command = "rm -rf " + std::string(folderName);
        int result = system(command.c_str());
        if(!result and printMsg)
            printf("Erro ao tentar criar pasta \"%s\"\n", folderName);
    }
    int status = mkdir(folderName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(!printMsg) return;
    if (status == 0) 
        printf("Pasta \"%s\" criada\n", folderName);
    else 
        printf("Erro ao criar pasta \"%s\"\n", folderName);
}


void printBarraProgresso(double progresso){
        int tamanhoBarra = 100;
        std::cout << '[';
        int pos = tamanhoBarra * progresso;
        for (int i = 0; i < tamanhoBarra; ++i) {
            if (i < pos) std::cout << '=';
            else if (i == pos) std::cout << '>';
            else std::cout << ' ';
        }
        std::cout << "] " << int(progresso * 100.0) << " %\r";
        std::cout.flush();
}


// Vetores para as palavras correspondentes aos dígitos e às potências de dez
const std::string unidades[] = {"", "um", "dois", "tres", "quatro", "cinco", "seis", "sete", "oito", "nove"};
const std::string especiais[] = {"", "onze", "doze", "treze", "catorze", "quinze", "dezesseis", "dezessete", "dezoito", "dezenove"};
const std::string dezenas[] = {"", "dez", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa"};
const std::string centenas[] = {"", "cento", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos"};
const std::string milhares_singular[] = {"", "mil", "milhao", "bilhao"};
const std::string milhares_plural[] = {"", "mil", "milhoes", "bilhoes"};

/*
    A funcao checa por centenas, constroi cada uma por extenso e segue para a proxima
*/

std::string numeroPorExtenso(int numero){
    if(numero == 0)
        return "zero";
    
    int posicao = 0;
    bool grupoMaiorQueZero = false;
    std::string numeroExtenso;

    while(numero > 0){
        int grupo = numero % 1000;
        int centena = grupo / 100;
        int dezena = (grupo % 100) / 10;
        int unidade = grupo % 10;

        if(grupo > 0){
            std::string extensoGrupo;

            if(centena > 0){
                extensoGrupo += centenas[centena];
                if((dezena > 0 or unidade > 0))
                    extensoGrupo += " e ";
            }

            if(dezena == 1 && unidade > 0){
                extensoGrupo += especiais[unidade];
            }else{
                if(dezena > 0){
                    extensoGrupo += dezenas[dezena];
                    if(unidade > 0)
                        extensoGrupo += " e ";
                }
                if(unidade > 0 && dezena != 1)
                    extensoGrupo += unidades[unidade];
            }

            if(posicao > 0){
                extensoGrupo += " ";
                extensoGrupo += grupo > 1 and posicao > 1 ? milhares_plural[posicao] : milhares_singular[posicao];
                if(grupoMaiorQueZero)
                    extensoGrupo += " e";
            }

            grupoMaiorQueZero = true;

            if(not numeroExtenso.empty()){
                numeroExtenso = extensoGrupo + " " + numeroExtenso;
            }else{
                numeroExtenso = extensoGrupo;
            }
        }

        numero /= 1000;
        posicao++;
    }
    return numeroExtenso;
}