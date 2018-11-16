#include "PointLocation.hpp"

#include <cassert>
#include <utility>
#include <algorithm>
#include <tuple>

namespace point_location {
struct event {
	unsigned i, j;
};

Point Point::operator - (const Point &o) const {
	return {x - o.x, y - o.y};
}

bool Point::operator == (const Point &o) const {
	return x == o.x && y == o.y;
}

bool Point::operator < (const Point &o) const {
	if(x != o.x)
		return x < o.x;
	return y < o.y;
}

Segment::Segment() {}

Segment::Segment(Point a, Point b, int pol) : from(a), to(b), polygon(pol), top(false) {
	if(!(a < b)) {
		std::swap(from, to);
		top = true;
	}
}

namespace {

double cross(Point a, Point b) {
	return a.x * b.y - a.y * b.x;
}

void checkPolygon(const Polygon &p) {
	double area = 0;
	for(unsigned i = 0; i < p.size(); i++)
		area += cross(p[i], p[(i + 1) % p.size()]);
	assert(area > 0); // Poligono deve ser dado em sentido anti-horario
}

}

bool Segment::operator < (const Segment &o) const {
	if(from == o.from && to == o.to) return false;
	if(o.from < from) return !(o < *this);
	if(from == o.from) {
		if(to < o.to) return !(o < *this);
		return cross(to - from, o.to - from) > 0;
	} else {
		return cross(to - from, o.from - from) > 0;
	}
}

PointLocationSolver::PointLocationSolver(std::vector<Polygon> polygons) {
	for(const Polygon &p : polygons)
		checkPolygon(p);
	std::vector<event> events;

	for(unsigned i = 0; i < polygons.size(); i++)
		for(unsigned j = 0; j < polygons[i].size(); j++)
			events.push_back(event{i, j});
	std::sort(events.begin(), events.end(), [&polygons](const event &a, const event &b) {
		return polygons[a.i][a.j] < polygons[b.i][b.j];
	});

	slabs.push_back(std::make_pair(Point{-1.0 / 0.0, 0}, rbt.current()));
	for(const event &e : events) {
		unsigned i = e.i, j = e.j;
		unsigned jm1 = j? j - 1 : polygons[i].size() - 1;
		if(polygons[i][jm1] < polygons[i][j])
			assert(rbt.Remove(Segment(polygons[i][jm1], polygons[i][j], i)) != nullptr);
		else
			rbt.Insert(Segment(polygons[i][jm1], polygons[i][j], i));
		unsigned jp1 = j == polygons[i].size() - 1? 0 : j + 1;
		if(polygons[i][j] < polygons[i][jp1])
			rbt.Insert(Segment(polygons[i][j], polygons[i][jp1], i));
		else
			assert(rbt.Remove(Segment(polygons[i][j], polygons[i][jp1], i)) != nullptr);
		slabs.push_back(std::make_pair(polygons[i][j], rbt.current()));
	}
}

int PointLocationSolver::WhichPolygon(const Point p) const {
	int i = prev(std::upper_bound(slabs.begin(), slabs.end(), std::make_pair(p, -1)))->second;
	assert(i >= 0);
	using namespace persistence::red_black_tree;
	Node<Segment> *u = rbt.roots[i];
	const Segment *below = nullptr;
	while(u != nullptr) {
		bool u_is_below = (cross(u->value.to - u->value.from, p - u->value.from) >= 0);
		if(u_is_below)
			below = &u->value;
		u = rbt.Child(u, u_is_below, i);
	}

	if(below == nullptr)
		return -1;
	// if below is on the "bottom" of the polygon, or if p is on top of the segment
	if(!below->top || cross(below->to - below->from, p - below->from) == 0)
		return below->polygon;
	return -1;
}

} // namespace point_location
