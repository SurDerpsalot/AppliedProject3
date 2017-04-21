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

	bool parse(QJsonDocument File);


};




#endif // !INTERPRETER_H
