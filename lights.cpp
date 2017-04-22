#include <lights.h>

lights::lights(){
	lite.intensity = 0;
	lite.location.x = 0;
	lite.location.y = 0;
	lite.location.z = 0;
}

lights::lights(double intense, QJsonObject locate) {
	
	lite.intensity = intense;
	lite.location.x = locate["x"].toDouble();
	lite.location.y = locate["y"].toDouble();
	lite.location.z = locate["z"].toDouble();
}