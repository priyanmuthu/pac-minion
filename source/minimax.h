/* This code uses minimax algorithm to simulate the PAC-Minion Game */
/* Agent Number: 0 - Pacman, 1 - Inky, 2 - Pinky, 3 - Blinky, 4 - Clyde*/

#define columnSize 28
#define rowSize 31
#define maxDepth 10

struct GameState{
	int maze[31][28];
	node agentPosition[5];
	int agentDirection[5];
};


//Function Headers
GameState generateState(GameState currentState, int agentNumber, char action);
bool checkNodePosition(GameState state, node temp);
void minimax();
float minimaxAgent(GameState state, int agentNumber, int depth);
float findMin(float valueUp, float valueDown, float valueLeft, float valueRight);
float findMax(float valueUp, float valueDown, float valueLeft, float valueRight);
int findNoOfFood(GameState state);
node nearestFoodNode(GameState state);

float evaluationFunction(GameState state){
	/**
	* This function evaluates the score of the given state
	* @param state: The state to be evaluated
	*/

	//Find the nearest food particle from pacman
	float totalScore;
	node nearfood = nearestFoodNode(state);
	// printf("nearest food: %d %d\n", nearfood.xpos,nearfood.ypos);
	int foodScore = h(state.agentPosition[0], nearfood);
	int foodLeft = findNoOfFood(state);
	int inkyScore = h(state.agentPosition[0], state.agentPosition[1]);
	int pinkyScore = h(state.agentPosition[0], state.agentPosition[2]);
	int blinkyScore = h(state.agentPosition[0], state.agentPosition[3]);
	int clydeScore = h(state.agentPosition[0], state.agentPosition[4]);
	// int ghostScore = (inkyScore + pinkyScore + blinkyScore /*+ clydeScore*/);
	float gs = findMin((float)inkyScore, (float)pinkyScore, (float)blinkyScore, (float)clydeScore);
	int ghostScore = (int)gs;
	if(Ghost::getGhostMode() == 2){
		totalScore = foodLeft*10;
		if(inkyScore < 8){
			totalScore+= (inkyScore)*10;
			totalScore+=foodScore;
		}
		else{
			totalScore+= (inkyScore);
			totalScore += foodScore*10;
		}
		if(pinkyScore < 8){
			totalScore+= (pinkyScore)*10;
			totalScore+=foodScore;
		}
		else{
			totalScore+= (pinkyScore);
			totalScore += foodScore*10;
		}
		if(blinkyScore < 8){
			totalScore+= (blinkyScore)*10;
			totalScore+=foodScore;
		}
		else{
			totalScore+= (blinkyScore);
			totalScore += foodScore*10;
		}
		if(clydeScore < 8){
			totalScore+= (clydeScore)*10;
			totalScore+=foodScore;
		}
		else{
			totalScore+= (clydeScore);
			totalScore += foodScore*10;
		}
	}
	// float totalScore =  (1.0/ghostScore)*100 + (foodLeft) + (foodScore*100);
	/*if(ghostScore <= 7){
		totalScore =  (1.0/ghostScore)*100 + (foodLeft) + (foodScore*10);	
	}
	else{
		totalScore =  (1.0/ghostScore)*10 + (foodLeft) + (foodScore*100);
	}*/
	else{
		totalScore = foodLeft*10;
		if(inkyScore < 4){
			totalScore+= (1.0/inkyScore)*100;
			totalScore+=foodScore/2;
		}
		else{
			totalScore+= (1.0/inkyScore)*10;
			totalScore += foodScore*10;
		}
		if(pinkyScore < 4){
			totalScore+= (1.0/pinkyScore)*100;
			totalScore+=foodScore/2;
		}
		else{
			totalScore+= (1.0/pinkyScore)*10;
			totalScore += foodScore*10;
		}
		if(blinkyScore < 4){
			totalScore+= (1.0/blinkyScore)*100;
			totalScore+=foodScore/2;
		}
		else{
			totalScore+= (1.0/blinkyScore)*10;
			totalScore += foodScore*10;
		}
		if(clydeScore < 4){
			totalScore+= (1.0/clydeScore)*100;
			totalScore+=foodScore/2;
		}
		else{
			totalScore+= (1.0/clydeScore)*10;
			totalScore += foodScore*10;
		}
	}
	// float totalScore =  (1.0/ghostScore)*100 + (foodLeft) + (foodScore*100);

	// printf("total Score: %f\n", totalScore);

	return totalScore;

}

void copyGameState(GameState* a, GameState *b){
	/**
	* Copies the one game state to the other. (Acts as a copy constructor).
	*/
	for (int i = 0; i < rowSize; ++i)
	{
		for (int j = 0; j < columnSize; ++j)
		{
			a->maze[i][j] = b->maze[i][j];
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		a->agentPosition[i].xpos = b->agentPosition[i].xpos;
		a->agentPosition[i].ypos = b->agentPosition[i].ypos;
	}

	for (int i = 0; i < 5; ++i)
	{
		a->agentDirection[i] = b->agentDirection[i];
	}
}

GameState generateState(GameState currentState, int agentNumber, char action){
	/**
	* Generates new state depending on the current state, the agent and the action taken by the agent.
	* @param currentState: Current state in the explored state space search graph.
	* @param agentNumber: The agent which is taking the action
	* @param action: A valid action taken by the agent.
	*/
	GameState newState;
	copyGameState(&newState, &currentState);
	
	// printf("PacPosition in current state: %d %d\n",currentState.agentPosition[0].xpos, currentState.agentPosition[0].ypos );
	if(action == 'u'){
		newState.agentPosition[agentNumber].ypos = currentState.agentPosition[agentNumber].ypos + 1;
		newState.agentDirection[agentNumber] = 'u';
	}
	else if(action == 'd'){
		newState.agentPosition[agentNumber].ypos = currentState.agentPosition[agentNumber].ypos - 1;
		newState.agentDirection[agentNumber] = 'd';
	}
	else if(action == 'l'){
		newState.agentPosition[agentNumber].xpos = currentState.agentPosition[agentNumber].xpos - 1;
		newState.agentDirection[agentNumber] = 'l';
	}
	else if(action == 'r'){
		newState.agentPosition[agentNumber].xpos = currentState.agentPosition[agentNumber].xpos + 1;
		newState.agentDirection[agentNumber] = 'r';
	}
	/*
	if(agentNumber == 0){
		if(currentState.maze[newState.agentPosition[0].ypos][newState.agentPosition[0].xpos] == 2 || currentState.maze[newState.agentPosition[0].ypos][newState.agentPosition[0].xpos] == 3){
			newState.maze[currentState.agentPosition[0].ypos][currentState.agentPosition[0].xpos] = 0;
		}
	}*/
	// printf("PacPosition in new state: %d %d\n",newState.agentPosition[0].xpos, newState.agentPosition[0].ypos );

	return newState;

}

bool checkNodePosition(GameState state, node temp){
	/**
	* Checks for the validity of the given node according to the game maze
	*/
 	if(temp.xpos >= 0 && temp.xpos < columnSize && temp.ypos >= 0 && temp.ypos < rowSize){
 		if(state.maze[temp.ypos][temp.xpos] == 1)
    		return false;
	}
	return true;
}

void minimax(){
	/**
	* Finds a direction for the PAC by using limited depth MINIMAX algorithm,
	* which explores the state space by dynamically adding action and forming a tree.
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
	
	minimaxAgent(initialState,0,0);
	// printf("pacDir: %d\n", pacNextDir);
	/*if(pacNextDir == GLUT_KEY_LEFT && Ghost::getGhostMode()!=2 && (hasGhost(pacCol - 1, pacRow) || hasGhost(pacCol - 1, pacRow-1) || hasGhost(pacCol - 1, pacRow+1) )) {
		// printf("Ghost left of me.\n");
		if(maze[pacRow][pacCol+1]!=1)
			pacNextDir = GLUT_KEY_RIGHT;
		else if(maze[pacRow+1][pacCol]!=1)
			pacNextDir = GLUT_KEY_UP;
		else if(maze[pacRow-1][pacCol]!=1)
			pacNextDir = GLUT_KEY_DOWN;
	}
	else if(pacNextDir == GLUT_KEY_RIGHT && (hasGhost(pacCol + 1, pacRow)|| hasGhost(pacCol + 1, pacRow-1) || hasGhost(pacCol + 1, pacRow+1)) && Ghost::getGhostMode()!=2){
		// printf("Ghost right of me.\n");
		if(maze[pacRow][pacCol-1]!=1)
			pacNextDir = GLUT_KEY_LEFT;
		else if(maze[pacRow+1][pacCol]!=1)
			pacNextDir = GLUT_KEY_UP;
		else if(maze[pacRow-1][pacCol]!=1)
			pacNextDir = GLUT_KEY_DOWN;
	}
	else if(pacNextDir == GLUT_KEY_UP && (hasGhost(pacCol, pacRow + 1)||hasGhost(pacCol-1, pacRow + 1)||hasGhost(pacCol+1, pacRow + 1)) && Ghost::getGhostMode()!=2){
		// printf("Ghost above me.\n");
		if(maze[pacRow - 1][pacCol]!=1)
			pacNextDir = GLUT_KEY_DOWN;
		else if(maze[pacRow][pacCol-1]!=1)
			pacNextDir = GLUT_KEY_LEFT;
		else if(maze[pacRow][pacCol+1]!=1)
			pacNextDir = GLUT_KEY_RIGHT;
	}
	else if(pacNextDir == GLUT_KEY_DOWN && (hasGhost(pacCol, pacRow - 1)||hasGhost(pacCol-1, pacRow - 1)||hasGhost(pacCol+1, pacRow - 1)) && Ghost::getGhostMode()!=2){
		// printf("Ghost below me.\n");
		if(maze[pacRow + 1][pacCol]!=1)
			pacNextDir = GLUT_KEY_UP;
		else if(maze[pacRow][pacCol-1]!=1)
			pacNextDir = GLUT_KEY_LEFT;
		else if(maze[pacRow][pacCol+1]!=1)
			pacNextDir = GLUT_KEY_RIGHT;
	}*/

}


float minimaxAgent(GameState state, int agentNumber, int depth){
	/**
	* Executes the actual minimax using recursion.
	* @param state: Current explored game state
	* @param agentNumber: The current agent to consider
	* param depth:current depth of the explored state tree.
	*/
	if(depth >= maxDepth){
		return evaluationFunction(state);
	}
	else{
		node temp;
		float valueUp, valueDown, valueLeft, valueRight;
		if(agentNumber == 0){
			valueUp = INT_MAX;
			valueDown = INT_MAX;
			valueLeft = INT_MAX;
			valueRight = INT_MAX;
		}
		else{
			valueUp = INT_MIN;
			valueDown = INT_MIN;
			valueLeft = INT_MIN;
			valueRight = INT_MIN;
		}
		//Up
		temp.xpos = state.agentPosition[agentNumber].xpos;
		temp.ypos = state.agentPosition[agentNumber].ypos + 1;
		if(checkNodePosition(state, temp) && (agentNumber == 0 || state.agentDirection[agentNumber] != GLUT_KEY_DOWN)){//Checking validity of the action
			// printf("up called %d\n", agentNumber);
			GameState newState = generateState(state, agentNumber, 'u');
			valueUp = minimaxAgent(newState, (agentNumber+1)%5, depth+1);
			valueUp += evaluationFunction(state);
		}
		//Down
		temp.xpos = state.agentPosition[agentNumber].xpos;
		temp.ypos = state.agentPosition[agentNumber].ypos - 1;
		if(checkNodePosition(state, temp) && (agentNumber == 0 || state.agentDirection[agentNumber] != GLUT_KEY_UP)){	//Checking validity of the action
			// printf("down called %d\n", agentNumber);
			GameState newState = generateState(state, agentNumber, 'd');
			valueDown = minimaxAgent(newState, (agentNumber+1)%5, depth+1);
			valueDown += evaluationFunction(state);
		}
		//Left
		temp.xpos = state.agentPosition[agentNumber].xpos - 1;
		temp.ypos = state.agentPosition[agentNumber].ypos;
		if(checkNodePosition(state, temp) && (agentNumber == 0 || state.agentDirection[agentNumber] != GLUT_KEY_RIGHT)){//Checking validity of the action
			// printf("left called %d\n", agentNumber);
			// printf("pacposition in minimax agent: %d  %d\n", state.agentPosition[agentNumber].xpos, state.agentPosition[agentNumber].ypos);
			GameState newState = generateState(state, agentNumber, 'l');
			valueLeft = minimaxAgent(newState, (agentNumber+1)%5, depth+1);
			valueLeft += evaluationFunction(state);
		}
		//Right
		temp.xpos = state.agentPosition[agentNumber].xpos + 1;
		temp.ypos = state.agentPosition[agentNumber].ypos;
		if(checkNodePosition(state, temp) && (agentNumber == 0 || state.agentDirection[agentNumber] != GLUT_KEY_LEFT)){//Checking validity of the action
			// printf("right called %d\n", agentNumber);
			GameState newState = generateState(state, agentNumber, 'r');
			valueRight = minimaxAgent(newState, (agentNumber+1)%5, depth+1);
			valueRight += evaluationFunction(state);
		}
		
		// printf("values: %f %f %f %f\n", valueUp, valueDown, valueLeft, valueRight);


		if(agentNumber == 0){
			//Updates the next direction if the given agent is PAC
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
				// printf("score: %f\n", minVal);
			}
			return minVal;
		}
		else{
			float maxVal = findMax(valueUp, valueDown, valueLeft, valueRight);
			// printf("minimax returns: %f %d\n",maxVal, agentNumber );
			return maxVal;
		}

	}
}

float findMin(float valueUp, float valueDown, float valueLeft, float valueRight){
	/**
	* Finds the minimum of the given values.
	*/
	float temp = INT_MAX;

	if(valueUp<temp)
		temp = valueUp;
	if(valueDown<temp)
		temp = valueDown;
	if(valueLeft<temp)
		temp = valueLeft;
	if(valueRight<temp)
		temp = valueRight;

	return temp;
}

float findMax(float valueUp, float valueDown, float valueLeft, float valueRight){
	/**
	* Finds the maximum of the given values.
	*/
	float temp = 0;

	if(valueUp>temp)
		temp = valueUp;
	if(valueDown>temp)
		temp = valueDown;
	if(valueLeft>temp)
		temp = valueLeft;
	if(valueRight>temp)
		temp = valueRight;

	return temp;
}

int findNoOfFood(GameState state){
	/**
	* Finds the no. of food particles left in the current explored game state during minimax.
	* @param state: current explored gamestate.
	*/
	int count = 0;
	for(int i=0;i<rowSize;i++){
		for (int j = 0; j < columnSize; j++)
		{
			if(state.maze[i][j] == 2 || state.maze[i][j] == 3){
				count++;
			}
		}
	}
	return count;
}

node nearestFoodNode(GameState state){
	/**
	* Finds the nearest food node using manhattan distance iteratively.
	* @param state: current explored game state.
	*/
	int minFoodDist=INT_MAX;
	node minFood;
	for(int i=0;i<31;i++){
		for (int j = 0; j < 28; ++j)
		{
			if(state.maze[i][j]==2 || state.maze[i][j]==3){
				node tmp;
				tmp.xpos = j, tmp.ypos = i;
				int tmpDist = h(state.agentPosition[0],tmp);
				if(tmpDist<minFoodDist){
					minFoodDist = tmpDist;
					minFood.xpos = j, minFood.ypos = i;
				}
			}
		}
	}
	if (minFoodDist < INT_MAX - 10000)	
	{
		return minFood;
	}
	else{
		minFood.xpos = getPacPosition().xpos+pow(((int)(rand()*10)),-1), minFood.ypos = getPacPosition().ypos+pow(((int)(rand()*10)),-1);
		return minFood;
	}
}