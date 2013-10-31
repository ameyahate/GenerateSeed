/*
 * SimpleNode.h
 *
 *  Created on: Jun 23, 2011
 *      Author: ameyahate
 */

#ifndef SIMPLENODE_H_
#define SIMPLENODE_H_

#include <utility>
#include <vector>
#include <set>
#include <iostream>
#include <stdlib.h>

class SimpleNode {

	typedef std::pair<int, double> Neighbor;

public:
	SimpleNode();
	SimpleNode(int, int, double);
	virtual ~SimpleNode();
    double getInfo(){return info;}
    void setInfo(double info){this->info = info;}
    int getNid() const;
    double getNlb() const;
    int getNtype() const;
    bool getVisited() const;
    bool getSelected() const;
    void printNode();
    void select();
    void unselect();
    void cover(){covered = true;}
    void uncover(){covered = false;}
    bool getCovered() const{return covered;}
    double getDegree(){return degree;}
    void addDegree(double val){degree += val;}
    bool isValueSet(){return this->valueset;}
    void setValue(){this->valueset = true;}
    void unSetValue(){this->valueset = false;}
    std::vector<Neighbor> neighborList;

    int deltaS; 	// The number of nodes that will be converted by this node.
    std::set<int>  containsV; 	// List of nodes that will convert this node.

private:
	int nid;
	int ntype;
	double nlb;
	double degree;
	double info;
	bool selected;
	bool covered;
	bool valueset;
};

#endif /* SIMPLENODE_H_ */
