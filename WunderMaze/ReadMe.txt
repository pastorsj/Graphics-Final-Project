========================================================================
    CONSOLE APPLICATION : WunderMaze Project Overview
========================================================================

AppWizard has created this WunderMaze application for you.

This file contains a summary of what you will find in each of the files that
make up your WunderMaze application.


WunderMaze.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

WunderMaze.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

ControlState.cpp
    This contains the implementation that is responsible for allowing the player to
	control their movement through the maze.

main.cpp
    This is the main application source file.

MazeGenerator.cpp
    This contains the implementation that randomly generates a maze of an appropriate size
	to be used in the maze.

Model.cpp
    This contains the implementation that is responsible for managing the data associated
	with a single model.

ModelManager.cpp
    This contains the implementation that is responsible for acting as a container for
	models and textures used in the game.

RenderEngine.cpp
	This contians the implementation of the code responsible for using OpenGL to
	display everything in the game properly.

ShaderManager.cpp
    This contains the code responsible for loading, compiling, and setting up
	the shaders used throughout the code.

WorldState.cpp
    This contains the implementation that is responsible for storing the proper
	vectors and variables for the game.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named WunderMaze.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
