/*******************************************************************************

  Class <sphereFiller> 

  Author: Andrew Stershic
          Duke Computational Mechanics Lab (DCML)
          Duke University - Durham, NC (USA)
  E-mail: ajs84@duke.edu

  Produced at: Oak Ridge National Lab (ORNL) as part of DOE CSGF practicum

  Copyright (c) 2013 Andrew Stershic. All rights reserved. No warranty. No
  liability.

  *Please cite ALL use of code in academic works, presentations, and
  publications, an example template of which is given by:
  http://lrweb.beds.ac.uk/guides/ref/cite_computer_program

  *Please cite the J. Ferellec, whose works underlie this code:
   Ferellec, J. & McDowell, G. (2010). Geotechnique 60, No. 3, 227–232
   [doi: 10.1680/geot.9.T.015]
  

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
	if (argc > 1) {
		sf.inFile = argv[1];
		cout << " input file = " << sf.inFile << endl;
	}

	sf.nSphere = 1;
	if (argc > 2) {
		sf.nSphere = atoi(argv[2]);
		cout << " number of spheres = " << sf.nSphere << endl;
	}

	sf.minDist = 0.0;
	if (argc > 3) {
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
			sf.meshroster[i].buildSpheres(sf.nSphere, sf.minDist, sf.inFile);
		}
	}

	/*
	//build nodal connectivity graph
	sf.buildNodeGraph(); //	sf.printNodeGraph();

	// use graph theory to find individual particles
	sf.buildMeshes();
	*/
	return 1;
}

template<class T> void printVector(vector<T> in) {
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


template <class T> void deleteObjects (map<int,T*> a) {
	for(typename map<int,T*>::iterator it = a.begin(); it != a.end(); it++) {
		T* ptr = it->second;
		delete &ptr;
	}
	return;
}

void Mesh::buildSpheres(int nSphere, double minDist, string inFile) {

	vector<int> idList;
	vector<Node*> bases;
	vector<Sphere> sphereList;
	

	for (int i = 0; i < nSphere; ++i) {

		//pick random nodes
		map<int,Node*>::iterator item = noderoster.begin();
		Node* n1; int id;

		//check valid base node generation
		bool okay1 = false;
		bool okay2 = false;
		while (!okay1 || !okay2) {

			std::advance( item, rand() % noderoster.size() );
			n1 = item->second;
			id = item->first;

//			cout << "checking node " << okay1 << " " << okay2 << endl;
			//correct if node already used
			okay1 = std::find(idList.begin(), idList.end(), id)==idList.end();

			//check distance
			okay2 = true;
			for (unsigned j = 0; j < bases.size(); ++j) {
				if (n1->dist(bases[j]) < minDist) {okay2 = false; break;}
			}		
		}

		//max and min distance
		double max = 0.0;
		double min = std::numeric_limits<double>::max();
		for(map<int,Node*>::iterator it = noderoster.begin(); it != noderoster.end(); it++) {
			if (it->first == id) continue;
			Node* n = it->second;
			double dist = n1->dist(n);
			if (dist < min) {min = dist;}
			if (dist > max) {max = dist;}
		}

		//find normal direction
		Vec3d normal = generateNormal(n1);
		//make spheres - iteratively blowing them up
		Sphere sph1 = Sphere(n1, min, normal, 1);

		//get right size of sphere
		bisectRadius(&sph1,min*0.1*0.5,max*10.0*0.5,0);

//		cout << "radius = " << sph1.getRadius() << endl;
//		cout << "center = " << sph1.getCentroid().print() << endl;

		//save Sphere to lists
		sphereList.push_back(sph1);
		bases.push_back(sph1.getBase());
		idList.push_back(sph1.getBase()->getID());
	}

	//write spheres to file
	string outFile = inFile.substr(0,inFile.size()-3) + "out";
	ofstream myfile;
	myfile.open (outFile.c_str(), ios::app);
	for (unsigned i = 0; i < sphereList.size(); ++i) {
		myfile << sphereList[i].print() << endl;
	}
	myfile.close();

	cout << "*SPHERES BUILT" << endl;

}

void Mesh::bisectRadius(Sphere* sph, double rSmall, double rBig, int count) {
	if (count > 10) {
		return;	
	}

	double test = sqrt(rSmall*rBig);
	sph->setRadius(test);

	if (!clearSphere(sph)) {
		//if bisection is too big, make smaller
		bisectRadius(sph,rSmall,test,count+1);
	} else {
		//if bisection is works/too small, make bigger
		bisectRadius(sph,test,rBig,count+1);
	}
	return;
}

bool Mesh::clearSphere(Sphere* sph) {
	int count = 0;
	for(map<int,Node*>::iterator it = noderoster.begin(); it != noderoster.end(); it++) {
		Node* node = it->second;
		
		//don't count it if it's the base point
		if (node->getID() == sph->getBase()->getID()) continue;

		//count if a node is contained
		if (sph->containsPoint(node)) count++;
	}
	if (count > 0) return false;
	return true;
}

Vec3d Mesh::generateNormal(Node* n1) {
	vector<Facet*> facets = n1->getFacets();
	assert(facets.size() > 0);
	Vec3d normal = facets[0]->normal();
	
	for (unsigned i = 1; i < facets.size(); ++i) {
		Vec3d norm = facets[i]->normal();
		//reverse orientation if necessary
		Vec3d scaled = normal.mult(1.0/static_cast<double> (i));
		double dot = scaled.dot(norm);
		if (dot < 0) norm = norm.mult(-1.0);
		normal = normal.plus(norm); 
	}

	normal = normal.mult(1.0/static_cast<double> (facets.size()));

	//check orientation vs centroid
	Vec3d centroid = meshCentroid();
	Vec3d diff = centroid.minus(n1->getCoordinates());
	if (diff.dot(normal) < 0) normal = normal.mult(-1.0);	

	return normal;
}

void SphereFiller::parseInputFile (bool load_all)  {
	string path = this->inFile;
	ifstream infile(path.c_str());
	while (!infile.eof()) {
		Mesh mesh = Mesh();

		deleteObjects(mesh.noderoster);
		deleteObjects(mesh.facetroster);
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
				Node* node = new Node(tag,x,y,z);
				mesh.noderoster.insert(pair<int,Node*> (tag,node));
//				mesh.addNode(&node);
			}

			if (element) {
				//make element
				vector<string> split = strSplit(line);
				
				//check for triangle element - 3 nodes
				if (split.size() != 4) break;
				int tag = atoi(split[0].c_str()); 
				int t1 = atoi(split[1].c_str()); Node* n1 = mesh.noderoster[t1];
				int t2 = atoi(split[2].c_str()); Node* n2 = mesh.noderoster[t2];
				int t3 = atoi(split[3].c_str()); Node* n3 = mesh.noderoster[t3];
				Facet* facet = new Facet(tag, n1, n2, n3);
				mesh.facetroster.insert(pair<int,Facet*> (tag,facet));
				n1->addFacet(facet);
				n2->addFacet(facet);
				n3->addFacet(facet);
				assert(facet->getNodes().size() == 3);
			}
		}
	
		//if mesh is not empty, save or process it
		if (mesh.noderoster.size() > 0 && mesh.facetroster.size() > 0) {
			if (load_all) {
				//save
				meshroster.push_back(mesh);
			} else {
				//process
				mesh.buildSpheres(nSphere, minDist, inFile);
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

	for(map<int,Facet*>::iterator it = facetroster.begin(); it != facetroster.end(); it++) {
		Facet* facet = it->second;
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

	for(map<int,Node*>::iterator it = noderoster.begin(); it != noderoster.end(); it++) {

		Node* node = it->second;
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
		cout << "    " << i << "    " << meshroster[i]->nodeCount() << endl;
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

