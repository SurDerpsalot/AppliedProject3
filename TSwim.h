#ifndef TSWIM_H
#define TSWIM_H

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
#include <string>

#include <message_queue.h>
#include <PixelStruct.h>
#include <interpreter.h>

typedef message_queue<int> MessIn;
typedef message_queue<Pixall> MessOut;
class Calculate
{
public:
	Calculate(interpreter interp, int position, int nT);
	Calculate();

	void run();

	void sphereintersect(int i, std::vector<double> origin, std::vector<double> destination);
	void planeintersect(int i, std::vector<double> origin, std::vector<double> destination);

	void findshadow(int w);
	void thread_func();
	double dot_product(std::vector<double> one, std::vector<double> two);
	void checkShadow(std::vector<double> intersect, std::vector<double> start, int i);
	
	void ThreadPool(int n);

	void joinAll();
	mutable std::mutex the_mutex;
	std::vector<std::thread> pool;

	MessIn in;
	MessOut out;
	int pos;
	int numThreads;
	interpreter inter;
	bool ShadowSearch;
	Pixall pix;
};

	

	

#endif // !TSWIM_H