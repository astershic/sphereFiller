/*******************************************************************************

  Class <sphereFiller> 

  Author: Andrew Stershic
          Duke Computational Mechanics Lab (DCML)
          Duke University - Durham, NC (USA)
  E-mail: ajs84@duke.edu

  Produced at: Oak Ridge National Lab (ORNL) as part of DOE CSGF practicum

  Copyright (c) 2013 Andrew Stershic. All rights reserved. No warranty. No
  liability.

*******************************************************************************/
#include "sphereFiller.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>

using namespace std;

/*******************************************************************************

                M E T H O D S    F O R    R E S O L U T I O N 

*******************************************************************************/

/*------------------------------- P U B L I C --------------------------------*/


int main(int argc, const char *argv[]) {
	SphereFiller sf;
	if (argc > 0) {
		sf.inFile = argv[1];
		cout << " input file = " << sf.inFile << endl;
	}

	if (argc > 1) {
		sf.nSphere = atoi(argv[2]);
		cout << " number of spheres = " << sf.nSphere << endl;
	}

	if (argc > 2) {
		sf.minDist = atof(argv[3]);
		cout << " minimum distance = " << sf.minDist << endl;
	}

	sf.parseInputFile();

	//TODO: load file
/*	Node n1 = Node(0.0,0.0,0.0);
	Node n2 = Node(1.0,2.0,0.0);
	Node n3 = Node(0.0,2.0,0.0);
	Node n4 = Node(1.0,0.0,0.0);
	Node n5 = Node(0.0,0.0,3.0);
	Node n6 = Node(1.0,2.0,3.0);
	Node n7 = Node(0.0,2.0,3.0);
	Node n8 = Node(1.0,0.0,3.0);
*/

	//
	//	points to nodes
	//  faces to facets
	// find node/facet connectivity
	// use graph theory to find individual particles

	//pick random nodes
	//make spheres - iteratively blowing them up

	//write list to file
	

	return 1;
}

void printVector(vector<string> in) {
	for (unsigned i = 0; i < in.size(); ++i) {
		cout << in[i] + "|";
	}
	cout << endl;
}

string removeSpaces(string in) {
	string out;
	for (unsigned i = 0; i < in.length(); ++i) {
		if (in.substr(i,1) != " ") out += in.substr(i,1);
	}
	return out;
}

vector<string> strSplit (string in)  {
	vector<string> out;
	int index;
	while ((index=in.find(",")) >= 0) {
		string mini = in.substr(0,index);
		out.push_back(removeSpaces(mini));
		in = in.substr(index+1);
	}
	out.push_back(removeSpaces(in));
	return out;
}

void SphereFiller::parseInputFile ()  {
	string path = this->inFile;
	ifstream infile(path.c_str());

	map<int, Node*> noderoster;

	string line;
	bool node = false;
	bool element = false;
	while (getline(infile,line)) {
		if (line.substr(0,5) == "*Node") {
			node = true;
			continue;
		}
		if (line.substr(0,8) == "*Element") {
			element = true;
			continue;
		}
		if (line.substr(0,6) == "*Elset") {
			break;
		}
		
		if (node && !element) {
			//make node
			vector<string> split = strSplit(line);
//			cout << line << endl;		
			Node node = Node(atof(split[1].c_str()),atof(split[2].c_str()),atof(split[3].c_str()));
			noderoster.insert(pair<int,Node*> (atoi(split[0].c_str()),&node));
		}

		if (element) {
			//make element
		}
				
	}

	return;
}



/*------------------------------ P R I V A T E -------------------------------*/

void Mesh::GenerateNormal(Node node) {
	

	return;
}

