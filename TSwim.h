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

//Basic work class
class WorkBase
{
public:
	virtual void run() = 0;
};

// make the message a WorkBase pointer
typedef WorkBase * MessageType;
typedef message_queue<MessageType> MessageQueue;

// now create a work units
class Calculate
{
public:
	Calculate(interpreter interp, int position);

	void run();

	void sphereintersect(int i, std::vector<double> origin, std::vector<double> destination);
	void planeintersect(int i, std::vector<double> origin, std::vector<double> destination);

	void findshadow(int w);

	double dot_product(std::vector<double> one, std::vector<double> two);

	int pos;
	interpreter inter;
	bool ShadowSearch;
	Pixall pix;
};

#endif // !TSWIM_H