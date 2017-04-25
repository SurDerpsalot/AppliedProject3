Put Your design decisions here. What modules you defined and how they
interact to solve the ray tracing problem.

vtray : This is the main program. It reads in the command line input, creates a QImage that will be read out to the file specified. It also sends the initial
		JSON document to be parsed in the INTERPRETER class. This information is then passed to a function based on whether or not there is multithreading needed. 
		That function then creates n number of classes that calculate the color for a single pixel and then draws it to the QImage after adjusting the color
		and all the classes are finished running. This class also draws the QImage.

interpreter : This is the class that parses the JSON document and throws any errors if the document will not parse correctly. It will also output the errors to the
			standard error instead of the standard output. It also creates three objects: the Camera object, a vector with all the light sources stored in it,
			and a vector with all the objects to be drawn in it.

camera : This class is really just two structs that hold all the information needed for the camera. It stores the focus, center, normal, size, and resolution.

lights : This class is really just two structs that hold all the information needed for the light source. It stores the location and intensity.

shapes : This class is really just three structs that hold all the information needed for the shapes. It stores the color, center, type (sphere or plane), normal, radius, and lambert.

message_queue: This is the threadsafe queue from the example code. It holds a queue of operations to be completed by the threadpool.

TSwim : This is the threadpool.It creates a class per pixel and passes every class into a vector. That vector then feeds the classes to threads to be ran. The threads are set up ahead
		of time so they don't take long to set up for each call (and there will be many calls). It also contains the class CALCULATE that does all the
		math for finding the closest shape to the camera and setting that pixel with that color, and then calculate the shadows that will be cast onto the pixel. The results
		are stored in a struct called 'pix' for usage in the vtray.cpp.

PixelStruct : This is the instantiaion and creation of the pixel struct, which holds all the information about the pixels being used. It stores the information about the
		shadows impacting the pixel as well as the pixel length and color. The information about shadows includes the location of the intersection between the
		camera ray and the shape. It also includes a vector of potential intersections with the any of the shapes in the ShapeList while the ray is between 
		the camera ray and shape intersection and any of the lightsources in the LightsList.

I decided on this design because it was supposed to help isolate bugs. However, with this current iteration of the program, the math should be fine but yet the colors are
coming out incorrect. In addition to that, scene1 and scene2 don't even work because I need to manage the resources better so I don't overflow the vectors. I would implement
that, but right now (2:32 am) I am more worried about getting any point that I can so my first priority right now is to get a test coverage higher than 80%.

and just like that(4:04am) Everything is gone. apparently my latest update is 10 hours even though I UPDATED BETA4 AT 2:14 AM!!!!! I have my log state that
I pushed Beta4 at 2:14:18 am. Then everything disappeared in Visual Studio. So I did git push origin Beta2 so I could go back. Then I did git push origin Beta4 so that I could
get back to where I was before, but I guess that just overwrote everything I had done. I guess thats it then. Those two lines are going to make me have to withdraw.
Everything saved except the math, you know THE CORE OF THE PROJECT.
 
5:00am
I managed to salvage the sphereIntersection, so that's nice. I am now attempting to fix all the build errors in the Vagrant.

5:13am
It now builds in the VM and now I am back to writing tests to get that percentage up.

5:17am
I tried to run the coverage report and I recieved the following error:
No such file or directory
CMakeFiles/coverage.dir/build.make:57: recipe for target 'CMakeFiles/coverage' failed
make[3]: *** [CMakeFiles/coverage] Error 1
CMakeFiles/Makefile2:141: recipe for target 'CMakeFiles/coverage.dir/all' failed
make[2]: *** [CMakeFiles/coverage.dir/all] Error 2
CMakeFiles/Makefile2:148: recipe for target 'CMakeFiles/coverage.dir/rule' failed
make[1]: *** [CMakeFiles/coverage.dir/rule] Error 2
Makefile:155: recipe for target 'coverage' failed
make: *** [coverage] Error 2
so I am just going to move on and try to cover stuff manually and re-implement the planeIntersection

5:41am
I finished implementing my planeIntersect. I still have no idea why my scene0.png is so colorful. It looks like it has the 
light amount correct, just the color placement is not right at all. My multithreading still likes to not work, which kinda sucks
since I don't really know how to fix the issue I am running into. I will definitely be hitting up the office hours to get
help with that. I also stopped making the unittests because they were working earlier (I created two json files to pass in
and they would test specific things) but now they aren't being read in and they aparently don't exist anymore, even though
they are in the same directory. Time to pray and pass out.