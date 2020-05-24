#pragma once
#include "Point.h"
#include <vector>
#include "Boundary.h"
#include "GLM/glm.hpp"
// Quadtree class
class Quadtree
{
public:
	// Constructors
	Quadtree() = default;
	Quadtree(Boundary boundary) : boundary(boundary), divided(false), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr) {}


	// Methods
	// Insert new point to the Quadtree
	bool insert(Point point);

	// Get points using VP matrix
	std::vector<Point> queryRange(glm::mat4 VP);

	// Set boundary
	 void setBoundary(Boundary boundary);
	

	// TODO: make destructor
	~Quadtree();
private:
	// Subdivide node (now it becomes rectangle-node i.e. node which contains 0 points)
	void subdivide();

public:
	const int NODE_CAPACITY = 4;	// Node capacity
	std::vector<Point> points;		// Vector of points
	Boundary boundary;				// Node boundary
	bool divided;					// Is node divided
	Quadtree* nw;					// North-west
	Quadtree* ne;					// North-east
	Quadtree* sw;					// South-west
	Quadtree* se;					// South-east
};