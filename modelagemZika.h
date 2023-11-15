#ifndef ZIKA_H
#define ZIKA_H
    
    double calcularDeslocamentoHorizontal(double pressaoHemodinamica, double velocidadeInicial, double incrementoTempo, double coeficienteDifusao, double incrementoWiener);
    double calcularDeslocamentoVertical(int forca, double pressaoVenosa, double incrementoTempo, double coeficienteDifusao, double incrementoWiener);
    bool atendeCriterioParadaY(const double resistenciaVascular[], const int rota[], int arteriaAtual, double posYAtual);
    bool atendeCriterioParadaX(const double totalD[], const double comprimentoArteria[], const int rota[], int arteriaAtual, double posXAtual);
    bool atendeCriterioAbsorcaoReflexao(const double resistenciaVascular[], const int rota[], const int arteriaAtual, double posAtualY);
#endif