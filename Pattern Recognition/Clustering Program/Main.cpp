#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

struct sample
{
	float features[3];
};

struct cluster
{
    int num_samples;
	float center[3];
	sample* samples;
};

const int MAX_CLUSTERS = 20;
const int CHAR_OFFSET = 48; //used to convert from ASCII value for a digit 
							//to the actual value of the digit

//function prototypes
void calc_intra_centers(cluster* clstrs, int num_clstrs);
void calc_inter_center(cluster* clstrs, int num_clstrs, float center[3]);
float calc_W(int num_clstr, cluster* clstrs);
float calc_B(int num_clstr, cluster* clstrs, float inter_center[3]);
float calc_CH(float w, float b, int num_clstr, int num_smpls);
float calc_norm(float dis[3]);
void end_program();

int main()
{
	string file_name = "", line = "";
	ifstream input_file;
	int which_cluster = 0, num_samples_total = 0, num_clusters = 0;
	cluster* clusters = new cluster[MAX_CLUSTERS];
	float* intercluster_center = new float[3];
	float within_scatter, between_scatter, ch_index;
	char *pEnd1, *pEnd2;

	//initialize clusters: num_samples and center
	for(int i = 0; i < MAX_CLUSTERS; i++)
	{
		clusters[i].num_samples = 0;
		clusters[i].center[0] = 0;
		clusters[i].center[1] = 0;
		clusters[i].center[2] = 0;
	}//end for

	//get data file name
	cout << "Input data file name (name.txt): ";
	cin >> file_name;

	//read through data to find num_samples_total, num_clusters and number of samples per cluster
	input_file.open(file_name, ios::in);

	if( input_file.is_open() )
	{
		while( input_file.good() )
		{
			num_samples_total++; //find num_samples_total

			getline(input_file, line);
			which_cluster = line.at(line.length() - 1) - CHAR_OFFSET;
			
			if( which_cluster + 1 > num_clusters ) //adjust number of clusters
				num_clusters = which_cluster + 1;
			clusters[which_cluster].num_samples++;  //increment # samples in cluster
		}//end while
		
		input_file.close();
	}//end if
	else
	{
		cout << "File not found. Press enter to exit.";
		end_program();
		exit(0);
	}//end else

	//initialize clusters: samples
	for(int i = 0; i < num_clusters; i++)
	{
		clusters[i].samples = new sample[clusters[i].num_samples];
		
		for(int j = 0; j < clusters[i].num_samples; j++) //initialize sample array in each cluster
		{
			clusters[i].samples[j].features[0] = 0;
			clusters[i].samples[j].features[1] = 0;
			clusters[i].samples[j].features[2] = 0;
		}//end for

		clusters[i].num_samples = 0; // resetting to 0 allows for this value to be used as a counter
	}//end for


	//read in data and store each sample in clusters under its associated cluster
	input_file.open(file_name, ios::in);

	if( input_file.is_open() )
	{
		while( input_file.good() )
		{
			getline(input_file, line);
			which_cluster = line.at(line.length() - 1) - CHAR_OFFSET; //get cluster number of the sample

			clusters[which_cluster].samples[clusters[which_cluster].num_samples].features[0] = strtod(line.c_str(), &pEnd1);
			clusters[which_cluster].samples[clusters[which_cluster].num_samples].features[1] = strtod(pEnd1, &pEnd2);
			clusters[which_cluster].samples[clusters[which_cluster].num_samples].features[2] = strtod(pEnd2, &pEnd1);

			clusters[which_cluster].num_samples++;
		}//end while
		
		input_file.close();
	}//end if

	//calculate intra cluster centers
	calc_intra_centers(clusters, num_clusters);

	//calculate inter cluster center
	calc_inter_center(clusters, num_clusters, intercluster_center);

	//calculate within cluster scatter
	within_scatter = calc_W(num_clusters, clusters);
	
	//calculate between cluster scatter
	between_scatter = calc_B(num_clusters, clusters, intercluster_center);

	//calculate Calinski-Harabasz (CH) index
	ch_index = calc_CH(within_scatter, between_scatter, num_clusters, num_samples_total);

	//display CH index
	cout << "Calinski-Harabasz Index: ";
	if( num_clusters == 1 )
		cout << "Undefined for one cluster";
	else cout << setprecision(10) << ch_index;

	//memory cleanup
	delete[] clusters;
	delete[] intercluster_center;

	end_program();
}//end main

void calc_intra_centers(cluster* clstrs, int num_clstrs)
{
	float center[3];

	for(int i = 0; i < num_clstrs; i++) //for each cluster
	{
		center[0] = 0; center[1] = 0; center[2] = 0;

		for(int j = 0; j < clstrs[i].num_samples; j++) //for each sample in the cluster
		{
			center[0] += clstrs[i].samples[j].features[0]; //sum all samples in the cluster
			center[1] += clstrs[i].samples[j].features[1]; 
			center[2] += clstrs[i].samples[j].features[2]; 
		}//end for
		
		clstrs[i].center[0] = center[0] / clstrs[i].num_samples; //divide by number samples in the cluster
		clstrs[i].center[1] = center[1] / clstrs[i].num_samples; //assign cluster center
		clstrs[i].center[2] = center[2] / clstrs[i].num_samples;
	}//end for

}//end calc_intra_centers

void calc_inter_center(cluster* clstrs, int num_clstrs, float center[3])
{
	center[0] = 0; center[1] = 0; center[2] = 0; 

	for(int i = 0; i < num_clstrs; i++) //for each cluster
	{
		center[0] += clstrs[i].center[0]; //sum centers of each cluster
		center[1] += clstrs[i].center[1];
		center[2] += clstrs[i].center[2];
	}//end for

	center[0] = center[0] / num_clstrs; //divide by number of clusters
	center[1] = center[1] / num_clstrs; 
	center[2] = center[2] / num_clstrs;

}//end calc_inter_center

float calc_W(int num_clstr, cluster* clstrs)
{
	float w = 0, d = 0;
	float dist[3];

	for(int i = 0; i < num_clstr; i++)//for each cluster
	{
		for(int j = 0; j < clstrs[i].num_samples; j++)//for each sample in the cluster
		{
			dist[0] = clstrs[i].samples[j].features[0] - clstrs[i].center[0]; //distance from sample to cluster center
			dist[1] = clstrs[i].samples[j].features[1] - clstrs[i].center[1];
			dist[2] = clstrs[i].samples[j].features[2] - clstrs[i].center[2];
			d = calc_norm(dist); //calculate norm of dist
			w += d*d; //add norm squared to sum
		}//end for
	}//end for

	return w;
}//end calc_W

float calc_B(int num_clstr, cluster* clstrs, float inter_center[3])
{
	float b = 0, d = 0;
	float dist[3];

	for(int i = 0; i < num_clstr; i++)
	{
		dist[0] = clstrs[i].center[0] - inter_center[0]; //distance from cluster center to inter-cluster center
		dist[1] = clstrs[i].center[1] - inter_center[1];
		dist[2] = clstrs[i].center[2] - inter_center[2];
		d = calc_norm(dist); //calculate norm of dist
		b += d*d*clstrs[i].num_samples; // norm squared times number of samples in the cluster
	}//end for

	return b;
}//end calc_B

float calc_CH(float w, float b, int num_clstr, int num_smpls)
{
	float ch = 0;

	ch = b * (num_smpls - num_clstr);
	ch = ch / (w * (num_clstr - 1));

	return ch;
}//end calc_CH

float calc_norm(float dis[3])
{
	float n = 0;

	for(int i = 0; i < 3; i++)
		n += (dis[i] * dis[i]); //sum of each component squared

	n = sqrt(n);

	return n;
}//end calc_norm

void end_program()
{
	cin.ignore();
	cin.get();
}//end end_program