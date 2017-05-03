Put Your design decisions here. What modules you defined and how they
interact to solve the ray tracing problem.

vtray : This is the main program. It reads in the command line input, creates a QImage that will be read out to the file specified. It also sends the initial
		JSON document to be parsed in the INTERPRETER class. This information is then passed to a function based on whether or not there is multithreading needed. 
		That function then calculates the color for a single pixel for each pixel on the camera and then draws it to the QImage after adjusting the color
		and all the classes are finished running. This class also draws the QImage.

interpreter : This is the class that parses the JSON document and throws any errors if the document will not parse correctly. It will also output the errors to the
			standard error instead of the standard output. It also creates three objects: the Camera object, a vector with all the light sources stored in it,
			and a vector with all the objects to be drawn in it.

camera : This class is really just two structs that hold all the information needed for the camera. It stores the focus, center, normal, size, and resolution.
		Interpreter has a public instance of the camera class that holds the camera information from the JSON document.

lights : This class is really just two structs that hold all the information needed for the light source. It stores the location and intensity.
		Interpreter has a vector of this class for storing all the lights from the JSON document.
		
shapes : This class is really just three structs that hold all the information needed for the shapes. It stores the color, center, type (sphere or plane), normal, radius, and lambert.
		Interpreter has a vector of this class for storing all the shapes from the JSON document.

message_queue: This is the threadsafe queue from the example code. It holds a queue of operations to be completed by the threadpool.

TSwim : This is the calculation class that contains a threadpool. It runs per pixel and creates a creates a threadpool when it needs to be multithreaded.
		Currently dies every time I try to run Scene1 or Scene2 due to a bad_alloc error. I have been unable to debug it because I can't figure out how to debug it
		through the Vagrant environment (Visual Studio is broken). The calculation class finds the closest intersection for each pixel and then sets the base RGB 
		values for each pixel and passes them out to a vector for sorting and exposure manipulation. The exposure is handled back in vtray.cpp.
		The calculation class also has an instance of the Interpreter class in it to hold all the JSON information for finding intersections and 
		the base shape rgb values;

PixelStruct : This is the instantiaion and creation of the pixel struct, which holds all the information about the pixels being used. It stores the information about the
		shadows impacting the pixel as well as the pixel length and color. The information about shadows includes the location of the intersection between the
		camera ray and the shape. It also includes a vector of potential intersections with the any of the shapes in the ShapeList while the ray is between 
		the camera ray and shape intersection and any of the lightsources in the LightsList.
