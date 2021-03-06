#ifndef SHAPES_H
#define SHAPES_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <vector>
#include <iostream>

class shapes {
public:
	struct loc {
		double x;
		double y;
		double z;
	};
	struct RGB {
		int r;
		int g;
		int b;
	};
	struct obj {
		std::string type;
		loc center;
		loc normal;
		double radius;
		double lambert;
		RGB color;
	};
	
	shapes();
	shapes(std::string version, QJsonObject middle, QJsonObject rgb,double rad, double lamb);
	shapes(std::string version, QJsonObject middle, QJsonObject norm, QJsonObject rgb, double lamb);
	
	obj Shps;
};





#endif // !SHAPES_H