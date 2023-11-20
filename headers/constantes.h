#include <vector>
#include <random>

#ifndef ZIKA_CONSTANTS_H
#define ZIKA_CONSTANTS_H

/* Variaveis de configuracao */
extern const char nomePastaResultados[];

/* extern indica que a variavel está declarada e inicializada em outro local (constantes.cpp) */
/* Variaveis utilizadas na simulacao */
extern const int NUM_ARTERIAS;
extern const int rota[];
extern const double comprimentoArteria[];
extern const double resistenciaVascular[];
extern const double totalD[];
extern const double elasticidadeParede[4][13];

extern const double INCREMENTO_TEMPO;
extern const double coeficienteDifusao;
extern const double pressaoInicial;
extern const double pressaoVenosa;
extern const double resistenciaVenosa;
extern const double fracaoAbsorvida;
extern const double fracaoRefletida;
extern const double limiteY;
extern const double passoY;

extern const std::vector<int> estagio1;
extern const std::vector<int> estagio2;
extern const std::vector<int> estagio3;
extern const std::vector<std::vector<int>> ESTAGIOS;

/* Geradores de numeros aleatorios */
extern std::default_random_engine geradorDistNormal;
extern std::normal_distribution<double> distribuicaoNormal;

extern std::default_random_engine geradorAbsorcaoReflexao;
extern std::discrete_distribution<int> distribuicaoAbsorcaoReflexao;

#endif
