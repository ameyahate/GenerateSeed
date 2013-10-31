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

	const int nCount = 100000;
	const int p05 = nCount/20;
	const int p10 = nCount/10;
	const int p20 = nCount/5;

	const int seed05 = p05/5;
	const int seed10 = p10/5;
	const int seed20 = p20/5;

	char * edgefile = "edges.txt";			//edgelist file
	char * nodefile = "nodes.txt";			//node file

	double nodeTh = 0.4;             		 //Upper threshold
	char * thNo = "0";

	const double sourceVal = 0.95;			//Source information value
	const double sourceTrust = 0.90; 	    //Source trust
	int u,v;

	if(argc==5)
	{
		nodefile = argv[1];
		edgefile = argv[2];
		nodeTh = atof(argv[3]);
		thNo = argv[4];
	}
	else
	{
		cout << "Usage:\n";
		cout << "  ./main argv[1]... argv[6]" << endl;
		cout << "argv[1] = node file" << endl;
		cout << "argv[2] = edge file " << endl;
		cout << "argv[3] = threshold " << endl;
		cout << "argv[4] = threshold number" << endl;
		exit(1);
	}

	const int nSources [5] = {nCount+1, nCount+2, nCount+3, nCount+4, nCount+5};

	// Preparing the random seed list
	srand((unsigned)time(0));

	//To Calculate running time.
	time_t t1,t2;
	t1 = time(NULL);  //get time at the beginning of process



	//Selecting 5% seeds from rSeed10
	vector<int> rSeeds05;
	int im = 0;
	int in;

	for (in = 0; in < nCount && im < p05; ++in)
	{
	  int rn = nCount - in;
	  int rm = p05 - im;
	  if (rand() % rn < rm)
	  { /* Take it */
	    rSeeds05.push_back( in + 1);
	    ++im;
	  }
	}

	assert(im == p05);
	random_shuffle(rSeeds05.begin(),rSeeds05.end());

	//	cout<< "Test1" << endl;

//	vector<int>::iterator pos;
//	for(pos = rSeeds05.begin(); pos!= rSeeds05.end(); ++pos)
//	{
//		cout<< *pos <<"\t";
//	}
//	cout << endl;
	if(atoi(thNo) == 0)
	{
		ofstream randSeed05("./random.txt");
		if(!randSeed05)
		{
			cerr<< "Cannot open ./random.txt"<< endl;
			exit(1);
		}

		for(int i = 0; i < p05; i++)
			randSeed05 << nSources[(int) i/seed05] << '\t' << rSeeds05[i] << endl;

		randSeed05.close();
	}

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
		int group;
		if(v < 26)
			group = 1;
		else
			group = 2;
		SimpleNode n( u, group, nodeTh);
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
	if(atoi(thNo) ==0)
	{
		multimap<double, int> NodeDeg;			// Storing <degree, nodeId>
		multimap<double, int>::iterator DegItr;

		vector<SimpleNode>::iterator NodeItr;
		NodeItr = Nodes.begin();

		for(int counter = 0; counter < p05; ++NodeItr, ++counter)
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
		ofstream highDegreeSeed05("./highDegree.txt");
		if(!highDegreeSeed05)
		{
			cerr<< "Cannot open ./highDegree.txt"<< endl;
			exit(1);
		}

		int counter = 0;

	//	for(DegItr = NodeDeg.begin(); DegItr != NodeDeg.end(); ++DegItr)
	//		cout<< DegItr->first << "=>" << DegItr->second << endl;

		for(DegItr = NodeDeg.begin(); DegItr != NodeDeg.end(); ++counter, ++DegItr)
		{
			highDegreeSeed05 << nSources[counter%5] << '\t' << DegItr->second << endl;
		}

		highDegreeSeed05.close();

		assert(DegItr == NodeDeg.end());
//	cout << "High degree written count:"<< counter << endl;
//	cout << "Size of high degree multimap:" <<NodeDeg.size()<<endl;
	}



//	cout << "Calling CalcSeed..."<< endl;
	CalcSeed(thNo, sourceTrust, sourceVal, Nodes);


	t2 = time(NULL); //get time at the end of process
	printf("[t:%f] Time executed = %d min %d sec\n", Nodes.at(0).getNlb(), (int)(t2-t1)/60, (int)(t2-t1)%60);
	return 0;
}
