
void setGhostBoolMaze(int ghostNumber, bool value);
/**
* This Header file contains the Class which defines the ghosts and their properties
*/
class Ghost{
	static int ghostMode; //0 - Chase 1 - Scatter 2 - Panic
	int ghostNumber; //0 - Inky 1 - Pinky 2 - Blinky 3 - Clyde
	float x, y;
	int curDir;
	int prevRow, prevCol;
	bool computeRequired;
public:
	int row, col;
	GLfloat color[4];
	float moveDist;

	Ghost(int n){
		/**
		* Constructor to initialize the ghost.
		* @param n: This represents the type of the ghost (0- Inky, 1-Pinky, 2- Blinky, 3- Clyde)
		*/
		ghostNumber = n;
		curDir = GLUT_KEY_LEFT;
		computeRequired = true;
		if(n==0){
			color[0] = 0.1f;
			color[1] = 0.5f;
			color[2] = 0.8f;
			color[3] = 0.8f;
		}
		else if(n==2){
			color[0] = 0.9f;
			color[1] = 0.1f;
			color[2] = 0.1f;
			color[3] = 0.8f;
		}
		else if(n==1){
			color[0] = 0.8f;
			color[1] = 0.5f;
			color[2] = 0.1f;
			color[3] = 0.8f;
		}
		else if(n==3){
			color[0] = 0.8f;
			color[1] = 0.1f;
			color[2] = 0.5f;
			color[3] = 0.8f;
		}
	}

	float getX(){
		return x;
	}

	float getY(){
		return y;
	}

	void init(){
		/**
		* sets the exact coordinates of the ghost depending on the row, col position and the window size
		*/
		prevCol = col;
		prevRow = row;
		x = gridCellSide * (col);
	    y = gridCellSide * (row);
	    gridHeightby2 = 31.0 * 0.5 * gridCellSide;
	    gridWidthby2 = 28.0 * 0.5 * gridCellSide;
	    col = divRound(x,gridCellSide);
	    row = divRound(y,gridCellSide);
	}

	static int getGhostMode(){
		return ghostMode;
	}

	void setGhostMode(int newMode){
		/**
		* Sets the ghost mode depending on the mode passed in the parameter
		* @param newMode: The mode to which the current mode should be changed.
		*/
		ghostMode = newMode;
		if(pacStartTime != -1){
			if(newMode != 2 && moveDist>0.01){
				moveDist = pacMoveDist * ghostSpeedPercent;
				panicStartTime = 0;
			}
			else{
				panicStartTime = glutGet(GLUT_ELAPSED_TIME);
				moveDist = pacMoveDist * 0.5;
			}
		}
		if((newMode == 1 || newMode == 2)&& pacStartTime!=-1){
			if(curDir == GLUT_KEY_LEFT)
				curDir = GLUT_KEY_RIGHT;
			else if(curDir == GLUT_KEY_RIGHT)
				curDir = GLUT_KEY_LEFT;
			else if(curDir == GLUT_KEY_UP)
				curDir = GLUT_KEY_DOWN;
			else if(curDir == GLUT_KEY_DOWN)
				curDir = GLUT_KEY_UP;
			/*if(curDir == GLUT_KEY_LEFT){
				if(maze[row][col+1]!=1)
					curDir = GLUT_KEY_RIGHT;
				else if(maze[row+1][col]!=1)
					curDir = GLUT_KEY_UP;
				else if(maze[row-1][col]!=1)
					curDir = GLUT_KEY_DOWN;
			}
			else if(curDir == GLUT_KEY_RIGHT){
				if(maze[row][col-1]!=1)
					curDir = GLUT_KEY_LEFT;
				else if(maze[row+1][col]!=1)
					curDir = GLUT_KEY_UP;
				else if(maze[row-1][col]!=1)
					curDir = GLUT_KEY_DOWN;
			}
			else if(curDir == GLUT_KEY_UP){
				if(maze[row-1][col]!=1)
					curDir = GLUT_KEY_DOWN;
				else if(maze[row][col-1]!=1)
					curDir = GLUT_KEY_LEFT;
				else if(maze[row][col+1]!=1)
					curDir = GLUT_KEY_RIGHT;
			}
			else if(curDir == GLUT_KEY_DOWN){
				if(maze[row+1][col]!=1)
					curDir = GLUT_KEY_UP;
				else if(maze[row][col-1]!=1)
					curDir = GLUT_KEY_LEFT;
				else if(maze[row][col+1]!=1)
					curDir = GLUT_KEY_RIGHT;
			}*/
		}
	}

	int getGhostNumber(){
		return ghostNumber;
	}

	int getDirection(){
		return curDir;
	}
	void move(){
		/**
		* Determines how the ghost should move and moves the ghost accordingly.
		* Uses the getGhostDirection function to determine the direction using IDA* (present in ghostAI.h)
		*/
		int getGhostDirection(Ghost *);
		float xtmp, ytmp;
	    int colTemp, rowTemp;
	    float timeDiff = curFrameTime - prevFrameTime;
	    float cMoveDist = moveDist * timeDiff / ANIM;
	    //Uncomment following part after getting priyn's code.
	    // printf("y = %f and row*gridCellSide = %f and abs difference = %f\n", y, row*gridCellSide,absm(y-(row*gridCellSide)));
	    // printf("x = %f and col*gridCellSide = %f and abs difference = %f\n", x, col*gridCellSide,absm(x-(col*gridCellSide)));
		if(absm(y-(row*gridCellSide)) < pacMoveDist && absm(x-(col*gridCellSide)) < pacMoveDist){
			//printf("moveDist=%f\n", moveDist);
			int tmpPrevRow = prevRow, tmpPrevCol = prevCol;
			bool changeBoolMaze = false;

			int tmpPacRow = pacRow, tmpPacCol = pacCol; 
			if(ghostNumber==0 || ghostNumber==1)
				boolmaze[tmpPacRow][tmpPacCol]=1;
			if (((curDir == GLUT_KEY_LEFT || curDir == GLUT_KEY_RIGHT) && (maze[row+1][col]!=1 || maze[row-1][col]!=1)) ||
				((curDir == GLUT_KEY_UP || curDir == GLUT_KEY_DOWN) && (maze[row][col+1]!=1 || maze[row][col-1]!=1))) {
				
				if(!boolmaze[tmpPrevRow][tmpPrevCol]){
					boolmaze[tmpPrevRow][tmpPrevCol] = true;
					changeBoolMaze = true;
					// printf("Change in boolmaze according to prev ghost position!\n");
					// printf("prev row and col:%d %d and boolmaze at that point: %d\n", tmpPrevRow, tmpPrevCol,boolmaze[tmpPrevRow][tmpPrevCol]);
					// printf("Current Row and column %d %d\n", row, col);
				}
				setGhostBoolMaze(ghostNumber, true);
				
				int newDir = getGhostDirection(this);
				if(newDir != -1)
					curDir = newDir;
				else{
					if(curDir != GLUT_KEY_LEFT && !boolmaze[pacRow][pacCol+1])
						curDir = GLUT_KEY_RIGHT;
					else if(curDir != GLUT_KEY_RIGHT && !boolmaze[pacRow][pacCol-1])
						curDir = GLUT_KEY_LEFT;
					else if(curDir != GLUT_KEY_DOWN && !boolmaze[pacRow+1][pacCol])
						curDir = GLUT_KEY_UP;
					else 
						curDir = GLUT_KEY_DOWN;
				}
				//printf("Got new direction : %d\n", curDir);
				setGhostBoolMaze(ghostNumber, false);

				computeRequired = false;
				if(changeBoolMaze){
					boolmaze[tmpPrevRow][tmpPrevCol] = false;
					changeBoolMaze = false;
				}
			}
			
			if(ghostNumber==0 || ghostNumber==1)
				boolmaze[tmpPacRow][tmpPacCol]=0;
		}

		//movement starts here
		if(curDir == GLUT_KEY_LEFT){
			xtmp = x - cMoveDist - PACSIZE;
	    	colTemp = divRound(xtmp,gridCellSide);
	    	if((colTemp>=0 && maze[row][colTemp] != 1) || (row == 16 && colTemp <= 2)){
	    		x = x-cMoveDist;
	    		/*if(x<-gridCellSide)
	    			x += gridWidthby2 * 2;*/
	    		colTemp = divRound(x,gridCellSide);
	    		if(colTemp != col){
	    			prevCol = col;
	    			col = colTemp;
	    			// printf("Changing column to %d to %d :P\n", prevCol, col);
	    			computeRequired = true;
	    			prevRow = row;
	    		}
	    	}
		}

		else if(curDir == GLUT_KEY_RIGHT){
			xtmp = x + cMoveDist + PACSIZE;
	    	colTemp = divRound(xtmp,gridCellSide);
	    	if((colTemp>=0 && maze[row][colTemp] != 1) || (row == 16 && colTemp >= 26)){
	    		x = x+cMoveDist;
	    		/*if(x>gridCellSide)
	    			x -= gridWidthby2 * 2;*/
	    		colTemp = divRound(x,gridCellSide);
	    		if(colTemp != col){
	    			prevCol = col;
	    			col = colTemp;
	    			// printf("Changing column to %d to %d\n", prevCol, col);
	    			computeRequired = true;
	    			prevRow = row;
	    		}
	    	}
		}

		else if(curDir == GLUT_KEY_DOWN){
	    	ytmp = y-cMoveDist-PACSIZE;
	    	rowTemp = divRound(ytmp,gridCellSide);
	    	if(rowTemp>=0 && maze[rowTemp][col] != 1){
	    		y = y-cMoveDist;
	    		rowTemp = divRound(y,gridCellSide);
	    		if(rowTemp != row){
	    			prevRow = row;
	    			row = rowTemp;
	    			// printf("Changing row to %d to %d\n", prevRow, row);
	    			computeRequired = true;
	    			prevCol = col;
	    		}
		    }
	    }

	    else if(curDir == GLUT_KEY_UP){
	    	ytmp = y+cMoveDist+PACSIZE;
	    	rowTemp = divRound(ytmp,gridCellSide);
	    	if(rowTemp>=0 && maze[rowTemp][col] != 1){
	    		y = y+cMoveDist;
	    		rowTemp = divRound(y,gridCellSide);
	    		if(rowTemp != row){
	    			prevRow = row;
	    			row = rowTemp;
	    			// printf("Changing row to %d to %d\n", prevRow, row);
	    			computeRequired = true;
	    			prevCol = col;
	    		}
		    }
	    }

	    if(col<0){
	    	col = 27;
	    	x = gridCellSide * (col) + moveDist*4;
	    }
	    else if(col>27){
	    	col = 0;
	    	x = gridCellSide * (col) - moveDist*4;
	    }

	}

	int getDistFrom(int r, int c){
		int xd,yd;
		xd = col - c;
		yd = row - r;
		if(xd<0)
			xd*=-1;
		if(yd<0)
			yd*=-1;
		return xd+yd;
	}

	node getPosition(){
		node tmp;
		tmp.xpos = col;
		tmp.ypos = row;
		return tmp;
	}
}inky(0), pinky(1), blinky(2), clyde(3); //Defaults: inky(0), pinky(1), blinky(2), clyde(3);

int Ghost::ghostMode = 0;


void setGhostBoolMaze(int ghostNumber, bool value){
	/**
	* Changes the value of the bool maze to restrict the movement of the other ghosts and the PAC
	* @param ghostNumber: The ghost for which is affecting the bool maze
	* @param value: Value to which it should be changed.
	*/
	if(ghostNumber == 1){
		boolmaze[blinky.row][blinky.col] = value;
	}
	else if(ghostNumber == 0){
		boolmaze[blinky.row][blinky.col] = value;
		boolmaze[pinky.row][pinky.col] = value;
	}
	else if(ghostNumber == 3){
		boolmaze[blinky.row][blinky.col] = value;
		boolmaze[pinky.row][pinky.col] = value;
		boolmaze[inky.row][inky.col] = value;
	}
}