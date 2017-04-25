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
	Calculate(interpreter interp, int position) {
		inter = interp;
		pos = position;
		ShadowSearch = false;
		pix.pip.r = 0;
		pix.pip.g = 0;
		pix.pip.b = 0;
	}
	void run() {
		std::vector<double> screen;
		std::vector<double> focal;
		screen.push_back((inter.Cams.CamStruct.resolution[0] * ((pos % inter.Cams.CamStruct.size[1]) - (inter.Cams.CamStruct.size[0] / 2))));
		screen.push_back((inter.Cams.CamStruct.resolution[1] * ((pos / inter.Cams.CamStruct.size[0]) - (inter.Cams.CamStruct.size[1] / 2))));
		screen.push_back(inter.Cams.CamStruct.center.z);
		focal.push_back(inter.Cams.CamStruct.center.x);
		focal.push_back(inter.Cams.CamStruct.center.y);
		if (inter.Cams.CamStruct.normal.z > 0)
			focal.push_back(inter.Cams.CamStruct.center.z - inter.Cams.CamStruct.focus);
		else
			focal.push_back(inter.Cams.CamStruct.center.z + inter.Cams.CamStruct.focus);
		for (size_t i = 0; i < inter.ShapeList.size(); i++)
		{
			if (inter.ShapeList[i].Shps.type == "sphere")
			{
				sphereintersect(i,screen,focal);
			}
			else
			{
				planeintersect(i, screen, focal);
			}
		}
	}
	void findshadow()
	{
		std::vector<double> origin;
		std::vector<double> dest;
		double scale = 0;
		origin.push_back(pix.shadow.x);
		origin.push_back(pix.shadow.y);
		origin.push_back(pix.shadow.z);
		for (size_t j = 0; j < inter.LightList.size(); j++)
		{
			dest.push_back(inter.LightList[j].lite.location.x);
			dest.push_back(inter.LightList[j].lite.location.y);
			dest.push_back(inter.LightList[j].lite.location.z);
			for (size_t i = 0; i < inter.ShapeList.size(); i++)
			{
				if (inter.ShapeList[i].Shps.type == "sphere")
				{
					sphereintersect(i, origin, dest);
					double nx = (origin[0] - inter.ShapeList[i].Shps.center.x) / inter.ShapeList[i].Shps.radius;
					double ny = (origin[1] - inter.ShapeList[i].Shps.center.y) / inter.ShapeList[i].Shps.radius;
					double nz = (origin[2] - inter.ShapeList[i].Shps.center.y) / inter.ShapeList[i].Shps.radius;
					std::vector<double> Normal{ nx,ny,nz };
					std::vector<double> shadow{ dest[0] - origin[0],dest[1] - origin[1],dest[2] - origin[2] };
					scale = dot_product(Normal, shadow) * inter.ShapeList[i].Shps.lambert;
					if (scale < 0)
						scale = 0;
				}
				else
				{
					planeintersect(i, origin, dest);
				}
				if (pix.shadow.collisions.back())
				{
					pix.shadow.collisions.pop_back();
					pix.pip.r = pix.pip.r + 0;
					pix.pip.g = pix.pip.g + 0;
					pix.pip.b = pix.pip.b + 0;
				}
				else
				{
					pix.shadow.collisions.pop_back();
					pix.pip.r = pix.pip.r + (scale *inter.LightList[j].lite.intensity * inter.ShapeList[i].Shps.color.r);
					pix.pip.g = pix.pip.g + (scale *inter.LightList[j].lite.intensity * inter.ShapeList[i].Shps.color.g);
					pix.pip.b = pix.pip.b + (scale *inter.LightList[j].lite.intensity * inter.ShapeList[i].Shps.color.b);
				}
			}
		}
	}
	void sphereintersect(int i, std::vector<double> origin, std::vector<double> destination)
	{
		double A, B, C, D;
		double xd = origin[0] - destination[0];
		double yd = origin[1] - destination[1];
		double zd = origin[2] - destination[2];
		double ox = origin[0];
		double oy = origin[1];
		double oz = origin[2];
		double rad = inter.ShapeList[i].Shps.radius;
		double sy = inter.ShapeList[i].Shps.center.y;
		double sx = inter.ShapeList[i].Shps.center.x;
		double sz = inter.ShapeList[i].Shps.center.z;
		A = (xd*xd) + (yd*yd) + (zd*zd);
		B = 2 * (xd*(ox - sx) + yd*(oy - sy) + zd*(oz - sz));
		C = ((ox - sx)*(ox - sx)) + ((oy - sy)*(oy - sy)) + ((oz - sz)*(oz - sz)) - (rad*rad);
		D = B*B - 4 * A * C;
		if (D >= 0) {
			if (!ShadowSearch)
			{
				double T0, T1, Rix, Riy, Riz;
				T0 = ((-B - sqrt(D)) / (2 * A));
				if (T0 < 0)
				{
					T1 = ((-B + sqrt(D)) / (2 * A));
					Rix = ox + xd*T1;
					Riy = oy + yd*T1;
					Riz = oz + zd*T1;
				}
				else
				{
					Rix = ox + xd*T0;
					Riy = oy + yd*T0;
					Riz = oz + zd*T0;
				}
				pix.shadow.x = (Rix - sx) / rad;
				pix.shadow.y = (Riy - sy) / rad;
				pix.shadow.z = (Riz - sz) / rad;
				ShadowSearch = true;
				findshadow();
			}
			else
			{
				pix.shadow.collisions.push_back(true);
			}
		}
		else
		{
			if (ShadowSearch)
				pix.shadow.collisions.push_back(false);
		}
	}
	void planeintersect(int i, std::vector<double> origin, std::vector<double> destination)
	{
		std::vector<double> ray{ destination[0] - origin[0], destination[1] - origin[1], destination[2] - origin[2] };
		std::vector<double> normal{ inter.ShapeList[i].Shps.normal.x,inter.ShapeList[i].Shps.normal.y ,inter.ShapeList[i].Shps.normal.z };
		double denom = dot_product(normal, ray);
		if (fabs(denom) > 0.0001f)
		{
			std::vector<double> another{ inter.ShapeList[i].Shps.center.x - origin[0],inter.ShapeList[i].Shps.center.y - origin[1] ,inter.ShapeList[i].Shps.center.z - origin[2] };
			double t = dot_product(another, normal) / denom;
			double Rix, Riy, Riz;
			if (t >= 0)
			{
				Rix = origin[0] + ray[0] * t;
				Riy = origin[1] + ray[1] * t;
				Riz = origin[2] + ray[2] * t;
				if (!ShadowSearch)
				{
					pix.shadow.x = (Rix - inter.ShapeList[i].Shps.center.x);
					pix.shadow.y = (Riy - inter.ShapeList[i].Shps.center.y);
					pix.shadow.z = (Riz - inter.ShapeList[i].Shps.center.z);
					ShadowSearch = true;
					findshadow();
				}
				else
				{
					pix.shadow.collisions.push_back(true);
				}
			}
			else
				pix.shadow.collisions.push_back(false);
		}
	}
	double dot_product(std::vector<double> one, std::vector<double> two)
	{
		return std::inner_product(one.begin(), one.end(), two.begin(), 0.0);
	}



	interpreter inter;
	int pos;
	bool ShadowSearch;
	Pixall pix;
};

#endif // !TSWIM_H
