#include <interpreter.h>

interpreter::interpreter() {
	badCamera = false;
	badLight = false;
	badShape = false;
}

interpreter interpreter::fromJSON(QString File) {
	QJsonParseError error;
	QFile file(File);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
		std::cerr << "Error : Could not open input JSON file" << std::endl;
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
		badCamera = !cameraParse(camra);
		if(!badCamera)
			newinterpreter.Cams = Cams;
		
	}
	QJsonValue lites = obj["lights"];
	if (lites.isArray())	{	
		badLight = !lightParse(lites);
		if(!badLight)
			newinterpreter.LightList = LightList;
	}
	QJsonValue shpes = obj["objects"];
	if (shpes.isArray())	{
		badShape = !shapeParse(shpes);
		if(!badShape)
			newinterpreter.ShapeList = ShapeList;
	}
	if (!camra.isObject() || !lites.isArray() || !shpes.isArray()){
		std::cerr << "Error : Camera, lights, or shapes are using incorrect format. " << std::endl;
		throw error;
	}
	if (badShape || badLight || badCamera)
	{
		throw error;
	}
	return newinterpreter;
}

bool interpreter::lightParse(QJsonValue LghtArray) {
	QJsonArray parser2 = LghtArray.toArray();
	int x = 0;
	bool exit = true;
	for (QJsonValue LightData : parser2){
		QJsonObject LightObjs = LightData.toObject();
		QJsonObject locate = LightObjs["location"].toObject();
		if (!locate["x"].isDouble() || !locate["y"].isDouble() || !locate["z"].isDouble()){
			std::cerr << "Error : the light location is not using doubles" << std::endl;
			exit = false;
		}
		double intense = LightObjs["intensity"].toDouble();
		if (intense < 0)	{
			std::cerr << "Error : The light intensity is negative" << std::endl;
			exit = false;
		}
		lights Shine(intense, locate);
		LightList.push_back(Shine);
		x = x++;
	}
	return exit;
}

bool interpreter::shapeParse(QJsonValue ShpArray) {
	QJsonArray parser3 = ShpArray.toArray();
	int x = 0;
	bool exit = true;
	for (QJsonValue ShapesData : parser3)	{
		QJsonObject ShapeObjs = ShapesData.toObject();
		QString type = ShapeObjs["type"].toString();
		QJsonObject center = ShapeObjs["center"].toObject();
		if (!center["x"].isDouble() || !center["y"].isDouble() || !center["z"].isDouble())		{
			std::cerr << "Error : The center coordinates are not doubles" << std::endl;
			exit = false;
		}
		double lambert = ShapeObjs["lambert"].toDouble();
		if (lambert > 1 || lambert < 0)		{
			std::cerr << "Error : the lambert is not within [0,1]" << std::endl;
		}
		QJsonObject normal = ShapeObjs["normal"].toObject();
		double radius = ShapeObjs["radius"].toDouble();
		QJsonObject rgb = ShapeObjs["color"].toObject();
		bool color = rgbchecker(rgb);
		if (color)	{
			if (type == "sphere") {
				std::string version = type.toStdString();
				if (radius < 0) {
					std::cerr << "Error : The radius is a negative number" << std::endl;
					exit = false;
				}
				shapes round(version, center, rgb, radius, lambert);
				ShapeList.push_back(round);
			}
			else if (type == "plane") {
				std::string version = type.toStdString();
				if (!normal["x"].isDouble() || !normal["y"].isDouble() || !normal["z"].isDouble()) {
					std::cerr << "Error : Normal Coordinates are not doubles" << std::endl;
					exit = false;
				}
				shapes flat(version, center, normal, rgb, lambert);
				ShapeList.push_back(flat);
			}
		}
		else	{
			std::cerr << "Error : RGB Colors are out of range" << std::endl;
			exit = false;
		}
		x++;
	}
	return exit;
}

bool interpreter::cameraParse(QJsonValue CamJect) {
	QJsonObject CamObjs = CamJect.toObject();
	QJsonObject Center = CamObjs["center"].toObject();
	bool exit = true;
	if (!Center["x"].isDouble() || !Center["y"].isDouble() || !Center["z"].isDouble())
	{
		std::cerr << "Error : Center coordinates are not doubles" << std::endl;
		exit = false;
	}
	QJsonObject Normal = CamObjs["normal"].toObject();
	if (!Normal["x"].isDouble() || !Normal["y"].isDouble() || !Normal["z"].isDouble())
	{
		std::cerr << "Error : Normal Coordinates are not doubles" << std::endl;
		exit = false;
	}
	if (!CamObjs["focus"].isDouble())
	{
		std::cerr << "Error : Focus is not a double" << std::endl;
		exit = false;
	}
	double focus = CamObjs["focus"].toDouble();
	QJsonArray Size = CamObjs["size"].toArray();
	if (!Size[0].isDouble() || !Size[1].isDouble())
	{
		std::cerr << "Error : Size values are not integers" << std::endl;
		exit = false;
	}
	QJsonArray Resolution = CamObjs["resolution"].toArray();
	if (!Resolution[0].isDouble() || !Resolution[1].isDouble())
	{
		std::cerr << "Error : Resolution are not doubles" << std::endl;
		exit = false;
	}
	camera Camera(Center,Normal,focus,Size,Resolution);
	Cams = Camera;
	return exit;
}



interpreter & interpreter::operator=(const interpreter& s) {
	// check for self-assignment
	if (&s == this) { return *this; }
	Cams = s.Cams;
	LightList = s.LightList;
	ShapeList = s.ShapeList;
	badCamera = s.badCamera;
	badLight = s.badLight;
	badShape = s.badShape;
	return *this;
}

bool interpreter::rgbchecker(QJsonObject rgb) {
	int red = rgb["r"].toInt();
	int green = rgb["g"].toInt();
	int blue = rgb["b"].toInt();
	bool good = true;
	if (red < 0 || red > 255) {
		good = false;
	}
	if (green < 0 || green > 255) {
		good = false;
	}
	if (blue < 0 || blue > 255) {
		good = false;
	}
	return good;
}