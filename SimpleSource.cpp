/*
 * SimpleSource.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: ameyahate
 */

#include "SimpleSource.h"

SimpleSource::SimpleSource() {
	// TODO Auto-generated constructor stub

}

SimpleSource::SimpleSource(int id, int seed, double val): nid(id), nseed(seed), nval(val){
	nseedLeft = nseed;
}

void SimpleSource::addNType(NodeType nodeType)
{
	nodeTrust.push_back(nodeType);
}

double SimpleSource::getTrust(int nType){
	std::vector<NodeType>::iterator pos;
	for(pos = nodeTrust.begin(); pos !=nodeTrust.end(); ++pos)
	{
		if(pos->first == nType){return pos->second;}
	}
	std::cerr<<"Error:- Source:" <<nid <<" Node Type:"<<nType <<" not found!" <<std::endl;
	exit(1);
}


void SimpleSource::printSource()
{
	std::cout<<"Source Id:"<<nid <<", No. of Seeds:"<< nseed << ", Info Value:"<< nval <<std::endl;
	std::vector<NodeType>::iterator pos;
	for(pos = nodeTrust.begin(); pos !=nodeTrust.end(); ++pos)
	{
		std::cout<<"(Node Type:"<< pos->first <<" Trust: "<< pos->second << ")";
	}
	std::cout<<std::endl;
}

int SimpleSource::getNid() const
{
    return nid;
}

int SimpleSource::getNseed() const
{
    return nseed;
}

double SimpleSource::getNval() const
{
    return nval;
}

int SimpleSource::getNseedLeft() const
{
	return nseedLeft;
}

SimpleSource::~SimpleSource() {
	// TODO Auto-generated destructor stub
}
