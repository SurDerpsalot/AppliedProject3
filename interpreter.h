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


class interpreter {
public:
	
	interpreter();
	
	~interpreter();

	interpreter interpreter::fromJSON(QString File);

	interpreter & interpreter::operator=(const interpreter& s);
	
	bool interpreter::shapeParse(QJsonValue ShpArray);

private:
	camera Cam;
	lights Light;
	std::vector<shapes> ShapeList;
};




#endif // !INTERPRETER_H
