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
#include <vector>
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

	void set(double inx, double iny, double inz) {
		x = inx;
		y = iny;
		z = inz;
	}
	std::string print() const {
		std::stringstream sstm;
		sstm << "( " << x << ", " << y << ", " << z << ")";
		return sstm.str();
	}

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
	double minDist;
	int nSphere;
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
    Node (int inID){
		id = inID;
	};
    Node (int inID, Vec3d in) {
		coordinates = in;
		id = inID;
	};
    Node (int inID, double x, double y, double z) {
		coordinates = Vec3d(x,y,z);
		id = inID;
	};

    ~Node (){}; 
	
	set<Node*> neighbors;

	vector<Facet*> getFacets() {return facets;};
	void addFacet (Facet * in) {facets.push_back(in);};
	Vec3d getCoordinates () const {return coordinates;};
	void setCoordinates (Vec3d in) {coordinates = in;};

	int getID() {return id;}
	void setID (int inID) {
		id = inID;
	}

	string print() const {
		std::stringstream sstm;
		sstm << id << ": " << coordinates.print();
		return sstm.str();
	};

private:
	vector<Facet*> facets;
	Vec3d coordinates;
	int id;

};

class Facet {
public:
    Facet (){};
    ~Facet (){}; 

    Facet (int inID){
		id = inID;
	};

	Facet(Node* n1, Node* n2, Node* n3) {
		nodes.push_back(n1);
		nodes.push_back(n2);
		nodes.push_back(n3);
	};

	Facet(int inID, Node* n1, Node* n2, Node* n3) {
		nodes.push_back(n1);
		nodes.push_back(n2);
		nodes.push_back(n3);
		id = inID;
	};

	void addNode (Node* inNode) {
		nodes.push_back(inNode);
	}
	vector<Node*> getNodes() {
		return nodes;
	}
	Node* getNode(unsigned num) {
		assert( (num >= 0) && (num <= 2));
		assert( num < nodes.size() );
		return nodes[num];
	}

	int getID() {return id;}
	void setID (int inID) {
		id = inID;
	}

	string print() const {
		std::stringstream sstm;
		assert(nodes[0]);
		assert(nodes[1]);
		assert(nodes[2]);
		sstm << id << ":: " << nodes[0]->print() << " ; " <<  nodes[1]->print() << " ; " << nodes[2]->print();
		return sstm.str();
	};

	string printID() const {
		std::stringstream sstm;
		assert(nodes[0]);
		assert(nodes[1]);
		assert(nodes[2]);
		sstm << id << ":: " << nodes[0]->getID() << " ; " <<  nodes[1]->getID() << " ; " << nodes[2]->getID();
		return sstm.str();
	};


private:
	Vec3d	normal;
	vector<Node*> nodes;
	int id;
};

class Mesh {
public:
    Mesh (){};
    Mesh (int in){tag = in;};
    ~Mesh (){}; 

	int tag;
	map<int, Node> noderoster;
	map<int, Facet> facetroster;

	void buildNodeGraph();
	void printNodeGraph();
//	void buildMeshes();
	void buildSpheres();
	void GenerateNormal(Node node);
	void removeConnected(set<Node*>& nodework, Node* node);

	void addNode (Node* inNode) {
		nodes.push_back(inNode);
	}
	vector<Node*> getNodes() {
		return nodes;
	}
	int nodeCount() {
		return nodes.size();
	}

//	vector<Facet*> getFacets() {return facets;};
//	void addFacet (Facet * in) {facets.push_back(in);};

private:
	vector<Node*> nodes;
//	vector<Facet*> facets;

};


class Sphere {
public:
    Sphere (){};
    ~Sphere (){}; 

    Sphere (Vec3d invec, double inrad) {
		centroid = invec;
		radius = inrad;
	};

	void setRadius(double inrad) {radius = inrad;};
	void setCentroid(Vec3d invec) {centroid = invec;};
	double getRadius() {return radius;};
	Vec3d getCentroid() {return centroid;};

	std::string print() {
		std::stringstream sstm;
		sstm << centroid.print() << " , " << radius << " , " << mass;
		return sstm.str();
	};

	bool containsPoint(Vec3d in) {
		Vec3d diff = centroid.minus(in);
		double dist = diff.norm();
		return (dist < radius);
	}
	bool containsPoint(double inx, double iny, double inz) {
		return containsPoint(Vec3d(inx,iny,inz));
	}


private:

	double radius;
	Vec3d centroid;
	double mass;

};


#endif//__SPHEREFILLER_H__
