#include <string>
#include <sys/stat.h>
#include <iostream>
#include <ctime>
#include <filesystem>
#include <chrono> // para contar o tempo de simulacao
#include "constantes.h"
#include "debug.h"

namespace sch = std::chrono;
typedef sch::time_point<sch::high_resolution_clock> chpoint;
typedef sch::duration<double> chdd;

#ifndef UTEIS_H
#define UTEIS_H
    void printBarraProgresso(double progresso);
    void criaPasta(const char* nomePasta, bool infoMsg);
    void filtrarArquivosPorSufixo(const char *nomePastaASerRealizadoFiltro, const char *sufixoASerFiltrado, const char *nomePastaDestino);
    void informaSeModoDebug();
    void informarTempoDecorridoSimulacao(chpoint inicial, chpoint final);
    char* criaPastaEstagioSemanaAtual(int estagio, int semana);
    std::string numeroPorExtenso(int numero);
    std::string numeroComSeparador(int numero);
    std::string diferencaTempo(chpoint inicio, chpoint fim);
    unsigned int seedParaRand();
#endif