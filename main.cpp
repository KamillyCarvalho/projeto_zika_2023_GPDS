/*
    Modelo computacional para simular o fluxo sanguíneo em artérias específicas do corpo humano.
    Nesse modelo, cada partícula representa uma pequena porção do fluido sanguíneo.
    A função atualizaPosicao é responsável por calcular como cada partícula se move ao longo do tempo devido a diferentes forças e influências.
*/

#include "headers/funcsimu.h" // funcoes utilizadas na modelagem
#include "headers/constantes.h" // constantes utilizadas na simulacao
#include "headers/uteis.h" // utilidade geral
#include "headers/nucleo.h" // funcoes principais de execucao do algoritmo 

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
    informaSeModoDebug();
    criaPasta(nomePastaResultados, false);
    for(int numEstagio = 0; numEstagio < (int) ESTAGIOS.size(); numEstagio++){
        std::cout << "ESTAGIO " << numEstagio + 1 << '\n';
        for(int semanaAtual = 0; semanaAtual < (int) ESTAGIOS[numEstagio].size(); semanaAtual++){
            /* Cria pasta com resultados do estagio atual e semana atual e retorna nome da pasta */
            char *nomePastaEstagioSemana = criaPastaEstagioSemanaAtual(numEstagio + 1, semanaAtual + 1);
            
            /* Seta nomes de arquivos e variaveis antes de chamar a rotina */
            sprintf(nomeArquivoSaida1, "%s/%s/E%d_S%d_particleTime.csv", nomePastaResultados, nomePastaEstagioSemana, numEstagio + 1, semanaAtual + 1);
            sprintf(nomeArquivoSaida2, "%s/%s/E%d_S%d_first_delay.csv", nomePastaResultados, nomePastaEstagioSemana, numEstagio + 1, semanaAtual + 1);
            sprintf(nomeArquivoSaida3, "%s/%s/E%d_S%d_received_particles.csv", nomePastaResultados, nomePastaEstagioSemana, numEstagio + 1, semanaAtual + 1);
            semana = semanaAtual; // seta variavel global
            quantidadeParticulas = ESTAGIOS[numEstagio][semanaAtual]; // seta variavel global
            
            /* Print msg no terminal */
            std::cout << "SEMANA " << semanaAtual + 1 << " -> Simulando " << quantidadeParticulas << \
            ' ' << '(' << numeroPorExtenso(quantidadeParticulas) << ')' << " particulas" << '\n';

            /* Chama rotina */
            rotina();
            
            /* Reset variavel para proxima iteracao */
            primeiraASerRecebida = true;
        }
    }
    filtrarArquivosPorSufixo(nomePastaResultados, "received_particles.csv");
    return 0;
}