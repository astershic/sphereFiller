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

	//load all then process all, or do one at a time?
	bool load_all = true;

	//parse input file, save nodes and facets
	sf.parseInputFile(load_all);

	//build Spheres
	if (load_all) {
		for (unsigned i = 0; i < sf.meshroster.size(); ++i) {
			sf.meshroster[i].buildSpheres();
		}
	}

	/*
	//build nodal connectivity graph
	sf.buildNodeGraph(); //	sf.printNodeGraph();

	// use graph theory to find individual particles
	sf.buildMeshes();
	*/

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

void Mesh::buildSpheres() {

}

void SphereFiller::parseInputFile (bool load_all)  {
	string path = this->inFile;
	ifstream infile(path.c_str());
	while (!infile.eof()) {
		Mesh mesh = Mesh();

		mesh.noderoster.clear();
		mesh.facetroster.clear();
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
			if (line.substr(0,6) == "*Elset" || line.substr(0,5) == "*Nset") {
				break;
			}
		
			if (node && !element) {
				//make node
				vector<string> split = strSplit(line);
				int tag = atoi(split[0].c_str());
				double x = atof(split[1].c_str());			
				double y = atof(split[2].c_str());
				double z = atof(split[3].c_str());
				Node node = Node(tag,x,y,z);
				mesh.noderoster.insert(pair<int,Node> (tag,node));
			}

			if (element) {
				//make element
				vector<string> split = strSplit(line);
				
				//check for triangle element - 3 nodes
				if (split.size() != 4) break;
				int tag = atoi(split[0].c_str()); 
				int t1 = atoi(split[1].c_str()); Node* n1 = &mesh.noderoster[t1];
				int t2 = atoi(split[2].c_str()); Node* n2 = &mesh.noderoster[t2];
				int t3 = atoi(split[3].c_str()); Node* n3 = &mesh.noderoster[t3];
				Facet facet = Facet(tag, n1, n2, n3);
				mesh.facetroster.insert(pair<int,Facet> (tag,facet));
				n1->addFacet(&facet);
				n2->addFacet(&facet);
				n3->addFacet(&facet);
			}
		}
	
		//if mesh is not empty, save or process it
		if (mesh.noderoster.size() > 0 && mesh.facetroster.size() > 0) {
			if (load_all) {
				//save
				meshroster.push_back(mesh);
			} else {
				//process
				mesh.buildSpheres();
			}
		}
				
	}

	cout << "*INPUT FILE PARSED" << endl;
	cout << "    mesh roster size = " << meshroster.size() << endl;	
	for (unsigned i = 0; i < meshroster.size(); ++i) {
		cout << "    node roster size = " << meshroster[i].noderoster.size() << endl;
		cout << "    facet roster size = " << meshroster[i].facetroster.size() << endl;
	}

	return;
}

void Mesh::buildNodeGraph() {

	for(map<int,Facet>::iterator it = facetroster.begin(); it != facetroster.end(); it++) {
		Facet* facet = &it->second;
		Node* n1 = facet->getNode(0);
		Node* n2 = facet->getNode(1);
		Node* n3 = facet->getNode(2);
		n1->neighbors.insert(n2); n1->neighbors.insert(n3);
		n2->neighbors.insert(n1); n2->neighbors.insert(n3);
		n3->neighbors.insert(n1); n3->neighbors.insert(n2);
	}

	cout << "*NODE GRAPH BUILT" << endl;
	
	return;
}

void Mesh::printNodeGraph() {

	for(map<int,Node>::iterator it = noderoster.begin(); it != noderoster.end(); it++) {

		Node* node = &it->second;
		cout << endl;
		cout << "<" << node->getID() << ">" << endl;
		for (Node* n : node->neighbors){
			cout << n->getID() << endl;
		}
	}

}
/*
void SphereFiller::buildMeshes() {
	
	//copy map
	set<Node*> nodelist;	
    for( map<int, Node>::iterator it = noderoster.begin(); it != noderoster.end(); ++it ) {
    	nodelist.insert( &it->second );
    }

	for(Node* node : nodelist) {
		Mesh mesh = Mesh();
		mesh.removeConnected(nodelist, node);

		meshroster.push_back(mesh);

		if (nodelist.size() == 0) break;
	}

	cout << "*MESHES BUILT" << endl;
	cout << "    mesh roster size = " << meshroster.size() << endl;
	for (unsigned i = 0; i < meshroster.size() ; ++i) {
		cout << "    " << i << "    " << meshroster[i].nodeCount() << endl;
	}

	return;
}


void Mesh::removeConnected(set<Node*>& nodelist, Node* node) {
	//see if this node is already in this mesh. if so, stop.
	bool is_in = false;
	for (Node* n : nodes) {
		if (n->getID() == node->getID()) is_in = true;
	}	
	if (is_in)  	return;

//cout << "pass 1" << endl;
//cout << nodelist.size() << endl;

	//if already removed from main list, stop
	bool is_off = true;
	for (Node* n : nodelist) {
		if (n->getID() == node->getID()) is_off = false;
	}
	if (is_off)  	return;

//cout << "pass 2" << endl;

	//if not, add it, remove from overall list and perform recursive call on neighbors
	addNode(node);
	set<Node*> neighbors = node->neighbors;
	nodelist.erase(node);	

cout << node->getID() << endl;

	for(Node* n : neighbors) {
		removeConnected(nodelist, n);
	}
	
	return;	
}
*/

/*------------------------------ P R I V A T E -------------------------------*/

void Mesh::GenerateNormal(Node node) {
	

	return;
}


