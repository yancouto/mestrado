#include "PointLocation.hpp"

#include <cassert>
#include <cmath>
#include <utility>
#include <algorithm>
#include <tuple>

namespace point_location {
struct event {
	double x;
	Segment s;
	inline bool add() const { return x == s.from.x; }
	bool operator < (const event &o) const {
		return x < o.x;
	}
};

void Point::Rotate(double angle) {
	double nx = cos(angle) * x - sin(angle) * y,
	       ny = sin(angle) * x + cos(angle) * y;
	x = nx;
	y = ny;
}

Segment::Segment() {}

Segment::Segment(Point a, Point b, int pol) : from(a), to(b), polygon(pol) {
	assert(a.x != b.x);
	if(a.x > b.x)
		std::swap(from, to);
}

double Segment::HeightAtX(double x) const {
	assert(from.x <= x && x <= to.x);
	if(from.x == to.x)
		return from.y;
	double ratio = (to.x - x) / (to.x - from.x);
	return to.y + ratio * (from.y - to.y);
}

bool Segment::operator < (const Segment &o) const {
	double xl = std::max(from.x, o.from.x);
	double xr = std::min(to.x, o.to.x);
	double x = (xl + xr) / 2;
	assert(xl <= xr);
	double y = HeightAtX(x), oy = o.HeightAtX(x);
	if(y != oy)
		return y < oy;
	return std::make_tuple(from.x, from.y, to.x, to.y, polygon) <
	       std::make_tuple(o.from.x, o.from.y, o.to.x, o.to.y, o.polygon);
}

PointLocationSolver::PointLocationSolver(std::vector<Polygon> polygons) {
	angle = (double(rand()) / RAND_MAX) * M_PI * 2.;
	for(Polygon &pol : polygons)
		for(Point &pt : pol)
			pt.Rotate(angle);
	std::vector<event> events;

	unsigned p_i = 0;
	for(const Polygon &p : polygons) {
		for(unsigned i = 0; i < p.size(); i++) {
			Point a = p[i], b = p[(i + 1) % p.size()];
			Segment s(a, b, a.x < b.x? -1 : p_i);
			events.push_back(event{s.from.x, s});
			events.push_back(event{s.to.x, s});
		}
		p_i++;
	}
	std::sort(events.begin(), events.end());

	slabs.push_back(-1.0 / 0.0);
	for(const event &ev : events) {
		if(ev.add()) rbt.Insert(ev.s);
		else assert(rbt.Remove(ev.s) != nullptr);
		slabs.push_back(ev.x);
	}
}

int PointLocationSolver::WhichPolygon(Point p) const {
	p.Rotate(angle);
	int i = int(std::upper_bound(slabs.begin(), slabs.end(), p.x) - slabs.begin()) - 1;
	assert(i >= 0);
	using namespace persistence::red_black_tree;
	Node<Segment> *u = rbt.roots[i];
	const Segment *below = nullptr;
	while(u != nullptr) {
		if(u->value.HeightAtX(p.x) <= p.y)
			below = &u->value;
		u = rbt.Child(u, u->value.HeightAtX(p.x) <= p.y, i);
	}

	return below == nullptr? -1 : below->polygon;
}

} // namespace point_location
