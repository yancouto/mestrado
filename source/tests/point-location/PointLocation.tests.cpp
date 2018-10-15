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
	reverse(p.begin(), p.end());
	PointLocationSolver pls({p});
	for(Point p : {Point{0, 0}, Point{-0.9, 0}, Point{0.1, 0.1}})
		EXPECT_EQ(pls.WhichPolygon(p), 0) << "Ponto dentro";
	for(Point p : {Point{2, 0}, Point{-1, -2}, Point{1.1, 0.9}})
		EXPECT_EQ(pls.WhichPolygon(p), -1) << "Ponto fora";
	//for(Point p : {Point{0, -1}, Point{1, 0}})
	//	EXPECT_EQ(pls.WhichPolygon(p), 0) << "Ponto no vertice";
	for(Point p : {Point{-0.5, -0.5}, Point{0.5, 0.5}})
		EXPECT_EQ(pls.WhichPolygon(p), 0) << "Ponto na aresta";
}

TEST(PointLocation, SimpleSquare) {
	Polygon p = {Point{-1, -1}, Point{1, -1}, Point{1, 1}, Point{-1, 1}};
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
	reverse(p1.begin(), p1.end());
	reverse(p2.begin(), p2.end());
	PointLocationSolver pls({p1, p2});
	for(Point p : {Point{-13.34, 17.96}, Point{-10.72, 7.36}, Point{-2.02, 12.14}, Point{12.19, 17.57}, Point{20.44, 20.71}, Point{19, 5.72}})
		EXPECT_EQ(pls.WhichPolygon(p), 0);
	for(Point p : {Point{-25.78, 12.72}, Point{-20.8, 1.4}, Point{15.72, -4.16}, Point{18.8, 12.4}, Point{15.98, 26.08}})
		EXPECT_EQ(pls.WhichPolygon(p), 1);
	for(Point p : {Point{-9.81, 28.37}, Point{-14.91, 8.6}, Point{13.04, 12.33}, Point{25.67, 3.04}, Point{11.4, -13.98}, Point{51.07, 15.41}})
		EXPECT_EQ(pls.WhichPolygon(p), -1);
}

// https://www.geogebra.org/graphing/smm9wx3t
TEST(PointLocation, T1) {
	Polygon p1 = {{-6.69, 2.52}, {-3.85, -1.62}, {1.16, -1.98}, {4.56, 1.71}, {3.8, 6.67}, {-0.56, 9.17}, {-5.22, 7.32}};
	Polygon p2 = {{7.04, 7.02}, {8.25, 4.7}, {9.66, 6.92}};
	Polygon p3 = {{-10.29, 12.01}, {-6.96, 10.39}, {-5.34, 13.71}, {-8.66, 15.33}};
	Polygon p4 = {{-10, 4}, {-2.93, 10.25}, {4.12, 9.52}, {6, 4}, {5.79, 11.21}, {-3.69, 11.94}};
	std::vector<Polygon> pols = {p1, p2, p3, p4};
	PointLocationSolver pls(pols);
	std::vector<Point> queries = {{-2.43, 5.01}, {-0.86, 11.14}, {8.27, 6.3}, {-6.8, 13.16}, {0.68, 12.93}, {2.15, 8.66}, {3.24, 4.71}};
	std::vector<int> answers =   {0            , 3             , 1          , 2            , -1           , -1          , 0};
	for(unsigned i = 0; i < queries.size(); i++)
		EXPECT_EQ(pls.WhichPolygon(queries[i]), answers[i]);
}

TEST(PointLocation, Large) {
	std::vector<Polygon> pols;
	const int N = 100000, Q = 1000000;
	for(int i = 0; i < N; i++) {
		Polygon p = {{3. * i, 3. * i}, {3. * i, 3. * i + 2}, {3. * i + 2, 3. * i + 2}, {3. * i + 2, 3. * i}};
		reverse(p.begin(), p.end());
		pols.push_back(p);
	}
	PointLocationSolver pls(pols);
	for(int i = 0; i < Q; i++) {
		int j = (rand() % (N + 1)) - 1;
		double x, y;
		if(j == -1) x = -double(rand()) - 0.5, y = -double(rand()) - 0.5;
		else x = 3. * j + 1 + (double(rand()) * .5 / RAND_MAX),
		     y = 3. * j + 1 + (double(rand()) * .5 / RAND_MAX);
		EXPECT_EQ(pls.WhichPolygon(Point{x, y}), j);
	}
}
