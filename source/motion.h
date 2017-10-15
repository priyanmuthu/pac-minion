#define PI 3.141593
void computePacDirection();
void minimax();
void minimax2();
void agentsInit();

void camera(){
    /**
    * Sets the camera position and orientation
    */
	double tmp;
	tmp = abs(cameraSetting.upx + cameraSetting.upy + cameraSetting.upz);
	cameraSetting.upx/=tmp;
	cameraSetting.upy/=tmp;
	cameraSetting.upz/=tmp;
	//printf("%f %f %f %f %f %f\n", cameraSetting.dirx,cameraSetting.diry,cameraSetting.dirz,cameraSetting.upx,cameraSetting.upy,cameraSetting.upz );
	gluLookAt (cameraSetting.posx, cameraSetting.posy, cameraSetting.posz, 
		cameraSetting.dirx, cameraSetting.diry, cameraSetting.dirz, 
		cameraSetting.upx, cameraSetting.upy, cameraSetting.upz);
}

void moveCamera(float px, float py, float pz){
    /**
    * Moves the camera depending on the parameters passed
    * param px,py,pz: the distance to be moved along each axis.
    */
	cameraSetting.posx+=px;
	cameraSetting.posy+=py;
	cameraSetting.posz+=pz;
	cameraSetting.dirx+=px;
	cameraSetting.diry+=py;
	cameraSetting.dirz+=pz;
}

void checkKeys(){
    /**
    * Move the camera based on the key input.
    */
	float forwardX =  cameraSetting.posx - cameraSetting.dirx,
	forwardY =  cameraSetting.posy - cameraSetting.diry,
	forwardZ =  cameraSetting.posz - cameraSetting.dirz;

	float rightX = (forwardY * cameraSetting.upz) - (forwardZ * cameraSetting.upy),
	rightY = (forwardZ * cameraSetting.upx) - (forwardX * cameraSetting.upz),
	rightZ = (forwardX * cameraSetting.upy) - (forwardY * cameraSetting.upx);
	float sumRight = rightX + rightY + rightZ;
	rightX/=sumRight;
	rightZ/=sumRight;
	rightY/=sumRight;

    //Uses key states instead of keyrepeat handler to ensure smooth movement of the camera

	if(keystates[config->camFront]){
		moveCamera(-forwardX,-forwardY,-forwardZ);
	}

	if(keystates[config->camBack]){
		moveCamera(forwardX,forwardY,forwardZ);
	}

	if(keystates[config->strafeLeft]){ //todo: take dir cross up vector, move along that direction
		moveCamera(-rightX,-rightY,-rightZ);
	}

	if(keystates[config->strafeRight]){
		moveCamera(rightX,rightY,rightZ);
	}
	if(keystates[config->camUp]){
		moveCamera(cameraSetting.upx,cameraSetting.upy,cameraSetting.upz);
	}
	if(keystates[config->camDown]){
		moveCamera(-cameraSetting.upx,-cameraSetting.upy,-cameraSetting.upz);
	}
}
static void motion(int dummy=0){
    /**
    * Moving the Camera and PAC based on the Key inputs and the object property
    * @param dummy: parameter not used, just a dummy variable since its required by C++.
    */
    //printf("Motion happening!\n");
    if(nfood<=0){
        paused = true;
    }
    bool hasGhost(int, int);
    checkKeys();
    float pacxtmp, pacytmp;
    int pacColTemp, pacRowTemp;
    float timeDiff = curFrameTime - prevFrameTime;
    if(curFrameTime - panicStartTime > panicDuration && Ghost::getGhostMode()!=0){
    	inky.setGhostMode(0);
		pinky.setGhostMode(0);
		blinky.setGhostMode(0);
		clyde.setGhostMode(0);
    }
    /*int scDiff = curFrameTime - pacStartTime - npanic*panicDuration;
    if(scDiff > chaseScatterDuration[0] && scDiff < chaseScatterDuration[1] && Ghost::getGhostMode() != 2 && Ghost::getGhostMode() != 1){
    	inky.setGhostMode(1);
		pinky.setGhostMode(1);
		blinky.setGhostMode(1);
		clyde.setGhostMode(1);
    }
    else if(scDiff > chaseScatterDuration[1] && scDiff < chaseScatterDuration[2] && Ghost::getGhostMode() != 2 && Ghost::getGhostMode() != 0){
    	inky.setGhostMode(0);
		pinky.setGhostMode(0);
		blinky.setGhostMode(0);
		clyde.setGhostMode(0);
    }
    else if(scDiff > chaseScatterDuration[2] && scDiff < chaseScatterDuration[3] && Ghost::getGhostMode() != 2 && Ghost::getGhostMode() != 1){
    	inky.setGhostMode(1);
		pinky.setGhostMode(1);
		blinky.setGhostMode(1);
		clyde.setGhostMode(1);
    }
    else if(scDiff > chaseScatterDuration[3] && Ghost::getGhostMode() != 2 && Ghost::getGhostMode() != 0){
    	inky.setGhostMode(0);
		pinky.setGhostMode(0);
		blinky.setGhostMode(0);
		clyde.setGhostMode(0);
    }*/
    float cpacMoveDist = pacMoveDist * timeDiff / ANIM;
    //printf("%f %f %f\n", cpacMoveDist, pacMoveDist, timeDiff);
    glutTimerFunc(ANIM, motion, 0);
    if(paused){
        glutPostRedisplay();
        return;
    }
    if(config->antiAlias){
        glEnable(GL_MULTISAMPLE);
	}
	else{
		glDisable(GL_MULTISAMPLE);
	}
    //Computes the PAC direction using the AI algorithms for PAC simulation
    if((pacCurDir == GLUT_KEY_LEFT && hasGhost(pacCol - 1, pacRow)) ||
        (pacCurDir == GLUT_KEY_RIGHT && hasGhost(pacCol + 1, pacRow)) ||
        (pacCurDir == GLUT_KEY_UP && hasGhost(pacCol, pacRow + 1)) ||
        (pacCurDir == GLUT_KEY_DOWN && hasGhost(pacCol, pacRow - 1))){
        if(pacControlAgent == 1){
			computePacDirection();
		}
        else if(pacControlAgent == 2){
			minimax();
		}
        else if(pacControlAgent == 3){
            minimax2();
        }
	}
    //Code for changing the direction
    if(pacNextDir == GLUT_KEY_LEFT && maze[pacRow][pacCol-1]!=1)
    	pacCurDir = pacNextDir;
    else if(pacNextDir == GLUT_KEY_RIGHT && maze[pacRow][pacCol+1]!=1)
    	pacCurDir = pacNextDir;
    else if(pacNextDir == GLUT_KEY_DOWN && maze[pacRow-1][pacCol]!=1)
    	pacCurDir = pacNextDir;
    else if(pacNextDir == GLUT_KEY_UP && maze[pacRow+1][pacCol]!=1)
    	pacCurDir = pacNextDir;

    //Code for moving in the current direction
    
    if(pacCurDir == GLUT_KEY_LEFT){
    	pacxtmp = pacx- cpacMoveDist -PACSIZE;
    	pacColTemp = divRound(pacxtmp,gridCellSide);
    	if((pacColTemp>=0 && maze[pacRow][pacColTemp] != 1) || (pacRow == 16 && pacColTemp <= 2)){
    		pacx = pacx-cpacMoveDist;
    		if(pacx<-gridCellSide)
    			pacx += gridWidthby2 * 2;
    		pacCol = divRound(pacx,gridCellSide);
    	}
    }

    if(pacCurDir == GLUT_KEY_RIGHT){
    	pacxtmp = pacx+cpacMoveDist+PACSIZE;
    	pacColTemp = divRound(pacxtmp,gridCellSide);
    	if((pacColTemp>=0 && maze[pacRow][pacColTemp] != 1) || (pacRow ==16 && pacColTemp >= 26)){
    		pacx = pacx+cpacMoveDist;
    		if(pacx>gridWidthby2*2 - gridCellSide/2)
    			pacx -= (gridWidthby2) * 2;
    		pacCol = divRound(pacx,gridCellSide);
    	}
    }

    if(pacCurDir == GLUT_KEY_DOWN){
    	pacytmp = pacy-cpacMoveDist-PACSIZE;
    	pacRowTemp = divRound(pacytmp,gridCellSide);
    	if(pacRowTemp>=0 && maze[pacRowTemp][pacCol] != 1){
    		pacy = pacy-cpacMoveDist;
    		pacRow = divRound(pacy,gridCellSide);
    	}
    }

    if(pacCurDir == GLUT_KEY_UP){
    	pacytmp = pacy+cpacMoveDist+PACSIZE;
    	pacRowTemp = divRound(pacytmp,gridCellSide);
    	if(pacRowTemp>=0 && maze[pacRowTemp][pacCol] != 1){
    		pacy = pacy+cpacMoveDist;
    		pacRow = divRound(pacy,gridCellSide);
    	}
    }

    if(pacCurDir == GLUT_KEY_LEFT || pacCurDir == GLUT_KEY_RIGHT){
    	if((pacy - pacRow * gridCellSide) > pacMoveDist)
    		pacy-=pacMoveDist;
    	else if((pacy - pacRow * gridCellSide) < -pacMoveDist)
    		pacy+=pacMoveDist;
    	if(pacx-pacCol*gridCellSide < pacMoveDist*3 && pacx-pacCol*gridCellSide > -pacMoveDist*3 && maze[pacRow][pacCol]!=1){
    		if (maze[pacRow][pacCol] == 3)
    		{
    			inky.setGhostMode(2);
				pinky.setGhostMode(2);
				blinky.setGhostMode(2);
				clyde.setGhostMode(2);
				npanic++;
                score+=20;
    		}
            if(maze[pacRow][pacCol]==2){
                score+=10;
            }
    		maze[pacRow][pacCol] = 0;
    	}
    	if(pacx-pacCol*gridCellSide < pacMoveDist && pacx-pacCol*gridCellSide > -pacMoveDist){
    		if(pacControlAgent==1)
    			computePacDirection();
    		else if(pacControlAgent==2)
    			minimax();
            else if(pacControlAgent==3)
                minimax2();
    	}
    }

    if(pacCurDir == GLUT_KEY_UP || pacCurDir == GLUT_KEY_DOWN){
    	if((pacx - pacCol * gridCellSide) > pacMoveDist)
    		pacx-=pacMoveDist;
    	else if((pacx - pacCol * gridCellSide) < -pacMoveDist)
    		pacx+=pacMoveDist;
    	if(pacy-pacRow*gridCellSide < pacMoveDist*3 && pacy-pacRow*gridCellSide > -pacMoveDist*3 && maze[pacRow][pacCol]!=1){
    		if (maze[pacRow][pacCol] == 3)
    		{
    			inky.setGhostMode(2);
				pinky.setGhostMode(2);
				blinky.setGhostMode(2);
				clyde.setGhostMode(2);
				npanic++;
                score+=20;
    		}
            if(maze[pacRow][pacCol]==2){
                score+=10;
            }
    		maze[pacRow][pacCol] = 0;
    	}
    	if(pacy-pacRow*gridCellSide < pacMoveDist && pacy-pacRow*gridCellSide > -pacMoveDist){
    		if(pacControlAgent==1)
    			computePacDirection();
    		else if(pacControlAgent==2)
    			minimax();
            else if(pacControlAgent==3)
                minimax2();
    	}
    }

    /**
    * Detects the collision between the ghost and the PAC, and performs action according to the GHost mode and resets the positions of the Agents if required
    */

    if(Ghost::getGhostMode()!=2){
        if((pacRow == inky.row && pacCol == inky.col) || (pacRow == pinky.row && pacCol == pinky.col) || 
            (pacRow == blinky.row && pacCol == blinky.col) || (pacRow == clyde.row && pacCol == clyde.col)){
            score-=50;
            agentsInit();
            if(pacControlAgent == 0){
                blinky.moveDist = 0;
                inky.moveDist = 0;
                pinky.moveDist = 0;
                clyde.moveDist = 0;
                pacCurDir = pacNextDir = 0;
            }
            deathCount++;
        }
    }
    else{

        if(pacRow == inky.row && pacCol == inky.col){
            inky.row=16;
            inky.col=9;
            inky.init();
            score+=50;
        }
        if(pacRow == pinky.row && pacCol == pinky.col){
            pinky.row=16;
            pinky.col=18;
            pinky.init();
            score+=50;
        }
        if(pacRow == blinky.row && pacCol == blinky.col){
            blinky.row=19;
            blinky.col=14;
            blinky.init();
            score+=50;
        }
        if(pacRow == clyde.row && pacCol == clyde.col){
            clyde.row=13;
            clyde.col=14;
            clyde.init();
            score+=50;
        }
    }
    /**
    * Changing camera positions according to the present camera mode
    */
    if(cameraSetting.cameraMode==1){
        cameraSetting.posx = 0+(pacx - gridWidthby2)/4;
        cameraSetting.posy = -47.35+(pacy - gridHeightby2)/9;
        cameraSetting.posz = -21.15;

        cameraSetting.dirx = 0+(pacx - gridWidthby2)/3, cameraSetting.diry = -10.94 + (pacy - gridHeightby2)/7, cameraSetting.dirz = -100.0f;
        

        cameraSetting.upx = 0;
        cameraSetting.upy = 1;
        cameraSetting.upz = 0;
    }
    else if(cameraSetting.cameraMode==2){
        cameraSetting.posx = pacx - gridWidthby2;
        cameraSetting.posy = pacy - gridHeightby2;
        cameraSetting.posz = -25.0f;

        cameraSetting.upz = -55.0;
        cameraSetting.upx = pacx - gridWidthby2;
        cameraSetting.upy = pacy+1 - gridHeightby2;

        cameraSetting.dirx = pacx - gridWidthby2, cameraSetting.diry = pacy - gridHeightby2, cameraSetting.dirz = -100.0f;
        if(pacCurDir==GLUT_KEY_LEFT){

            cameraSetting.dirx = pacx-0.1 - gridWidthby2;
        }
        else if(pacCurDir==GLUT_KEY_RIGHT){
            cameraSetting.dirx = pacx+0.1 - gridWidthby2;
        }
        else if(pacCurDir==GLUT_KEY_DOWN){
            cameraSetting.diry = pacy-0.1 - gridHeightby2;
        }
        else if(pacCurDir==GLUT_KEY_UP){
            cameraSetting.diry = pacy+0.1 - gridHeightby2;
        }

        /*cameraSetting.upx = pacx;
        cameraSetting.upy = pacy;
        cameraSetting.upz = -95.0;*/
    }
    else if(cameraSetting.cameraMode==3){
        float tempposx = 0,
        tempposy = 35;
        cameraSetting.posz = -75.0;

        cameraSetting.upz = 1.0;
        cameraSetting.upx = 0;
        cameraSetting.upy = 0;

        float tempdirx = 0, tempdiry = -50;
        cameraSetting.dirz = -100.0f;
        float cost = cos(pacAngle*PI/180);
        float sint = sin(pacAngle*PI/180);
        cameraSetting.posx = cost * tempposx - sint * tempposy + pacx - gridWidthby2;
        cameraSetting.posy = sint * tempposx + cost * tempposy + pacy - gridHeightby2;
        cameraSetting.dirx = cost * tempdirx - sint * tempdiry + pacx - gridWidthby2;
        cameraSetting.diry = sint * tempdirx + cost * tempdiry + pacy - gridHeightby2;


    }
    else if(cameraSetting.cameraMode==4){
        float tempposx = 0,
        tempposy = -35;
        cameraSetting.posz = -55.0;

        cameraSetting.upz = 1.0;
        cameraSetting.upx = 0;
        cameraSetting.upy = 0;

        float tempdirx = 0, tempdiry = 50;
        cameraSetting.dirz = -100.0f;
        cameraSetting.posx = tempposx + pacx - gridWidthby2;
        cameraSetting.posy = tempposy + pacy - gridHeightby2;
        cameraSetting.dirx = tempdirx + pacx - gridWidthby2;
        cameraSetting.diry = tempdiry + pacy - gridHeightby2;


    }
    glutPostRedisplay(); 
}