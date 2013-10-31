/*
 * CalcSeed.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: ameyahate
 */

#include "CalcSeed.h"
#include <queue>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define MIN(a, b) ((a < b) ? a : b)
#define INF 10.00
#define INFO(a) (-1.0*log(a)/log(2.0))
#define REVINFO(a) pow(2.0, -1.0*a)

typedef priority_queue <pair <double,int>, vector<pair<double, int> >, greater<pair<double, int> > > minheap;

set<int> deltaCalc(int node, double sourceTrust, double info, vector<SimpleNode>& Nodes)
{
	int count = 0;
	set<int> converted;

	//If source does not convert node or the node is already selected:
	//set deltaS to count and return converted.
	if( sourceTrust*info < Nodes.at(node -1).getNlb())
	{
		Nodes.at(node -1).deltaS = count;
		return converted;
	}

	// Clear status and information value.
	vector<SimpleNode>::iterator pos_n;
	for(pos_n=Nodes.begin(); pos_n!=Nodes.end(); ++pos_n)
	{
		pos_n->unSetValue();
		pos_n->setInfo(INF);
	}

	minheap minq;
	Nodes.at(node -1).setValue();
	Nodes.at(node -1).setInfo(INFO(info*sourceTrust));

	vector<pair<int, double> >::iterator nbr;
	for(nbr = Nodes.at(node - 1).neighborList.begin(); nbr!= Nodes.at(node - 1).neighborList.end(); ++nbr)
	{
		int nbrNode = nbr->first;
		double nbrTrust = nbr->second;
		if(!Nodes.at(nbrNode -1).getSelected())
			minq.push(make_pair(Nodes.at(node -1).getInfo() + INFO(nbrTrust),nbrNode));
	}


	while(!minq.empty())
	{
		double t_info = minq.top().first;
		int t_node = minq.top().second;
		minq.pop();
		Nodes.at(t_node -1).setValue();
		Nodes.at(t_node -1).setInfo(t_info);
		for(nbr = Nodes.at(t_node - 1).neighborList.begin(); nbr!= Nodes.at(t_node - 1).neighborList.end(); ++nbr)
		{
			int nbrNode = nbr->first;
			double nbrTrust = nbr->second;
			if(!Nodes.at(nbrNode -1).isValueSet() && !Nodes.at(nbrNode -1).getSelected())
			{
				double tempInfo = t_info + INFO(nbrTrust);
				if(REVINFO(tempInfo) >= Nodes.at(nbrNode -1).getNlb())
					minq.push(make_pair(tempInfo,nbrNode));
			}
		}
		while(!minq.empty() && Nodes.at(minq.top().second -1).isValueSet())
			minq.pop();
	}

	//Sanity check: check information of neighboring nodes.
//	for(nbr = Nodes.at(node - 1).neighborList.begin(); nbr!= Nodes.at(node - 1).neighborList.end(); ++nbr)
//		cout << "[Node: "<< nbr->first << ", Information: " <<Nodes.at(nbr->first -1).getInfo() <<" | " << REVINFO(Nodes.at(nbr->first -1).getInfo()) << "]";
//	cout << endl;


	for(pos_n=Nodes.begin(); pos_n != Nodes.end(); ++pos_n)
	{
		double finalInfo = REVINFO(pos_n->getInfo());
		if(!pos_n->getCovered() && finalInfo >= pos_n->getNlb())
		{
			converted.insert(pos_n->getNid());
			count++;
		}
	}
	//Assigning count to deltaS of node
	Nodes.at(node -1).deltaS = count;
	return converted;
}





void CalcSeed(char* thNo, double sourceTrust, double info, vector<SimpleNode> & Nodes)
{

	/* Print imported data */
	int nCount = 100000;
	vector<SimpleNode>::iterator pos_n, pos_n2;

//	for(pos_n = Nodes.begin(); pos_n !=Nodes.end(); ++pos_n)
//		pos_n->printNode();
//	cout<< "Node List Size:" << Nodes.size()<< endl;
//	exit(0);


	const int p05 = nCount/20;
	const int p10 = nCount/10;
	const int p20 = nCount/5;

	const int seed05 = p05/5;
	const int seed10 = p10/5;
	const int seed20 = p20/5;
//	const double e = 2.71828182845904523536;

	const int nSources [5] = {nCount+1, nCount+2, nCount+3, nCount+4, nCount+5};
	double t_run = Nodes.at(0).getNlb();
	double threshold = 2000+ (t_run-0.2)*8000/(0.35);

	/*******************************************************************************/
	//	Building the deltaS and containsV list (Using DFS)
	/*******************************************************************************/

//  Printing containsV

//	for(pos_n=Nodes.begin(); pos_n!=Nodes.end(); ++pos_n)
//	{
//		cout<< "Node " << pos_n->getNid() << ":";
//		map<int, int> ::iterator pos_d;
//		for(pos_d = pos_n->containsV.begin(); pos_d != pos_n->containsV.end(); ++pos_d)
//		{
//			cout<< "("<< pos_d->first << "," << pos_d->second <<") ";
//		}
//		cout<< endl;
//	}





	/*******************************************************************************/
	//	Making the list of selections
	/*******************************************************************************/

//	cout<<"Making seed selection..."<< endl;

	unsigned int selections = p05;
//	for(pos_s = Sources.begin(); pos_s != Sources.end(); ++pos_s)
//		selections += pos_s->second.getNseed();

	vector< pair<int,int> > selected;	//Vector of Selected Node.
	set<int>::iterator temp, setItr;

	//Building the priority queue of pair<N,I>: N = Nodes converted, I = Node Id
	priority_queue< pair<unsigned int,unsigned int> > que;
	priority_queue< pair<double, unsigned int> > degreeQ;

	set<int> maxSet;
	unsigned int maxNode;

//	for(pos_n = Nodes.begin(); pos_n != Nodes.end(); ++pos_n)
//	{
//		pos_n->uncover();
//		pos_n->unselect();
//	}
	bool deltaZero = false, switchop = true, listbuild = false, quebuild = false;
	if(t_run < 0.2)
	{
		threshold = 1000;
		switchop = false;
	}
	//Building PQ for choosing best node without using data-structure
	if(!switchop)
	for(pos_n = Nodes.begin(); pos_n != Nodes.end(); ++pos_n)
	{
		set<int> tempSet = deltaCalc(pos_n->getNid(),sourceTrust,info, Nodes);
		que.push(make_pair(pos_n->deltaS,pos_n->getNid()));
	}



	while(selected.size() != selections)
	{
		if(!deltaZero && !switchop)
		{
			bool found = false;
			while(!found)
			{
				pair<int,int> p = que.top();
				que.pop();
				maxNode = p.second;
				if(!Nodes.at(maxNode -1).getSelected())
				{
					maxSet = deltaCalc(Nodes.at(maxNode -1).getNid(),sourceTrust,info, Nodes);
					if(Nodes.at(maxNode -1).deltaS >= que.top().first)
						found = true;
					else
						que.push(make_pair(Nodes.at(maxNode -1).deltaS, maxNode));
				}
			}

			unsigned int maxDelta = Nodes.at(maxNode -1).deltaS;

			if (maxDelta == 0)
			{
				deltaZero = true;
				cout << "[t:"<< t_run<< "] DeltaZero 1, Selected Size:" << selected.size() << endl;
				continue;
			}

			if (maxDelta <= int(threshold))
			{
				switchop = true;
				cout << "[t:"<< t_run<< "] Switching Op, Selected Size:" << selected.size() << "\t Max Delta:" << maxDelta << endl;
			}

			selected.push_back(make_pair(maxDelta,maxNode));
			Nodes.at(maxNode -1).select();
			for(setItr = maxSet.begin(); setItr !=maxSet.end(); ++setItr)
			{
				Nodes.at(*setItr -1).cover();
			}
//			cout<<"Node selected: "<< maxNode << "| deltaS: " << maxDelta<< endl;
		}

		else if( !deltaZero && switchop)
		{
			//Build List for remaining nodes
			if(!listbuild)
			{
				for(pos_n=Nodes.begin(); pos_n!=Nodes.end(); ++pos_n)
				{
					if(!pos_n->getSelected())
					{
						int node = pos_n->getNid();
//						if(node%10000 == 0)
//							cout<< "deltaS for node: " << node << endl;
						set<int> tempSet = deltaCalc(node,sourceTrust,info, Nodes);
						for(setItr = tempSet.begin(); setItr != tempSet.end(); ++setItr)
								Nodes.at(*setItr -1).containsV.insert(node);
					}
				}
				listbuild = true;
			}
			//Finding the largest
			int maxNode;
			int maxDelta = 0;
			double maxDegree = 0.0;
			for(pos_n=Nodes.begin(); pos_n!=Nodes.end(); ++pos_n)
			{
				if(!pos_n->getSelected())
				{
					if(pos_n->deltaS > maxDelta)
					{
						maxDelta = pos_n->deltaS;
						maxNode = pos_n->getNid();
						maxDegree = pos_n->getDegree();
					}
					else if(pos_n->deltaS == maxDelta && pos_n->getDegree() > maxDegree)				//Choosing higher degree in case of a tie
					{
						maxNode = pos_n->getNid();
						maxDegree = pos_n->getDegree();
					}
				}
			}
			if (maxDelta == 0)
			{
				deltaZero = true;
				cout << "[t:"<< t_run<< "] DeltaZero 2, Selected Size:" << selected.size() << endl;
			}
			else
			{
				selected.push_back(make_pair(maxDelta,maxNode));
				Nodes.at(maxNode -1).select();

				// If deltaS > 0 then : Decrease deltaS values for all nodes that convert nodes converted by <maxNode>
				//Also cover() all nodes converted by <maxNode>
				int testcounter = 0;
				for(pos_n=Nodes.begin(); pos_n!=Nodes.end(); ++pos_n)
				{
					if(!pos_n->getCovered() && pos_n->containsV.find(maxNode) != pos_n->containsV.end())
					{
						testcounter++;
						//This node is converted
						for(temp = pos_n->containsV.begin(); temp != pos_n->containsV.end(); ++temp)
						{
							--(Nodes.at( (*temp) -1).deltaS);
						}
						pos_n->cover();
					}
				}
//				if(selected.size()%100 ==0)
//				{
//					cout<<"Node selected: "<< maxNode << "| deltaS: " << maxDelta<< endl;
//					cout<<"testcounter value :" << testcounter<<endl;
//				}
			}	
		}

//	We dont care about the different sources right now
//		Sources[maxSource].decSeedLeft();
//		if(Sources[maxSource].getNseedLeft() == 0)
//		{
//			for(pos_n=Nodes.begin(); pos_n!=Nodes.end(); ++pos_n)
//				for(temp = pos_n->deltaS.begin(); temp != pos_n->deltaS.end(); ++temp)
//					if(temp->first == maxSource)
//					{
//						pos_n->deltaS.erase(temp);
//						break;
//					}
//		}

//	Now MaxDelta is zero so we chose rest of the seeds according to highest degree criteria 
		else
		{	
			if(!quebuild)
			{
				for(pos_n=Nodes.begin(); pos_n!=Nodes.end(); ++pos_n)
				{
					if(!pos_n->getSelected())
						degreeQ.push(make_pair(pos_n->getDegree(),pos_n->getNid()));
				}
				quebuild = true;
			}
			if(degreeQ.empty())
				cout << "[t:"<< t_run<< "] Error max degree priority queue is empty!" << endl;

			pair<double, unsigned int> maxNode = degreeQ.top();
			degreeQ.pop();
			Nodes.at(maxNode.second -1).select();
			selected.push_back(make_pair(0,maxNode.second));

//			if(selected.size()%100 ==0)
//				cout<<"Node selected: "<< maxNode << "| Degree: " << maxDegree<< endl;
		}


//		if(selected.size()%1000 ==0)
//			cout << "Selections made: " << selected.size() << endl;

	}


	//Print Result
	//Sanity check for Source seeds left: should be all zero
//	for(pos_s = Sources.begin(); pos_s != Sources.end(); ++pos_s)
//	{
//		cout<< "Source: "<< pos_s->first << ", Seed left: "<< pos_s->second.getNseedLeft()<< endl;
//	}

//	//The Selected list:
//	ofstream calcseed;
//	calcseed.open("calcseed.txt");
//
//	vector<pair<int,int> >::iterator sel;
//	for(sel = selected.begin(); sel != selected.end(); ++ sel)
//	{
//		calcseed<< sel->first<< '\t' <<sel->second <<endl;
////		cout<< "Source: "<< sel->first << " Node: " << sel->second<< endl;
//	}
//
//	calcseed.close();

	// Writing the seed files for greedy selection.

	char greedy[20];
	sprintf(greedy,"./greedy%s.txt",thNo);
	ofstream greedySeed05(greedy);
	if(!greedySeed05)
	{
		cerr<< "Cannot open "<<greedy << endl;
		exit(1);
	}


	vector<pair<int,int> >::iterator sel;
	int counter = 0;

	for(sel = selected.begin(); sel != selected.end(); ++ sel, ++ counter)
	{
		greedySeed05 << nSources[counter%5] << '\t' << sel->second << endl;
	}
	greedySeed05.close();

//	cout<<"Seeds calculated. Returning calcseed"<< endl;
//	return "calcseed.txt";
	return;
}
