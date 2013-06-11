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


	//TODO: load file into meshes (one per particle)
	Node n1 = Node(0.0,0.0,0.0);
	Node n2 = Node(1.0,2.0,0.0);
	Node n3 = Node(0.0,2.0,0.0);
	Node n4 = Node(1.0,0.0,0.0);
	Node n5 = Node(0.0,0.0,3.0);
	Node n6 = Node(1.0,2.0,3.0);
	Node n7 = Node(0.0,2.0,3.0);
	Node n8 = Node(1.0,0.0,3.0);
	

	//
	//	points to nodes
	//  faces to facets
	//  node/facet connectivity

	//pick random nodes
	//make spheres

	//write list to file
	

	return 1;
}

SphereFiller::SphereFiller ()  {}
SphereFiller::~SphereFiller ()  {}



/*------------------------------ P R I V A T E -------------------------------*/

void Mesh::GenerateNormal(Node node) {
	

	return;
}

