# PlanetRenderer

## **Work in this repository has been discontinued. The project lives on in a new repository, as it has been merged with my openGL framework: https://github.com/Illation/ETEngine**


### A repo for my research on planet rendering in c++ | opengl

#### Description
Basic Planet rendering using a version of the CDLOD algorithm with triangles instead of quads on an icosahedron.
I regularly post updates about this project on my blog: http://robert-lindner.com/blog/category/project/planet-renderer/

#### Terrain
There is a recursively subdividing icosahedron. Triangles subdivide based on the distance from the camera and get culled when outside of the frustum. When drawn, every triangle instances a patch geometry, which allows for insane level of detail at high framerates, and morphs smoothly between subdivision levels.

![Text Rendering Moon Planet terrain LOD Robert Lindner](http://i.imgur.com/csAW0tV.jpg)
![OpenGL Planet terrain LOD Rendering Robert Lindner](http://i.imgur.com/qubk7gj.jpg)

### Demo Video (Click to open)
[![Link](https://i.ytimg.com/vi/66VysDSQ8Mw/maxresdefault.jpg)](https://www.youtube.com/watch?v=66VysDSQ8Mw)

#### Controls
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
| UP - Down | Change max subdivisions |
| LMB | Rotate View |

#### Framework
A lot of the code for the framework is copied from my openGL framework, but since I want a barebones techdemo as a result in order to keep all the code relevant, I did not code this in the framework itself.

Currently there is context managment, time, input managment, window settings, opengl and sdl initialization, shaders, camera, transforms, textures and text rendering.

#### Libraries
Libraries used: SDL2, GLM, OpenGL, DevIL

Font files created with BMFont.

#### Building

This framework uses Genie to generate project files: https://github.com/bkaradzic/GENie
You need to install it and use it from a terminal, the easiest way to do that is adding it to your PATH
You should obviously also be running on a computer that has the hardware and drivers for modern openGL.

##### Windows

Navigate to the folder where this readme is located in a terminal

    genie --file=build\genie.lua vs2015
    
Open the generated visual studio solution

To build the project you need to define an environment variable or [a macro for visual studio](https://imgur.com/a/6uMFN) to find the above mentioned libraries.
The variable is called "S_LIBS", and the value would be something like "C:\Dev\StaticLibraries".
That folder needs to contain the .lib files and the include folders of SDL2, GLM and DevIL.
For DevIL I suggest downloading the [Windows SDK at version 1.8](https://sourceforge.net/projects/openil/files/DevIL%20Windows%20SDK/1.8.0/).

Hit F5

If there are any linker errors, check if your library directory matches the include and library paths, you might need to remove the version name from your folder.
Alternativly you can modify the visual studio project include and library paths to suit your setup. 

##### Linux

Make sure you have libraries for SDL2, GLM and DevIL installed, with -dev packages where applicable.
You might need to change the path in the includedirs configuration in "build/genie.lua"

Navigate to the folder where this readme is located in a terminal

    genie --file=build\genie.lua gmake
    
    make
    
    cp -R ./PlanetFramework/Fonts/ ./bin/release/
    cp -R ./PlanetFramework/Textures/ ./bin/release/
    cp -R ./PlanetFramework/Shaders/ ./bin/release/
Navigate to the release folder to make sure that it is your working directory (for file loading)

    cd ./bin/release
    ./PlanetFramework

