# PlanetRenderer
###A repo for my research on planet rendering in c++ | opengl

####Description
This will grow over the next few months as I add more content. Nothing is set in stone yet, I am currently researching terrain algorithms.
I regularly post updates about this project on my blog: http://robert-lindner.com/blog/category/project/planet-renderer/

####Terrain
There is a recursively subdividing icosahedron. Triangles subdivide based on the distance from the camera and get culled when outside of the frustum.
<div
<blockquote class="imgur-embed-pub" lang="en" data-id="a/58LZH"><a href="//imgur.com/58LZH">OpenGL planet terrain generation</a></blockquote><script async src="//s.imgur.com/min/embed.js" charset="utf-8"></script>
</div>

####Framework
A lot of the code for the framework is copied from my openGL framework, but since I want a barebones techdemo as a result in order to keep all the code relevant, I did not code this in the framework itself.

Currently there is context managment, time, input managment, window settings, opengl and sdl initialization, shaders, camera and transforms.

####Libraries
Libraries used: SDL2, GLM, OpenGL - in the future also DevIL
