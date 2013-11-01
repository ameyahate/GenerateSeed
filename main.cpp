/*
 * main.cpp
 *
 *  Created on: Jul 17, 2011
 *      Author: ameyahate
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <utility>			//for pair<>
#include <assert.h>
#include <map>
#include "SimpleSource.h"
#include "SimpleNode.h"
#include "CalcSeed.h"

using namespace std;

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

// main use: ./main {node_file} {edge_file} {threshold} {average trust} {networkScenario}  {trust diff}

int main(int argc, char *argv[])
{

	const int nCount = 1134890;
    const unsigned int selections = nCount/2;

	string edgefile = "edges.txt";			//edgelist file
	string nodefile = "nodes.txt";			//node file

	double nodeTh = 0.4;             		 //Upper threshold

	const double sourceVal = 0.95;			//Source information value
	const double sourceTrust = 0.90; 	    //Source trust
	int u,v;

	if(argc==4)
	{
		nodefile = argv[1];
		edgefile = argv[2];
		nodeTh = atof(argv[3]);
	}
	else
	{
		cout << "Usage:\n";
		cout << "  ./main argv[1] argv[2] argv[3]" << endl;
		cout << "argv[1] = node file" << endl;
		cout << "argv[2] = edge file " << endl;
		cout << "argv[3] = threshold " << endl;
		exit(1);
	}

	// Preparing the random seed list
	srand((unsigned)time(0));

	//To Calculate running time.
	time_t t1,t2;
	t1 = time(NULL);  //get time at the beginning of process

	//Selecting 50% seeds from rSeed10
	vector<int> rSeeds50;
	int im = 0;
	int in;

	for (in = 0; in < nCount && im < selections; ++in)
	{
	  int rn = nCount - in;
	  int rm = selections - im;
	  if (rand() % rn < rm)
	  { /* Take it */
	    rSeeds50.push_back( in + 1);
	    ++im;
	  }
	}

	assert(im == selections);
	random_shuffle(rSeeds50.begin(),rSeeds50.end());

	ofstream randSeed50("./random.txt");
	if(!randSeed50)
	{
		cerr<< "Cannot open ./random.txt"<< endl;
		exit(1);
	}

	for(int i = 0; i < selections; i++)
		randSeed50 << rSeeds50[i] << endl;

	randSeed50.close();

	// Generating high degree seeds
	// Reading nodes and building up node list.

	vector<SimpleNode> Nodes;

//	cout << "Reading node file" << endl;
	ifstream input1(nodefile);
	if(!input1) {
		cerr << "Error opening " << nodefile << endl;
		exit(1);
	}
	input1 >> u >> v;
	while(!input1.eof())
	{
		SimpleNode n( u, v, nodeTh);
		Nodes.push_back(n);
		input1 >> u >> v;
	}
	input1.close();

//	cout << "Reading edge file" << endl;

	ifstream input2(edgefile);
	double w;

	if(!input2) {
		cerr << "Error opening "<< edgefile  << endl;
		exit(1);
	}
	input2 >> u >> v >> w;
	while(!input2.eof())
	{
		Nodes.at(u-1).neighborList.push_back(make_pair(v,w));
		Nodes.at(u-1).addDegree(w);
		Nodes.at(v-1).neighborList.push_back(make_pair(u,w));
		Nodes.at(v-1).addDegree(w);
		input2 >> u >> v >> w;
	}
	input2.close();

//	cout << "Files read" << endl;
	multimap<double, int> NodeDeg;			// Storing <degree, nodeId>
	multimap<double, int>::iterator DegItr;
	multimap<double, int>::reverse_iterator RevDegItr;

	vector<SimpleNode>::iterator NodeItr;
	NodeItr = Nodes.begin();

	for(int counter = 0; counter < selections; ++NodeItr, ++counter)
		NodeDeg.insert(make_pair(NodeItr->getDegree(), NodeItr->getNid()));

	DegItr = NodeDeg.begin();
	for(;NodeItr != Nodes.end(); ++NodeItr)
	{
		if(NodeItr->getDegree() > DegItr->first)
		{
			NodeDeg.erase(DegItr);
			NodeDeg.insert(make_pair(NodeItr->getDegree(), NodeItr->getNid()));
			DegItr = NodeDeg.begin();
		}
	}
	
	// Writing the seed files for highest degree
	ofstream highDegreeSeed50("./highDegree.txt");
	if(!highDegreeSeed50)
	{
		cerr<< "Cannot open ./highDegree.txt"<< endl;
		exit(1);
	}
	for(RevDegItr = NodeDeg.rbegin(); RevDegItr != NodeDeg.rend(); ++RevDegItr)
	{
		highDegreeSeed50 << RevDegItr->second << endl;
	}
	highDegreeSeed50.close();

//	cout << "Calling CalcSeed..."<< endl;
	CalcSeed(sourceTrust, sourceVal, Nodes);

	t2 = time(NULL); //get time at the end of process
	printf("[t:%f] Time executed = %d min %d sec\n", Nodes.at(0).getNlb(), (int)(t2-t1)/60, (int)(t2-t1)%60);
	return 0;
}
