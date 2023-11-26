#include "constantes.h"
#include "funcsimu.h"
#include "uteis.h"
#include <fstream> // funcoes para manipulacao de arquivos

#ifndef NUCLEO_H
#define NUCLEO_H
    /* Funcoes gerais */
    void rotina();
    void atualizaPosicao(double x, double y);

    /* Variaveis GLOBAIS utilizadas na simulacao */
    extern int semana;
    extern int quantidadeParticulas;
    extern int arteriaAtual;
    extern int contadorAbsorvidas;
    extern int contadorTempoParticula;
    extern int contadorParticulaIrma;
    extern bool primeiraASerRecebida;
    extern double pressaoHemodinamica;
    extern std::vector<std::vector<double>> perfilVelocidade;

    /* Variaveis GLOBAIS para arquivos de saida */
    extern char nomeArquivoSaida1[255];
    extern char nomeArquivoSaida2[255];
    extern char nomeArquivoSaida3[255];
    extern std::ofstream arquivoSaida1;
    extern std::ofstream arquivoSaida2;  
    extern std::ofstream arquivoSaida3;
#endif