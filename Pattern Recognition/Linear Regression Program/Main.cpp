#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;


struct sample
{
    double x;
    double y;
};

//function prototypes
void end_program();

int main()
{
    string file_name = "";
    string line = "";
    ifstream input_file;
    int num_samples = 0;
    sample* samples;
    int num_in_array = 0;
    char* pEnd;
    double sumXY = 0;       //Sum(XY)
    double sumX = 0;        //Sum(X)
    double sumY = 0;        //Sum(Y)
    double sumXX = 0;       //Sum(X^2)
    double slope = 0;
    double intercept = 0;
 
    //get input data file name
    cout << "Input data file name (name.txt): ";
	cin >> file_name;

    //get number of data samples
    input_file.open(file_name, ios::in);

    if( input_file.is_open() )
	{
		while( input_file.good() )
		{
            num_samples++;
            getline(input_file, line);

        }//end while

        input_file.close();
    }// end if
    else
	{
		cout << "File not found. Press enter to exit.";
		end_program();
		exit(0);
	}//end else

    samples = new sample[num_samples]; //initialize sample array

    //read in data
    input_file.open(file_name, ios::in);

    if(input_file.is_open())
    {
        for(num_in_array = 0; num_in_array < num_samples, input_file.good(); num_in_array++)
        {
            getline(input_file, line);

            samples[num_in_array].x = strtod(line.c_str(), &pEnd); //get x-value of sample
            samples[num_in_array].y = strtod(pEnd, &pEnd); //get y-value of sample
        }//end while

        input_file.close();

        if(num_in_array != num_samples)
        {
		    cout << "Error processing file. Press enter to exit.";
		    end_program();
		    exit(0);
        }//end if
	}//end if
    else
	{
		cout << "Error opening file. Press enter to exit.";
		end_program();
		exit(0);
	}//end else

    //compute slope - (Sum(XY) - (1/num_samples) * Sum(X) * Sum(Y)) / (Sum(X^2) - (1/num_samples) * Sum(X)^2)
    for(int i = 0; i < num_samples; i++) //calculate Sum(X), Sum(Y), Sum(XY), Sum(XX)
    {
        sumX += samples[i].x;
        sumY += samples[i].y;
        sumXY += samples[i].x * samples[i].y;
        sumXX += samples[i].x * samples[i].x;
    }//end for

    slope = (sumXY - (sumX * sumY) / num_samples) / (sumXX - (sumX * sumX) / num_samples);

    //compute intercept - (Sum(Y) - slope * Sum(X)) / num_samples
    intercept = (sumY - slope * sumX) / num_samples;

    //output linear regression equation
    cout << "Linear Regression Equation: " << "y = " << intercept << " + " << slope << "x";

    end_program();
    return 0;
}//end main

void end_program()
{
	cin.ignore();
	cin.get();
}//end end_program