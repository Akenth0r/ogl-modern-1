#include "Quadtree.h"

bool Quadtree::insert(Point point)
{
	int count = 0;
	// If point is not in the bounding-box, go away from here
	if (!(count = boundary.countInside(point)))
		return false;
	// If node contains lessthan NODE_CAPACITY points
	// and not divided
	if (points.size() < NODE_CAPACITY && !divided)
	{
		// Append and go away
		points.push_back(point);
		return true;
	}

	// Otherwise subdivide it
	if (!divided)
		subdivide();

	Quadtree* tree = nullptr;
	int temp;
	if ( (temp = nw->boundary.countInside(point)) >= count)
	{
		count = temp;
		tree = nw;
	}
	if ((temp = ne->boundary.countInside(point)) >= count)
	{
		count = temp;
		tree = ne;
	}
	if ((temp = sw->boundary.countInside(point)) >= count)
	{
		count = temp;
		tree = sw;
	}
	if ((temp = se->boundary.countInside(point)) >= count)
	{
		tree = se;
	}
	if (tree)
	{
		tree->insert(point);
		return true;
	}
	points.push_back(point);
	return true;

}

std::vector<Point> Quadtree::queryRange(glm::mat4 VP)
{
	std::vector<Point> result, nwPoints, nePoints, swPoints, sePoints;

	if (!boundary.inFrustum(VP))
		return result;

	if (divided)
	{
		nwPoints = nw->queryRange(VP);
		nePoints = ne->queryRange(VP);
		swPoints = sw->queryRange(VP);
		sePoints = se->queryRange(VP);

		result.insert(result.end(), nwPoints.begin(), nwPoints.end());
		result.insert(result.end(), nePoints.begin(), nePoints.end());
		result.insert(result.end(), swPoints.begin(), swPoints.end());
		result.insert(result.end(), sePoints.begin(), sePoints.end());
	}
	
	result.insert(result.end(), points.begin(), points.end());

	return result;
	
}

void Quadtree::setBoundary(Boundary boundary)
{
	this->boundary = boundary;
}


Quadtree::~Quadtree()
{
	if (!divided)
		return;

	delete nw;
	delete ne;
	delete sw;
	delete se;
}

void Quadtree::subdivide()
{
	if (divided)
		return;

	float x  = boundary.pos.x;
	float y  = boundary.pos.y;
	float z  = boundary.pos.z;
	float hs = boundary.halfSize / 2;

	// North-west quadrant
	nw = new Quadtree(
		Boundary(glm::vec3(x - hs, y,  z + hs), hs)
	);

	// North-east quadrant
	ne = new Quadtree(
		Boundary(glm::vec3(x + hs, y, z + hs), hs)
	);

	// South-west quadrant
	sw = new Quadtree(
		Boundary(glm::vec3(x - hs, y, z - hs), hs)
	);

	// South-east qudrant
	se = new Quadtree(
		Boundary(glm::vec3(x + hs, y, z - hs), hs)
	);

		
	divided = true;
}