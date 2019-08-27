#pragma once
#include "Headers.h"

class Triangle
{
private:
	glm::vec3 calculateNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{
		return glm::normalize(glm::cross(p2 - p1, p3 - p1));
	}

public:
	Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		pos = glm::vec3((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3, (p1.z + p2.z + p3.z) / 3);
		this->normal = calculateNormal(p1, p2, p3);
	}

	/** /
	inline Triangle operator+(const glm::vec3& offSet)
	{
		p1 += offSet;
		p2 += offSet;
		p3 += offSet;
		pos += offSet;
		return this;
	}*/

	Triangle& operator+(const glm::vec3& offSet)
	{
		p1 += offSet;
		p2 += offSet;
		p3 += offSet;
		pos += offSet;
		return *this;
	}

	//parameters
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
	glm::vec3 pos;
	glm::vec3 normal;
};

class Bounds
{
private:
	bool boundingBoxCheck(std::shared_ptr<Triangle> triangle)
	{
		double maxX = fmax(triangle->p1.x, fmax(triangle->p2.x, triangle->p3.x));
		double minX = fmin(triangle->p1.x, fmin(triangle->p2.x, triangle->p3.x));
		double maxY = fmax(triangle->p1.y, fmax(triangle->p2.y, triangle->p3.y));
		double minY = fmin(triangle->p1.y, fmin(triangle->p2.y, triangle->p3.y));
		double maxZ = fmax(triangle->p1.z, fmax(triangle->p2.z, triangle->p3.z));
		double minZ = fmin(triangle->p1.z, fmin(triangle->p2.z, triangle->p3.z));

		if (maxX >= low.x && minX <= high.x && maxY >= low.y && minY <= high.y && maxZ >= low.z && minZ <= high.z)
			return true;
		return false;
	}

	bool pointInRect(float px, float py, float lowX, float highX, float lowY, float highY)
	{
		return px >= lowX && px <= highX && py >= lowY && py <= highY;
	}

	bool lineIntersectsBoundaries(glm::vec3 p1, glm::vec3 p2)
	{
		//check all the planes...

		//Z:
		double zSlope = p2.z - p1.z;
		if (zSlope != 0.0)
		{
			//TOP
			double t1 = (high.z - p1.z) / zSlope;
			if (t1 >= 0.0 && t1 <= 1.0)
			{
				double x = p1.x + t1 * (p2.x - p1.x);
				double y = p1.y + t1 * (p2.y - p1.y);
				if (x >= low.x && x <= high.x && y >= low.y && y <= high.y) return true;
			}
			//BOTTOM
			double t2 = (low.z - p1.z) / zSlope;
			if (t2 >= 0.0 && t2 <= 1.0)
			{
				double x = p1.x + t2 * (p2.x - p1.x);
				double y = p1.y + t2 * (p2.y - p1.y);
				if (x >= low.x && x <= high.x && y >= low.y && y <= high.y) return true;
			}
		}

		//X:
		double xSlope = p2.x - p1.x;
		if (xSlope != 0.0)
		{
			//TOP
			double t1 = (high.x - p1.x) / xSlope;
			if (t1 >= 0.0 && t1 <= 1.0)
			{
				double z = p1.z + t1 * (p2.z - p1.z);
				double y = p1.y + t1 * (p2.y - p1.y);
				if (z >= low.z && z <= high.z && y >= low.y && y <= high.y) return true;
			}
			//BOTTOM
			double t2 = (low.x - p1.x) / xSlope;
			if (t2 >= 0.0 && t2 <= 1.0)
			{
				double z = p1.z + t2 * (p2.z - p1.z);
				double y = p1.y + t2 * (p2.y - p1.y);
				if (z >= low.z && z <= high.z && y >= low.y && y <= high.y) return true;
			}
		}

		//Y:
		double ySlope = p2.y - p1.y;
		if (ySlope != 0.0)
		{
			//TOP
			double t1 = (high.y - p1.y) / ySlope;
			if (t1 >= 0.0 && t1 <= 1.0)
			{
				double z = p1.z + t1 * (p2.z - p1.z);
				double x = p1.x + t1 * (p2.x - p1.x);
				if (z >= low.z && z <= high.z && x >= low.x && x <= high.x) return true;
			}
			//BOTTOM
			double t2 = (low.y - p1.y) / ySlope;
			if (t2 >= 0.0 && t2 <= 1.0)
			{
				double z = p1.z + t2 * (p2.z - p1.z);
				double x = p1.x + t2 * (p2.x - p1.x);
				if (z >= low.z && z <= high.z && x >= low.x && x <= high.x) return true;
			}
		}
		return false;
	}

	bool triangleIntersectsBoundaries(std::shared_ptr<Triangle> triangle)
	{
		if (lineIntersectsBoundaries(triangle->p1, triangle->p2) ||
			lineIntersectsBoundaries(triangle->p2, triangle->p3) ||
			lineIntersectsBoundaries(triangle->p3, triangle->p1)) return true;
		return false;
	}

	/*
	bool pointInTriangle(float px, float py, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y)
	{
	float Area = (float)(0.5 *(-p1y * p2x + p0y * (-p1x + p2x) + p0x * (p1y - p2y) + p1x * p2y));
	float s = 1 / (2 * Area)*(p0y*p2x - p0x * p2y + (p2y - p0y)*px + (p0x - p2x)*py);
	float t = 1 / (2 * Area)*(p0x*p1y - p0y * p1x + (p0y - p1y)*px + (p1x - p0x)*py);
	return s >= 0 && t >= 0 && 1.0f - s - t >= 0;
	}
	*/

	void solveSystem(double a, double b, double c, double d, double e, double f, double *x, double *y)
	{
		*x = (c - ((b * (f - ((d*c) / a))) / (e - ((d*b) / a)))) / a;
		*y = (f - ((d*c) / a)) / (e - ((d*b) / a));
	}

	bool pointInTriangle(float x, float y, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y)
	{
		double t = 0.0;
		double s = 0.0;
		solveSystem(p1x - p0x, p2x - p0x, x - p0x, p1y - p0y, p2y - p0y, y - p0y, &t, &s);
		if (t >= 0.0 && s >= 0.0 && t + s <= 1.0) return true;
		return false;
	}

	bool lineInteresectsLine(float A1x, float A1y, float A2x, float A2y, float B1x, float B1y, float B2x, float B2y)
	{
		if (B2x * A2y - B2y * A2x == 0.0) return false; // parallel
		float u = (A2x * (B1y - A1y) + A2y * (A1x - B1x)) / (B2x * A2y - B2y * A2x);
		float t = (B2x * (A1y - B1y) + B2y * (B1x - A1x)) / (B2x * A2y - B2y * A2x);
		if (u >= 0.0 && u <= 1.0 && t >= 0.0 && t <= 0.0) return true;
		return false;
	}

	bool triangleIntersectsLine(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float A1x, float A1y, float A2x, float A2y)
	{
		if (lineInteresectsLine(p0x, p0y, p1x, p1y, A1x, A1y, A2x, A2y) ||
			lineInteresectsLine(p1x, p1y, p2x, p2y, A1x, A1y, A2x, A2y) ||
			lineInteresectsLine(p2x, p2y, p0x, p0y, A1x, A1y, A2x, A2y)) return true;
		return false;
	}

	bool triangleIntersectsRect(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float lowX, float highX, float lowY, float highY)
	{
		if (triangleIntersectsLine(p0x, p0y, p1x, p1y, p2x, p2y, lowX, lowY, highX, lowY) ||
			triangleIntersectsLine(p0x, p0y, p1x, p1y, p2x, p2y, highX, lowY, highX, highY) ||
			triangleIntersectsLine(p0x, p0y, p1x, p1y, p2x, p2y, highX, highY, lowX, highY) ||
			triangleIntersectsLine(p0x, p0y, p1x, p1y, p2x, p2y, lowX, highY, lowX, lowY)) return true;
		return false;
	}

	bool triangleInRect(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float lowX, float highX, float lowY, float highY)
	{
		//check all triangle verts in rect
		if (pointInRect(p0x, p0y, lowX, highX, lowY, highY)) return true;
		if (pointInRect(p1x, p1y, lowX, highX, lowY, highY)) return true;
		if (pointInRect(p2x, p2y, lowX, highX, lowY, highY)) return true;
		//check all rect verts in triangle
		if (pointInTriangle(lowX, lowY, p0x, p0y, p1x, p1y, p2x, p2y)) return true;
		if (pointInTriangle(lowX, highY, p0x, p0y, p1x, p1y, p2x, p2y)) return true;
		if (pointInTriangle(highX, lowY, p0x, p0y, p1x, p1y, p2x, p2y)) return true;
		if (pointInTriangle(highX, highY, p0x, p0y, p1x, p1y, p2x, p2y)) return true;
		//check if the triangle intersects the boundary of the rectangle
		if (triangleIntersectsRect(p0x, p0y, p1x, p1y, p2x, p2y, lowX, highX, lowY, highY)) return true;
		return false;
	}

	bool rectInTriangle(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float lowX, float highX, float lowY, float highY)
	{
		//check all rect verts in triangle
		if (pointInTriangle(lowX, lowY, p0x, p0y, p1x, p1y, p2x, p2y)) return true;
		if (pointInTriangle(lowX, highY, p0x, p0y, p1x, p1y, p2x, p2y)) return true;
		if (pointInTriangle(highX, lowY, p0x, p0y, p1x, p1y, p2x, p2y)) return true;
		if (pointInTriangle(highX, highY, p0x, p0y, p1x, p1y, p2x, p2y)) return true;
		return false;
	}

	bool boundsInTriangle(std::shared_ptr<Triangle> triangle)
	{
		if (rectInTriangle(triangle->p1.x, triangle->p1.y, triangle->p2.x, triangle->p2.y, triangle->p3.x, triangle->p3.y, low.x, high.x, low.y, high.y) &&
			rectInTriangle(triangle->p1.y, triangle->p1.z, triangle->p2.y, triangle->p2.z, triangle->p3.y, triangle->p3.z, low.y, high.y, low.z, high.z) &&
			rectInTriangle(triangle->p1.z, triangle->p1.x, triangle->p2.z, triangle->p2.x, triangle->p3.z, triangle->p3.x, low.z, high.z, low.x, high.x)) return true;
		return false;
	}

	bool triangleInBounds(std::shared_ptr<Triangle> triangle)
	{
		//check if bounding box intersects
		//	--HAVE--
		if (boundingBoxCheck(triangle))
		{
			//better one...
	//		if (pointInBounds(triangle->p1) ||
	//			pointInBounds(triangle->p2) ||
	//			pointInBounds(triangle->p3)) return true;

	//		if (boundsInTriangle(triangle)) return true;

	//		if (triangleIntersectsBoundaries(triangle)) return true;
			//check projections: (true for all)
			//	See if the vertexes of triangle are in the rectangle
			//	if (triangleInRect(triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y, low.x, high.x, low.y, high.y) &&
			//		triangleInRect(triangle.p1.y, triangle.p1.z, triangle.p2.y, triangle.p2.z, triangle.p3.y, triangle.p3.z, low.y, high.y, low.z, high.z) &&
			//		triangleInRect(triangle.p1.z, triangle.p1.x, triangle.p2.z, triangle.p2.x, triangle.p3.z, triangle.p3.x, low.z, high.z, low.x, high.x)) return true;
			return true;
			//maybe good enough
		}
		return false;
	}

public:
	glm::vec3 low;
	glm::vec3 high;
	Bounds() {};
	Bounds(glm::vec3 low, glm::vec3 high)
	{
		this->low = low;
		this->high = high;
	}

	void copy(Bounds bounds) 
	{
		this->low.x = bounds.low.x;
		this->low.y = bounds.low.y;
		this->low.z = bounds.low.z;
		this->high.x = bounds.high.x;
		this->high.y = bounds.high.y;
		this->high.z = bounds.high.z;
	}

	bool pointInBounds(glm::vec3 p)
	{
		return p.x >= low.x && p.x <= high.x && p.y >= low.y && p.y <= high.y && p.z >= low.z && p.z <= high.z;
	}

	std::vector<std::shared_ptr<Triangle>> trianglesInBounds(std::vector<std::shared_ptr<Triangle>> triangles)
	{
		std::vector<std::shared_ptr<Triangle>> out;
		for (int i = 0; i < triangles.size(); i++)
		{
			if (triangleInBounds(triangles[i]))
			{
				out.push_back(triangles[i]);
			}
		}
		return out;
	//	return triangles;
	}

	static Bounds boundsFromTriangles(std::vector<std::shared_ptr<Triangle>> triangles)
	{
		Bounds bounds;
		
		if (triangles.size() == 0) return bounds;

		bounds.low.x =	triangles[0]->p1.x;
		bounds.high.x = triangles[0]->p1.x;
		bounds.low.y =	triangles[0]->p1.y;
		bounds.high.y = triangles[0]->p1.y;
		bounds.low.z =	triangles[0]->p1.z;
		bounds.high.z = triangles[0]->p1.z;

		for (int i = 0; i < triangles.size(); i++)
		{
			if (triangles[i]->p1.x < bounds.low.x) bounds.low.x =	triangles[i]->p1.x;
			if (triangles[i]->p1.x > bounds.high.x) bounds.high.x = triangles[i]->p1.x;
			if (triangles[i]->p1.y < bounds.low.y) bounds.low.y =	triangles[i]->p1.y;
			if (triangles[i]->p1.y > bounds.high.y) bounds.high.y = triangles[i]->p1.y;
			if (triangles[i]->p1.z < bounds.low.z) bounds.low.z =	triangles[i]->p1.z;
			if (triangles[i]->p1.z > bounds.high.z) bounds.high.z = triangles[i]->p1.z;

			if (triangles[i]->p2.x < bounds.low.x) bounds.low.x =	triangles[i]->p2.x;
			if (triangles[i]->p2.x > bounds.high.x) bounds.high.x = triangles[i]->p2.x;
			if (triangles[i]->p2.y < bounds.low.y) bounds.low.y =	triangles[i]->p2.y;
			if (triangles[i]->p2.y > bounds.high.y) bounds.high.y = triangles[i]->p2.y;
			if (triangles[i]->p2.z < bounds.low.z) bounds.low.z =	triangles[i]->p2.z;
			if (triangles[i]->p2.z > bounds.high.z) bounds.high.z = triangles[i]->p2.z;

			if (triangles[i]->p3.x < bounds.low.x) bounds.low.x =	triangles[i]->p3.x;
			if (triangles[i]->p3.x > bounds.high.x) bounds.high.x = triangles[i]->p3.x;
			if (triangles[i]->p3.y < bounds.low.y) bounds.low.y =	triangles[i]->p3.y;
			if (triangles[i]->p3.y > bounds.high.y) bounds.high.y = triangles[i]->p3.y;
			if (triangles[i]->p3.z < bounds.low.z) bounds.low.z =	triangles[i]->p3.z;
			if (triangles[i]->p3.z > bounds.high.z) bounds.high.z = triangles[i]->p3.z;
		}
		return bounds;
	}
};



