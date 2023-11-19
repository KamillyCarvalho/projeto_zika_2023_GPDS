/*
    Modelo computacional para simular o fluxo sanguíneo em artérias específicas do corpo humano.
    Nesse modelo, cada partícula representa uma pequena porção do fluido sanguíneo.
    A função atualizaPosicao é responsável por calcular como cada partícula se move ao longo do tempo devido a diferentes forças e influências.
*/

#include "modelagemZika.h" // funcoes utilizadas na modelagem
#include "constantes.h" // constantes utilizadas na simulacao
#include <fstream> // funcoes para manipulacao de arquivos

/* Variaveis GLOBAIS utilizadas na simulacao */
int semana = 0;
int quantidadeParticulas = 0;
int arteriaAtual = 0;
int contadorAbsorvidas = 0;
int contadorTempoParticula = 0;
int contadorParticulaIrma = 0;
bool primeiraASerRecebida = true;
double pressaoHemodinamica = 0.0;
std::vector<std::vector<double>> perfilVelocidade(NUM_ARTERIAS);

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

/*  
    Função de atualizacao de posicao da particula.
    Recursiva e atualiza a posicao da particula a cada chamada.
    Também faz algumas checagens de absorcao ou reflexao. 
*/
void atualizaPosicao(double x, double y)
{
    int indiceY = (int) (y * 1000); // indice do perfil de velocidade baseado na posicao de y
    double incrementoWiener = distribuicaoNormal(geradorDistNormal); // incremento da iteracao
    double* perfilVelocidadeArteriaAtual = perfilVelocidade[arteriaAtual].data();

    /* Calculando a posicao */
    int forca = 0;
    if (y > resistenciaVascular[rota[arteriaAtual]] or y < resistenciaVascular[rota[arteriaAtual]])
        forca = 1;
    else
    {
        srand(time(NULL));
        #ifdef DEBUG_MODE
            srand(RANDOM_SEED_CONSTANT);
        #endif

        if (rand() % 2 == 1) forca = -1;
        else forca = 1;
    }
    
    double deltaX = calcularDeslocamentoHorizontal(elasticidadeParede[semana][arteriaAtual], perfilVelocidadeArteriaAtual[indiceY], INCREMENTO_TEMPO, coeficienteDifusao, incrementoWiener);
    double deltaY = calcularDeslocamentoVertical(forca, pressaoVenosa, INCREMENTO_TEMPO, coeficienteDifusao, incrementoWiener);
    
    x = x + deltaX; // atualiza posicao x
    y = y + deltaY; // atualiza posicao x

    contadorTempoParticula++;
    
    /* Determinacao se Absorcao ou Reflexao */
    if (atendeCriterioAbsorcaoReflexao(resistenciaVascular, rota, arteriaAtual, y))
    {
        int ehReflexao = distribuicaoAbsorcaoReflexao(geradorAbsorcaoReflexao);

        if (ehReflexao) // Reflexao
        {
            y = y - 2 * deltaY; 
        }
        else // Absorcao
        {
            contadorAbsorvidas++;
            return;
        }
    }

    /* Arteria e Criterio de Parada */
    if (atendeCriterioParadaX(totalD, comprimentoArteria, rota, arteriaAtual, x))
    {
        if (primeiraASerRecebida)
        {
            arquivoSaida2 << rota[arteriaAtual] << "," << contadorTempoParticula * INCREMENTO_TEMPO << ",\n";
        }
        if (arteriaAtual == NUM_ARTERIAS - 1)
        {   
            arquivoSaida1 << contadorTempoParticula * INCREMENTO_TEMPO << ",\n";
            primeiraASerRecebida = false;
            return;
        }
        else
        {
            arteriaAtual++;
            /* Arteria irma */
            if (atendeCriterioParadaY(resistenciaVascular, rota, arteriaAtual, y))
            {
                contadorParticulaIrma++;
                return;
            }
        }
    }
    atualizaPosicao(x, y); // proxima posicao
    return;
}
