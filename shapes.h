#ifndef SHAPES_H
#define SHAPES_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <vector>


class shapes {
public:
	shapes();
	~shapes();
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
		bool plain;
		loc center;
		loc normal;
		double radius;
		double lambert;
		RGB color;
	};
	std::vector<obj> ShapeList;

};





#endif // !SHAPES_H