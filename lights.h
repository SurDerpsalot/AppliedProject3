#ifndef LIGHTS_H
#define LIGHTS_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <vector>

class lights {
public:
	lights();
	~lights();
	struct loc {
		double x;
		double y;
		double x;
	};
	struct LightSources {
		double intensity;
		loc location;
	};
	std::vector<LightSources> LightList;

};


#endif // !LIGHTS_H
