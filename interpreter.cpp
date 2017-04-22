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
	QJsonValue camra = obj["camera"];
	if (camra.isObject())
	{
		cameraParse(camra);
		newinterpreter.Cam = Cam;
	}
	QJsonValue lites = obj["lights"];
	if (lites.isArray())
	{
		lightParse(lites);
		newinterpreter.LightList = LightList;
	}
	QJsonValue shpes = obj["objects"];
	if (shpes.isArray())
	{
		shapeParse(shpes);
		newinterpreter.ShapeList = ShapeList;
	}
	return newinterpreter;
}

bool interpreter::lightParse(QJsonValue lghtArray) {
	QJsonArray parser2 = lghtArray.toArray();
	for (QJsonValue LightData : parser2)
	{
		QJsonObject LightObjs = LightData.toObject();
		QJsonObject locate = LightObjs["location"].toObject();
		double intense = LightObjs["intensity"].toDouble();
		lights Shine(intense, locate);
		LightList.push_back(Shine);
	}
	return true;
}

bool interpreter::shapeParse(QJsonValue ShpArray) {
	QJsonArray parser3 = ShpArray.toArray();
	for (QJsonValue ShapesData : parser3) 
	{
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

bool interpreter::cameraParse(QJsonValue CamJect) {
	QJsonObject CamObjs = CamJect.toObject();
	QJsonObject Center = CamObjs["center"].toObject();
	QJsonObject Normal = CamObjs["normal"].toObject();
	double focus = CamObjs["focus"].toDouble();
	QJsonArray Size = CamObjs["size"].toArray();
	QJsonArray Resolution = CamObjs["resolution"].toArray();
	camera Camera(Center,Normal,focus,Size,Resolution);
	Cam = Camera;
	return true;
}

interpreter & interpreter::operator=(const interpreter& s) {
	// check for self-assignment
	if (&s == this)
		return *this;

	Cam = s.Cam;
	LightList = s.LightList;
	ShapeList = s.ShapeList;
	return *this;
}