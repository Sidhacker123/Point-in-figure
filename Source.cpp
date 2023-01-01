#include <iostream>
using namespace std;
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Point>
#include <osgViewer/Viewer>

// Define Infinite (Using INT_MAX caused overflow problems)
#define INF 10000

struct Point
{
	int x;
	int y;
};

// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;
	return false;
}

int orientation(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0; // collinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}


bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{

	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and p2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

// Returns true if the point p lies inside the polygon[] with n vertices
int isInside(Point polygon[], int n, Point p)
{
	// There must be at least 3 vertices in polygon[]
	if (n < 3) return -1;

	// Create a point for line segment from p to infinite
	Point extreme = { INF, p.y };

	// To count number of points in polygon
	// whose y-coordinate is equal to
	// y-coordinate of the point
	int decrease = 0;

	// Count intersections of the above line with sides of polygon
	int count = 0, i = 0;
	do
	{
		int next = (i + 1) % n;

		if (polygon[i].y == p.y) decrease++;

		if (doIntersect(polygon[i], polygon[next], p, extreme))
		{
			if (orientation(polygon[i], p, polygon[next]) == 0) {
				if (onSegment(polygon[i], p, polygon[next]) == true) {
					return 0;
				}
			}
			//return onSegment(polygon[i], p, polygon[next]);

			count++;
			//cout<<i<<"count"<<count<<endl;
		}
		i = next;
	} while (i != 0);


	count -= decrease;


	if (count % 2 == 1) {
		return 1;
	}
	else {
		return 2;
	}
}
void displayPoints(vector<Point>& points, vector<Point>& inside, vector<Point>& on, vector<Point>& outside)
{
	osg::ref_ptr<osg::Vec3Array> polygonVertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> insideVertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> onsideVertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> outsideVertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> colors1 = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> colors3 = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> colors2 = new osg::Vec4Array;
	for (int i = 0; i < points.size(); i++)
	{
		polygonVertices->push_back(osg::Vec3(points[i].x, 0.0f, points[i].y));
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	for (int i = 0; i < inside.size(); i++)
	{
		insideVertices->push_back(osg::Vec3(inside[i].x, 0.0f, inside[i].y));
		colors1->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	for (int i = 0; i < on.size(); i++)
	{
		onsideVertices->push_back(osg::Vec3(on[i].x, 0.0f, on[i].y));
		colors3->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	}