# CPU-Renderer
The given CPU renderer can render an image with multiple objects which are lit according to distance and presents them in a ppm format

learnings from the project

std::ofstream is not an image function
It is a file output stream

This line:
Creates a file called output.ppm
Opens it in write mode
From now on, img << ... writes text into that file

PPM header (this is crucial)
img << "P3\n" << width << " " << height << "\n255\n";

This is the PPM contract.
Line
Meaning
P3
ASCII PPM format

800 600 (height width)
Width × Height

255
Max color value per channel

In C++, structs and classes can contain functions.
These functions are called member functions.
Every member function is implicitly attached to a specific object.

Each pixel on the image corresponds to one ray fired from the camera into the 3D world. 

Window dimensions (width, height)
Yes — these are the image resolution.
Example:
width  = 800;
height = 600;

These define:
How many pixels across

How many pixels down

⚠️ Important:
 They do NOT define the physical size of the scene, only the number of rays.

“The first two components of rayDir select which pixel on the viewport we shoot through, and the third component places the viewport in front of the camera; the camera position itself is stored separately as the ray origin.” 

