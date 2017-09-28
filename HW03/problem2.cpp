#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "stopwatch.hpp"

struct point3D {
	float x, y, z;
};

struct point3D_B {
	float xB, yB, zB;
	char chB;
};

struct point3D_C {
	float xC, yC, zC;
	char chC[52];
};

int main() {
	std::ofstream fout{"problem2.out"};

	auto min_time = [&fout](array_view<float> const& x) {
		fout << *std::min_element(x.begin(), x.end()) << '\n';
	};

	constexpr auto size = 1'000'000UL;

	stopwatch<std::milli, float> sw;
	std::vector<point3D> x(size);
	std::vector<point3D_B> xB(size);
	std::vector<point3D_C> xC(size);

	//size
	printf("2.A size of point3D  : %d\n", sizeof(point3D));
	printf("2.B size of point3D_B: %d\n", sizeof(point3D_B));
	printf("2.C size of point3D_C: %d\n", sizeof(point3D_C));

	volatile float sum{};
	auto f = [&x, &sum]() {
		sum = std::accumulate(x.begin(), x.end(), 0.0f, [](float total, point3D const& p){ return total + p.x;});
	};
	sw.time_it(10UL, f, min_time);

	volatile float sumB{};
	auto g = [&xB, &sumB]() {
		sumB = std::accumulate(xB.begin(), xB.end(), 0.0f, [](float total, point3D_B const& p){ return total + p.xB;});
	};
	sw.time_it(10UL, g, min_time);

	volatile float sumC{};
	auto h = [&xC, &sumC]() {
		sumC = std::accumulate(xC.begin(), xC.end(), 0.0f, [](float total, point3D_C const& p){ return total + p.xC;});
	};
	sw.time_it(10UL, h, min_time);

}