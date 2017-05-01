#ifndef PIXELSTRUCT_H
#define PIXELSTRUCT_H
#include <QImage>


class Pixall {
public:
	struct PixStruct {
		double x;
		double y;
		uint rgb;
		double r;
		double g;
		double b;
	};
	struct shadowStruct {
		double x;
		double y;
		double z;
		int index;
		std::vector<bool> collisions;
		std::string type;
	};
	Pixall() {
		pip.x = 0;
		pip.y = 0;
		pip.r = 0;
		pip.g = 0;
		pip.b = 0;
		pip.rgb = 0;
		shadow.x = 0;
		shadow.y = 0;
		shadow.z = 0;
		shadow.index = 0;
	}
	Pixall(double X, double Y, uint RGB) { 
		pip.x = X;
		pip.y = Y;
		pip.rgb = RGB;
	}
	PixStruct pip;
	shadowStruct shadow;
};


#endif // !PIXELSTRUCT_H

