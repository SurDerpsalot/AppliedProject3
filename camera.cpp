#include <camera.h>

camera::camera() {
	CamStruct.center.x = 0;
	CamStruct.center.y = 0;
	CamStruct.center.z = 0;
	CamStruct.normal.y = 0;
	CamStruct.normal.z = 0;
	CamStruct.focus = 0;
	CamStruct.size[0] = 0;
	CamStruct.size[1] = 0;
	CamStruct.resolution[0] = 0;
	CamStruct.resolution[1] = 0;
}

camera::camera(QJsonObject middle, QJsonObject norm, double foc, QJsonArray sz, QJsonArray res) {
	CamStruct.center.x = middle["x"].toDouble();
	CamStruct.center.y = middle["y"].toDouble();
	CamStruct.center.z = middle["z"].toDouble();
	CamStruct.normal.x = norm["x"].toDouble();
	CamStruct.normal.y = norm["y"].toDouble();
	CamStruct.normal.z = norm["z"].toDouble();
	CamStruct.focus = foc;
	CamStruct.size[0] = sz[0].toInt();
	CamStruct.size[1] = sz[1].toInt();
	CamStruct.resolution[0] = res[0].toDouble();
	CamStruct.resolution[1] = res[1].toDouble();
}