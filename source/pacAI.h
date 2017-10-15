/*
 * For use when pac is controlled by the AI.
 */
 #define GHOST_NEAR_DIST 7

int inkyDist, pinkyDist, blinkyDist, clydeDist;
int pacAIState; //0 - Nearest ghost state 	1 - Nearest Food state	2 - Random food state
node pacTarget;

void buildPacAIMaze(){
	// printf("buildPacAIMaze\n");
	for (int i = 0; i < 31; ++i)
	{
		for (int j = 0; j < 28; ++j)
		{
			if (maze[i][j]==0)
			{
				pacAIMaze[i][j] = 14;
			}
			else if (maze[i][j]==1)
			{
				//pacAIMaze[i][j]==INT_MAX - 10000;
			}
			else if (maze[i][j]==2)
			{
				pacAIMaze[i][j] = 10;
			}
			else if (maze[i][j]==3)
			{
				pacAIMaze[i][j] = (inkyDist + pinkyDist + blinkyDist + clydeDist)/10;
			}
		}
	}
	// printf("buildPacAIMaze basic step done.\n");
	if(Ghost::getGhostMode()==2){
		pacAIMaze[inky.row][inky.col] = 0;
		pacAIMaze[pinky.row][pinky.col] = 0;
		pacAIMaze[blinky.row][blinky.col] = 0;
		pacAIMaze[clyde.row][clyde.col] = 0;
	}
	else{
		if(inkyDist < 20)
			pacAIMaze[inky.row][inky.col] = 500000.0/inkyDist;
		if(pinkyDist < 20)
			pacAIMaze[pinky.row][pinky.col] = 1000000.0/pinkyDist;
		if(blinkyDist < 20)
			pacAIMaze[blinky.row][blinky.col] = 1000000.0/blinkyDist;
		if(clydeDist < 20)
			pacAIMaze[clyde.row][clyde.col] = 500000.0/clydeDist;
	}
	// printf("buildPacAIMaze over.\n");
}

void unbuildPacAIMaze(){
	// printf("unbuildPacAIMaze\n");
	for (int i = 0; i < 31; ++i)
	{
		for (int j = 0; j < 28; ++j)
		{
			pacAIMaze[i][j] = 1;
		}
	}
}

bool hasGhost(int c, int r){
	// printf("hasGhost\n");
	if((inky.row == r && inky.col ==c)||
		(pinky.row == r && pinky.col ==c)||
		(blinky.row == r && blinky.col ==c)||
		(clyde.row == r && clyde.col ==c))
		return true;
	return false;
}

node nearestGhostNode();
node randomFoodNode();
node nearestFoodNode();

void computePacDirection(){
	// printf("computePacDirection\n");
	inkyDist = inky.getDistFrom(pacRow, pacCol);
	pinkyDist = pinky.getDistFrom(pacRow, pacCol);
	blinkyDist = blinky.getDistFrom(pacRow, pacCol);
	clydeDist = clyde.getDistFrom(pacRow, pacCol);
	buildPacAIMaze();
	// printf("pacAIState: %d\n", pacAIState);
	// printf("pacTarget: %d %d\n", pacTarget.xpos, pacTarget.ypos);
	// printf("maze value at that point: %d\n", maze[pacTarget.ypos][pacTarget.xpos]);
	if(inkyDist < GHOST_NEAR_DIST || pinkyDist<GHOST_NEAR_DIST || blinkyDist <GHOST_NEAR_DIST || clydeDist<GHOST_NEAR_DIST){
		if(Ghost::getGhostMode() == 2){	//todo: also check the time remaining in panic mode
			pacTarget = nearestGhostNode();
			if(pacAIState != 0){
				pacAIState = 0;
			}
		}
		else if(pacAIState != 2){
			pacTarget = randomFoodNode();
			pacAIState = 2;
		}
		else if(maze[pacTarget.ypos][pacTarget.xpos] != 2 || inkyDist < GHOST_NEAR_DIST/2 || pinkyDist<GHOST_NEAR_DIST/2 || blinkyDist <GHOST_NEAR_DIST/2 || clydeDist<GHOST_NEAR_DIST/2){
			pacTarget = randomFoodNode();
		}
	}
	else {
		if(pacAIState != 1){
			pacTarget = nearestFoodNode(); 
			pacAIState = 1;
		}
		else if(maze[pacTarget.ypos][pacTarget.xpos] != 2)
			pacTarget = nearestFoodNode();
	}

	// printf("Target change done.\n");
	node start = getPacPosition();
	// printf("PAC idaStar from %d %d to %d %d\n", start.xpos, start.ypos, pacTarget.xpos, pacTarget.ypos);
	pacNextDir = idaStar(start,pacTarget);
	// printf("pacNextDir is %d\n",pacNextDir);
	if(pacNextDir == -1){
		if(maze[pacRow][pacCol-1]!=1 && !hasGhost(pacRow, pacCol-1))
			pacNextDir = GLUT_KEY_LEFT;
		else if(maze[pacRow][pacCol+1]!=1 && !hasGhost(pacRow, pacCol+1))
			pacNextDir = GLUT_KEY_RIGHT;
		else if(maze[pacRow+1][pacCol]!=1 && !hasGhost(pacRow+1, pacCol))
			pacNextDir = GLUT_KEY_UP;
		else if(maze[pacRow-1][pacCol]!=1 && !hasGhost(pacRow-1, pacCol))
			pacNextDir = GLUT_KEY_DOWN;
		else{
			while(pacNextDir==-1){
				pacTarget = randomFoodNode();
				idaStar(start,pacTarget);
			}
		}
	}
	if(pacNextDir == GLUT_KEY_LEFT && Ghost::getGhostMode()!=2 && (hasGhost(pacCol - 1, pacRow) || hasGhost(pacCol - 1, pacRow-1) || hasGhost(pacCol - 1, pacRow+1) )) {
		// printf("Ghost left of me.\n");
		pacTarget = randomFoodNode();
		if(maze[pacRow][pacCol+1]!=1)
			pacNextDir = GLUT_KEY_RIGHT;
		else if(maze[pacRow+1][pacCol]!=1)
			pacNextDir = GLUT_KEY_UP;
		else if(maze[pacRow-1][pacCol]!=1)
			pacNextDir = GLUT_KEY_DOWN;
	}
	else if(pacNextDir == GLUT_KEY_RIGHT && (hasGhost(pacCol + 1, pacRow)|| hasGhost(pacCol + 1, pacRow-1) || hasGhost(pacCol + 1, pacRow+1)) && Ghost::getGhostMode()!=2){
		// printf("Ghost right of me.\n");
		pacTarget = randomFoodNode();
		if(maze[pacRow][pacCol-1]!=1)
			pacNextDir = GLUT_KEY_LEFT;
		else if(maze[pacRow+1][pacCol]!=1)
			pacNextDir = GLUT_KEY_UP;
		else if(maze[pacRow-1][pacCol]!=1)
			pacNextDir = GLUT_KEY_DOWN;
	}
	else if(pacNextDir == GLUT_KEY_UP && (hasGhost(pacCol, pacRow + 1)||hasGhost(pacCol-1, pacRow + 1)||hasGhost(pacCol+1, pacRow + 1)) && Ghost::getGhostMode()!=2){
		// printf("Ghost above me.\n");
		pacTarget = randomFoodNode();
		if(maze[pacRow - 1][pacCol]!=1)
			pacNextDir = GLUT_KEY_DOWN;
		else if(maze[pacRow][pacCol-1]!=1)
			pacNextDir = GLUT_KEY_LEFT;
		else if(maze[pacRow][pacCol+1]!=1)
			pacNextDir = GLUT_KEY_RIGHT;
	}
	else if(pacNextDir == GLUT_KEY_DOWN && (hasGhost(pacCol, pacRow - 1)||hasGhost(pacCol-1, pacRow - 1)||hasGhost(pacCol+1, pacRow - 1)) && Ghost::getGhostMode()!=2){
		// printf("Ghost below me.\n");
		pacTarget = randomFoodNode();
		if(maze[pacRow + 1][pacCol]!=1)
			pacNextDir = GLUT_KEY_UP;
		else if(maze[pacRow][pacCol-1]!=1)
			pacNextDir = GLUT_KEY_LEFT;
		else if(maze[pacRow][pacCol+1]!=1)
			pacNextDir = GLUT_KEY_RIGHT;
	}
	unbuildPacAIMaze();
}

node nearestGhostNode(){
	// printf("nearestGhostNode!\n");
	node result;
	result.xpos = pinky.col;
	result.ypos = pinky.row;
	int minDist = pinkyDist;
	if(inkyDist < minDist){
		minDist = inkyDist;
		result.xpos = inky.col;
		result.ypos = inky.row;
	}
	if(blinkyDist < minDist){
		minDist = blinkyDist;
		result.xpos = blinky.col;
		result.ypos = blinky.row;
	}
	if(clydeDist < minDist){
		minDist = clydeDist;
		result.xpos = clyde.col;
		result.ypos = clyde.row;
	}
	return result;
}

node randomFoodNode(){
	// printf("RandomFood!\n");
	node res;
	int i=0;
	do{
		//printf("randomFoodNode do while.\n");
		res.xpos = (int)(abs(rand()*100)%columnSize);
		res.ypos = (int)(abs(rand()*100)%rowSize);
		if(res.xpos<0)
			res.xpos*=-1;
		if(res.ypos<0)
			res.ypos*=-1;
	}while(maze[res.ypos][res.xpos] != 2 && i++<100);
	int npower = 0;
	int closestPowerDist = INT_MAX;
	node closestPower;
	for (i = 0; i < 31; ++i)
	{
		for (int j = 0; j < 28; ++j)
		{
			if (maze[i][j]==2 && maze[res.ypos][res.xpos]!=2)
			{
				res.ypos = i;
				res.xpos = j;
				return res;
			}
			if(maze[i][j]==3){
				npower++;
				node tmp;
				tmp.xpos = j, tmp.ypos = i;
				int dis = h(getPacPosition(),tmp);
				if(dis<INT_MAX){
					closestPower.xpos = j;
					closestPower.ypos = i;
				}
			}
		}
	}
	if(npower>0 && h(getPacPosition(),closestPower) <= GHOST_NEAR_DIST){
		return closestPower;
	}
	return res;
}

node nearestFoodNode(){
	int minFoodDist=INT_MAX;
	node minFood;
	for(int i=0;i<31;i++){
		for (int j = 0; j < 28; ++j)
		{
			if(maze[i][j]==2 || maze[i][j]==3){
				node tmp;
				tmp.xpos = j, tmp.ypos = i;
				int tmpDist = h(getPacPosition(),tmp);
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