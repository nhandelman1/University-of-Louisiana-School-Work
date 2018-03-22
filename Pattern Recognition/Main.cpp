#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;


//function prototypes
void end_program();

int main()
{
    int num_samples;
    int num_features;
    int min_val;
    int max_val;
    int num_decimals;
    string file_name;
    ofstream output_file;

    cout << "Enter the number of samples: ";
    cin >> num_samples;

    cout << "Enter the output file name: ";
    cin >> file_name;

    output_file.open(file_name);
    
    for(int i = 0; i < num_samples; i++)
    {
        output_file << rand() << " " << rand() << endl;
    }//end for i

    output_file.close();
    
    end_program();
    return 0;
}//end main

void end_program()
{
	cin.ignore();
	cin.get();
}//end end_program