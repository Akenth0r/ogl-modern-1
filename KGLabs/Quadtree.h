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

	// Get points from range
	//std::vector<Point> queryRange(Boundary range);

	// Set boundary
	 void setBoundary(Boundary boundary);
	
	 int outNodes(int depth)
	 {
		 printf("depth: %i points.size() %i boundary(%f, %f, %f, %f) divided: %i boundary: (%f %f %f %f)\n", depth, points.size(), boundary.pos.x, boundary.pos.y, boundary.pos.z, boundary.halfSize, divided, boundary.pos.x, boundary.pos.y, boundary.pos.z, boundary.halfSize);
		 if (divided)
		 {
			 depth = nw->outNodes(depth + 1);
			 depth = ne->outNodes(depth + 1);
			 depth = sw->outNodes(depth + 1);
			 depth = se->outNodes(depth + 1);
		 }


		 return depth;
	 }

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

