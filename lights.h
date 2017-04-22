#ifndef LIGHTS_H
#define LIGHTS_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <vector>

class lights {
public:
	lights();
	lights(double intense, QJsonObject locate);
	struct loc {
		double x;
		double y;
		double z;
	};
	struct LightSources {
		double intensity;
		loc location;
	};

};


#endif // !LIGHTS_H
