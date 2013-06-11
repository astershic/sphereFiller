/*******************************************************************************

  Class <sphereFiller> 

  Author: Andrew Stershic
          Duke Computational Mechanics Lab (DCML)
          Duke University - Durham, NC (USA)
  E-mail: ajs84@duke.edu

  Copyright (c) 2013 Andrew Stershic. All rights reserved. No warranty. No
  liability.

*******************************************************************************/
#include <vector>
#include <string>
#include <algorithm>

#ifndef __SPHEREFILLER_H__
#define __SPHEREFILLER_H__

using namespace::std;

class Node;
class Facet;
class Mesh;

class Vec3d {
public:
    Vec3d ();
    Vec3d (double inx, double iny, double inz) {
		set(inx,iny,inz);
	};
	void setX(double inx) {
		x = inx;
	}
	void setY(double iny) {
		y = iny;
	}
	void setZ(double inz) {
		z = inz;
	}
	void set(double inx, double iny, double inz) {
		x = inx;
		y = iny;
		z = inz;
	}
	std::string print() {
		std::string out = "( ";
		out += x;
		out += ", ";
		out += y;
		out += ", ";
		out += z;
		out += ")";
		return out;
	}
    ~Vec3d (); 

	double x;
	double y;
	double z;
};


class SphereFiller {
public:
    SphereFiller ();
    ~SphereFiller (); 

	std::string _inFile;
	Mesh * mesh;

private:

};

class Node {
public:
    Node ();
    Node (Vec3d in) {
		coordinates = in;
	};
    ~Node (); 
	

	vector<Facet*> getFacets() {return facets;};
	void addFacet (Facet * in) {facets.push_back(in);};
	Vec3d getCoordinates () {return coordinates;};
	void setCoordinates (Vec3d in) {coordinates = in;};

private:
	vector<Facet*> facets;
	Vec3d coordinates;

};

class Facet {
public:
    Facet ();
    ~Facet (); 


private:
	Vec3d	normal;
	vector<Node*> nodes;
};

class Mesh {
public:
    Mesh ();
    ~Mesh (); 

	void GenerateNormal(Node node);


private:

};

#endif//__SPHEREFILLER_H__
