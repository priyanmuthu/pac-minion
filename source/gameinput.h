/***
* To be used with pac_1.c
* Functions and variables related to keyboard and mouse input in the game are placed in this file
*/

/*Keyboard functions start here
*/
static void key(unsigned char c, int x, int y)
{
	/**
	* This function is the Key-Handler for PAC
	* Uses key states instead of a key repeat handler to make the long press key functions faster
	*/
	if (c == 27) {
		exit(0);  /*Escape quits. */
	}
	keystates[c]=1;
	if(c == config->toggleAntiAlias || c == config->toggleAntiAlias - 'a' + 'A'){
		/**
		* Press 't' for anti-aliasing
		*/
		if(config->antiAlias){
			printf("Disabling anti aliasing.\n");
			config->antiAlias = 0;
		}
		else{
			printf("Enabling anti aliasing.\n");
			config->antiAlias = 1;
		}
	}
	else if(c == config->resetCamera || c == config->resetCamera - 'a' + 'A'){
		/**
		* Press 'r' for reset camera
		*/
		cameraSetting.posx = 0.0;
		cameraSetting.posy = 0.0;
		cameraSetting.posz = 0.0;
		cameraSetting.dirx = 0.0;
		cameraSetting.diry = 0.0;
		cameraSetting.dirz = -1.0;
		cameraSetting.upx = 0.0;
		cameraSetting.upy = 1.0;
		cameraSetting.upz = 0.0;
		prevMouseX = INT_MAX;
		prevMouseY = INT_MAX;
	}
	else if(c == '0'){
		/**
		* Self control the PAC
		*/
		pacControlAgent = 0;
	}
	/**
	* Keys for the PAC Simulation
	*/
	else if(c == '1'){
		pacControlAgent = 1;
		computePacDirection();
		if(blinky.moveDist < 0.00001)
        	blinky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(inky.moveDist < 0.00001)
        	inky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(pinky.moveDist < 0.00001)
        	pinky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(clyde.moveDist < 0.00001)
        	clyde.moveDist = pacMoveDist * ghostSpeedPercent;
        if(pacStartTime < 0)
        	pacStartTime = glutGet(GLUT_ELAPSED_TIME);
	}
	else if(c == '2'){
		pacControlAgent = 2;
		minimax();
		if(blinky.moveDist < 0.00001)
        	blinky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(inky.moveDist < 0.00001)
        	inky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(pinky.moveDist < 0.00001)
        	pinky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(clyde.moveDist < 0.00001)
        	clyde.moveDist = pacMoveDist * ghostSpeedPercent;
        if(pacStartTime < 0)
        	pacStartTime = glutGet(GLUT_ELAPSED_TIME);
	}
	else if(c == '3'){
		pacControlAgent = 3;
		minimax2();
		if(blinky.moveDist < 0.00001)
        	blinky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(inky.moveDist < 0.00001)
        	inky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(pinky.moveDist < 0.00001)
        	pinky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(clyde.moveDist < 0.00001)
        	clyde.moveDist = pacMoveDist * ghostSpeedPercent;
        if(pacStartTime < 0)
        	pacStartTime = glutGet(GLUT_ELAPSED_TIME);
	}
	else if(c == 'p'){
		paused = !paused;
	}
	else if(c == 'c'){
		// printf("gluLookat values: %f %f %f %f %f %f\n", cameraSetting.posx,cameraSetting.posy,cameraSetting.posz,cameraSetting.dirx,cameraSetting.diry,cameraSetting.dirz);
		switch(cameraSetting.cameraMode){
			case 0:
			cameraSetting.cameraMode = 1;
			break;

			case 1:
			cameraSetting.cameraMode = 2;
			break;

			case 2:
			cameraSetting.cameraMode = 3;
			angleChange = 3;
			break;

			case 3:
			cameraSetting.cameraMode = 4;
			angleChange = 10;
			break;

			case 4:
			cameraSetting.cameraMode = 0;
			cameraSetting.posx = 0.0;
			cameraSetting.posy = 0.0;
			cameraSetting.posz = 0.0;
			cameraSetting.dirx = 0.0;
			cameraSetting.diry = 0.0;
			cameraSetting.dirz = -1.0;
			cameraSetting.upx = 0.0;
			cameraSetting.upy = 1.0;
			cameraSetting.upz = 0.0;
			prevMouseX = INT_MAX;
			prevMouseY = INT_MAX;
			break;
		}
	}
	else if(c=='g'){
		agentsInit();
		loadMaze();
        inky.setGhostMode(0);
		pinky.setGhostMode(0);
		blinky.setGhostMode(0);
		clyde.setGhostMode(0);
		cameraSetting.cameraMode = 0;
		cameraSetting.posx = 0.0;
		cameraSetting.posy = 0.0;
		cameraSetting.posz = 0.0;
		cameraSetting.dirx = 0.0;
		cameraSetting.diry = 0.0;
		cameraSetting.dirz = -1.0;
		cameraSetting.upx = 0.0;
		cameraSetting.upy = 1.0;
		cameraSetting.upz = 0.0;
		prevMouseX = INT_MAX;
		prevMouseY = INT_MAX;
		blinky.moveDist = 0;
        inky.moveDist = 0;
        pinky.moveDist = 0;
        clyde.moveDist = 0;
        pacCurDir = pacNextDir = 0;
        score = 0;
        paused = false;
        nfood = 212;
        pacStartTime = -1;
        pacControlAgent = 0;
        angleChange = 10;
        pacAngle = 0;
        deathCount = 0;
	}
	else if(c == 'm'){
		minionEnabled = !minionEnabled;
	}
	//printf("%c\n", c);
	//glutPostRedisplay();
}

static void keyUp(unsigned char c, int x, int y){
	/**
	* Called when a key is realesed
	*/
	int i,j;
	keystates[c]=0;
	if(c>='a' && c<='z' && keystates[c-'a'+'A'] == 1 && c-'a'+'A' >=0){
		keystates[c - ('a' - 'A')] = 0;
	}
	else if(c>='A' && c<='Z' && keystates[c+'a'-'A'] == 1){
		keystates[c + 'a' - 'A'] = 0;
	}
}

void keyListen(int key, int x, int y){
	/**
	* Callback for Key press operations
	*/
    if(key==GLUT_KEY_LEFT || key==GLUT_KEY_RIGHT || key==GLUT_KEY_UP || key==GLUT_KEY_DOWN){
        if(pacControlAgent == 0){
        	pacNextDir = key;
        }
        if(blinky.moveDist < 0.00001)
        	blinky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(inky.moveDist < 0.00001)
        	inky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(pinky.moveDist < 0.00001)
        	pinky.moveDist = pacMoveDist * ghostSpeedPercent;
        if(clyde.moveDist < 0.00001)
        	clyde.moveDist = pacMoveDist * ghostSpeedPercent;
        if(pacStartTime < 0)
        	pacStartTime = glutGet(GLUT_ELAPSED_TIME);
    }
    //printf("%d\n", key);
    //glutPostRedisplay();
}

/*Mouse functions start here
*/

void mouseMotion(int x, int y){
	/**
	* Callback for mouse based mootions
	*/
	if(cameraSetting.cameraMode == 0){
		if(prevMouseX == INT_MAX){
			prevMouseX = x;
			prevMouseY = y;
			return;
		}
		float xdiff = (x - prevMouseX)/100.0, ydiff = (y - prevMouseY)/100.0;
		cameraSetting.dirx -= xdiff;
		cameraSetting.diry += ydiff;
		//cameraSetting.upz -= xdiff;
		prevMouseX = x;
		prevMouseY = y;
	}
	//printf("%d %d %f %f %f %f %f %f\n", xdiff,ydiff, cameraSetting.dirx,cameraSetting.diry,cameraSetting.dirz,cameraSetting.upx,cameraSetting.upy,cameraSetting.upz );
}

void mouse(int button, int state,int x, int y){
	/**
	* Callback when the mouse button is clicked
	*/
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		prevMouseX = INT_MAX;
	}
	
}
