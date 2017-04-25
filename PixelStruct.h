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
	Pixall() {}
	Pixall(double X, double Y, uint RGB) { 
		pip.x = X;
		pip.y = Y;
		pip.rgb = RGB;
	}
	PixStruct pip;
};


#endif // !PIXELSTRUCT_H

