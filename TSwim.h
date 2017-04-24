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

//Basic work class
class WorkBase
{
public:
	virtual void run() = 0;
};

// make the message a WorkBase pointer
typedef WorkBase * MessageType;
typedef message_queue<MessageType> MessageQueue;

// a thread function to remove work and run it
// exits on a nullptr message
void thread_func(MessageQueue * inq, MessageQueue * outq)
{
	while (true) {

		// take a unit fo work from the input queue
		MessageType m;
		inq->wait_and_pop(m);

		if (m == nullptr) break;

		// execute m
		m->run();

		// put the result back into the output queue
		outq->push(m);
	}
}

class ThreadPool
{
public:

	ThreadPool(int n, MessageQueue * input_queue, MessageQueue * output_queue) {

		for (int i = 0; i < n; ++i) {
			pool.emplace_back(std::thread(&thread_func, input_queue, output_queue));
		}
	}

	void joinAll() {
		for (auto &t : pool) t.join();
	}

private:

	std::vector<std::thread> pool;
};

// now create a couple of work units
class Calculate : public WorkBase
{
public:
	Calculate(interpreter interp) {
		inter = interp;
	}
	void run() {
			







	}




private:
	interpreter inter;
	Pixall pix;
};

class Draw : public WorkBase
{
public:
	void run() {
		/*
		 to set the colors for the pixels, use:
		color = 4278190080 + (R*65536) + (G*256) + B;
		*/
	}

	int get() {
		return result;
	}

private:

	int result;
};

#endif // !TSWIM_H
