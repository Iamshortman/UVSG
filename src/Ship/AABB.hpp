#ifndef AABB_HPP
#define AABB_HPP

#include <algorithm>

#include "glmInclude.hpp"
#include "Ship/Directions.hpp"

using std::max;
using std::min;
using std::swap;

class AABB
{
public:
	vector3D minPos;
	vector3D maxPos;
	vector3D pos;

	AABB(vector3D min = vector3D(-0.5), vector3D max = vector3D(0.5))
	{
		minPos = min;
		maxPos = max;
	};

	void setPos(vector3D position)
	{
		pos = position;
	};

	//The simple AABB collision check
	bool aabbIntersect(const AABB& other)
	{
		bool intersectX = ((this->pos.x + this->minPos.x) < (other.pos.x + other.maxPos.x) && (this->pos.x + this->maxPos.x) > (other.pos.x + other.minPos.x));
		bool intersectY = ((this->pos.y + this->minPos.y) < (other.pos.y + other.maxPos.y) && (this->pos.y + this->maxPos.y) > (other.pos.y + other.minPos.y));
		bool intersectZ = ((this->pos.z + this->minPos.z) < (other.pos.z + other.maxPos.z) && (this->pos.z + this->maxPos.z) > (other.pos.z + other.minPos.z));

		return intersectX && intersectY && intersectZ;
	};

	bool pointIntersect(vector3D point)
	{
		bool intersectX = (this->pos.x + this->minPos.x) <= point.x && (this->pos.x + this->maxPos.x) >= point.x;
		bool intersectY = (this->pos.y + this->minPos.y) <= point.y && (this->pos.y + this->maxPos.y) >= point.y;
		bool intersectZ = (this->pos.z + this->minPos.z) <= point.z && (this->pos.z + this->maxPos.z) >= point.z;

		return intersectX && intersectY && intersectZ;
	};

	bool rayIntersect(const vector3D& rayOrigin, const vector3D& rayEnd, vector3D& out_Pos, DIRECTIONS& hitFace)
	{
		double f_low = 0.0;
		double f_high = 1.0;

		if (!ClipLine(0, rayOrigin, rayEnd, f_low, f_high))
			return false;

		if (!ClipLine(1,rayOrigin, rayEnd, f_low, f_high))
			return false;

		if (!ClipLine(2, rayOrigin, rayEnd, f_low, f_high))
			return false;

		vector3D b = rayEnd - rayOrigin;
		out_Pos = rayOrigin + b * f_low;

		//The actual center of the AABB
		vector3D centerOfAABB = (minPos + maxPos) / 2.0;

		//The center point of each face
		vector3D faceMidPoints[] =
		{
			vector3D(centerOfAABB.x, maxPos.y, centerOfAABB.z),//UP
			vector3D(centerOfAABB.x, minPos.y, centerOfAABB.z),//DOWN
			vector3D(centerOfAABB.x, centerOfAABB.y, maxPos.z),//FORWARD
			vector3D(centerOfAABB.x, centerOfAABB.y, minPos.z),//BACKWARDS
			vector3D(maxPos.x, centerOfAABB.y, centerOfAABB.z),//LEFT
			vector3D(minPos.x, centerOfAABB.y, centerOfAABB.z),//RIGHT
		};

		const double EPSI = 0.01;
		const vector3D minPosWorld = minPos + pos;
		const vector3D maxPosWorld = maxPos + pos;

		if (abs(out_Pos.y - maxPosWorld.y) < EPSI)
		{
			hitFace = UP;
		}
		if (abs(out_Pos.y - minPosWorld.y) < EPSI)
		{
			hitFace = DOWN;
		}
		if (abs(out_Pos.z - maxPosWorld.z) < EPSI)
		{
			hitFace = FORWARD;
		}
		if (abs(out_Pos.z - minPosWorld.z) < EPSI)
		{
			hitFace = BACKWARD;
		}
		if (abs(out_Pos.x - maxPosWorld.x) < EPSI)
		{
			hitFace = LEFT;
		}
		if (abs(out_Pos.x - minPosWorld.x) < EPSI)
		{
			hitFace = RIGHT;
		}

		if (hitFace == -1)
		{
			printf("FaceNotFound- NOT GOOD!!! \n");
		}

		return true;
	};

private:
	bool ClipLine(int i, const vector3D& rayOrigin, const vector3D& rayEnd, double& f_low, double& f_high)
	{
		// f_low and f_high are the results from all clipping so far. We'll write our results back out to those parameters.

		// f_dim_low and f_dim_high are the results we're calculating for this current dimension.
		double f_dim_low, f_dim_high;

		// Find the point of intersection in this dimension only as a fraction of the total vector
		f_dim_low = ((this->minPos[i] + this->pos[i]) - rayOrigin[i]) / (rayEnd[i] - rayOrigin[i]);
		f_dim_high = ((this->maxPos[i] + this->pos[i]) - rayOrigin[i]) / (rayEnd[i] - rayOrigin[i]);

		// Make sure low is less than high
		if (f_dim_high < f_dim_low)
			swap(f_dim_high, f_dim_low);

		// If this dimension's high is less than the low we got then we definitely missed.
		if (f_dim_high < f_low)
			return false;

		// Likewise if the low is less than the high.
		if (f_dim_low > f_high)
			return false;

		// Add the clip from this dimension to the previous results
		f_low = max(f_dim_low, f_low);
		f_high = min(f_dim_high, f_high);

		if (f_low > f_high)
			return false;

		return true;
	}

};

#endif //AABB_HPP