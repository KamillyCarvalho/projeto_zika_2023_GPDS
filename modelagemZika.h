#include <sys/stat.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#include <iomanip>
#include <iostream>

#ifndef ZIKA_H
#define ZIKA_H

    void criaPasta(const char* folderName, bool printMsg);
    void rotina();
    void atualizaPosicao(double x, double y);
    void printBarraProgresso(double progresso);
    double calcularDeslocamentoHorizontal(double pressaoHemodinamica, double velocidadeInicial, double incrementoTempo, double coeficienteDifusao, double incrementoWiener);
    double calcularDeslocamentoVertical(int forca, double pressaoVenosa, double incrementoTempo, double coeficienteDifusao, double incrementoWiener);
    double calculaPerfilVelocidadePonto(double resistenciaVenosa, double passoY, double pressaoHemodinamica, int pontoAtual);
    bool atendeCriterioParadaY(const double resistenciaVascular[], const int rota[], int arteriaAtual, double posYAtual);
    bool atendeCriterioParadaX(const double totalD[], const double comprimentoArteria[], const int rota[], int arteriaAtual, double posXAtual);
    bool atendeCriterioAbsorcaoReflexao(const double resistenciaVascular[], const int rota[], const int arteriaAtual, double posAtualY);
    std::string numeroPorExtenso(int numero);

#endif