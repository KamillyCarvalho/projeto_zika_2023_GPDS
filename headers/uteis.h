#include <string>
#include <sys/stat.h>
#include <iostream>
#include <ctime>
#include <filesystem>
#include "constantes.h"
#include "debug.h"

#ifndef UTEIS_H
#define UTEIS_H
    void printBarraProgresso(double progresso);
    void criaPasta(const char* nomePasta, bool infoMsg);
    void filtrarArquivosPorSufixo(const char *nomePastaASerRealizadoFiltro, const char *sufixoASerFiltrado, const char *nomePastaDestino);
    void informaSeModoDebug();
    char* criaPastaEstagioSemanaAtual(int estagio, int semana);
    std::string numeroPorExtenso(int numero);
    unsigned int seedParaRand();

#endif