#include <thread>
#include <string>
#include <interpreter.h>
#include <TSwim.h>
// implement vtray main here
using std::cout;
using std::endl;

QImage threadedDrawingandMath(int NT, interpreter interpret, QImage Picture) {

	std::vector<Calculate> Calcs;
	MessageQueue in, out;
	ThreadPool P(NT, &in, &out);
	int CamSize = interpret.Cams.CamStruct.size[0] * interpret.Cams.CamStruct.size[1];
	int sizer = CamSize;
	for (int i = 0; i < CamSize; i++) {
		Calculate w1(interpret, sizer);
		w1.pix.pip.x = (sizer%interpret.Cams.CamStruct.size[0]);
		w1.pix.pip.y = (sizer / interpret.Cams.CamStruct.size[0]);
		sizer--;
		in.push(&w1);
	}
	for (int i = 0; i < NT; i++) {
		in.push(nullptr);
	}
	P.joinAll();
	std::vector<double> rVals;
	std::vector<double> gVals;
	std::vector<double> bVals;
	while (!out.empty()) {
		MessageType m;
		out.wait_and_pop(m);
		if (Calculate *rp = dynamic_cast<Calculate *>(m)) {
			rVals.push_back(rp->pix.pip.r);
			gVals.push_back(rp->pix.pip.g);
			bVals.push_back(rp->pix.pip.b);
			Calcs.push_back(*rp);
		}
		else
			std::cerr << "Error : Unknown Work Message Type!" << std::endl;
	}
	std::sort(rVals.begin(), rVals.end());
	std::sort(gVals.begin(), gVals.end());
	std::sort(bVals.begin(), bVals.end());
	double RSCALE = rVals.back();
	double GSCALE = gVals.back();
	double BSCALE = bVals.back();
	double Rscale = 1;
	double Gscale = 1;
	double Bscale = 1;
	if (RSCALE >255)
		Rscale = RSCALE / 255;
	if (GSCALE >255)
		Gscale = GSCALE / 255;
	if (BSCALE >255)
		Bscale = BSCALE / 255;
	for (size_t i = 0; i < Calcs.size(); i++) {
		int newR = Calcs[i].pix.pip.r / Rscale;
		int newG = Calcs[i].pix.pip.g / Gscale;
		int newB = Calcs[i].pix.pip.b / Bscale;
		int x = Calcs[i].pix.pip.x;
		int y = Calcs[i].pix.pip.y;
		uint rgb = 4278190080 + (newR * 65536) + (newG * 256) + newB;
		Picture.setPixel(x, y, rgb);
	}
	return Picture;
}

QImage SingleThread(interpreter interpret, QImage Picture)
{
	std::vector<Calculate> Calcs;
	std::vector<double> rVals;
	std::vector<double> gVals;
	std::vector<double> bVals;
	double largest = 0;
	int CamSize = interpret.Cams.CamStruct.size[0] * interpret.Cams.CamStruct.size[1];
	int sizer = CamSize;
	bool first = true;
	for (int i = 0; i < CamSize; i++) {
		Calculate w1(interpret, sizer);
		w1.run();
		sizer--;
		w1.pix.pip.x = (sizer%interpret.Cams.CamStruct.size[0]);
		w1.pix.pip.y = (sizer/ interpret.Cams.CamStruct.size[0]);
		Calcs.push_back(w1);
		rVals.push_back(w1.pix.pip.r);
		gVals.push_back(w1.pix.pip.g);
		bVals.push_back(w1.pix.pip.b);
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
	if (RSCALE > 255)
		Rscaler = RSCALE/ 255;
	if (GSCALE > 255)
		Gscaler = RSCALE / 255;
	if (BSCALE > 255)
		Bscaler = RSCALE / 255;
	for (size_t i = 0; i < Calcs.size(); i++) {
		int newR = Calcs[i].pix.pip.r / Rscaler;
		int newG = Calcs[i].pix.pip.g / Gscaler;
		int newB = Calcs[i].pix.pip.b / Bscaler;
		int x = Calcs[i].pix.pip.x;
		int y = Calcs[i].pix.pip.y;
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
			try { interp = interp.fromJSON(inFile); }
			catch (QJsonParseError & error) { exit = EXIT_FAILURE; }
			QImage picture(interp.Cams.CamStruct.size[0], interp.Cams.CamStruct.size[1], QImage::Format_RGB32);
			QString outFile = argc[4];
			QFile out(outFile);
			out.open(QIODevice::WriteOnly);
			picture = threadedDrawingandMath(numThreads, interp, picture);
			picture.save(&out, "PNG", 100);

		}
		else {
			std::cerr << "Error : Incorrect input arguments" << std::endl;
			exit = EXIT_FAILURE;
		}
	}
	else if (argv == 3) {
		inFile = argc[1];
		try { interp = interp.fromJSON(inFile); }
		catch (QJsonParseError & error) { exit = EXIT_FAILURE; }
		QImage picture(interp.Cams.CamStruct.size[0], interp.Cams.CamStruct.size[1], QImage::Format_RGB32);
		picture = SingleThread(interp, picture);
		QString outFile = argc[2];
		QFile out(outFile);
		out.open(QIODevice::WriteOnly);

		picture.save(&out, "PNG", 100);
	}
	else {
		std::cerr << "Error : Incorrect number of input arguments" << std::endl;
		exit = EXIT_FAILURE;
	}
	return exit;
}