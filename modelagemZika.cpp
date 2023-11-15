#include "modelagemZika.h"

double calcularDeslocamentoHorizontal(double pressaoHemodinamica, double velocidadeInicial, double incrementoTempo, double coeficienteDifusao, double incrementoWiener){
    /*
        É influenciado pela pressão hemodinâmica,
        velocidade inicial -> (firstSpeedPointer[indiceY]),
        e um termo relacionado à difusão (sqrt(2 * coeficienteDifusao) * incrementoWiener).
        O movimento é proporcional à pressão hemodinâmica e à velocidade da partícula, e inclui uma componente aleatória modelada pela distribuição de Wiener.
    */

    return pressaoHemodinamica * velocidadeInicial * incrementoTempo + sqrt(2 * coeficienteDifusao) * incrementoWiener;
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
