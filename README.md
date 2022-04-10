# Peisel

Peisel is a program that let's you create png files and sprite sheets.

Peisel uses Pen Engine for rendering which you can grab from https://github.com/fentacoder/pen_engine.

In order to run this in Visual Studio, set these locations in Preferences -> C/C++ -> General -> Additional Include Directories:

	$(SolutionDir)dependencies\include;$(SolutionDir)pen_engine\dependencies\glfw\include;$(SolutionDir)pen_engine\dependencies\glad;

Next in Preferences -> Linker -> General set this:

	$(SolutionDir)dependencies\libs;$(SolutionDir)pen_engine\dependencies\glfw\lib-win;

Lastly in Preferences -> Linker -> Input add this:

	glfw3.lib;opengl32.lib;libpng16.lib;zlib.lib;

-------------------------------------------------------------------------------------------------------------------

# 1.0 - Modes

There are currently three modes in Peisel:

	- Draw Mode
	- Sprite Sheet Mode
	- Tile Mode

-------------------------------------------------------------------------------------------------------------------

# 1.1 - Draw Mode

Draw mode is a png file creator that let's you use shapes and brushes to draw.

-------------------------------------------------------------------------------------------------------------------

# 1.2 - Sprite Sheet Mode

Sprite sheet mode is a mode for combining multiple images into a single png file.

-------------------------------------------------------------------------------------------------------------------

# 1.3 - Tile Mode

Tile mode is used for creating tile maps.  The tiles consist of an image with the sprites and a binary file for the collision information.
To add sprites to the tile map, select one by pressing Q to toggle through the loaded list and Left Clicking on the tile.
Tab can be pressed to toggle backwards through the loaded list.
To remove a sprite press Left Shift to switch to tile alt mode and Left Click on the tile.
To add a collision block for a solid tile Right Click on that tile.

When saving your tile map, the image created is a .png file while the tile collision information is a .pentm file in the same directory.