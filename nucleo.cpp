#include "nucleo.h"

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