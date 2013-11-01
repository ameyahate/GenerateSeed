/*
 * CalcSeed.h
 *
 *  Created on: Jun 23, 2011
 *      Author: ameyahate
 */

#ifndef CALCSEED_H_
#define CALCSEED_H_


#include <utility>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <assert.h>
#include <time.h>
#include "SimpleSource.h"
#include "SimpleNode.h"



void CalcSeed(double sourceTrust, double info, std::vector<SimpleNode>&);
std::set<int> deltaCalc(int node, int nCount, double sourceTrust, double info, std::vector<SimpleNode>& Nodes);




#endif /* CALCSEED_H_ */
