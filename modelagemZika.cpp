#include "modelagemZika.h"

double calcularDeslocamentoHorizontal(double elasticidadeParede, double velocidadeInicial, double incrementoTempo, double coeficienteDifusao, double incrementoWiener){
    /*
        É influenciado pela pressão hemodinâmica,
        velocidade inicial -> (firstSpeedPointer[indiceY]),
        e um termo relacionado à difusão (sqrt(2 * coeficienteDifusao) * incrementoWiener).
        O movimento é proporcional à pressão hemodinâmica e à velocidade da partícula, e inclui uma componente aleatória modelada pela distribuição de Wiener.
    */

    return elasticidadeParede * velocidadeInicial * incrementoTempo + sqrt(2 * coeficienteDifusao) * incrementoWiener;
}

double calcularDeslocamentoVertical(int forca, double pressaoVenosa, double incrementoTempo, double coeficienteDifusao, double incrementoWiener){
    /*
        É influenciado pela pressão venosa, um termo relacionado à difusão -> (sqrt(2 * coeficienteDifusao) * incrementoWiener), e uma força determinada por condições na artéria.
        A força é definida com base na posição atual (y) em relação à resistência vascular da artéria (resistenciaVascular[rota[arteriaAtual]]).
        A direção da força é determinada aleatoriamente se a posicao da partícula for igual à resistência vascular.
    */

    return forca * pressaoVenosa * incrementoTempo + sqrt(2 * coeficienteDifusao) * incrementoWiener;
}

bool atendeCriterioParadaY(const double resistenciaVascular[], const int rota[], int arteriaAtual, double posYAtual){
    return posYAtual >= 2 * resistenciaVascular[rota[arteriaAtual]];
}

bool atendeCriterioParadaX(const double totalD[], const double comprimentoArteria[], const int rota[], int arteriaAtual, double posXAtual){
    return posXAtual >= totalD[arteriaAtual] + comprimentoArteria[rota[arteriaAtual]];
}

bool atendeCriterioAbsorcaoReflexao(const double resistenciaVascular[], const int rota[], const int arteriaAtual, double posAtualY){
    return posAtualY >= 2 * resistenciaVascular[rota[arteriaAtual]] or posAtualY <= 0;
}


void createFolder(const char* folderName){
    struct stat st;
    if (stat(folderName, &st) == 0)
    {
        // Se o diretório já existe, remove-o
        std::string command = "rm -rf " + std::string(folderName);
        system(command.c_str());
    }
    int status = mkdir(folderName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status == 0) 
        printf("%s folder created\n", folderName);
    else 
        printf("Error creating %s folder\n", folderName);
}

double calculaPerfilVelocidadePonto(double resistenciaVenosa, double passoY, double pressaoHemodinamica, int pontoAtual){
    const double razaoPassoPressao = passoY / pressaoHemodinamica;
    const double coeficienteVelocidade = 12 / (1.4 * resistenciaVenosa);
    const double alfa = 1 - exp(-resistenciaVenosa * pontoAtual * razaoPassoPressao);
    const double beta = 1 - exp(-resistenciaVenosa);
    return coeficienteVelocidade * (-pontoAtual * razaoPassoPressao + alfa / beta);
}