#include <interpreter.h>

interpreter::interpreter() {
	
}

interpreter interpreter::fromJSON(QString File) {
	QJsonParseError error;
	QFile file(File);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
		std::cerr << "Error : Could not open input JSON file" << std::endl;
		throw error;
	}
	QString json = file.readAll();
	file.close();
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8(), &error);
	interpreter newinterpreter;
	if (error.error != QJsonParseError::NoError) {
		std::cerr << "Error : Input file could not be parsed as a JSON" << std::endl;
		throw error;
	}
	QJsonObject obj = jsonDoc.object();
	QJsonObject parser;
	QJsonValue camra = obj["camera"];
	if (camra.isObject())	{
		try { cameraParse(camra); }
		catch (QJsonParseError & error) { throw error; }
		newinterpreter.Cams = Cams;
		
	}
	QJsonValue lites = obj["lights"];
	if (lites.isArray())	{
		try { lightParse(lites); }
		catch (QJsonParseError & error) { throw error;}
		newinterpreter.LightList = LightList;
	}
	QJsonValue shpes = obj["objects"];
	if (shpes.isArray())	{
		try { shapeParse(shpes); }
		catch (QJsonParseError & error) { throw error; }
		newinterpreter.ShapeList = ShapeList;
	}
	if (!camra.isObject() || !lites.isArray() || !shpes.isArray()){
		std::cerr << "Error : Camera, lights, or shapes are using incorrect format. " << std::endl;
		throw error;
	}
	return newinterpreter;
}

bool interpreter::lightParse(QJsonValue lghtArray) {
	QJsonArray parser2 = lghtArray.toArray();
	QJsonParseError error;
	int x = 0;
	for (QJsonValue LightData : parser2){
		QJsonObject LightObjs = LightData.toObject();
		QJsonObject locate = LightObjs["location"].toObject();
		if (!locate["x"].isDouble() || !locate["y"].isDouble() || !locate["z"].isDouble()){
			std::cerr << "Error : the light location is not using doubles" << std::endl;
		}
		double intense = LightObjs["intensity"].toDouble();
		if (intense < 0)	{
			std::cerr << "Error : The light intensity is negative" << std::endl;
			throw error;
		}
		lights Shine(intense, locate);
		LightList.push_back(Shine);
		x = x++;
	}
	return true;
}

bool interpreter::shapeParse(QJsonValue ShpArray) {
	QJsonArray parser3 = ShpArray.toArray();
	QJsonParseError error;
	int x = 0;
	for (QJsonValue ShapesData : parser3)	{
		QJsonObject ShapeObjs = ShapesData.toObject();
		QString type = ShapeObjs["type"].toString();
		QJsonObject center = ShapeObjs["center"].toObject();
		if (!center["x"].isDouble() || !center["y"].isDouble() || !center["z"].isDouble())		{
			std::cerr << "Error : The center coordinates are not doubles" << std::endl;
			throw error;
		}
		double lambert = ShapeObjs["lambert"].toDouble();
		if (lambert > 1 || lambert < 0)		{
			std::cerr << "Error : the lambert is not within [0,1]" << std::endl;
			throw error;
		}
		QJsonObject normal = ShapeObjs["normal"].toObject();
		double radius = ShapeObjs["radius"].toDouble();
		QJsonObject rgb = ShapeObjs["color"].toObject();
		if ((0<= rgb["x"].toDouble() <= 255 )|| (0 <= rgb["y"].toDouble()<= 255 )||( 0<= rgb["z"].toDouble()<=255))	{
			if (type == "sphere") {
				std::string version = type.toStdString();
				if (radius < 0) {
					std::cerr << "Error : The radius is a negative number" << std::endl;
					throw error;
				}
				shapes round(version, center, rgb, radius, lambert);
				ShapeList.push_back(round);
			}
			else if (type == "plane") {
				std::string version = type.toStdString();
				if (!normal["x"].isDouble() || !normal["y"].isDouble() || !normal["z"].isDouble()) {
					std::cerr << "Error : Normal Coordinates are not doubles" << std::endl;
					throw error;
				}
				shapes flat(version, center, normal, rgb, lambert);
				ShapeList.push_back(flat);
			}
		}
		else	{
			std::cerr << "Error : RGB Colors are not doubles" << std::endl;
			throw error;
		}
		x++;
	}
	return true;
}

bool interpreter::cameraParse(QJsonValue CamJect) {
	QJsonObject CamObjs = CamJect.toObject();
	QJsonObject Center = CamObjs["center"].toObject();
	if (!Center["x"].isDouble() || !Center["y"].isDouble() || !Center["z"].isDouble())
	{
		std::cerr << "Error : Center coordinates are not doubles" << std::endl;
	}
	QJsonObject Normal = CamObjs["normal"].toObject();
	if (!Normal["x"].isDouble() || !Normal["y"].isDouble() || !Normal["z"].isDouble())
	{
		std::cerr << "Error : Normal Coordinates are not doubles" << std::endl;
	}
	if (!CamObjs["focus"].isDouble())
	{
		std::cerr << "Error : Focus is not a double" << std::endl;
	}
	double focus = CamObjs["focus"].toDouble();
	QJsonArray Size = CamObjs["size"].toArray();
	if (!Size[0].isDouble() || !Size[1].isDouble())
	{
		std::cerr << "Error : Size values are not integers" << std::endl;
	}
	QJsonArray Resolution = CamObjs["resolution"].toArray();
	if (!Resolution[0].isDouble() || !Resolution[1].isDouble())
	{
		std::cerr << "Error : Resolution are not doubles" << std::endl;
	}
	camera Camera(Center,Normal,focus,Size,Resolution);
	Cams = Camera;
	return true;
}

interpreter & interpreter::operator=(const interpreter& s) {
	// check for self-assignment
	if (&s == this)
		return *this;
	Cams = s.Cams;
	LightList = s.LightList;
	ShapeList = s.ShapeList;
	return *this;
}

void interpreter::Eval(QString File) {

}