#include <stdio.h>
#include <stdlib.h>

#include <random>
#include <chrono>

#define timetype double

using std::exponential_distribution;
using std::default_random_engine;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::system_clock;

constexpr timetype mu = 1;
constexpr timetype lamda = 0.9;
constexpr auto numArrivals = 2000;
constexpr auto runs = 200;

int main() {
	default_random_engine rand_gen((unsigned int)system_clock::now().time_since_epoch().count());
	exponential_distribution<timetype> jobSize(mu);
	exponential_distribution<timetype> interarrivalTime(lamda);
	timetype queueSize;
	timetype averageResponseTime = 0;

	auto startTime = high_resolution_clock::now();

	for(auto j = 0; j < runs; ++j) {
		queueSize = 0;
		for(auto i = 0; i <= numArrivals; ++i) {
			auto arrivalDelta = interarrivalTime(rand_gen);
			queueSize -= arrivalDelta;
			if(queueSize < 0) queueSize = 0;
			queueSize += jobSize(rand_gen);
		}

		// response time for the n+1-th job is stored in the queueSize
		// this is because the "current time" is the arrival time, and the queueSize contains all earlier jobs that
		// have not completed, plus the size of the arriving job.
		averageResponseTime += queueSize;
	}

	auto endTime = high_resolution_clock::now();
	auto ns = duration_cast<nanoseconds>(endTime - startTime).count();
	printf("Simulation processing time: %.2fus\n", ns/1000.);
	printf("Average response time of %d-th job: %f\n", numArrivals + 1, averageResponseTime / runs);

	return 0;
}