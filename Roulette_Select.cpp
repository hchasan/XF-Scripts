/*
  A selection program that uses roulette-based selection on a generation of dipole antennas. Can replace Evolved_Dipole

  By Hannah Hasan
  02/17/2018
*/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>                                                          // in case we want to use time as seed for random number generator. For now using SEED (below)
#include <iomanip>
#include <random>

using namespace std;

// GLOBAL VARIABLES
const int NPOP = 5;                                                       // number of individuals per generation
const int SEED = 42;                                                      // seed for generating random number
const int HSHAKELINES = NPOP+8;                                           // number of lines in handshake file
const int HSHOOKLINES = NPOP+2;                                           // number of lines in handshook file
const double DEFAULT_R_MEAN = 0.5;
const double DEFAULT_R_STDEV = 0.25;
const double DEFAULT_L_MEAN = 0.5;
const double DEFAULT_L_STDEV = 0.25;

void Mutate(double* radii, double* lengths, double R_mean, double R_stdev, double L_mean, double L_stdev){
  default_random_engine generator;
  generator.seed(time(NULL));
  normal_distribution<double> distributionR(R_mean,R_stdev);
  for(int i=0; i<NPOP; i++){
    double R = distributionR(generator);
    if(R<=0) i-=1;
    else if(R>0) radii[i] = R;
  }
  normal_distribution<double> distributionL(L_mean,L_stdev);
  for(int j=0; j<NPOP; j++){
    double L = distributionL(generator);
    if(L<=0) j-=1;
    else if(L>0) lengths[j] = L;
  }
}

void ReadHShake(char* filename, ifstream& ifs, string* arr, double* radii, double* lengths){ // reads handshake and stores individuals' radii and lengths

  string file = filename;

  int comma = 0;
  string currentLine;
  string Rad;
  string Len;
  double radius = 0.0;
  double length = 0.0;
  
  ifs.open(file.c_str());
  if(!ifs.is_open()) cout << endl << "Error! File could not be opened" << endl << endl;
  else{
    for(int line=0; line<HSHAKELINES; line++){                            // store lines of handshake.csv
      getline(ifs,arr[line]);
    }
    ifs.close();
    ifs.clear();
  }

  for(int L=8; L<HSHAKELINES; L++){
    currentLine = arr[L];
    comma = currentLine.find(",");
    Rad = currentLine.substr(0,comma);
    Len = currentLine.substr(comma+1);
    radius = atof(Rad.c_str());
    length = atof(Len.c_str());
    radii[L-8] = radius;
    lengths[L-8] = length;
  }  
}

void ReadHShook(char* filename, ifstream& ifs, string* arr, double* scores){  // reads handshook and stores fitness scores

  string file = filename;
  double score = 0;
  
  ifs.open(file.c_str());
  if(!ifs.is_open()) cout << endl << "Error! File could not be opened" << endl << endl;
  else{
    for(int line=0; line<HSHOOKLINES; line++){
      getline(ifs,arr[line]);
    }
    ifs.close();
    ifs.clear();
  }
  for(int i=2; i<HSHOOKLINES; i++){
    score = atof(arr[i].c_str());
    scores[i-2] = score*100000;
  }
}

void FindBest(double* scores, double* radii, double* lengths, double* best){
  double highscore = scores[0];
  int bestindiv = 0;
  for(int i=1; i<NPOP; i++){
    if(scores[i]>=highscore){
      highscore = scores[i];
      bestindiv = i;
    }
  }
  best[0] = highscore/100000;
  best[1] = radii[bestindiv];
  best[2] = lengths[bestindiv];
}

int Roulette(double* scores, double* radii, double* lengths, double sum, int n){
  int individual=0;
  int remainder=-1;
  int rand1=0;
  double runningsum=0;

  srand(time(NULL)+n);
  while(rand1 <= (5*sum)) rand1 = rand();

  while(remainder<0) remainder = rand1 % static_cast<int>(sum);
  //cout << endl << "-" << remainder << "-" << endl;
  if(remainder>=0 && remainder <= scores[0]) individual = 0;
  else{
    runningsum += scores[0];
    //cout << fixed << setprecision(0) <<runningsum << endl;
    for(int m=1; m<NPOP; m++){
      if(remainder > runningsum && remainder<=(runningsum + scores[m])) {individual = m; break;}
      else runningsum += scores[m];
      //cout << runningsum << endl;
    }
  }
  return individual;
}

double STDEV(double* arr){
  double mean=0;
  double sumsq=0;
  double stdev=0;
  for(int s=0; s<NPOP; s++){
    mean += arr[s];
  }
  mean = mean/NPOP;
  for(int d=0; d<NPOP; d++){
    sumsq += pow((arr[d]-mean),2);
  }
  stdev = sqrt(sumsq/(NPOP-1));
}

void WriteHShake(ofstream& ofs, double* radii, double* lengths, double R_mean, double R_stdev, double L_mean, double L_stdev, double* best){
  ofs.open("handshake.csv");
  ofs << "My Hannah can beat your Hannah in a fight." << endl;
  ofs << "Mean R,L:" << endl;
  ofs << R_mean << "," << L_mean << endl;
  ofs << "Deviation R,L:" << endl;
  ofs << R_stdev << "," << L_stdev << endl;
  ofs << "Best:" << endl;
  ofs << best[0] << "," << best[1] << "," << best[2] << endl;
  ofs << "Generation R,L:" << endl;
  for(int p=0; p<NPOP; p++){
    ofs << radii[p] << "," << lengths[p] << endl;
  }
}

int main(int argc, char** argv){

  //variables for reading in handshake.csv
  ifstream shake;
  string *HShakeLines = NULL;
  double *Radii = NULL;
  double *Lengths = NULL;
  char handshakename[] = "handshake.csv";
  
  //variables for reading in handshook.csv
  ifstream shook;
  string *HShookLines = NULL;
  double *Fitness = NULL;
  char handshookname[] = "handshook.csv";

  //variables for writing new handshake.csv
  ofstream newshake;
  
  if(string(argv[1]) == "--start"){
    Radii = new double[NPOP];
    Lengths = new double[NPOP];

    double *Best = NULL;
    Best = new double[3];
    Best[0]= -42.0; Best[1]=0.0; Best[2]=0.0;
    Mutate(Radii, Lengths, DEFAULT_R_MEAN, DEFAULT_R_STDEV, DEFAULT_L_MEAN, DEFAULT_L_STDEV);

    //error checking
    //for(int q=0; q<NPOP; q++) cout << Radii[q] << "," << Lengths[q] << endl;
    
    WriteHShake(newshake, Radii, Lengths, DEFAULT_R_MEAN, DEFAULT_R_STDEV, DEFAULT_L_MEAN, DEFAULT_L_STDEV, Best);
  }

  else if(string(argv[1]) == "--cont"){
      // generation's best antenna
      double *Best = NULL; 
  
      // Arrays for the selected parents
      double ParentR[NPOP];
      double ParentL[NPOP];
  
      // Array for new individuals
      double newGen[NPOP][NPOP];
  
      HShakeLines = new string[HSHAKELINES];
      Radii = new double[NPOP];
      Lengths = new double[NPOP];

      // store radii and lengths of this generations
      ReadHShake(handshakename, shake, HShakeLines, Radii, Lengths);

      // just a check to make sure R,Ls stored properly
      //for(int i=0; i<NPOP; i++) cout << Radii[i] << " " << Lengths[i] << endl;
  
      HShookLines = new string[HSHOOKLINES];
      Fitness = new double[NPOP];
      
      // store fitness scores
      ReadHShook(handshookname, shook, HShookLines, Fitness);  

      // just a check to make sure fitness stored properly
      //for(int j=0; j<NPOP; j++) cout << Fitness[j] << endl;

      Best = new double[3];
      FindBest(Fitness, Radii, Lengths, Best);

      // just a check to make sure the best individual is stored correctly
      //for(int k=0; k<3; k++) cout << Best[k] << endl;

      //Compute sum of the scaled-up fitness scores. Will be passed to Roulette function
      double sumfit=0;
      for(int f=0; f<NPOP; f++) sumfit+=Fitness[f];

      int selected;
      for(int n=0; n<NPOP; n++){
	selected = Roulette(Fitness, Radii, Lengths, sumfit, n);
	ParentR[n]=Radii[selected];
	ParentL[n]=Lengths[selected];
	}
      
      double rstdev=STDEV(ParentR);
      double lstdev=STDEV(ParentL);
      double rmean=0;
      double lmean=0;
      for(int a=0; a<NPOP; a++){
	rmean += ParentR[a]/NPOP;
	lmean += ParentL[a]/NPOP;
      }
      Mutate(ParentR, ParentL, rmean, rstdev, lmean, lstdev);
      //find best individual
      
      WriteHShake(newshake, ParentR, ParentL, rmean, rstdev, lmean, lstdev, Best);
    }

    return 0;
}
