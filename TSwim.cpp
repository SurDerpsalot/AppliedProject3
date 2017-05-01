#include <TSwim.h>	

Calculate::Calculate(interpreter interp, int position) {
		inter = interp;
		pos = position;
		ShadowSearch = false;
		pix.pip.r = 0;
		pix.pip.g = 0;
		pix.pip.b = 0;
	}

	void Calculate::run() {
		std::vector<double> screen;
		std::vector<double> focal;
		screen.push_back((inter.Cams.CamStruct.resolution[0] * ((pos % inter.Cams.CamStruct.size[1]) - (inter.Cams.CamStruct.size[0] / 2))));
		screen.push_back((inter.Cams.CamStruct.resolution[1] * ((pos / inter.Cams.CamStruct.size[0]) - (inter.Cams.CamStruct.size[1] / 2))));
		screen.push_back(inter.Cams.CamStruct.center.z);
		focal.push_back(inter.Cams.CamStruct.center.x);
		focal.push_back(inter.Cams.CamStruct.center.y);
		if (inter.Cams.CamStruct.normal.z > 0) {
			focal.push_back(inter.Cams.CamStruct.center.z - inter.Cams.CamStruct.focus);
		}
		else {
			focal.push_back(inter.Cams.CamStruct.center.z + inter.Cams.CamStruct.focus);
		}
		for (size_t i = 0; i < inter.ShapeList.size(); i++)
		{
			if (inter.ShapeList[i].Shps.type == "sphere")
			{
				sphereintersect(i, screen, focal);
			}
			else
			{
				//		std::cout << "what does segmentation fault even mean" << std::endl;
				planeintersect(i, screen, focal);
			}
		}
	}

	void Calculate::sphereintersect(int i, std::vector<double> origin, std::vector<double> destination)
	{
		double A, B, C, D;
		double xd = origin[0] - destination[0];
		double yd = origin[1] - destination[1];
		double zd = origin[2] - destination[2];
		double ox = origin[0];
		double oy = origin[1];
		double oz = origin[2];
		//	std::cout << "X: " << origin[0] << " Y: " << origin[1] << " Z: " <<origin[2] << std::endl;
		double rad = inter.ShapeList[i].Shps.radius;
		double sy = inter.ShapeList[i].Shps.center.y;
		double sx = inter.ShapeList[i].Shps.center.x;
		double sz = inter.ShapeList[i].Shps.center.z;
		A = (xd*xd) + (yd*yd) + (zd*zd);
		B = 2 * (xd*(ox - sx) + yd*(oy - sy) + zd*(oz - sz));
		C = ((ox - sx)*(ox - sx)) + ((oy - sy)*(oy - sy)) + ((oz - sz)*(oz - sz)) - (rad*rad);
		D = B*B - 4 * A * C;
		if (D >= 0) {
			if (!ShadowSearch) {
				double T0, T1, Rix, Riy, Riz;
				T0 = ((-B - sqrt(D)) / (2 * A));
				if (T0 < 0)
				{
					T1 = ((-B + sqrt(D)) / (2 * A));
					Rix = ox + xd*T1;
					Riy = oy + yd*T1;
					Riz = oz + zd*T1;
				}
				else
				{
					Rix = ox + xd*T0;
					Riy = oy + yd*T0;
					Riz = oz + zd*T0;
				}
				//std::cout << "hit" << std::endl;
				if (pix.shadow.type == "no hit")
				{
					pix.shadow.x = (Rix);// -sx) / rad;
					pix.shadow.y = (Riy);// -sy) / rad;
					pix.shadow.z = (Riz);// -sz) / rad;
					pix.shadow.type = "hit";
					pix.shadow.index = i;
					ShadowSearch = true;
					findshadow(i);
				}
				else
				{
					std::vector<double> distOld{ pix.shadow.x - origin[0], pix.shadow.y - origin[1], pix.shadow.z - origin[2] };
					std::vector<double> distNew{ Rix - origin[0], pix.shadow.y - origin[1], pix.shadow.z - origin[2] };
					int old = 0;
					int notold = 0;
					if (distOld[0] > distNew[0]) { old++; }
					else { notold++; }
					if (distOld[1] > distNew[1]) { old++; }
					else { notold++; }
					if (distOld[2] > distNew[2]) { old++; }
					else { notold++; }
					if (notold > old) {
						pix.shadow.x = Rix;
						pix.shadow.y = Riy;
						pix.shadow.z = Riz;
						pix.shadow.index = i;
						ShadowSearch = true;
						findshadow(i);
					}
				}
			}
			else { pix.shadow.collisions.push_back(true); }
		}
		else {
			if (ShadowSearch)
				pix.shadow.collisions.push_back(false);
			else
				pix.shadow.type = "no hit";
		}
	}

	void Calculate::planeintersect(int i, std::vector<double> origin, std::vector<double> destination)
	{
		std::vector<double> ray{ origin[0] - destination[0], origin[1] - destination[1], origin[2] - destination[2] };
		std::vector<double> normal{ inter.ShapeList[i].Shps.normal.x,inter.ShapeList[i].Shps.normal.y ,inter.ShapeList[i].Shps.normal.z };
		double denom = dot_product(normal, ray);
		//std::cout << "first dot product, yay" << std::endl;
		if (denom > 0 || denom < 0)
		{
			//std::cout << "made it into the if statement" << std::endl;
			std::vector<double> another{ inter.ShapeList[i].Shps.center.x - origin[0],inter.ShapeList[i].Shps.center.y - origin[1] ,inter.ShapeList[i].Shps.center.z - origin[2] };
			double t = dot_product(normal, another) / denom;
			double Rix, Riy, Riz;
			if (t >= 0)
			{
				Rix = origin[0] + ray[0] * t;
				Riy = origin[1] + ray[1] * t;
				Riz = origin[2] + ray[2] * t;
					if (!ShadowSearch)
					{
						if (pix.shadow.type == "no hit") {
							pix.shadow.x = (Rix);
							pix.shadow.y = (Riy);
							pix.shadow.z = (Riz);
							pix.shadow.type = "hit";
							pix.shadow.index = i;
							ShadowSearch = true;
							findshadow(i);
						}
						else
						{
							std::vector<double> distOld{pix.shadow.x - origin[0], pix.shadow.y - origin[1], pix.shadow.z - origin[2]};
							std::vector<double> distNew{Rix-origin[0], pix.shadow.y - origin[1], pix.shadow.z - origin[2]};
							int old = 0;
							int notold = 0;
							if (distOld[0] > distNew[0]) {	old++;	}
							else { notold++; }
							if (distOld[1] > distNew[1]) { old++; }
							else { notold++; }
							if (distOld[2] > distNew[2]) { old++; }
							else { notold++; }
							if (notold > old)	{
								pix.shadow.x = Rix;
								pix.shadow.y = Riy;
								pix.shadow.z = Riz;
								pix.shadow.index = i;
								ShadowSearch = true;
								findshadow(i);
							}
						}
					}
					else
					{
						pix.shadow.collisions.push_back(true);
					}
			}
			else
			{
				if (ShadowSearch)
					pix.shadow.collisions.push_back(false);
				else
					pix.shadow.type = "no hit";
			}

		}
		else
		{
			if (ShadowSearch)
				pix.shadow.collisions.push_back(false);
			else
				pix.shadow.type = "no hit";
		}
	}

	void Calculate::findshadow(int i)
	{
		std::vector<double> origin;
		std::vector<double> dest;
		double scale = 0;
		double nx = 0;
		double ny = 0;
		double nz = 0;
		origin.push_back((pix.shadow.x - inter.ShapeList[i].Shps.center.x) / inter.ShapeList[i].Shps.radius);
		origin.push_back((pix.shadow.y - inter.ShapeList[i].Shps.center.y) / inter.ShapeList[i].Shps.radius);
		origin.push_back((pix.shadow.z - inter.ShapeList[i].Shps.center.z) / inter.ShapeList[i].Shps.radius);
		for (size_t j = 0; j < inter.LightList.size(); j++)
		{
			dest.push_back(inter.LightList[j].lite.location.x);
			dest.push_back(inter.LightList[j].lite.location.y);
			dest.push_back(inter.LightList[j].lite.location.z);
			for (size_t i = 0; i < inter.ShapeList.size(); i++)
			{
				if (inter.ShapeList[i].Shps.type == "sphere")
				{
					sphereintersect(i, dest, origin);
				}
				else
				{
					planeintersect(i, dest, origin);
				}
				if (pix.shadow.collisions.back())
				{
					pix.shadow.collisions.pop_back();
					pix.pip.r = pix.pip.r + 0;
					pix.pip.g = pix.pip.g + 0;
					pix.pip.b = pix.pip.b + 0;
				}
				else
				{
					pix.shadow.collisions.pop_back();
					if (inter.ShapeList[i].Shps.type == "sphere") {
						nx = (origin[0] - inter.ShapeList[i].Shps.center.x) / inter.ShapeList[i].Shps.radius;
						ny = (origin[1] - inter.ShapeList[i].Shps.center.y) / inter.ShapeList[i].Shps.radius;
						nz = (origin[2] - inter.ShapeList[i].Shps.center.y) / inter.ShapeList[i].Shps.radius;
					}
					else {
						nx = inter.ShapeList[i].Shps.normal.x;
						ny = inter.ShapeList[i].Shps.normal.y;
						nz = inter.ShapeList[i].Shps.normal.z;
					}
					std::vector<double> Normal{ nx,ny,nz };
					std::vector<double> shadow{ dest[0] - pix.shadow.x,dest[1] - pix.shadow.y,dest[2] - pix.shadow.z };
					scale = dot_product(Normal, shadow) * inter.ShapeList[i].Shps.lambert;
					if (scale < 0) { scale = 0; }
					pix.pip.r = pix.pip.r + (scale *inter.LightList[j].lite.intensity * inter.ShapeList[i].Shps.color.r);
					pix.pip.g = pix.pip.g + (scale *inter.LightList[j].lite.intensity * inter.ShapeList[i].Shps.color.g);
					pix.pip.b = pix.pip.b + (scale *inter.LightList[j].lite.intensity * inter.ShapeList[i].Shps.color.b);
				}
			}
		}
	}

	double Calculate::dot_product(std::vector<double> one, std::vector<double> two)
	{
		double dotout = one[0] * two[0] + one[1] * two[1] + one[2] * two[2];
		return dotout;
	}