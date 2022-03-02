# Peisel

Peisel is a program that let's you create png files and sprite sheets.

Peisel uses Pen Engine for rendering which you can grab from https://github.com/fentacoder/PenEngine.

In order to run this in Visual Studio, set these locations in Preferences -> C/C++ -> General -> Additional Include Directories:

	$(SolutionDir)dependencies\include;$(SolutionDir)pen_engine\dependencies\glfw\include;$(SolutionDir)pen_engine\dependencies\glad;

Next in Preferences -> Linker -> General set this:

	$(SolutionDir)dependencies\libs;$(SolutionDir)pen_engine\dependencies\glfw\lib-vc2019;

Lastly in Preferences -> Linker -> Input add this:

	glfw3.lib;opengl32.lib;libpng16.lib;zlib.lib;