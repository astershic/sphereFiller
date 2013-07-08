/*******************************************************************************

  Class <sphereFiller> 

  Author: Andrew J. Stershic
          Duke Computational Mechanics Lab (DCML)
          Duke University - Durham, NC (USA)
  E-mail: ajs84@duke.edu
  Web:    www.duke.edu/~ajs84

  Produced at: Oak Ridge National Lab (ORNL) as part of DOE CSGF practicum

  Copyright (c) 2013 Andrew Stershic. All rights reserved. No warranty. No
  liability.

	This program is free software: you can redistribute it and/or modify it under 		the terms of the GNU General Public License as published by the Free Software 		Foundation, either version 3 of the License, or (at your option) any later 			version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY 	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with 		this program.  If not, see <http://www.gnu.org/licenses/>.

  *Please cite ALL use of code in academic works, presentations, and
  publications, an example template of which is given by:
  http://lrweb.beds.ac.uk/guides/ref/cite_computer_program

  *Please cite J. Ferellec, whose works underlie this code:
   Ferellec, J., and G. McDowell. "Modelling realistic shape and particle inertia in DEM." Géotechnique 60.3 (2010): 227-232.
   [doi: 10.1680/geot.9.T.015]
  

*******************************************************************************/#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <map>
#include <set>

#ifndef __SPHEREFILLER_H__
#define __SPHEREFILLER_H__

using namespace::std;

class Node;
class Facet;
class Mesh;
class Sphere;

class Vec3d {
public:
    Vec3d (){};
    ~Vec3d (){};

    Vec3d (double inx, double iny, double inz) {
		set(inx,iny,inz);
	};
	void setX(double inx) {	x = inx;};
	void setY(double iny) {	y = iny;};
	void setZ(double inz) {	z = inz;};

	double getX() {	return x;};
	double getY() {	return y;};
	double getZ() {	return z;};

	double norm() {return sqrt(x*x+y*y+z*z);};
	Vec3d plus(Vec3d in) {return Vec3d(x+in.x,y+in.y,z+in.z);};
	Vec3d minus(Vec3d in) {return Vec3d(x-in.x,y-in.y,z-in.z);};
	double dot(Vec3d in) {return (x*in.x + y*in.y + z*in.z);};
	Vec3d cross(Vec3d in) {
		Vec3d crossV = Vec3d(y*in.z - z*in.y, z*in.x - x*in.z, x*in.y-y*in.x);
		return crossV;
	};
	Vec3d mult(double mul) {
		return Vec3d(x*mul,y*mul,z*mul);
	};

	void set(double inx, double iny, double inz) {
		x = inx;
		y = iny;
		z = inz;
	};
	std::string print() {
		std::stringstream sstm;
		sstm << "( " << x << ", " << y << ", " << z << ")";
		return sstm.str();
	};

private:
	double x;
	double y;
	double z;
};


class SphereFiller {
public:
    SphereFiller (){};
    ~SphereFiller (){}; 

	std::string inFile;
	double density;
	double minDist;
	long nSphere;
	vector<Mesh> meshroster;

	void parseInputFile(bool load_all);

private:

};

class Node {
public:
    Node (){};
    Node (Vec3d in) {
		coordinates = in;
	};
    Node (double x, double y, double z) {
		coordinates = Vec3d(x,y,z);
	};
    Node (long inID){
		id = inID;
	};
    Node (long inID, Vec3d in) {
		coordinates = in;
		id = inID;
	};
    Node (long inID, double x, double y, double z) {
		coordinates = Vec3d(x,y,z);
		id = inID;
	};

    ~Node (){}; 
	
	set<Node*> neighbors;

	vector<Facet*> getFacets() {return facets;};
	void addFacet (Facet * in) {facets.push_back(in);};
	Vec3d getCoordinates () {return coordinates;};
	void setCoordinates (Vec3d in) {coordinates = in;};

	long getID() {return id;}
	void setID (long inID) {
		id = inID;
	};

	double dist(Node* other) {
		Vec3d c1 = getCoordinates();
		Vec3d c2 = other->getCoordinates();
		Vec3d diff = c1.minus(c2);
		return diff.norm();
	};

	string print() {
		std::stringstream sstm;
		sstm << id << ": " << coordinates.print();
		return sstm.str();
	};

private:
	vector<Facet*> facets;
	Vec3d coordinates;
	long id;

};

class Facet {
public:
    Facet (){};
    ~Facet (){}; 

    Facet (long inID){
		id = inID;
	};

	Facet(Node* n1, Node* n2, Node* n3) {
		nodes.push_back(n1);
		nodes.push_back(n2);
		nodes.push_back(n3);
	};

	Facet(long inID, Node* n1, Node* n2, Node* n3) {
		nodes.push_back(n1);
		nodes.push_back(n2);
		nodes.push_back(n3);
		id = inID;
	};

	Vec3d normal() {
		assert(nodes.size() == 3);
		Vec3d c1 = nodes[0]->getCoordinates();
		Vec3d c2 = nodes[1]->getCoordinates();
		Vec3d c3 = nodes[2]->getCoordinates();
		Vec3d v1 = c1.minus(c3);
		Vec3d v2 = c2.minus(c3);
		Vec3d x = v1.cross(v2);
		//normalize vector
		return x.mult(1.0/x.norm());
	};

	void addNode (Node* inNode) {
		nodes.push_back(inNode);
	};
	vector<Node*> getNodes() {
		return nodes;
	};
	Node* getNode(unsigned num) {
		assert( (num >= 0) && (num <= 2));
		assert( num < nodes.size() );
		return nodes[num];
	};

	long getID() {return id;}
	void setID (long inID) {
		id = inID;
	};

	string print() {
		std::stringstream sstm;
		assert(nodes[0]);
		assert(nodes[1]);
		assert(nodes[2]);
		sstm << id << ":: " << nodes[0]->print() << " ; " <<  nodes[1]->print() << " ; " << nodes[2]->print();
		return sstm.str();
	};

	string printID() {
		std::stringstream sstm;
		assert(nodes[0]);
		assert(nodes[1]);
		assert(nodes[2]);
		sstm << id << ":: " << nodes[0]->getID() << " ; " <<  nodes[1]->getID() << " ; " << nodes[2]->getID();
		return sstm.str();
	};

	Vec3d getCentroid() {
		assert(nodes.size() == 3);
		Vec3d c1 = nodes[0]->getCoordinates();
		Vec3d c2 = nodes[1]->getCoordinates();
		Vec3d c3 = nodes[2]->getCoordinates();
		Vec3d out = c1.plus(c2);
		out = out.plus(c3);
		out = out.mult(1.0/3.0);
		return out;
	}

	double getArea() {
		assert(nodes.size() == 3);
		Vec3d c1 = nodes[0]->getCoordinates();
		Vec3d c2 = nodes[1]->getCoordinates();
		Vec3d c3 = nodes[2]->getCoordinates();
		Vec3d v1 = c1.minus(c2);
		Vec3d v2 = c1.minus(c3);
		Vec3d x = v1.cross(v2);
		return 0.5*x.norm();
	}


private:
	vector<Node*> nodes;
	long id;
};

class Mesh {
public:
    Mesh (){};
    Mesh (int in){tag = in;};
    ~Mesh (){}; 

	int tag;
	map<long, Node*> noderoster;
	map<long, Facet*> facetroster;

	bool clearSphere(Sphere* sph);
	void bisectRadius(Sphere* sph, double rSmall, double rBig, int count);
	void buildNodeGraph();
	void printNodeGraph();
//	void buildMeshes();
	void buildSpheres(int particleNum, double density, int nSpheres, double minDist, string inFile);
	Vec3d meshCentroid() {
		if (&centroid) return centroid;
		centroid = Vec3d(0.0,0.0,0.0);
		for(map<long,Node*>::iterator it = noderoster.begin(); it != noderoster.end(); it++) {
			Node* node = it->second;
			centroid = centroid.plus(node->getCoordinates());
		}
		centroid = centroid.mult(1.0/static_cast<double>(noderoster.size()));
		return centroid;
	};
	Vec3d generateNormal(Node* node);
	void removeConnected(set<Node*>& nodework, Node* node);

	double calculateVolume();
/*
	void addNode (Node* inNode) {
		nodes.push_back(inNode);
	}
	vector<Node*> getNodes() {
		return nodes;
	}
	int nodeCount() {
		return nodes.size();
	}
*/
//	vector<Facet*> getFacets() {return facets;};
//	void addFacet (Facet * in) {facets.push_back(in);};

private:
//	vector<Node*> nodes;
////	vector<Facet*> facets;
	Vec3d centroid;
};


class Sphere {
public:
    Sphere (){};
    ~Sphere (){}; 

    Sphere (Vec3d invec, double inrad, double inmass) {
		centroid = invec;
		radius = inrad;
		mass = inmass;
	};

    Sphere (Node* n1, double inrad, double inmass) {
		centroid = n1->getCoordinates();
		radius = inrad;
		mass = inmass;
	};

    Sphere (Node* n1, double inrad, Vec3d inNormal, double inmass) {
		base = n1;
		normal = inNormal;
		centroid = base->getCoordinates();
		centroid = centroid.plus(normal.mult(inrad));
		radius = inrad;
		mass = inmass;
	};

	void setRadius(double inrad) {
		radius = inrad;
		centroid = base->getCoordinates();
		centroid = centroid.plus(normal.mult(inrad));	
	};
	void setCentroid(Vec3d invec) {centroid = invec;};
	double getRadius() {return radius;};
	Vec3d getCentroid() {return centroid;};
	Node* getBase() {return base;};

	std::string print() {
		calcDensity();
		std::stringstream sstm;
		double x = centroid.getX();
		double y = centroid.getY();
		double z = centroid.getZ();
		//print diameter, density, x, y, z
		sstm << radius*2.0 << " " << density << " " << x << " " << y << " " << z << endl;
		return sstm.str();
	};

	bool containsPoint(Vec3d in) {
		Vec3d diff = centroid.minus(in);
		double dist = diff.norm();
		return (dist < radius);
	};
	bool containsPoint(double inx, double iny, double inz) {
		return containsPoint(Vec3d(inx,iny,inz));
	};
	bool containsPoint(Node* node) {
		return containsPoint(node->getCoordinates());
	};



private:
	double radius;
	Vec3d centroid;
	double mass;
	double density;
	Node* base;
	Vec3d normal;

	void calcDensity() {
		double PI = 3.14159265359;
		double volume = PI*4.0/3.0*radius*radius*radius;
		density = mass/volume;
	}
};


#endif//__SPHEREFILLER_H__
