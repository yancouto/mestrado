#include "PointLocation.hpp"

#include <cassert>
#include <utility>
#include <algorithm>
#include <tuple>

namespace point_location {
struct event {
	bool add;
	Segment s;
	const Point& point() const { return add? s.from : s.to; }
	bool operator < (const event &o) const {
		return point() < o.point();
	}
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

	unsigned p_i = 0;
	for(const Polygon &p : polygons) {
		for(unsigned i = 0; i < p.size(); i++) {
			Point a = p[i], b = p[(i + 1) % p.size()];
			Segment s(a, b, p_i);
			events.push_back(event{true, s});
			events.push_back(event{false, s});
		}
		p_i++;
	}
	std::sort(events.begin(), events.end());

	slabs.push_back({-1.0 / 0.0, 0});
	for(const event &ev : events) {
		if(ev.add) rbt.Insert(ev.s);
		else assert(rbt.Remove(ev.s) != nullptr);
		slabs.push_back(ev.point());
	}
}

int PointLocationSolver::WhichPolygon(const Point p) const {
	int i = int(std::upper_bound(slabs.begin(), slabs.end(), p) - slabs.begin()) - 1;
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
