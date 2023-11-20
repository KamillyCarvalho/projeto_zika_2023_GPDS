#include <string>
#include <sys/stat.h>
#include <iostream>
#include <ctime>
#include "constantes.h"
#include "debug.h"

#ifndef UTEIS_H
#define UTEIS_H

    void printBarraProgresso(double progresso);
    void criaPasta(const char* folderName, bool printMsg);
    void filtrarArquivosPorSufixo(const char *nomePastaASerRealizadoFiltro, const char *sufixoASerFiltrado);
    char* criaPastaEstagioSemanaAtual(int estagio, int semana);
    std::string numeroPorExtenso(int numero);
    unsigned int seedParaRand();

#endif