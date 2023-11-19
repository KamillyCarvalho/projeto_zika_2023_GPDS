#include "headers/constantes.h"

/*
    Configuracoes de nomes de pastas e arquivos
*/
const char nomePastaResultados[] = "resultados";

/*
    Constantes utilizadas na simulacao
*/
const int NUM_ARTERIAS = 13;
const double comprimentoArteria[] = {1, 10, 3, 3, 3.5, 3.5, 16.75, 13.5, 39.75, 22, 22.25, 4, 19.25, 5.5, 10.5, 7.25, 3.5, 13.5, 39.75, 22.25, 22, 2, 2, 6.5, 5.75, 5.5, 5.25, 5, 1.5, 3, 1.5, 3, 12.5, 3.75, 8, 5.75, 14.5, 4.5, 11.25, 44.25};
const double resistenciaVascular[] = {1.502, 0.3, 1.42, 1.342, 0.7, 0.407, 0.4, 0.2, 0.25, 0.175, 0.175, 1.246, 0.4, 1.124, 0.924, 0.5, 0.407, 0.2, 0.25, 0.175, 0.175, 0.3, 0.25, 0.25, 0.15, 0.2, 0.838, 0.35, 0.814, 0.275, 0.792, 0.275, 0.627, 0.175, 0.55, 0.37, 0.314, 0.2, 0.2, 0.2};
const double totalD[] = {0, 1, 4, 7, 11, 16.5, 27, 32.25, 33.75, 35.25, 47.75, 55.75, 61.5};
const double elasticidadeParede[4][13] = { 
                                        {2.80255395, 2.70463342, 2.73086925, 2.70044097, 3.09967097, 3.09625142, 3.05128033, 2.37339617, 1.99734367, 2.3752644 , 2.68141248, 2.95806939, 4.58291478},
                                        {3.37870718, 3.32129288, 3.52361929, 3.61554463, 4.21037942, 4.50704167, 4.56984255, 3.67562146, 3.15379636, 3.82559375, 4.35170993, 4.92655864, 7.74529303},
                                        {3.69878565, 3.64307815, 3.8862281 , 4.00476166, 4.67167833, 5.04437305, 5.13396467, 4.14835062, 3.56917331, 4.34186109, 4.9445025 , 5.61936806, 8.85432724},
                                        {3.80334596, 3.74537948, 3.99331575, 4.11346024, 4.79770086, 5.17621006, 5.26625837, 4.25338507, 3.65858461, 4.44940732, 5.0664316 , 5.75579298, 9.06732555}
                                        };
const int rota[] = {0, 2, 3, 11, 13, 14, 26, 28, 30, 32, 34, 35, 37};

const double INCREMENTO_TEMPO = 1e-1;
const double coeficienteDifusao = 1e-2;
const double pressaoInicial = 2 * resistenciaVascular[0];
const double pressaoVenosa = 1e-2;
const double resistenciaVenosa = 1.0;
const double fracaoAbsorvida = 0.02;
const double fracaoRefletida = 1 - fracaoAbsorvida;
const double limiteY = 1e-3;
const double passoY = 1e-3;

const std::vector<int> estagio1 = {6'558'050, 7'906'264, 8'655'256, 8'899'930};
const std::vector<int> estagio2 = {13'201'581, 36'097'470, 53'530'880, 43'530'297};
const std::vector<int> estagio3 = {2'627'406, 7'205'390, 10'647'480, 8'617'887};
const std::vector<std::vector<int>> ESTAGIOS = {estagio1, estagio2, estagio3};

/* Geradores de numeros aleatorios */
std::default_random_engine geradorDistNormal;
std::normal_distribution<double> distribuicaoNormal(0, sqrt(INCREMENTO_TEMPO));

std::default_random_engine geradorAbsorcaoReflexao;
std::discrete_distribution<int> distribuicaoAbsorcaoReflexao(fracaoAbsorvida, fracaoRefletida);