/**
* This header file consists of functions and structure definition for the Artificial intelligence used in Ghosts.
*/

node getPacPosition(){
	/**
	* This function returns the pacposition as a node variable (Which will be used in IDA*)
	*/
	node n;
	n.xpos = pacCol;
	n.ypos = pacRow;
	return n;
}

int idaStar(node start, node end){
	/**
	* Implements Iterative deepening A*(IDA*) algorithm using recursion.
	* @param start: The starting node for IDA* (root)
	* @param end: The goal node for IDA*
	*/
	// printf("idaStar from %d %d to %d %d\n", start.xpos, start.ypos, end.xpos, end.ypos);
    int res = ida_star(start,end);
    // printf("Next node is %d %d\n", nextNode.xpos, nextNode.ypos);
    if(res!=-1){
    	// printf("Error!\n %d  %d  %d  %d\n",start.xpos,start.ypos,end.xpos,end.ypos);
    	return -1;
    }
    else{
    	// printf("start node is %d %d and Next node is %d %d\n", start.xpos, start.ypos, nextNode.xpos, nextNode.ypos);
    	if(start.xpos == nextNode.xpos){
    		if(start.ypos > nextNode.ypos){
    			// printf("idaStar returning %d GLUT_KEY_DOWN\n", GLUT_KEY_DOWN);
    			return GLUT_KEY_DOWN;
    		}
    		else{
    			// printf("idaStar returning %d GLUT_KEY_UP\n", GLUT_KEY_UP);
    			return GLUT_KEY_UP;
    		}
    	}
    	else{
    		if(start.xpos > nextNode.xpos){
    			// printf("idaStar returning %d\n GLUT_KEY_LEFT", GLUT_KEY_LEFT);
    			return GLUT_KEY_LEFT;
    		}
    		else{
    			// printf("idaStar returning %d\n GLUT_KEY_RIGHT", GLUT_KEY_RIGHT);
    			return GLUT_KEY_RIGHT;
    		}
    	}
    }
}

bool checkPosition(node temp){
	/**
	* Checks whether the given position is valid according to the gam maze.
	*/
 	if(temp.xpos >= 0 && temp.xpos < columnSize && temp.ypos >= 0 && temp.ypos < rowSize){
    	if(!boolmaze[temp.ypos][temp.xpos]){
    		return true;
    	}
	}
	return false;
}

int getGhostDirection(Ghost *ghost){
	/**
	* Gets the next Gohst direction which depends upon the Target.
	* The target is determined depending on the ghost behaviour, pac position and ghost mode.
	* This function determines the direction using the target which is passed to IDA*.
	* @param ghost: The ghost for which direction is to be determined.
	*/
	node start,end,temp;
	int ghostMode = Ghost::getGhostMode();
	int pacDir = pacCurDir;
	node pacPosition = getPacPosition();
    end = pacPosition;
    start.xpos = ghost->col;
    start.ypos = ghost->row;
    int ghostDirection = ghost->getDirection();

    if(ghostMode == 2){		//Panic Mode

		end.xpos = (int)((rand()*100)%columnSize);
		end.ypos = (int)((rand()*100)%rowSize);
		if(end.xpos<0)
			end.xpos*=-1;
		if(end.ypos<0)
			end.ypos*=-1;
		// printf("Setting ghost target as %d %d\n", end.xpos, end.ypos);
    }

	else if(ghost->getGhostNumber() == 0){		//Code for Inky

	    temp.xpos = pacPosition.xpos;
	    temp.ypos = pacPosition.ypos;

	    if(ghostMode == 0) {	//Chase Mode

	    	if(pacDir == GLUT_KEY_UP){
		    	temp.ypos = temp.ypos + 2;
		    }
		    else if(pacDir == GLUT_KEY_DOWN){
		    	temp.ypos = temp.ypos - 2;
		    }
		    else if(pacDir == GLUT_KEY_LEFT){
		    	temp.xpos = temp.xpos - 2;
		    	if(temp.xpos == -1)
		    		temp.xpos = 27;
		    	else if(temp.xpos == -2)
		    		temp.xpos = 26;
		    }
		    else if(pacDir == GLUT_KEY_RIGHT){
		    	temp.xpos = (temp.xpos + 2)%28;
		    }
		    
		    end.xpos = temp.xpos + (temp.xpos - blinky.row);
		    end.ypos = temp.ypos + (temp.ypos - blinky.col);
	    }
	    else if(ghostMode == 1){	//Scatter : Inky goes to bottom left corner
	    	end.ypos = 1;
	    	end.xpos = 1;
	    }
	}
	else if(ghost->getGhostNumber() == 1){		//Code for Pinky
		
		if(ghostMode == 0) {	//Chase Mode

	    	if(pacDir == GLUT_KEY_UP){
		    	end.ypos = pacPosition.ypos + 4;
		    }
		    else if(pacDir == GLUT_KEY_DOWN){
		    	end.ypos = pacPosition.ypos - 4;
		    }
		    else if(pacDir == GLUT_KEY_LEFT){
		    	end.xpos = pacPosition.xpos - 4;
		    }
		    else if(pacDir == GLUT_KEY_RIGHT){
		    	end.xpos = pacPosition.xpos + 4;
		    }
		}
		else if(ghostMode == 1){		//Scatter: Pinky will go to bottom right
			end.ypos = 1;
			end.xpos = 26;
		}
	}
	else if (ghost->getGhostNumber() == 2){		//Code for BLinky
		if(ghostMode == 0){
			end = pacPosition;
		}
		else if(ghostMode == 1){	//Scatter: Blinky will go to top left
			end.ypos = 29;
			end.xpos = 1;
		}
	}
	else{		//Code for Clyde

		if(ghostMode == 0){		//Chase mode

			int totalDist,t1,t2;
			t1 = (pacPosition.xpos-start.xpos);
			t2 = (pacPosition.ypos-start.ypos);
			totalDist = (t1<0?-t1:t1)+(t2<0?-t2:t2);
			if(totalDist > 8){
				end = pacPosition;
			}
			else{
				end.ypos = 29;
				end.xpos = 26;
			}
		}
		else if(ghostMode == 1){		//Scatter: Clyde goes to top right
			end.ypos = 29;
			end.xpos = 26;
		}
	}

	
	/*for(int i = 0; i<columnSize && !checkPosition(end); i++){		//Functionality shifted to idaStar.h
		
		temp.xpos = end.xpos - i;
		temp.ypos = end.ypos;
		if(checkPosition(temp)){
			end.xpos = end.xpos - i;
			break;
		}
		
		temp.xpos = end.xpos + i;
		temp.ypos = end.ypos;
		if(checkPosition(temp)){
			end.xpos = end.xpos + i;
			break;
		}
		
		temp.xpos = end.xpos;
		temp.ypos = end.ypos - i;
		if(checkPosition(temp)){
			end.ypos = end.ypos - i;
			break;
		}
		
		temp.xpos = end.xpos;
		temp.ypos = end.ypos + i;
		if(checkPosition(temp)){
			end.ypos = end.ypos + i;
			break;
		}
		
	}*/
	//if(ghostMode == 1){
	//	printf("idastar from %d %d to %d %d and boolmaze values: %d %d\n", start.xpos, start.ypos, end.xpos, end.ypos, boolmaze[start.ypos][start.xpos],boolmaze[end.ypos][end.xpos]);
	//}
    return idaStar(start, end);
}