/*
    Modelo computacional para simular o fluxo sanguíneo em artérias específicas do corpo humano.
    Nesse modelo, cada partícula representa uma pequena porção do fluido sanguíneo.
    A função atualizaPosicao é responsável por calcular como cada partícula se move ao longo do tempo devido a diferentes forças e influências.
*/

#include "modelagemZika.h" // funcoes utilizadas na modelagem
#include <fstream> // funcoes para manipulacao de arquivos

/*
Define para DEBUG MODE
Manter a linha abaixo comentada para funcionamento normal
Descomentar para definir o modo DEBUG
*/

// #define DEBUG_MODE

#ifdef DEBUG_MODE
    #define RANDOM_SEED_CONSTANT 42
#endif

/* Constantes utilizadas na simulacao */
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

/* Variaveis GLOBAIS utilizadas na simulacao */
int semana = 0;
int quantidadeParticulas = 0;
int arteriaAtual = 0;
int contadorAbsorvidas = 0;
int contadorTempoParticula = 0;
int contadorParticulaIrma = 0;
bool primeiraASerRecebida = true;
double pressaoHemodinamica = 0.0;
std::vector<double> perfilVelocidade[NUM_ARTERIAS];

/* Variaveis GLOBAIS para arquivos de saida */
char nomeArquivoSaida1[255];
char nomeArquivoSaida2[255];
char nomeArquivoSaida3[255];
std::ofstream arquivoSaida1;
std::ofstream arquivoSaida2;  
std::ofstream arquivoSaida3;

/* Funcao principal */
int main()
{
    char nomePastaResultados[] = "resultados";
    criaPasta(nomePastaResultados, false);
    for(int numEstagio = 0; numEstagio < (int) ESTAGIOS.size(); numEstagio++){
        std::cout << "ESTAGIO " << numEstagio + 1 << '\n';
        for(int semanaAtual = 0; semanaAtual < (int) ESTAGIOS[numEstagio].size(); semanaAtual++){
            std::cout << "SEMANA " << semanaAtual + 1 << '\n';

            /* Cria pasta com resultados do estagio atual e semana atual */
            char nomePastaEstagioSemana[30];
            sprintf(nomePastaEstagioSemana, "E%dS%d", numEstagio + 1, semanaAtual + 1);
            std::string temp = std::string(nomePastaResultados) + "/" + std::string(nomePastaEstagioSemana);
            criaPasta(temp.c_str(), false);
            
            /* Seta nomes de arquivos e variaveis antes de chamar a rotina */
            sprintf(nomeArquivoSaida1, "%s/%s/E%d_S%d_particleTime.csv", nomePastaResultados, nomePastaEstagioSemana, numEstagio + 1, semanaAtual + 1);
            sprintf(nomeArquivoSaida2, "%s/%s/E%d_S%d_first_delay.csv", nomePastaResultados, nomePastaEstagioSemana, numEstagio + 1, semanaAtual + 1);
            sprintf(nomeArquivoSaida3, "%s/%s/E%d_S%d_received_particles.csv", nomePastaResultados, nomePastaEstagioSemana, numEstagio + 1, semanaAtual + 1);
            semana = semanaAtual;
            quantidadeParticulas = ESTAGIOS[numEstagio][semanaAtual];

            /* Chamar rotina */
            rotina();
            
            /* Reset variavel para proxima iteracao */
            primeiraASerRecebida = true;
        }
    }

    return 0;
}

void rotina()
{
    arquivoSaida1.open(nomeArquivoSaida1);
    arquivoSaida1 << "Time (s),\n";

    arquivoSaida2.open(nomeArquivoSaida2);   
    arquivoSaida2 << "artery" << "," << "time" << ",\n";

    arquivoSaida3.open(nomeArquivoSaida3);
    arquivoSaida3 << "particle" << "," << "artery" << "," << "time" << ",\n";

    srand(time(NULL));
    #ifdef DEBUG_MODE
        srand(RANDOM_SEED_CONSTANT);
    #endif

    for (int i = 0; i < NUM_ARTERIAS; i++)
    {
        pressaoHemodinamica = 2 * resistenciaVascular[rota[i]];
        for (int j = 0; j < static_cast<int>((pressaoHemodinamica + passoY) / passoY); j++)
        {
            double velocidadePontoAtual = calculaPerfilVelocidadePonto(resistenciaVenosa, passoY, pressaoHemodinamica, j);
            perfilVelocidade[i].push_back(velocidadePontoAtual);
        }
    }

    for (int particulaAtual = 1, percentualFinalizado = -1; particulaAtual <= quantidadeParticulas; particulaAtual++)
    {
        /* Reset variaveis relacionadas a particula atual */
        arteriaAtual = 0;
        contadorTempoParticula = 0;
        contadorAbsorvidas = 0;
        contadorParticulaIrma = 0;
        double posXInicial = 0.0;
        double posYInicial = static_cast<double>(rand()) / (RAND_MAX / (pressaoInicial - 2. * limiteY)) + limiteY;

        double progressoAtual = 100. * particulaAtual / quantidadeParticulas;
        if((int) progressoAtual > percentualFinalizado)
        {
            percentualFinalizado = progressoAtual;
            printBarraProgresso(progressoAtual / 100.);
        }

        atualizaPosicao(posXInicial, posYInicial);

        if (contadorAbsorvidas == 0 && contadorParticulaIrma == 0)
            arquivoSaida3 << particulaAtual << "," << rota[arteriaAtual] << "," << contadorTempoParticula * INCREMENTO_TEMPO << ",\n";
    }
    std::cout << std::endl;
    std::cout << "Salvando resultados..." << std::endl;
    arquivoSaida1.close();
    arquivoSaida2.close();
    arquivoSaida3.close();
    return;
}

void atualizaPosicao(double x, double y){
    while(true){
        int indiceY = (int) (y * 1000); // indice do perfil de velocidade baseado na posicao de y
        double incrementoWiener = distribuicaoNormal(geradorDistNormal); // incremento da iteracao
        double* perfilVelocidadeArteriaAtual = perfilVelocidade[arteriaAtual].data();

        /* Calculando a posicao */
        int forca = 0;
        if(y > resistenciaVascular[rota[arteriaAtual]] or y < resistenciaVascular[rota[arteriaAtual]])
            forca = 1;
        else{
            srand(time(NULL));
            #ifdef DEBUG_MODE
                srand(RANDOM_SEED_CONSTANT);
            #endif

            if(rand() % 2 == 1) forca = -1;
            else forca = 1;
        }
        
        double deltaX = calcularDeslocamentoHorizontal(elasticidadeParede[semana][arteriaAtual], perfilVelocidadeArteriaAtual[indiceY], INCREMENTO_TEMPO, coeficienteDifusao, incrementoWiener);
        double deltaY = calcularDeslocamentoVertical(forca, pressaoVenosa, INCREMENTO_TEMPO, coeficienteDifusao, incrementoWiener);
        x += deltaX; // atualiza posicao x
        y += deltaY; // atualiza posicao x
        contadorTempoParticula++; // tempo da particula

        /* Determinacao se Absorcao ou Reflexao */
        if(atendeCriterioAbsorcaoReflexao(resistenciaVascular, rota, arteriaAtual, y)){
            int ehReflexao = distribuicaoAbsorcaoReflexao(geradorAbsorcaoReflexao);
            if(ehReflexao){ // Reflexao
                y -= 2 * deltaY; 
            }
            else{ // Absorcao
                contadorAbsorvidas++;
                break;
            }
        }
        /* Arteria e Criterio de Parada */
        if(atendeCriterioParadaX(totalD, comprimentoArteria, rota, arteriaAtual, x)){
            if(primeiraASerRecebida)
                arquivoSaida2 << rota[arteriaAtual] << "," << contadorTempoParticula * INCREMENTO_TEMPO << ",\n";
            
            if(arteriaAtual == NUM_ARTERIAS - 1){   
                arquivoSaida1 << contadorTempoParticula * INCREMENTO_TEMPO << ",\n";
                primeiraASerRecebida = false;
                break;
            }else{
                arteriaAtual++;
                /* Arteria irma */
                if(atendeCriterioParadaY(resistenciaVascular, rota, arteriaAtual, y)){
                    contadorParticulaIrma++;
                    break;
                }
            }
        }
    }
    return;
}