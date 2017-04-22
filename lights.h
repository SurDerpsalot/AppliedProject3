#ifndef LIGHTS_H
#define LIGHTS_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <vector>
#include <iostream>

class lights {
public:
	struct loc {
		double x;
		double y;
		double z;
	};
	struct LightSources {
		double intensity;
		loc location;
	};
	LightSources lite;
	lights();
	lights(double intense, QJsonObject locate);
	


};


#endif // !LIGHTS_H
