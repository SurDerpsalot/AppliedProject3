#include <thread>
#include <string>
#include <interpreter.h>
#include <TSwim.h>
// implement vtray main here

QImage threadedDrawingandMath(int NT, interpreter interpret, QImage Picture) {
	struct pixels {
		std::vector<Calculate> Calcs;
	};
	pixels drawin;
	MessageQueue in, out;
	ThreadPool P(NT, &in, &out);
	int CamSize = interpret.Cams.CamStruct.size[0] * interpret.Cams.CamStruct.size[1];
	for (int i = 0; i < CamSize; i++) {
		std::cout << i << std::endl;
		Calculate w1(interpret, CamSize);
		in.push(&w1);
	}
	std::cout << "the originals went in proper" << std::endl;
	for (int i = 0; i < NT; i++)
	{
		in.push(nullptr);
	}
	std::cout << "they all pushed in proper" << std::endl;
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
			drawin.Calcs.push_back(*rp);
		}
		else {
			std::cout << "Unknown Work Message Type!" << std::endl;
		}
	}
	std::sort(rVals.begin(), rVals.end());
	double RSCALE = rVals[rVals.size()-1];
	double GSCALE = gVals[gVals.size() - 1];
	double BSCALE = bVals[bVals.size() - 1];
	std::vector<double> actual_scale{ RSCALE, GSCALE,BSCALE };
	std::sort(actual_scale.begin(),actual_scale.end());
	double scaler = actual_scale[actual_scale.size() - 1]/255;
	for (size_t i = 0; i < drawin.Calcs.size(); i++)
	{
		double newR = drawin.Calcs[i].pix.pip.r / scaler;
		double newG = drawin.Calcs[i].pix.pip.g / scaler;
		double newB = drawin.Calcs[i].pix.pip.b / scaler;
		int x = drawin.Calcs[i].pix.pip.x;
		int y = drawin.Calcs[i].pix.pip.y;
		uint rgb = 4278190080 + (newR * 65536) + (newG * 256) + newB;
		Picture.setPixel(x, y, rgb);
	}
	return Picture;
}

int main(int argv, char * argc[]){
	int numThreads;
	int exit;
	QImage picture;
	interpreter interp;
	QString inFile;
	QString tds;
	std::string multthreads;
	
	exit = EXIT_SUCCESS;
	if (argv == 5)	{
		multthreads = argc[1];
		if (multthreads == "-t" )		{	
			inFile = argc[3];
			tds = argc[2];
			numThreads = tds.toInt();
			try {interp = interp.fromJSON(inFile);}
			catch (QJsonParseError & error)	{exit = EXIT_FAILURE;}
			picture = threadedDrawingandMath(numThreads,interp,picture);
			
			picture.save("scene0", ".png", 100);
			
		}
		else {
			std::cerr << "Error : Incorrect input arguments" << std::endl;
			exit =  EXIT_FAILURE;
		}
	}
	else if (argv == 3)	{
		inFile = argc[1];
		try {interp = interp.fromJSON(inFile);}
		catch (QJsonParseError & error)	{exit = EXIT_FAILURE;}
		picture = threadedDrawingandMath(1, interp,picture);
		picture.save("scene0", ".png", 100);
	}
	else {
		std::cerr << "Error : Incorrect number of input arguments" << std::endl;
		exit =  EXIT_FAILURE;
	}
	return exit;
}