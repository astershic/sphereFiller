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
#include <cmath>

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
private:
	double x;
	double y;
	double z;
};


class SphereFiller {
public:
    SphereFiller ();
    ~SphereFiller (); 

	std::string inFile;
	double minDist;
	int nSphere;

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


class Sphere {
public:
    Sphere ();
    ~Sphere (); 

    Sphere (Vec3d invec, double inrad) {
		centroid = invec;
		radius = inrad;
	};

	void setRadius(double inrad) {radius = inrad;};
	void setCentroid(Vec3d invec) {centroid = invec;};
	double getRadius() {return radius;};
	Vec3d getCentroid() {return centroid;};

	std::string print() {
		string out = centroid.print() + " , ";// + radius;
		return out;
	};


private:

	double radius;
	Vec3d centroid;


};


#endif//__SPHEREFILLER_H__
