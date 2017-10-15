Read me for PAC-MINION
----------------------

Note: PAC and minion are used interchangeably.

1. Running the game

	- in Linux: The game has been developed under a linux (ubuntu-based) environment, and under such environments, the executable binaries provided in the 'bin\linux' folder can be run from terminal:
		./PacMinion
	In case of any error, try installing freeglut:
		sudo apt-get install freeglut3-dev
	(If your internet connection is behind a proxy, try:
		sudo http_proxy="http://username:password@proxy.name:port/" apt-get install freeglut3-dev
	Replace the username, password, proxy.name and port with the actual values.
	This applies for all the apt-get commands mentioned in this read me file.)

	- in Windows: The windows executable is provided under 'bin\windows' folder. Run PacMinion.exe.
	(Note: In windows, performance of certain computer vs computer modes is very poor. This might be due to the fact that the compiling and generation of executables were done with old versions of gcc and glut.)

2. Compiling the source code

	- The source code is provided in the source folder. The code is platform independent, and can be compiled on both Windows and Linux environments. 'freeglut' is required to compile the code. Under ubuntu-based Linux environments, run (in terminal):
		sudo apt-get install freeglut3-dev
	to install freeglut. If g++ is not installed, install that as well:
		sudo apt-get install g++
	Now navigate to the src folder, and compile it as follows:
		g++ pac_1_cpp.cpp -lGL -lGLU -lglut -oPacMinion
	The executable binary PacMinion will be created. You can move it to any location, but make sure that the following additional files are also present in the same directory as that of the executable:
		maze1.txt
		minion.obj
		minion.mtl

	- Similarly, the code can be compiled in Windows if freeglut is installed, using MinGW or gcc (g++) or MS Visual C++ compiler. Instructions for configuring freeglut and including and linking the libraries vary according to the compiler and/or the IDE used. Many tutorials are available in the internet for setting up the environment for freeglut based development, and the code can be compiled once that's done. 

3. System Requirements
	- Windows (Windows Vista or later recommended) or Linux (Tested in ubuntu-based OS) Operating System (x86 or x64)
	- Minimum Processor Speed 1 GHz, recommended speed 2 GHz and above.
	- Minimum 512 MB of RAM
	- Video resolution of 1024*768; 16:9 aspect ratio is recommended (1366*768 and above).
	- A 3D graphics card is required, which supports OpenGL 2.0.

4. About the game play

	- Minion: The yellow humanoid who is the central agent in the game.
	- Ghosts: The four, coloured agents who start around the central box in the maze.
	- Briefly, the objective is to make the minion eat all the food pellets (white dots) in the maze.
	- Ghosts will try and chase you down. If you make contact with a ghost, they will eat you and the positions of the minion and the ghosts are reset.
	- If you eat power pellet (large green dots), for a short time, the ghosts will turn dark blue, when the minion can eat them.
	- The objectives and rules are shown in the game, when the game starts.
	- Controls are also shown in-game. 
	
	- You can watch Computer vs Computer, ie switch over the minion control to the computer. Press 1, 2 or 3 for different PAC AI modes. Press 0 to switch back to manual minion control.
	- Press c to switch to different camera modes. Some camera modes are difficult during manual minion control, but you can enjoy those in computer vs computer games. Press r to reset the camera.

5. Improving performance
	- If the game is laggy, there are two things which can potentially improve performance, without affecting game play:
	1. Toggle minion - Press m. The minion will be replaced with a simple sphere.
	2. Toggle anti-aliasing: Press t. MS-AA will be disabled, but preformance may increase at the cost of slightly poorer visuals (not apparent in the default camera, but apparent in the other camera modes).
	- For a even better performance and visually appealing graphics, try to run the game with the dedicated graphic card, if available.

6. Future Improvements
	- This is a basic game play. I'll add things like levels and increasing difficulty etc when I have time. Quite busy at the moment. ;)
