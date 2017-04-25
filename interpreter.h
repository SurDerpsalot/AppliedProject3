#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QString>
#include <camera.h>
#include <lights.h>
#include <shapes.h>
#include <QFile>

class interpreter {
public:
	
	interpreter();

	interpreter interpreter::fromJSON(QString File);

	interpreter & interpreter::operator=(const interpreter& s);
	
	bool interpreter::shapeParse(QJsonValue ShpArray);
	bool interpreter::lightParse(QJsonValue LghtArray);
	bool interpreter::cameraParse(QJsonValue CamJect);

	camera Cams;
	std::vector<shapes> ShapeList;
	std::vector<lights> LightList;
	bool badCamera;
	bool badLight;
	bool badShape;
};




#endif // !INTERPRETER_H
