#ifndef CAMERA_H
#define CAMERA_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <array>



class camera {
public:
	camera();
	~camera();
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

};


#endif // !CAMERA_H

