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

	interpreter fromJSON(QString File);

	interpreter & operator=(const interpreter& s);
	
	bool shapeParse(QJsonValue ShpArray);
	bool lightParse(QJsonValue LghtArray);
	bool cameraParse(QJsonValue CamJect);
	bool rgbchecker(QJsonObject rgb);
	camera Cams;
	std::vector<shapes> ShapeList;
	std::vector<lights> LightList;
	bool badCamera;
	bool badLight;
	bool badShape;
};




#endif // !INTERPRETER_H
