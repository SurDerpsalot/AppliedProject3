#include <shapes.h>

shapes::shapes()
{
	Shps.center.x = 0;
	Shps.center.y = 0;
	Shps.center.z = 0;
	Shps.color.r = 0;
	Shps.color.g = 0;
	Shps.color.b = 0;
	Shps.lambert = 0;
	Shps.normal.x = 0;
	Shps.normal.y = 0;
	Shps.normal.z = 0;
	Shps.type = "neither";
	Shps.radius = 0;
}

shapes::shapes(std::string version, QJsonObject middle, QJsonObject rgb, double rad, double lamb) {
	Shps.center.x = middle["x"].toDouble();
	Shps.center.y = middle["y"].toDouble();
	Shps.center.z = middle["z"].toDouble();
	Shps.color.r = rgb["r"].toInt();
	Shps.color.g = rgb["g"].toInt();
	Shps.color.b = rgb["b"].toInt();
	Shps.lambert = lamb;
	Shps.normal.x = 0;
	Shps.normal.y = 0;
	Shps.normal.z = 0;
	Shps.type = version;
	Shps.radius = rad;
}

shapes::shapes(std::string version, QJsonObject middle, QJsonObject norm, QJsonObject rgb, double lamb) {
	Shps.center.x = middle["x"].toDouble();
	Shps.center.y = middle["y"].toDouble();
	Shps.center.z = middle["z"].toDouble();
	Shps.color.r = rgb["r"].toInt();
	Shps.color.g = rgb["g"].toInt();
	Shps.color.b = rgb["b"].toInt();
	Shps.lambert = lamb;
	Shps.normal.x = norm["x"].toDouble();
	Shps.normal.y = norm["y"].toDouble();
	Shps.normal.z = norm["z"].toDouble();
	Shps.type = version;
	Shps.radius = 0;
}