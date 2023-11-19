#include <string>
#include <sys/stat.h>
#include <iostream>
#include <ctime>
#include "constantes.h"

#ifndef UTEIS_H
#define UTEIS_H

    std::string numeroPorExtenso(int numero);
    void printBarraProgresso(double progresso);
    void criaPasta(const char* folderName, bool printMsg);
    unsigned int seedParaRand();

#endif