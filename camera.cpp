#include <camera.h>

camera::camera() {
	cam Camra;
	Camra.center.x = 0;
	Camra.center.y = 0;
	Camra.center.z = 0;
	Camra.normal.y = 0;
	Camra.normal.z = 0;
	Camra.focus = 0;
	Camra.size[0] = 0;
	Camra.size[1] = 0;
	Camra.resolution[0] = 0;
	Camra.resolution[1] = 0;
}

camera::camera(QJsonObject middle, QJsonObject norm, double foc, QJsonArray sz, QJsonArray res) {
	cam Camra;
	Camra.center.x = middle["x"].toDouble();
	Camra.center.y = middle["y"].toDouble();
	Camra.center.z = middle["z"].toDouble();
	Camra.normal.x = norm["x"].toDouble();
	Camra.normal.y = norm["y"].toDouble();
	Camra.normal.z = norm["z"].toDouble();
	Camra.focus = foc;
	Camra.size[0] = sz[0].toInt();
	Camra.size[1] = sz[1].toInt();
	Camra.resolution[0] = res[0].toDouble();
	Camra.resolution[1] = res[1].toDouble();
}