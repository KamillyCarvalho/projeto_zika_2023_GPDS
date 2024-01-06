#include "../headers/uteis.h"

/* 
    Cria pasta no caminho especificado.
    Caso já exista, a pasta atual é removida e é criada uma nova.
*/
void criaPasta(const char* caminhoPastaASerCriada, bool infoMsg){
    namespace fs = std::filesystem;
    fs::path diretorio(caminhoPastaASerCriada);
    if(fs::exists(diretorio)){
        fs::remove_all(diretorio);
    }
    int ok = fs::create_directory(diretorio);
    if(infoMsg){
        if(ok){
            std::cout << "Pasta em \"" << caminhoPastaASerCriada <<  "\" criada com sucesso\n";
        }else{
            std::cout << "Falha ao criar pasta em \"" << caminhoPastaASerCriada << "\"\n";
        }
    }
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

unsigned int seedParaRand(){
    #ifdef DEBUG_MODE
        return RANDOM_SEED;
    #else
        return time(NULL);
    #endif
}

void filtrarArquivosPorSufixo(const char *nomePastaASerRealizadoFiltro, const char *sufixoASerFiltrado, const char *nomePastaDestino){
    char command[300];
    criaPasta(nomePastaDestino, false);
    #ifdef _WIN32
        sprintf(command, "PowerShell -Command \"Get-ChildItem -Path .\\%s -Recurse -Filter '*%s' | Copy-Item -Destination .\\%s -Force\"", nomePastaASerRealizadoFiltro, sufixoASerFiltrado, nomePastaDestino);
    #else
        sprintf(command, "find ./%s -type f -name \"*%s\" -exec cp {} ./%s \\;", nomePastaASerRealizadoFiltro, sufixoASerFiltrado, nomePastaDestino);
    #endif
    int status = system(command);
    if(status != 0)
        std::cout << "Nao foi possivel filtrar os resultados" << std::endl;
}

char* criaPastaEstagioSemanaAtual(int estagio, int semana){
    char temp[30];
    sprintf(nomePastaEstagioSemana, "E%dS%d", estagio, semana);
    sprintf(temp, "%s/%s", nomePastaResultados, nomePastaEstagioSemana);
    criaPasta(temp, false);
    return nomePastaEstagioSemana;
}

void informaSeModoDebug(){
    #ifdef DEBUG_MODE
        std::cout << std::endl << "Codigo rodando em modo ***DEBUG***" << std::endl << std::endl;
    #else
        std::cout << std::endl << "Codigo rodando em modo NORMAL" <<  std::endl << std::endl;
    #endif
}

std::string numeroComSeparador(int numero){
    std::string num = std::to_string(numero);
    std::string separada = "";
    for(int i = (int) num.size() - 1, cont = 0; i >= 0; i--, cont++){
        if(cont == 3){
            separada += '\'';
            cont = 0;
        }
        separada += num[i];
    }
    int n = (int) separada.size();
    for(int i = 0; i < n / 2; i++) std::swap(separada[i], separada[n - i - 1]);
    return separada;
}

std::string diferencaTempo(chpoint inicio, chpoint fim){
    chdd tempoTotal = sch::duration_cast<chdd>(fim - inicio);
    double tempoTotalEmSegundos = tempoTotal.count();

    std::stringstream ss;
    if(tempoTotalEmSegundos > 60.0){
        int minutos = static_cast<int>(tempoTotalEmSegundos) / 60;
        int segundos = static_cast<int>(tempoTotalEmSegundos) % 60;
        ss << minutos << " minuto" << (minutos > 1? "s" : "") << " e " << segundos << " segundo" << (segundos > 1? "s" : "");
    }else{
        ss << std::fixed << std::setprecision(2) << tempoTotalEmSegundos << " (s)";
    }
    return ss.str();
}

void informarTempoDecorridoSimulacao(chpoint inicial, chpoint final){
    std::cout << "Tempo decorrido: " << diferencaTempo(inicial, final) << std::endl;
}
