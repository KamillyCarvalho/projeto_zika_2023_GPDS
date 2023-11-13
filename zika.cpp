#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#include <fstream>
#include <sys/stat.h>

using namespace std;

/* Initializing Parameters - Begin */

             /* #################### Constants Untouchable #################### */

const int arteriesNumber = 13;
int       artery         = 0;

double arteryLength[] = {1, 10, 3, 3, 3.5, 3.5, 16.75, 13.5, 39.75, 22, 22.25, 4, 19.25, 5.5, 10.5, 7.25, 3.5, 13.5, 39.75, 22.25, 22, 2, 2, 6.5, 5.75, 5.5, 5.25, 5, 1.5, 3, 1.5, 3, 12.5, 3.75, 8, 5.75, 14.5, 4.5, 11.25, 44.25};
double resistenciaFundoVaso[]   = {1.502, 0.3, 1.42, 1.342, 0.7, 0.407, 0.4, 0.2, 0.25, 0.175, 0.175, 1.246, 0.4, 1.124, 0.924, 0.5, 0.407, 0.2, 0.25, 0.175, 0.175, 0.3, 0.25, 0.25, 0.15, 0.2, 0.838, 0.35, 0.814, 0.275, 0.792, 0.275, 0.627, 0.175, 0.55, 0.37, 0.314, 0.2, 0.2, 0.2};
double dTotal[]       = {0,1,4,7,11,16.5,27,32.25,33.75,35.25,47.75,55.75,61.5};
double Peu[4][13]    = { 
                       {2.80255395, 2.70463342, 2.73086925, 2.70044097, 3.09967097, 3.09625142, 3.05128033, 2.37339617, 1.99734367, 2.3752644 , 2.68141248, 2.95806939, 4.58291478},
                       {3.37870718, 3.32129288, 3.52361929, 3.61554463, 4.21037942, 4.50704167, 4.56984255, 3.67562146, 3.15379636, 3.82559375, 4.35170993, 4.92655864, 7.74529303},
                       {3.69878565, 3.64307815, 3.8862281 , 4.00476166, 4.67167833, 5.04437305, 5.13396467, 4.14835062, 3.56917331, 4.34186109, 4.9445025 , 5.61936806, 8.85432724},
                       {3.80334596, 3.74537948, 3.99331575, 4.11346024, 4.79770086, 5.17621006, 5.26625837, 4.25338507, 3.65858461, 4.44940732, 5.0664316 , 5.75579298, 9.06732555}
                   };

int pai[]            = {-1, 0, 0, 2, 2, 4, 4, 5, 5, 8, 8, 3, 3, 11, 13, 13, 11, 16, 16, 18, 18, 14, 21, 21, 22, 22, 14, 26, 26, 28, 28, 30, 30, 32, 32, 34, 35, 35, 36, 36};
int irmao[]          = {-1, 2, 1, 4, 3, 6, 5, 8, 7, 10, 9, 12, 11, 16, 15, 14, 13, 18, 17, 20, 19, 26, 23, 22, 25, 24, 21, 28, 27, 30, 29, 32, 31, 34, 33, 35, 37, 36, 39, 38};
int rota[]           = {0,2,3,11,13,14,26,28,30,32,34,35,37};

double dt             = 1e-1;              // Passo
double D              = 1e-2;              // Difusão
double H              = 0.0;
double H0             = 2*resistenciaFundoVaso[0];
double Pev            = 1e-2;
double Rev            = 1.0;
double Pabs           = 0.02;
double Pref           = 1 - Pabs;
double y_border       = 1e-3;
double passoy         = 1e-3;

vector<double> vetoru[arteriesNumber];

default_random_engine generator;
normal_distribution<double> distribution(0,sqrt(dt));

default_random_engine AbsRefGenerator;
discrete_distribution<int> AbsRefDistribution(Pabs,Pref);

             /* #################### Constants Untouchable #################### */

int s           = 0;    // Semanas: 0, 1, 2, 3
long int npart  = 6558050;
/* Valores de npart (considerar parte inteira):
Cada elemento dos vetores abaixo representam respectivamente semanas 0, 1, 2, 3.
E1: [6558050.4  , 7906264.245, 8655256.5  , 8899930.05 ]
E2: [13201581.47960918, 36097470.51368173, 53530880.08779096, 43530297.40378644]
E3: [ 2627406.34125941,  7205390.15518697, 10647480.43585068, 8617887.90675686]
*/
int particle    = 0;

bool isFirstReceived = false;

const vector<int> E1 = {6558050, 7906264, 8655256, 8899930};
const vector<int> E2 = {13201581, 36097470, 53530880, 43530297};
const vector<int> E3 = {2627406,  7205390, 10647480, 8617887};

const vector<vector<int>> ESTAGIOS = {E1, E2, E3};

const string pasta1 = "particle_time";
const string pasta2 = "atraso_primeira";
const string pasta3 = "particulas_recebidas";

const vector<string> nomesPastas = {pasta1, pasta2, pasta3};

const string fileName  = "validate_results/N0_S0_particleTime.csv";
const string fileName2 = "validate_results/N0_S0_atraso_primeira.csv";    
const string filename3 = "validate_results/N0_S0_particulas_recebidas.csv";
// string filename4 = "validate_results/N0_S0_propagacao_particulas_rota.csv";

ofstream file_results;
ofstream file_results2;  
ofstream file_results3;
// ofstream file_results4;

vector<double> particleTime(npart);

int absorptedCounter      = 0;
int particleTimeCounter   = 0;
int particleSisterCounter = 0;

/* Initializing Parameters - Begin */

void updatePosition(double x, double y)
{
    int yIdx      = static_cast <int> (y * 1000);
    int f         = 0;
    int absRef    = 0;
    
    double dx              = 0;
    double dy              = 0;
    double wienerIncrement = distribution(generator);
    double* p_yFirstSpeed  = vetoru[artery].data();   

    /* Calculating position - Begin */
    dx = Peu[s][artery] * *(p_yFirstSpeed + yIdx) * dt + sqrt(2*D) * wienerIncrement;
 
    if (y > resistenciaFundoVaso[rota[artery]])
    {
        f = 1;
    }
    else if (y < resistenciaFundoVaso[rota[artery]])
    {
        f = 1;
    }
    else
    {
        srand(time(NULL));

        if (rand() % 2 == 1) f = -1;
        else f = 1;
    }

    dy = f * Pev * dt + sqrt(2*D) * wienerIncrement;

    x  = x + dx;
    y  = y + dy;
    
    /* Calculating position - End */

    particleTimeCounter++;
    
    /* Absorption or Reflection - Begin */
    if (y >= 2 * resistenciaFundoVaso[rota[artery]] || y <= 0)
    {
        absRef = AbsRefDistribution(AbsRefGenerator);

        if (absRef) // Reflected
        {
            y  = y - 2 * dy; 
        }
        else
        {
            absorptedCounter++;
            return;
        }
    }
    /* Absorption or Reflection - End */

    /* End of the Artery and Stop criterion - Begin */
    if (x >= (dTotal[artery] + arteryLength[rota[artery]]) )
    {
        if (isFirstReceived == false)
        {
            file_results2 << rota[artery] << "," << particleTimeCounter * dt << ",\n";
        }
        // file_results4 << rota[artery] << "," << particleTimeCounter * dt << ",\n";
        if (artery == arteriesNumber - 1)
        {   
            file_results << particleTimeCounter * dt << ",\n";
            isFirstReceived = true;

            return;
        }
        else
        {
            artery++;

            /* Sister artery - Begin */
            if (y >= 2 * resistenciaFundoVaso[rota[artery]] )
            {
                particleSisterCounter++;

                return;
            }
            /* Sister artery - End */
        }
    }
    updatePosition(x, y);
    /* End of the Artery and Stop criterion - End*/

    return;
}


int rotina()
{
    cout << "ROTINA\n";
    file_results.open(fileName);
	file_results << "Time (s),\n";

    file_results2.open(fileName2);   
    file_results2 << "arteria" << "," << "tempo" << ",\n";

    file_results3.open(filename3);
    file_results3 << "particle" << "," << "arteria" << "," << "tempo" << ",\n";

    // file_results4.open(filename4);
    // file_results4 << "arteria" << "," << "tempo" << ",\n";

    srand(time(NULL));

    float x = 0.0;
    float y = static_cast <float> (rand()) / ( static_cast <float> ( RAND_MAX/( H0 - 2 * y_border ) ) ) + y_border;

    cout << "Numero de arterias = " << arteriesNumber << '\n';

    for (int i = 0; i < arteriesNumber; i++)
    {
        cout << "Arteria " << i << '\n';
        H = 2 * resistenciaFundoVaso[rota[i]];

        for (int j = 0; j < static_cast <int> ( (H + passoy) / passoy); j++)
        {
            vetoru[i].push_back( (12 / (1.4 * Rev)) * ( -(j*passoy/H) + (1 - exp(-Rev * (j*passoy/H)) ) / (1 - exp(-Rev) ) ) );
        }
    }

    for (int i = 0; i < npart; i++)
    {
        cout << "Particula " << i << '/' << npart << '\n';

        updatePosition(x, y);

        if (absorptedCounter == 0 && particleSisterCounter == 0)
        {
            file_results3 << i << "," << rota[artery] << "," << particleTimeCounter * dt << ",\n";
        }      

        particle++;
        x = 0.0;
        y = static_cast <float> (rand()) / ( static_cast <float> ( RAND_MAX/( H0 - 2 * y_border ) ) ) + y_border;

        artery                = 0;
        absorptedCounter      = 0;
        particleTimeCounter   = 0;
        particleSisterCounter = 0;

    }

    cout << "Saving results..." << endl;

    file_results.close();
    file_results2.close();
    file_results3.close();
    // file_results4.close();

    return 0;
}


void createFolder(const char * filename){
    struct stat st;
    if (stat(filename, &st) == 0) {
        // Se o diretório já existe, remove-o
        std::string command = "rm -rf " + std::string(filename);
        system(command.c_str());
    }
    int status = mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(status == 0) printf("%s folder created\n", filename);
    else printf("Error creating %s folder\n", filename);
}

int main(){
    rotina();
    return 0;
}