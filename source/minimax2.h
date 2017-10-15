/* This code uses minimax algorithm to simulate the PAC-Minion Game */
/* Agent Number: 0 - Pacman, 1 - Inky, 2 - Pinky, 3 - Blinky, 4 - Clyde*/

#define columnSize 28
#define rowSize 31
#define maxDepth2 7
//Function Headers
void minimax2();
float minimaxAgent2(GameState state, int depth);
// float findMin(float valueUp, float valueDown, float valueLeft, float valueRight);
// float findMax(float valueUp, float valueDown, float valueLeft, float valueRight);
void moveGhosts(GameState state);
node getNextGhostNode(GameState state,int agentNumber, node endNode, bool flag);

void minimax2(){
	/**
	* Uses MINIMAX algorithm same as the minimax() function, but uses tree pruning specific to the local problem to 
	* reduce computation and improving overall performance of the game.
	*/
	GameState initialState;
	for (int i = 0; i < rowSize; ++i)
	{
		for (int j = 0; j < columnSize; ++j)
		{
			initialState.maze[i][j] = maze[i][j];
		}
	}
	initialState.agentPosition[0] = getPacPosition();
	// printf("pac position in minimax: %d %d\n", initialState.agentPosition[0].xpos, initialState.agentPosition[0].ypos);
	initialState.agentPosition[1] = inky.getPosition();
	initialState.agentPosition[2] = pinky.getPosition();
	initialState.agentPosition[3] = blinky.getPosition();
	initialState.agentPosition[4] = clyde.getPosition();
	initialState.agentDirection[0] = pacCurDir;
	initialState.agentDirection[1] = inky.getDirection();
	initialState.agentDirection[2] = pinky.getDirection();
	initialState.agentDirection[3] = blinky.getDirection();
	initialState.agentDirection[4] = clyde.getDirection();


	// printf("minimax called\n");
	
	minimaxAgent2(initialState,0);
	// printf("pacDir: %d\n", pacNextDir);

}


float minimaxAgent2(GameState state, int depth){
	/**
	* Uses recursion similar to minimaxAgent() function, but uses tree pruning.
	* @param state: current explored game state
	* @param depth: current depth of the state space tree generated
	*/
	if(depth >= maxDepth2){
		return evaluationFunction(state);
	}
	else{
		node temp;
		float valueUp, valueDown, valueLeft, valueRight;
		valueUp = INT_MAX;
		valueDown = INT_MAX;
		valueLeft = INT_MAX;
		valueRight = INT_MAX;
		//Up
		temp.xpos = state.agentPosition[0].xpos;
		temp.ypos = state.agentPosition[0].ypos + 1;
		if(checkNodePosition(state, temp)){
			// printf("up called %d\n", agentNumber);
			GameState newState = generateState(state, 0, 'u');
			moveGhosts(newState);
			valueUp = minimaxAgent2(newState, depth+1);
			valueUp+=evaluationFunction(state);
		}
		//Down
		temp.xpos = state.agentPosition[0].xpos;
		temp.ypos = state.agentPosition[0].ypos - 1;
		if(checkNodePosition(state, temp)){
			// printf("down called %d\n", agentNumber);
			GameState newState = generateState(state, 0, 'd');
			moveGhosts(newState);
			valueDown = minimaxAgent2(newState, depth+1);
			valueDown+=evaluationFunction(state);
		}
		//Left
		temp.xpos = state.agentPosition[0].xpos - 1;
		temp.ypos = state.agentPosition[0].ypos;
		if(checkNodePosition(state, temp)){
			// printf("left called %d\n", agentNumber);
			// printf("pacposition in minimax agent: %d  %d\n", state.agentPosition[agentNumber].xpos, state.agentPosition[agentNumber].ypos);
			GameState newState = generateState(state, 0, 'l');
			moveGhosts(newState);
			valueLeft = minimaxAgent2(newState, depth+1);
			valueLeft+=evaluationFunction(state);
		}
		//Right
		temp.xpos = state.agentPosition[0].xpos + 1;
		temp.ypos = state.agentPosition[0].ypos;
		if(checkNodePosition(state, temp)){
			// printf("right called %d\n", agentNumber);
			GameState newState = generateState(state, 0, 'r');
			moveGhosts(newState);
			 valueRight = minimaxAgent2(newState, depth+1);
			 valueRight+=evaluationFunction(state);
		}
		
		// printf("values: %f %f %f %f\n", valueUp, valueDown, valueLeft, valueRight);


		float minVal = findMin(valueUp, valueDown, valueLeft, valueRight);
		if(minVal == valueUp)
			pacNextDir = GLUT_KEY_UP;
		else if(minVal == valueDown)
			pacNextDir = GLUT_KEY_DOWN;
		else if(minVal == valueLeft)
			pacNextDir = GLUT_KEY_LEFT;
		else
			pacNextDir = GLUT_KEY_RIGHT;

		// printf("pac direction: %d %f %f %f %f\n", pacNextDir, valueUp, valueDown, valueLeft, valueRight);
		// printf("minimax returns - agent 0: %f\n",minVal );
		if(depth == 0){
			//printf("score: %f, up: %f down: %f left: %f right: %f\n", minVal, valueUp, valueDown, valueLeft, valueRight);
		}
		return minVal;

	}
}

void moveGhosts(GameState state){
	/**
	* Moves the ghost according to their behaviour, PAC position and current ghost mode.
	*/
	node start,end, temp;
	temp.xpos = state.agentPosition[0].xpos;
    temp.ypos = state.agentPosition[0].ypos;
	

	//For inky(1)
	if(inky.getGhostMode() == 0){			//Chase mode
		if(state.agentDirection[0] == GLUT_KEY_UP){
	    	temp.ypos = temp.ypos + 2;
	    }
	    else if(state.agentDirection[0] == GLUT_KEY_DOWN){
	    	temp.ypos = temp.ypos - 2;
	    }
	    else if(state.agentDirection[0] == GLUT_KEY_LEFT){
	    	temp.xpos = temp.xpos - 2;
	    	if(temp.xpos == -1)
	    		temp.xpos = 27;
	    	else if(temp.xpos == -2)
	    		temp.xpos = 26;
	    }
	    else if(state.agentDirection[0] == GLUT_KEY_RIGHT){
	    	temp.xpos = (temp.xpos + 2)%28;
	    }
	    
	    end.xpos = temp.xpos + (temp.xpos - blinky.row);
	    end.ypos = temp.ypos + (temp.ypos - blinky.col);
	    node nextNode = getNextGhostNode(state,1,end, false);
	    state.agentPosition[1].xpos = nextNode.xpos;
	    state.agentPosition[1].ypos = nextNode.ypos;
	}
	else if(inky.getGhostMode() == 1){		//Scatter Mode
		end.ypos = 1;
    	end.xpos = 1;
    	node nextNode = getNextGhostNode(state,1,end, false);
	    state.agentPosition[1].xpos = nextNode.xpos;
	    state.agentPosition[1].ypos = nextNode.ypos;
    }
    else{					//Panic Mode
    	node nextNode = getNextGhostNode(state,1,state.agentPosition[0], true);
	    state.agentPosition[1].xpos = nextNode.xpos;
	    state.agentPosition[1].ypos = nextNode.ypos;
    }


   //For pinky(2)
    if(pinky.getGhostMode() == 0) {	//Chase Mode

    	if(state.agentDirection[0] == GLUT_KEY_UP){
	    	end.ypos = state.agentPosition[0].ypos + 4;
	    }
	    else if(state.agentDirection[0] == GLUT_KEY_DOWN){
	    	end.ypos = state.agentPosition[0].ypos - 4;
	    }
	    else if(state.agentDirection[0] == GLUT_KEY_LEFT){
	    	end.xpos = state.agentPosition[0].xpos - 4;
	    }
	    else if(state.agentDirection[0] == GLUT_KEY_RIGHT){
	    	end.xpos = state.agentPosition[0].xpos + 4;
	    }

	    node nextNode = getNextGhostNode(state,2,end, false);
	    state.agentPosition[2].xpos = nextNode.xpos;
	    state.agentPosition[2].ypos = nextNode.ypos;
	}
	else if(pinky.getGhostMode() == 1){		//Scatter: Pinky will go to bottom right
		end.ypos = 1;
		end.xpos = 26;
		node nextNode = getNextGhostNode(state,2,end, false);
	    state.agentPosition[2].xpos = nextNode.xpos;
	    state.agentPosition[2].ypos = nextNode.ypos;
	}
	else{
		node nextNode = getNextGhostNode(state,2,state.agentPosition[0], true);
	    state.agentPosition[2].xpos = nextNode.xpos;
	    state.agentPosition[2].ypos = nextNode.ypos;
	}

	//For Blinky(3)
	if(blinky.getGhostMode() == 0){
			end.xpos = state.agentPosition[0].xpos;
			end.ypos = state.agentPosition[0].ypos;
			node nextNode = getNextGhostNode(state,3,end, false);
		    state.agentPosition[3].xpos = nextNode.xpos;
		    state.agentPosition[3].ypos = nextNode.ypos;
		}
		else if(blinky.getGhostMode() == 1){	//Scatter: Blinky will go to top left
			end.ypos = 29;
			end.xpos = 1;
			node nextNode = getNextGhostNode(state,3,end, false);
		    state.agentPosition[3].xpos = nextNode.xpos;
		    state.agentPosition[3].ypos = nextNode.ypos;
		}
		else{
			node nextNode = getNextGhostNode(state,3,state.agentPosition[0], true);
		    state.agentPosition[3].xpos = nextNode.xpos;
		    state.agentPosition[3].ypos = nextNode.ypos;
		}


	//For Clyde
	if(clyde.getGhostMode() == 0){		//Chase mode

		int totalDist,t1,t2;
		t1 = (state.agentPosition[0].xpos-start.xpos);
		t2 = (state.agentPosition[0].ypos-start.ypos);
		totalDist = (t1<0?-t1:t1)+(t2<0?-t2:t2);
		if(totalDist > 8){
			end.xpos = state.agentPosition[0].xpos;
			end.ypos = state.agentPosition[0].ypos;
		}
		else{
			end.ypos = 29;
			end.xpos = 26;
		}
		node nextNode = getNextGhostNode(state,4,end, false);
	    state.agentPosition[4].xpos = nextNode.xpos;
	    state.agentPosition[4].ypos = nextNode.ypos;
	}
	else if(clyde.getGhostMode() == 1){		//Scatter: Clyde goes to top right
		end.ypos = 29;
		end.xpos = 26;
		node nextNode = getNextGhostNode(state,4,end, false);
	    state.agentPosition[4].xpos = nextNode.xpos;
	    state.agentPosition[4].ypos = nextNode.ypos;
	}
	else{
		node nextNode = getNextGhostNode(state,4,state.agentPosition[0], true);
	    state.agentPosition[4].xpos = nextNode.xpos;
	    state.agentPosition[4].ypos = nextNode.ypos;
	}
	if(state.maze[state.agentPosition[0].ypos][state.agentPosition[0].xpos] == 2 || state.maze[state.agentPosition[0].ypos][state.agentPosition[0].xpos] == 3){
		state.maze[state.agentPosition[0].ypos][state.agentPosition[0].xpos] = 0;
	}
}

node getNextGhostNode(GameState state, int agentNumber, node endNode, bool flag){
	/**
	* Gets the next ghost node depending on the state space explored and the state evaluation fucntion to maxumize the ghost score (or minimize the plaers score).
	* @param state: current explored game state.
	* @param agentNumber: The agent number.
	* @param endNode: The goal node for the agent
	* @param flag: true for normal mode, false for panic mode.
	*/
	
	int valueUp, valueDown, valueLeft, valueRight;
	node temp;
	if(flag){
		valueDown = INT_MIN;
		valueUp = INT_MIN;
		valueLeft = INT_MIN;
		valueRight = INT_MIN;
	}
	else{
		valueDown = INT_MAX;
		valueUp = INT_MAX;
		valueLeft = INT_MAX;
		valueRight = INT_MAX;
	}
	
	int oldValue = h(state.agentPosition[agentNumber], endNode);
	//for up direction
    temp.xpos = state.agentPosition[agentNumber].xpos;
    temp.ypos = state.agentPosition[agentNumber].ypos + 1;
    if(checkNodePosition(state, temp) && state.agentDirection[1] != GLUT_KEY_DOWN){
    	//To the necessarry
    	valueUp = h(endNode, temp) - oldValue;
    }
    //For down direction
    temp.xpos = state.agentPosition[agentNumber].xpos;
    temp.ypos = state.agentPosition[agentNumber].ypos - 1;
    if(checkNodePosition(state, temp) && state.agentDirection[1] != GLUT_KEY_UP){
    	valueDown = h(endNode, temp) - oldValue;
    }
    //For left
    temp.xpos = state.agentPosition[agentNumber].xpos - 1;
    temp.ypos = state.agentPosition[agentNumber].ypos;
    if(checkNodePosition(state, temp) && state.agentDirection[1] != GLUT_KEY_RIGHT){
    	 valueLeft= h(endNode, temp) - oldValue;
    }
    //For right
    temp.xpos = state.agentPosition[agentNumber].xpos + 1;
    temp.ypos = state.agentPosition[agentNumber].ypos;
    if(checkNodePosition(state, temp) && state.agentDirection[1] != GLUT_KEY_LEFT){
    	 valueRight= h(endNode, temp) - oldValue;
    }

    int minVal;
    if(flag){
    	minVal = findMax(valueUp, valueDown, valueLeft, valueRight);
    }
    else{
    	minVal = findMin(valueUp, valueDown, valueLeft, valueRight);
    }
    node ret;
    ret.xpos = state.agentPosition[agentNumber].xpos;
    ret.ypos = state.agentPosition[agentNumber].ypos;
    if(minVal == valueUp){
    	ret.ypos = state.agentPosition[agentNumber].ypos + 1;
    }
    else if(minVal == valueDown){
    	ret.ypos = state.agentPosition[agentNumber].ypos - 1;
    }
    else if(minVal == valueLeft){
    	ret.xpos = state.agentPosition[agentNumber].xpos - 1;
    }
    else{
    	ret.xpos = state.agentPosition[agentNumber].xpos + 1;
    }
    return ret;
}
