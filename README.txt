Put Your design decisions here. What modules you defined and how they
interact to solve the ray tracing problem.

vtray.cpp : This is the main program. It reads in the command line input, creates a QImage that will be read out to the file specified. It also sends the initial
		JSON document to be parsed in the INTERPRETER class. This information is then passed to a function based on whether or not there is multithreading needed. 
		That function then creates n number of classes that calculate the color for a single pixel and then draws it to the QImage after adjusting the color
		and all the classes are finished running.

interpreter.cpp : This is the class that parses the JSON document and throws any errors if the document will not parse correctly. It will also output the errors to the
			standard error instead of the standard output. It also creates three objects: the Camera object, a vector with all the light sources stored in it,
			and a vector with all the objects to be drawn in it.

camera.cpp : This class is really just two structs that hold all the information needed for the camera. It stores the focus, center, normal, size, and resolution.

lights.cpp : This class is really just two structs that hold all the information needed for the light source. It stores the location and intensity.

shapes.cpp : This class is really just three structs that hold all the information needed for the shapes. It stores the color, center, type (sphere or plane), normal, radius, and lambert.

message_queue: This is the threadsafe queue from the example code. It holds a queue of operations to be completed by the threadpool.

TSwim : This is the threadpool.