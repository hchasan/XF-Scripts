/*	This GA is adapted from CalPoly's hybrid roulette / tournament method to work with Ohio State's loop. 
	Written by David Liu
	Other comments to follow and instructions to follow. */
	
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <math.h>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <math.h>

using namespace std;

// Headers for subroutines we'll call in main

/*	Inputs for function handRead: 
	varInput = tensor for variables to be read by the program (parent population)
	fitness = vector storing the fitness scores for each parent
	popSize = population size
*/
void handRead(vector<vector<vector<float>>> &varInput, vector<float> &fitness, int popSize);

/*	Inputs for function handWrite: 
	numChildren = total number of children (population size)
	varVector = variable data for the children to be written to handshake
*/
void handWrite(int numChildren, vector<vector<vector<float>>> &varVector);

/*	Inputs for function roulette:
	varInput = variables we read from previous generation
	varOutput = variables we generate for next generation
	parents = tensor to store the variables used to generate an individual
	fitness = fitness scores
	popSize = size of the population
*/
void roulette(vector<vector<vector<float>>> &varInput, vector<vector<vector<float>>> &varOutput, vector<float> &fitness, int popSize);

// Declare some global constants

const int NSECTIONS = 1;  // at the moment our antenna is only one section
const int NVARS = 2; // at the moment we only care about length and radius
const int ROUL_X_NO = 3; // We generate 4 children via roulette
const int ROUL_MUT_NO = 2; // We mutate 1 child from roulette
const int TOUR_X_NO = 0; // We generate 4 children via tournament
const int TOUR_MUT_NO = 0; // We mutate 1 child from tournament
const int PARENT_NO = 2; // We find 2 parents per child
const int TOURNEY_LOTTERY_SIZE = 4; // We pick four individuals from the pool per tourney. 

void tournament(vector<vector<vector<float>>> &varInput, vector<vector<vector<float>>> &varOutput, vector<float> &fitness, int popSize);

int main(int argc, char const *argv[])
{
    /*
	argc = number of console commands
	argv[] = actual console commands 
	var_input = tensor for variables read by the program (parents)
	var_output = tensor for variables written by the program (children)
	fitness = vector for fitness scores read by the program
    */

	// determine our population size from the beginning. This is the maximum number of matrices we'll need to store all our data
	int popsize = ROUL_X_NO + ROUL_MUT_NO + TOUR_X_NO + TOUR_MUT_NO;
	
	/* Create vector for parsing in variables. We're only working with length and radius at the moment, so this doesn't need to be as big as it is.
	// But later on, we'll be working with many more variables, so this should theoretically save some time when we do by writing this way.
	
	// Vector var_input is an  n x NSECTIONS x NVARS tensor, where n is the number of individuals in the population. Basically, every matrix stores the data for each individual.
	// Honestly we probably don't need a matrix for all the variables, but we MIGHT need that much space down the line, so might as well. Data structure is:
	
	INDIVIDUAL 1
	            VAR 1      VAR 2      VAR 3      VAR 4      ...      VAR Z
	SECTION 1   LENGTH 1   RADIUS 1   VAR 3      VAR 4      ...      VAR Z
	SECTION 2	LENGTH 2   RADIUS 2   VAR 3      VAR 4      ...      VAR Z
	SECTION 3	LENGTH 3   RADIUS 3   VAR 3      VAR 4      ...      VAR Z
	SECTION 4	LENGTH 4   RADIUS 4   VAR 3      VAR 4      ...      VAR Z
	...
	SECTION Y	LENGTH Y   RADIUS Y   VAR 3      VAR 4      ...      VAR Z

	INDIVIDUAL 2
	            VAR 1      VAR 2      VAR 3      VAR 4      ...      VAR Z
	SECTION 1   LENGTH 1   RADIUS 1   VAR 3      VAR 4      ...      VAR Z
	SECTION 2	LENGTH 2   RADIUS 2   VAR 3      VAR 4      ...      VAR Z
	SECTION 3	LENGTH 3   RADIUS 3   VAR 3      VAR 4      ...      VAR Z
	SECTION 4	LENGTH 4   RADIUS 4   VAR 3      VAR 4      ...      VAR Z
	...
	SECTION Y	LENGTH Y   RADIUS Y   VAR 3      VAR 4      ...      VAR Z
	
	...
	
	INDIVIDUAL X
	            VAR 1      VAR 2      VAR 3      VAR 4      ...      VAR Z
	SECTION 1   LENGTH 1   RADIUS 1   VAR 3      VAR 4      ...      VAR Z
	SECTION 2	LENGTH 2   RADIUS 2   VAR 3      VAR 4      ...      VAR Z
	SECTION 3	LENGTH 3   RADIUS 3   VAR 3      VAR 4      ...      VAR Z
	SECTION 4	LENGTH 4   RADIUS 4   VAR 3      VAR 4      ...      VAR Z
	...
	SECTION Y	LENGTH Y   RADIUS Y   VAR 3      VAR 4      ...      VAR Z
	
	*/
	
    vector<vector<vector<float>>> var_input (popsize,vector<vector<float> >(NSECTIONS,vector <float>(NVARS, 0.0f)));
	
	// Vector fitness literally just exists to tell us the fitness scores later
	
    vector<float> fitness (popsize, 0.0f);
	
	// Create vector that will hold the children's variables. Same structure as var_input.
	// Probably don't need that much space, but just in case when we start doing AraSim stuff.
	
    vector<vector<vector<float>>> var_output (popsize,vector<vector<float> >(NSECTIONS,vector <float>(NVARS, 0.0f)));
	
	srand((unsigned)time(0)); // Let's just seed our random number generator off the bat
	
    // Read in input arguments and parse in data from files
	
    if(argc != 2)
        cout << "Error: Usage. Specify --start or --cont" << endl;
    else
    {
      if(string(argv[1]) == "--start")
      {
		  /*
			These loops should generate a random number from a normalized distribution with mean 0.5 and sigma 0.25
			to populate every entry in our matrix that we're using. Currently, that's only a 1x2 vector, but later
			it could be a lot bigger.
			Sample:
			
			Current:
			
			r r 0 0 0 ... 0
			0 0 0 0 0 ... 0
			.             .
			.             .
			.             .
			0 0 0 0 0 ... 0
			
			My thought process currently is each row represents sections of the antenna (to accommodate weird shapes)
			and then each column represents a specific variable in that section (currently just length and radius)
		  */
		std::default_random_engine generator;
		generator.seed(time(0));
		std::normal_distribution <float> distribution(0.5, 0.25);
		
		for(int i=0;i<popsize;i++)
		{
			for(int j=0;j<NSECTIONS;j++)
			{
				for(int k=0;k<NVARS;k++)
				{
					float r = distribution(generator);
					
					while(r<=0) // We don't accept negative or zero values
						r = distribution(generator);
					
					var_output[i][j][k]= r;
				}
			}
		}
		// Next up we write to file handshake
		handWrite(popsize, var_output);
	  }
	  else if(string(argv[1]) == "--cont")
	  {
		  handRead(var_input,fitness,popsize); // Read in the stuff from previous generation
		  roulette(var_input,var_output,fitness,popsize);
		  handWrite(popsize, var_output);
	  }
	  
	}
}

	/*		
		As currently written function handWrite outputs an NSECTIONS x NVARS sized matrix. 
		NSECTIONS represents how many segments in our antenna we're generating for.
		Currently this is only 1 since we're just working with evolving a dipole.
		NVARS represents how many variables in that section we're working with.
		Currently this is only 2 since we're just working with length and radius.
		
		Currently, this function should output something like:
		
		Roulette / Tournament Hybrid -- Thanks to Cal Poly / Jordan Potter
		Generation Matrices:
		Individual 1: 
		0.5,0.5
		Individual 2:
		0.5,0.5
		...
		Individual 10:
		0.5,0.5
		
		In the future, it'll output something like:
		
		Roulette / Tournament Hybrid -- Thanks to Cal Poly / Jordan Potter
		Generation Matrices:
		Individual 1:
		0.5 0.5 0.5 ... 0.5
		0.5 0.5 0.5 ... 0.5
		.                 .
		.                 .
		.                 .
		0.5 0.5 0.5 ... 0.5
		Individual 2:
		0.5 0.5 0.5 ... 0.5
		0.5 0.5 0.5 ... 0.5
		.                 .
		.                 .
		.                 .
		0.5 0.5 0.5 ... 0.5
		...
		Individual 10:
		0.5 0.5 0.5 ... 0.5
		0.5 0.5 0.5 ... 0.5
		.                 .
		.                 .
		.                 .
		0.5 0.5 0.5 ... 0.5
	*/
	
void handWrite(int numChildren, vector<vector<vector<float>>> &varVector)
{
	ofstream handshake;
	handshake.open("handshake.csv");
	handshake << "Hybrid of Roulette and Tournament -- Thanks to Cal Poly / Jordan Potter" << "\n";
	handshake << "Author was David Liu" << "\n";
	handshake << "Notable contributors: Julie Rolla, Hannah Hasan, and Adam Blenk" << "\n";
	handshake << "Done at The Ohio State University" << "\n";
	handshake << "Working on behalf of Dr. Amy Connolly" << "\n";
	handshake << "And the ANITA project" << "\n";
	handshake << "Revision date: 8 March 2018 1900 EST" << "\n";
	handshake << "Matrices for this Generation: " << "\n";
	for(int i=0;i<numChildren;i++)
		{
			for(int j=0;j<NSECTIONS;j++)
			{
				for(int k=0;k<NVARS;k++)
				{
					if(k==(NVARS-1))
					{
						handshake << varVector[i][j][k] << "\n";
					}
					else
					{
						handshake << varVector[i][j][k] << ",";
					}
				}
			}
		}
	handshake.close();
}

void handRead(vector<vector<vector<float>>> &varInput, vector<float> &fitness, int popSize)
{
	ifstream handshake;
	handshake.open("handshake.csv");
	int csv_file_size = 8 + (popSize * NSECTIONS);
	/* 
		This loop only exists to set the location of where each individual's data is
		Their location should be at locations[z-1] + 1 + NSECTIONS. This just takes the previous location and adds
		1 line for the header and NSECTIONS lines for the data.
		
		So in the case of NSECTIONS = 3, our file output will look like:
		
		Roulette / Tournament Hybrid -- Thanks to Cal Poly / Jordan Potter
		Generation Matrices:
		Individual 1: 
		0.5,0.5
		0.5,0.5
		0.5,0.5
		Individual 2:
		0.5,0.5
		0.5,0.5
		0.5,0.5
		Individual 3:
		0.5,0.5
		0.5,0.5
		0.5,0.5
		Individual 4:
		0.5,0.5
		0.5,0.5
		...
		Individual 10:
		0.5,0.5
		0.5,0.5
		0.5,0.5
		
		and this will set the location of data for each individual to:
		
		Individual 1: Line 3
		Individual 2: Line 7
		Individual 3: Line 11
		Individual 4: Line 15
		...
		Individual 10: Line 39
		Individual 11: Line 43 (this is a dummy location and shouldn't actually have any data with popSize = 10)
		We include individual 11 so we don't have to do anything complicated when we start pulling data out
		We look for data between location[j] and location[j+1]; then the 10th individual's data is between
		location[9] = line 39 and location[10] = line 43. Without an Individual popSize + 1, we'd need to write
		some more code specifically to pull out data for the last individual in the population.
		
		And this shouldn't need to be adjusted for when (if) we start incorporating multiple lines of data
		per individual.
	*/	
	string csvContent[csv_file_size+1]; //contain each line of csv
	string strToDbl; //data change from string to float, then written to varInput or fitness.
	
	/*
		This loop reads through the .csv file line by line.
		If we're in data (past line 8), it reads in the line.
		Then it converts it 
		
		
	*/
	for(int i=1;i<=csv_file_size;i++)
	{
		getline(handshake,csvContent[i]);
		if (i>8)
		{
			double j=floor((i-9)/NSECTIONS);  // Figure out which individual we're looking at
			int p=i-9-NSECTIONS*j; // pulls out which row of their matrix we're looking at 
			istringstream stream(csvContent[i]);
			for(int k=0;k<NVARS;k++)
				{
					getline(stream,strToDbl,',');
					varInput[j][p][k] = atof(strToDbl.c_str());
				}
		}
	}
	
	handshake.close();
	
	// Now we need to read the fitness scores:
	ifstream handshook;
	handshook.open("handshook.csv");
	string fitnessRead[popSize+2];
	for(int i=0;i<(popSize+2);i++)
    {
		getline(handshook,fitnessRead[i]);
		if(i>=2)
		{
			fitness[i-2] = atof(fitnessRead[i].c_str());
		}
    }
	handshook.close();
}

void roulette(vector<vector<vector<float>>> &varInput, vector<vector<vector<float>>> &varOutput, vector<float> &fitness, int popSize)
{
	/* Roulette selection works as follows:
		Every parent in the population has a chance of having their genes selected. Each gene is selected by the probability of:
		
			     fitness
		p = -------------------
		    sum of all fitness
		
		Which is to say higher fitness score parents are more likely to have their genes selected, but all genes have a chance of
		being expressed. We use a partial sum to select to guarantee that at least one gene will be selected by the end.
		So currently as written we do it this way:
		
		p1 = f1 / F
		p2 = (f1 + f2) / F
		p3 = (f1 + f2 + f3) / F
		...
		p10 = F / F
		
		We then create ROUL_MUT_NO mutant offspring; which is to say we run through and randomly change genes by a small amount.
		I currently am selecting two parents through roulette wheel selection for every 
	*/
	
	float fitness_total=0;
	int roulette_no=ROUL_MUT_NO+ROUL_X_NO;
	vector<vector<int>> selected (roulette_no,vector<int>(PARENT_NO,0));
	
	// Calculate total fitness for our probabilities.
	for(int i=0;i<popSize;i++)

	{
		fitness_total+=fitness[i];
	}
	
	/** Roulette Wheel Selection **/
	/*
		How these loops work:
		i loop runs through and makes the entire loop run for each individual offspring
		in other words, the i loop generates parents for the ith offpsring
		
		j loop runs through and runs the parent selection loop until we have enough parents for each offspring
		in other words, the j loop generates j parents for the ith offspring
		
		k loop runs through and actually selects parents. It does this by taking the partial sum of each successive parent in
		the fitness vector. Then it looks how what percentage of the total sum the partial sum is, and comparing that percentage to
		the random number 1-100 generated earlier. If the partial sum is greater than the random number previously generated,
		it records the location of which parent's fitness score was added to exceed the partial sum barrier
		(i.e. if it had to add 7 partial sums together, it records 6, since vectors record locations 0-9, so the 7th parent is at location [6])
		It then breaks the loop and returns to the j loop
		
		The j loop then records the parent's location in a matrix. Each row in our matrix represents a particular offspring,
		and each column represents parents. So the data structure for the selected matrix is as follows:
		
		              PARENT LOCATION    PARENT LOCATION    PARENT LOCATION    ...    PARENT LOCATION    
		OFFSPRING 1        count              count              count                     count
		OFFSPRING 2        count              count              count                     count
		OFFSPRING 3        count              count              count                     count
		...                ...                ...                ...                       ...
		OFFSPRING X        count              count              count                     count
		
		Incidentally, this matrix is of size roulette_no x PARENT_NO, which is currently 5x2.
		But if we increase the number of offspring (ROUL_MUT_NO + ROUL_X_NO), we increase rows
		And if we increase the number of parents, we increase columns.
	*/
	for(int i=0;i<roulette_no;i++)  // run for however many kids we are generating
	{
		for(int j=0;j<PARENT_NO;j++)  // run for however many parents per kid
		{
			float partial_sum=0.0f; // reset the partial sum to 0 every time we look for a new parent
			float r=rand()%100; // we randomly select a new chance that must be met every time we find a parent
			
			for(int k=0;k<popSize;k++) // running through the possible parents (10)
			{
				partial_sum = partial_sum + fitness[k];
				if(r<(100*partial_sum/fitness_total)) // if our randomly selected chance is less than the partial sum / total sum * 100, we have a parent
				{
					selected[i][j]=k; // assign parent nomber j for kid number i
					break;
				}
			}
		}
	}

	// Now we actually use the parents and begin breeding offpsring
	// These are just crossbreeds without any mutations yet.
	
	for(int i=0;i<roulette_no;i++)
	{
		for(int j=0;j<NSECTIONS;j++)
		{
			for(int k=0;k<NVARS;k++)
			{
				int pick1=rand()%PARENT_NO; // generates a random integer number between 0 and number of parents
				varOutput[i][j][k]=varInput[selected[i][pick1]][j][k]; // This looks hella messy, but really all it's saying is go to randomly chosen parent and give the offspring the allele there.
			}
		}
	}
	
	// Now we have roulette_no offspring, but we're not done yet! We need to mutate them. To do that we first need to know
	// the average and the deviation of the parent generation. So let's go ahead and calculate that here:
	
	vector<vector<float>> meanTensor (NSECTIONS,vector<float>(NVARS,0));
	vector<vector<float>> dvnTensor (NSECTIONS,vector<float>(NVARS,0));
	
	for (int i=0;i<NVARS;i++)
	{
		for (int j=0;j<NSECTIONS;j++)
		{
			float totalSum=0.0f;
			for (int k=0;k<popSize;k++)
			{
				totalSum+=varInput[k][j][i];
			}
			float mean = totalSum / popSize;
			meanTensor[j][i]=mean;
		}
	}
	
	for (int i=0;i<NVARS;i++)
	{
		for (int j=0;j<NSECTIONS;j++)
		{
			float dvnSum=0.0f;
			for (int k=0;k<popSize;k++)
			{
				dvnSum+=pow((varInput[k][j][i]-meanTensor[j][i]),2);
			}
			float dvn = pow((dvnSum / (popSize - 1)),1/2);
			dvnTensor[j][i]=dvn;
		}
	}
	
	// Now we can do the mutations!
	vector<bool> mutate_flag (roulette_no,false); // Stores if a kid has already been exposed to mutagens. No need to mutate them further
	
	for(int i=0;i<ROUL_MUT_NO;i++)
	{ 
		int r = rand()%roulette_no; // Let's pick a random kid to expose to mutagens
		
		while(mutate_flag[r] == true) // If we already mutated that kid, pick again
			r = rand()%roulette_no;
		for(int j=0;j<NSECTIONS;j++)
		{
			int varMutate = rand()%NVARS;
			std::default_random_engine generator;
			generator.seed(time(0));
			std::normal_distribution <float> distribution(meanTensor[j][varMutate],dvnTensor[j][varMutate]);
			int coefficient=rand()%2;
			if(coefficient==0)
				varOutput[r][j][varMutate]=varOutput[r][j][varMutate]+distribution(generator)/2;  // arbitrarily picked divide by 2 to modulate the effect so we don't get big mutations.
			else if(coefficient==1)
				varOutput[r][j][varMutate]=varOutput[r][j][varMutate]-distribution(generator)/2;  // arbitrarily picked divide by 2 to modulate the effect so we don't get big mutations.
			if(varOutput[r][j][varMutate]<=0) // we really don't want negative values or zero values
				varOutput[r][j][varMutate]=varOutput[r][j][varMutate]+distribution(generator);
			mutate_flag[r]=true;
		}
	}
}
/* This will be the tournament later; for now we're fixing other things. */


/* void tournament(vector<vector<vector<float>>> &varInput, vector<vector<vector<float>>> &varOutput, vector<float> &fitness, int popSize)
{
	/* Tournament selection works as follows:
		We draw a random lottery of individuals from the population and select the best.
		The non-select are then put back in the population, and we draw another random lottery.
		This goes on until we have enough parents.
	
	// Initialize arrays and integers we'll need to do this.
	int tourney_no = TOUR_MUT_NO + TOUR_X_NO;
	vector<bool> selection_flag (popSize, false);
	vector<vector<int> selection_matrix (tourney_no,vector<int>(PARENT_NO,0));
	
	// Incidentally we'll need mean and std dvn for the mutations later anyway, so:
	vector<vector<float>> meanTensor (NSECTIONS,vector<float>(NVARS,0));
	vector<vector<float>> dvnTensor (NSECTIONS,vector<float>(NVARS,0));
	
	for (int i=0;i<NVARS;i++)
	{
		for (int j=0;j<NSECTIONS;j++)
		{
			float totalSum=0.0f;
			for (int k=0;k<popSize;k++)
			{
				totalSum+=varInput[k][j][i];
			}
			float mean = totalSum / popSize;
			meanTensor[j][i]=mean;
		}
	}
	
	for (int i=0;i<NVARS;i++)
	{
		for (int j=0;j<NSECTIONS;j++)
		{
			float dvnSum=0.0f;
			for (int k=0;k<popSize;k++)
			{
				dvnSum+=pow((varInput[k][j][i]-meanTensor[j][i]),2);
			}
			float dvn = pow((dvnSum / (popSize - 1)),1/2);
			dvnTensor[j][i]=dvn;
		}
	}
	
	// Let's start selecting parents:
	
	vector<int> contenders (TOURNEY_LOTTERY_SIZE,0); // Array to store who we select
	vector<bool> contender_flag (popSize, false);

	
	for(int i=0; i<tourney_no; i++)
	{
		for(int j=0; j<PARENT_NO;j++)
		{		
			int s = rand()%popSize;
			contender_flag[s] = true;
			contenders[0] = s;
			for(int k=1; k<TOURNEY_LOTTERY_SIZE; k++)
			{
				s = rand()%popSize;
				while(contender_flag[s] = true || selection_flag[s] = true)
					s = rand()%popSize;
				contender_flag[s] = true;
				contenders[j] = s;
			}
	
			// Ok now we have a bunch of potential parents in the population. Now we sort through them and figure out who is the best.

			int most_fit = 0;
			for(int k=0; k<TOURNEY_LOTTERY_SIZE; k++)
			{
				if(fitness[contenders[i]]>fitness[most_fit])
					most_fit=i;
			}
			selection_matrix[i][j]=most_fit;
			selection_flag[most_fit] = true;
			
			for(int k=0; k<TOURNEY_LOTTERY_SIZE; k++)
			{
				contenders[k] = 0;
				contender_flag[k] = false;
			}
		}
		for(int j=0; j<popSize; j++)
		{
			selection_flag[j] = false;
		}
	}
	*/