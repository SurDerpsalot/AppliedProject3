#include <interpreter.h>

interpreter::interpreter() {
	
}
interpreter::~interpreter() {

}
interpreter interpreter::fromJSON(QString File) {
	QJsonParseError error;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(File.toUtf8(), &error);
	interpreter newinterpreter;
	if (error.error != QJsonParseError::NoError) {
		throw error;
	}
	QJsonObject obj = jsonDoc.object();
	QJsonObject parser;
	QJsonArray parser2;
	QJsonValue camra = obj["camera"];
	if (camra.isObject())
	{
		parser = camra.toObject();
		
	}
	QJsonValue lites = obj["lights"];
	if (lites.isArray())
	{
		parser2 = lites.toArray();
	}
	QJsonValue shpes = obj["objects"];
	if (shpes.isArray())
	{
		shapeParse(shpes);
		newinterpreter.ShapeList = ShapeList;
	}

	return newinterpreter;
}

bool interpreter::shapeParse(QJsonValue ShpArray) {
	QJsonArray parser3 = ShpArray.toArray();
	for (QJsonValue ShapesData : parser3) {
		QJsonObject ShapeObjs = ShapesData.toObject();
		QString type = ShapeObjs["type"].toString();
		QJsonObject center = ShapeObjs["center"].toObject();
		double lambert = ShapeObjs["lambert"].toDouble();
		QJsonObject normal = ShapeObjs["normal"].toObject();
		double radius = ShapeObjs["radius"].toDouble();
		QJsonObject rgb = ShapeObjs["color"].toObject();
		if (type == "sphere")
		{
			std::string version = type.toStdString();
			shapes round(version, center, rgb,radius,lambert);
			ShapeList.push_back(round);
		}
		else if (type == "plane")
		{
			std::string version = type.toStdString();
			shapes flat(version, center, normal, rgb, lambert);
			ShapeList.push_back(flat);
		}
	}
	return true;
}





interpreter & interpreter::operator=(const interpreter& s) {
	// check for self-assignment
	if (&s == this)
		return *this;

	Interp.Cam = s.Interp.Cam;
	Interp.Light = s.Interp.Light;
	Interp.Shape = s.Interp.Shape;
	return *this;
}