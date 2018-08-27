#include "../../point-location/PointLocation.cpp"
#include <gtest/gtest.h>

#include <vector>

using namespace point_location;

TEST(PointLocation, Empty) {
	PointLocationSolver pls({});
	for(Point p : {Point{0, 0}, Point{-1, 1}, Point{0.1, 0.1}})
		EXPECT_EQ(pls.WhichPolygon(p), -1);
}

TEST(PointLocation, SimpleDiamond) {
	Polygon p = {Point{0, -1}, Point{-1, 0}, Point{0, 1}, Point{1, 0}};
	PointLocationSolver pls({p});
	for(Point p : {Point{0, 0}, Point{-0.9, 0}, Point{0.1, 0.1}})
		EXPECT_EQ(pls.WhichPolygon(p), 0);
	for(Point p : {Point{2, 0}, Point{-1, -2}, Point{1.1, 0.9}})
		EXPECT_EQ(pls.WhichPolygon(p), -1);
}

TEST(PointLocation, SimpleSquare) {
	Polygon p = {Point{-1, -1}, Point{1, -1}, Point{1, 1}, Point{-1, 1}};
	std::reverse(p.begin(), p.end());
	PointLocationSolver pls({p});
	for(Point p : {Point{0, 0}, Point{-0.9, 0.9}, Point{0.1, 0.1}})
		EXPECT_EQ(pls.WhichPolygon(p), 0);
	for(Point p : {Point{2, 0}, Point{-1, -2}, Point{1.1, 0.9}})
		EXPECT_EQ(pls.WhichPolygon(p), -1);
}

// https://ggbm.at/gt4zx5fu
TEST(PointLocation, Tricky) {
	Polygon p1 = {{-9.08, 11.19}, {-19.18, 19.41}, {23.23, 21.36}, {7.29, 11.62}, {24.03, 3.98}, {-17.66, 5.28}};
	Polygon p2 = {{-23.43, 20.35}, {-13.05, 11.12}, {-24.01, 3.11}, {30.09, 1.45}, {15.8, 12.13}, {31.74, 24.32}, {-18.17, 23.09}, {43.28, 31.75}, {28.86, -13.26}, {-41.97, 5.35}};
	PointLocationSolver pls({p1, p2});
	for(Point p : {Point{-13.34, 17.96}, Point{-10.72, 7.36}, Point{-2.02, 12.14}, Point{12.19, 17.57}, Point{20.44, 20.71}, Point{19, 5.72}})
		EXPECT_EQ(pls.WhichPolygon(p), 0);
	for(Point p : {Point{-25.78, 12.72}, Point{-20.8, 1.4}, Point{15.72, -4.16}, Point{18.8, 12.4}, Point{15.98, 26.08}})
		EXPECT_EQ(pls.WhichPolygon(p), 1);
	for(Point p : {Point{-9.81, 28.37}, Point{-14.91, 8.6}, Point{13.04, 12.33}, Point{25.67, 3.04}, Point{11.4, -13.98}, Point{51.07, 15.41}})
		EXPECT_EQ(pls.WhichPolygon(p), -1);
}
