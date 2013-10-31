/*
 * SimpleSource.h
 *
 *  Created on: Jun 23, 2011
 *      Author: ameyahate
 */

#ifndef SIMPLESOURCE_H_
#define SIMPLESOURCE_H_
#include <utility>
#include <vector>
#include <iostream>
#include <stdlib.h>

class SimpleSource {

	typedef std::pair<int,double> NodeType;

public:
	SimpleSource();
	SimpleSource(int id, int seed, double val);
	virtual ~SimpleSource();
    int getNid() const;
    int getNseed() const;
    double getNval() const;
    int getNseedLeft() const;

    void addNType(NodeType);
    double getTrust(int nType);
    void decSeedLeft(){
    	--nseedLeft;
    }
    void printSource();
private:
    int nid;
    int nseed;
    double nval;
    int nseedLeft;
    std::vector<NodeType> nodeTrust;
};

#endif /* SIMPLESOURCE_H_ */
