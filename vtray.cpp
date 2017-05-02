#include <thread>
#include <string>
#include <interpreter.h>
#include <TSwim.h>
// implement vtray main here
using std::cout;
using std::endl;
/*
QImage threadedDrawingandMath(int NT, interpreter interpret, QImage Picture) {

	std::vector<Calculate> Calcs;
	std::vector<double> rVals;
	std::vector<double> gVals;
	std::vector<double> bVals;
	MessageQueue in, out;
	double scaler = 1;

	//ThreadPool P(NT, &in, &out);

	int CamSize = interpret.Cams.CamStruct.size[0] * interpret.Cams.CamStruct.size[1];
	int sizer = CamSize;
	int i = 0;
	Calculate w1(interpret,0);
	
	for ( i = 0; i < CamSize; i++) {
		w1.pos = i;
		w1.pix.pip.x = (i % interpret.Cams.CamStruct.size[0]);
		w1.pix.pip.y = (i / interpret.Cams.CamStruct.size[0]);
		in.push(&w1);
	}

	for ( i = 0; i < NT; i++) {
		in.push(nullptr);
	}

//	P.joinAll();

	while (!out.empty()) {
		MessageType m;
		out.wait_and_pop(m);

		if (Calculate *rp = dynamic_cast<Calculate *>(m)) {
			rVals.push_back(rp->pix.pip.r);
			gVals.push_back(rp->pix.pip.g);
			bVals.push_back(rp->pix.pip.b);
			Calcs.push_back(*rp);
		}
	}
	std::sort(rVals.begin(), rVals.end());
	std::sort(gVals.begin(), gVals.end());
	std::sort(bVals.begin(), bVals.end());

	double RSCALE = rVals.back();
	double GSCALE = gVals.back();
	double BSCALE = bVals.back();
	
	std::vector<double> actual_scale{ RSCALE, GSCALE,BSCALE };
	std::sort(actual_scale.begin(), actual_scale.end());
	
	if (actual_scale.back() >255)
		scaler = actual_scale.back() / 255;

	for (size_t i = 0; i < Calcs.size(); i++)	{
		int newR = Calcs[i].pix.pip.r / scaler;
		int newG = Calcs[i].pix.pip.g / scaler;
		int newB = Calcs[i].pix.pip.b / scaler;
		int x = Calcs[i].pix.pip.x;
		int y = Calcs[i].pix.pip.y;
		uint rgb = 4278190080 + (newR * 65536) + (newG * 256) + newB;
		Picture.setPixel(x, y, rgb);
	}

	return Picture;
}
*/
QImage SingleThread(interpreter interpret, QImage Picture)
{
	std::vector<Pixall> Calcs;
	std::vector<double> rVals;
	std::vector<double> gVals;
	std::vector<double> bVals;
	double largest = 0;
	int CamSize = interpret.Cams.CamStruct.size[0] * interpret.Cams.CamStruct.size[1];
	bool first = true;
	for (int i = 0; i < CamSize; i++) {
		Calculate w1(interpret, i);
		w1.run();
		w1.pix.pip.x = (i%interpret.Cams.CamStruct.size[0]);
		w1.pix.pip.y = (i/ interpret.Cams.CamStruct.size[0]);
		Calcs.push_back(w1.pix);
		rVals.push_back(w1.pix.pip.r);
		gVals.push_back(w1.pix.pip.g);
		bVals.push_back(w1.pix.pip.b);
		//std::cout << i << std::endl;
	}
	std::sort(rVals.begin(), rVals.end());
	std::sort(gVals.begin(), gVals.end());
	std::sort(bVals.begin(), bVals.end());
	double RSCALE = rVals.back();
	double GSCALE = gVals.back();
	double BSCALE = bVals.back();
	double Rscaler = 1;
	double Gscaler = 1;
	double Bscaler = 1;
	std::vector<double> actual_scale{ RSCALE, GSCALE,BSCALE };
	std::sort(actual_scale.begin(), actual_scale.end());
	double scaler = 1;
	if (actual_scale.back() >255)
		scaler = actual_scale.back() / 255;
	if (RSCALE > 255) { Rscaler = RSCALE / 255; }
	if (GSCALE > 255) { Gscaler = RSCALE / 255; }
	if (BSCALE > 255) { Bscaler = RSCALE / 255; }
	for (size_t i = 0; i < Calcs.size(); i++)
	{
		int newR = Calcs[i].pip.r / Rscaler;
		int newG = Calcs[i].pip.g / Gscaler;
		int newB = Calcs[i].pip.b / Bscaler;
		int x = Calcs[i].pip.x;
		int y = Calcs[i].pip.y;
		uint rgb = 4278190080 + (newR * (256 * 256)) + (newG * 256) + newB;
		Picture.setPixel(x, y, rgb);
	}
	return Picture;
}

int main(int argv, char * argc[]) {
	int numThreads;
	int exit;
	interpreter interp;
	QString inFile;
	QString tds;
	std::string multthreads;
	exit = EXIT_SUCCESS;
	if (argv == 5) {
		multthreads = argc[1];
		if (multthreads == "-t") {
			inFile = argc[3];
			tds = argc[2];
			numThreads = tds.toInt();
			try { 
				interp = interp.fromJSON(inFile); 
				QImage picture(interp.Cams.CamStruct.size[0], interp.Cams.CamStruct.size[1], QImage::Format_RGB32);
				QString outFile = argc[4];
				QFile out(outFile);
				out.open(QIODevice::WriteOnly);
				//picture = threadedDrawingandMath(numThreads, interp, picture);
				picture = SingleThread(interp, picture);
				picture.save(&out, "PNG", 100);
			}
			catch (QJsonParseError & error) { exit = EXIT_FAILURE; }
		}
		else {
			std::cerr << "Error : Incorrect input arguments" << std::endl;
			exit = EXIT_FAILURE;
		}
	}
	else if (argv == 3) {
		inFile = argc[1];
		try { 
			interp = interp.fromJSON(inFile); 
			QImage picture(interp.Cams.CamStruct.size[0], interp.Cams.CamStruct.size[1], QImage::Format_RGB32);
			picture = SingleThread(interp, picture);
			QString outFile = argc[2];
			QFile out(outFile);
			out.open(QIODevice::WriteOnly);
			picture.save(&out, "PNG", 100);
		}
		catch (QJsonParseError & error) { exit = EXIT_FAILURE; }
	}
	else {
		std::cerr << "Error : Incorrect number of input arguments" << std::endl;
		exit = EXIT_FAILURE;
	}
	return exit;
}