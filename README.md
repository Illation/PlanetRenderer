# PlanetRenderer
###A repo for my research on planet rendering in c++ | opengl

####Description
This will grow over the next few months as I add more content. Nothing is set in stone yet, I am currently researching terrain algorithms.
I regularly post updates about this project on my blog: http://robert-lindner.com/blog/category/project/planet-renderer/

####Terrain
There is a recursively subdividing icosahedron. Triangles subdivide based on the distance from the camera and get culled when outside of the frustum.

![Text Rendering Moon Planet terrain LOD Robert Lindner](http://i.imgur.com/csAW0tV.jpg)
![OpenGL Planet terrain LOD Rendering Robert Lindner](http://i.imgur.com/qubk7gj.jpg)

### Demo Video (Click to open)
[![Link](https://i.ytimg.com/vi/66VysDSQ8Mw/maxresdefault.jpg)](https://www.youtube.com/watch?v=66VysDSQ8Mw)

####Controls
| Keys | Function |
| :---: |:--- |
| F1 | Screenshot |
| F2 | Toggle Debug UI |
| W - S | Change Camera Altitude |
| A - D | Change Camera Longitude |
| NUM4 - NUM6 | Change Camera Zoom |
| Z | Toggle Wireframe |
| SPACE | Lock Frustum |
| R | Rotate Planet |

####Framework
A lot of the code for the framework is copied from my openGL framework, but since I want a barebones techdemo as a result in order to keep all the code relevant, I did not code this in the framework itself.

Currently there is context managment, time, input managment, window settings, opengl and sdl initialization, shaders, camera, transforms, textures and text rendering.

####Libraries
Libraries used: SDL2, GLM, OpenGL, DevIL

Font files created with BMFont.

####Building
To build the project you need to define an environment variable or macro for visual studio to find the above mentioned libraries.
The variable is called "S_LIBS", and the value would be something like "C:\Dev\StaticLibraries".
That folder needs to contain the .lib files and the include folders of SDL2, GLM and DevIL.

Alternativly you can modify the visual studio project include and library paths to suit your setup. You should obviously also be running on a computer that has the hardware and drivers for modern openGL.
