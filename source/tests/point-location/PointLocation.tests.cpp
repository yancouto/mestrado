#include "../../point-location/PointLocation.cpp"
#include <gtest/gtest.h>

#include <vector>

using namespace point_location;

TEST(PointLocation, Empty) {
	PointLocationSolver pls({});
	for(Point p : {Point{0, 0}, Point{-1, 1}, Point{0.1, 0.1}})
		EXPECT_EQ(pls.WhichPolygon(p), -1);
}

TEST(PointLocation, DISABLED_SimpleSquare) {
	Polygon p = {Point{-1, -1}, Point{1, -1}, Point{1, 1}, Point{-1, 1}};
	PointLocationSolver pls({p});
	for(Point p : {Point{0, 0}, Point{-1, 1}, Point{0.1, 0.1}})
		EXPECT_EQ(pls.WhichPolygon(p), 0);
	for(Point p : {Point{2, 0}, Point{-1, -2}, Point{1.1, 0.9}})
		EXPECT_EQ(pls.WhichPolygon(p), -1);
}
