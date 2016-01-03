#include "Timer.hpp"
#ifdef WIN32

#else
#include <sys/time.h>
#endif
#include <sstream>
#include <iostream>

Timer::Timer() {
}

Timer::~Timer() {
}

#ifdef WIN32

void Timer::Begin() {
	QueryPerformanceCounter(&startTime);
}

double Timer::End() {
	QueryPerformanceCounter(&endTime);
	double t = double(endTime.QuadPart - startTime.QuadPart);
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li)) {
		return 0;
	}
	double freq = double(li.QuadPart);
	double duration = (t) / freq;

	return duration;
}

#else

void Timer::Begin() {
	gettimeofday((struct timeval *)&t, NULL);
}

double Timer::End() {
    timeval t2;
    gettimeofday((struct timeval *)&t2, NULL);
#ifdef IPHONE
    double end = ((double)((__darwin_suseconds_t)(t2.tv_sec*1000000ULL+t2.tv_usec)))*0.000001;
    double start = ((double)((__darwin_suseconds_t)(t.tv_sec*1000000ULL+t.tv_usec)))*0.000001;
#elif ANDROID
    double end = ((double)((t2.tv_sec*1000000ULL+t2.tv_usec)))*0.000001;
    double start = ((double)((t.tv_sec*1000000ULL+t.tv_usec)))*0.000001;
#else
    double end = (t2.tv_sec+t2.tv_usec * 0.000001);
    double start = (t.tv_sec+t.tv_usec * 0.000001);
#endif
    return end - start;
}

#endif

