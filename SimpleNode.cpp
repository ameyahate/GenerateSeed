/*
 * SimpleNode.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: ameyahate
 */

#include "SimpleNode.h"

SimpleNode::SimpleNode() {
	// TODO Auto-generated constructor stub

}

SimpleNode::SimpleNode(int id, int type, double lb): nid(id), ntype(type), nlb(lb)
{
	valueset = false;
	selected = false;
	covered = false;
	degree = 0.0;
	info = 0.0;	
}

SimpleNode::~SimpleNode() {
	// TODO Auto-generated destructor stub
}

void SimpleNode::printNode()
{
	std::cout<<"Node Id:"<<nid <<", Node Type:"<< ntype << ", Node Threshold:"<< nlb;
	std::cout<<" Edge List:"<< neighborList.size();

	std::vector<Neighbor>::iterator pos_nl;
	for(pos_nl = neighborList.begin(); pos_nl != neighborList.end(); ++pos_nl)
	{
		Neighbor n = *(pos_nl);
		std::cout<<"(" <<n.first<< "," << n.second <<")";
	}
	std::cout << std::endl;
}


void SimpleNode::select()
{
	selected = true;
}

void SimpleNode::unselect()
{
	selected = false;
}

int SimpleNode::getNid() const
{
    return nid;
}

double SimpleNode::getNlb() const
{
    return nlb;
}

int SimpleNode::getNtype() const
{
    return ntype;
}

bool SimpleNode::getSelected() const
{
	return selected;
}

