#ifndef CAMERA_H
#define CAMERA_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <array>
#include <iostream>



class camera {
public:
	struct loc {
		double x;
		double y;
		double z;
	};
	struct cam {
		loc center;
		loc normal;
		double focus;
		std::array<int, 2> size;
		std::array<double, 2>	resolution;
	};
	camera();
	camera(QJsonObject middle, QJsonObject norm, double foc, QJsonArray sz, QJsonArray res);
	cam CamStruct;

};

#endif // !CAMERA_H

