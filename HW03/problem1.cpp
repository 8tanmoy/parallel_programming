#include <vector>
#include <list>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include "stopwatch.hpp"

int main() {
	FILE *fpa;
	FILE *fpl;
	fpa = fopen("problem1_array.out","w");
	fpl = fopen("problem1_list.out","w");

	std::vector<float> timings;
	stopwatch<std::milli, float> sw;
	constexpr static size_t sizes[] = { 1<<8, 1<<9, 1<<10, 1<<11, 1<<12, 1<<13};

	for (auto size : sizes) {
		std::vector<int> data;
		//middle
		for (size_t iter = 0; iter < 10; iter++) {
			sw.start();
			for (size_t n = 0; n < size; n++) {
				auto pos = data.begin();
				std::advance(pos, data.size() / 2UL);
				data.insert(pos, n);
			}
			sw.stop();
			timings.push_back(sw.count());
			data.clear();
		}
		const auto min_time_m = *std::min_element(timings.begin(), timings.end());
		timings.clear();
		//front
		for (size_t iter = 0; iter < 10; iter++) {
			sw.start();
			for (size_t n = 0; n < size; n++) {
				auto pos = data.begin();
				//std::advance(pos, data.size() / 2UL);
				data.insert(pos, n);
			}
			sw.stop();
			timings.push_back(sw.count());
			data.clear();
		}
		const auto min_time_f = *std::min_element(timings.begin(), timings.end());
		timings.clear();
		//end
		for (size_t iter = 0; iter < 10; iter++) {
			sw.start();
			for (size_t n = 0; n < size; n++) {
				auto pos = data.begin();
				std::advance(pos, data.size());
				data.insert(pos, n);
			}
			sw.stop();
			timings.push_back(sw.count());
			data.clear();
		}
		const auto min_time_e = *std::min_element(timings.begin(), timings.end());
		timings.clear();
		//const auto min_time = *std::min_element(timings.begin(), timings.end());
		//std::cout << std::fixed << std::setprecision(6) << size << " " << min_time << '\n';
		fprintf(fpa, "%d %.6f %.6f %.6f\n", size, min_time_f, min_time_e, min_time_m);
	}
	fclose(fpa);

	for (auto size : sizes) {
		//list
		std::list<int> data2;
		//middle
		for (size_t iter = 0; iter < 10; iter++) {
			sw.start();
			for (size_t n = 0; n < size; n++) {
				auto pos = data2.begin();
				std::advance(pos, data2.size() / 2UL);
				data2.insert(pos, n);
			}
			sw.stop();
			timings.push_back(sw.count());
			data2.clear();
		}
		const auto min_time_m = *std::min_element(timings.begin(), timings.end());
		timings.clear();
		//front
		for (size_t iter = 0; iter < 10; iter++) {
			sw.start();
			for (size_t n = 0; n < size; n++) {
				data2.push_front(n);
			}
			sw.stop();
			timings.push_back(sw.count());
			data2.clear();
		}
		const auto min_time_f = *std::min_element(timings.begin(), timings.end());
		timings.clear();
		//end
		for (size_t iter = 0; iter < 10; iter++) {
			sw.start();
			for (size_t n = 0; n < size; n++) {
				data2.push_back(n);
			}
			sw.stop();
			timings.push_back(sw.count());
			data2.clear();
		}
		const auto min_time_e = *std::min_element(timings.begin(), timings.end());
		timings.clear();
		fprintf(fpl, "%d %.6f %.6f %.6f\n", size, min_time_f, min_time_e, min_time_m);
	}
	fclose(fpl);

	return(0);
}