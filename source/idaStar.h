#include <queue>
#define FOUND -1 		// Value returned by the IDA* search algorithm to specify the result (Found/Not Found)
#define rowSize 31
#define columnSize 28
#define LOOPLIM 10000

node nextNode;
bool visited[31][28], reachable[31][28];
queue<node> q;

int h(node end, node n) { //Todo: modify the heuristic to include the tunnel in the middle row of the maze
	/**
	* This funtion returns the estimated cost to reach from the present state to the goal state.
	* The hueristics is based on the manhattan distance.
	* @param end: The goal state of the ghost.
	* @param n: The present state.
	*/
	int xd,yd;
	xd = end.xpos - n.xpos;
	yd = end.ypos - n.ypos;
	if(xd<0)
		xd*=-1;
	if(yd<0)
		yd*=-1;
	return xd+yd;
}

int ida_star(node start, node end) {
	/**
	* This function initiates the IDA* algorithm to find the next best move for the ghost to catch the minion.
	* @param start: The start state of the ghost.
	* @param end: The goal state of the ghost.
	*/

	if(boolmaze[start.ypos][start.xpos]){
		boolmaze[start.ypos][start.xpos] = false;
	}
	for(int i=0;i<31;i++){
        for(int j=0;j<28;j++){
        	visited[i][j] = false;
        	reachable[i][j] = false;
        	//printf("%d ",boolmaze[i][j]);
        }
       	//printf("\n");
    }
	int search(node start, node end, node curnode, node prevnode, int g, int bound);		//Function header for search which is defined later
	node makeEndReachable(node , node);		
	void buildReachable(node);
	buildReachable(start);
	for(int i=0;i<31;i++){
        for(int j=0;j<28;j++){
        	visited[i][j] = false;
        	//printf("%d ", reachable[i][j]);
        }
        //printf("\n");
	}
	while(end.ypos<0){
		end.ypos++;
	}
	while(end.ypos>30)
		end.ypos--;
	while(end.xpos<0)
		end.xpos++;
	while(end.xpos>27)
		end.xpos--;
	end = makeEndReachable(start, end);		//If end node is unreachable, make it the nearest node to the end node which is reachable
	if(end.xpos < 0){
		//printf("So true\n");
		return INT_MAX;
	}
	// printf("End is now %d %d with reachability %d\n", end.xpos, end.ypos, reachable[end.ypos][end.xpos]);
	int bound = h(end, start);							//Initial bound for the IDA* is the heuristic for the current node
    int i=0;
	while(true){//(i++<LOOPLIM){
		//Keep increasing the bound until the goal state is reached
		//printf("ida_star while loop.\n");
		for(int i=0;i<31;i++){
	        for(int j=0;j<28;j++){
	        	visited[i][j] = false;
	        }
    	}
		int t = search(start,end, start,start, 0, bound);
		if (t == FOUND || t == INT_MAX)
			return t;
		bound = t;
	}
    return INT_MAX;
}

bool is_goal(node end, node n) {
	/**
	* This function checks whether the given node is goal node or not.
	* @param end: The goal state of the ghost.
	* @param n: the node to test.
	*/
	return (n.xpos==end.xpos && n.ypos==end.ypos);
}

void swap(int &a, int &b) {
	/**
	* This function swaps the two variables provided.
	*/
	int temp;
	temp=a;
	a=b;
	b=temp;
}

int find_successors(node end, node n, node prev,node*succ, int g) {
	/**
	* This function finds the valid successors for the given node.
	* @param end: The goal state of the ghost.
	* @param n: The node for which the successors are needed
	* generates the successor array for the given node and sorts the successors in the increasing order of their heuristic 
	* value, so the first element of the array represents the better node for subsequent traversal.
	*/
	int f[4];
	succ[0].xpos = n.xpos;
	succ[0].ypos = n.ypos - 1;
	if(succ[0].ypos < 0 || boolmaze[succ[0].ypos][succ[0].xpos] || (succ[0].xpos==prev.xpos && succ[0].ypos==prev.ypos) || visited[succ[0].ypos][succ[0].xpos])
		f[0]=INT_MAX;
	else 
		f[0] = g+pacAIMaze[succ[0].ypos][succ[0].xpos]+h(end, succ[0]);
	
	succ[1].xpos = n.xpos + 1;
	succ[1].ypos = n.ypos;
	if(succ[1].xpos >= columnSize || boolmaze[succ[1].ypos][succ[1].xpos] || (succ[1].xpos==prev.xpos && succ[1].ypos==prev.ypos) || visited[succ[1].ypos][succ[1].xpos])
		f[1]=INT_MAX;
	else 
		f[1] = g+pacAIMaze[succ[1].ypos][succ[1].xpos]+h(end, succ[1]);
	if((n.xpos==27 && n.ypos==16) && !visited[16][0]){
		succ[1].xpos = 0;
		f[1] = g+pacAIMaze[succ[1].ypos][succ[1].xpos]+h(end, succ[1]);
	}
		
	succ[2].xpos = n.xpos;
	succ[2].ypos = n.ypos + 1;
	if(succ[2].ypos >= rowSize || boolmaze[succ[2].ypos][succ[2].xpos] || (succ[2].xpos==prev.xpos && succ[2].ypos==prev.ypos) || visited[succ[2].ypos][succ[2].xpos])
		f[2]=INT_MAX;
	else 
		f[2] = g+pacAIMaze[succ[2].ypos][succ[2].xpos]+h(end, succ[2]);
		
	succ[3].xpos = n.xpos - 1;
	succ[3].ypos = n.ypos;
	if(succ[3].xpos < 0 || boolmaze[succ[3].ypos][succ[3].xpos] || (succ[3].xpos==prev.xpos && succ[3].ypos==prev.ypos) || visited[succ[3].ypos][succ[3].xpos])
		f[3]=INT_MAX;
	else 
		f[3] = g+pacAIMaze[succ[3].ypos][succ[3].xpos]+h(end, succ[3]);
	if((n.xpos==0 && n.ypos==16) && !visited[16][27]){
		succ[3].xpos = 27;
		f[3] = g+pacAIMaze[succ[3].ypos][succ[3].xpos]+h(end, succ[3]);
	}
	for(int i=0;i<3;i++){
		if(f[1]<f[0])
		{
			swap(f[0],f[1]);
			swap(succ[0].xpos,succ[1].xpos);
			swap(succ[0].ypos,succ[1].ypos);
		}
		if(f[2]<f[1])
		{
			swap(f[2],f[1]);
			swap(succ[2].xpos,succ[1].xpos);
			swap(succ[2].ypos,succ[1].ypos);
		}
		if(f[3]<f[2])
		{
			swap(f[2],f[3]);
			swap(succ[2].xpos,succ[3].xpos);
			swap(succ[2].ypos,succ[3].ypos);
		}
	}
	if(f[0] == INT_MAX)
		return 0;
	else if(f[1] == INT_MAX)
		return 1;
	else if(f[2] == INT_MAX)
		return 2;
	else if(f[3] == INT_MAX)
		return 3;
	else
	    return 4;
}

int search(node start, node end, node curnode, node prevnode, int g, int bound) {
	/**
	* This function finds the path from the current node to the goal node using recursion
	* @param start: The start state of the ghost.
	* @param end: The goal state of the ghost.
	* @param curnode: Represents the current node.
	* @param g: The actual cost incurred for the travel from initial node to the current node.
	* @param bound: the bound of the IDA* algorithm
	*/
	//printf("search fn started.. curnode: %d %d\n", curnode.xpos, curnode.ypos);
	visited[curnode.ypos][curnode.xpos] = true;
	//printf("Visited value is : %d\n", visited[curnode.ypos][curnode.xpos]);
	// std::cout<<"->"<<curnode.xpos<<","<<curnode.ypos;
	int f = g + h(end, curnode);
	if (f > bound)
	{
		//printf("\n");
		return f;
	}
	if (is_goal(end, curnode))
	{
		//printf("\nFound the node. The path is described in reverse:\n->(%d,%d)",curnode.xpos,curnode.ypos);
		nextNode.xpos = curnode.xpos;
		nextNode.ypos = curnode.ypos;
		return FOUND;
	}
	int min = INT_MAX;
	int nsucc;
	node succ[4];
	nsucc = find_successors(end, curnode,prevnode,succ, g);
	for (int i=0;i<nsucc;i++)
	{
		int t = search(start,end, succ[i],curnode, g + 1, bound);
		if (t == FOUND)
		{
			// std::cout<<"->("<<curnode.xpos<<","<<curnode.ypos<<")";
			if(!(curnode.xpos == start.xpos && curnode.ypos == start.ypos)){
				nextNode.xpos = curnode.xpos;
				nextNode.ypos = curnode.ypos;
			}
			return FOUND;
		}
		if (t < min) 
			min = t;
	}
	return min;
}

bool checkPosition(node start, node temp){
	/**
	* This function checks if the given position is valid according to the game maze
	*/
 	if(temp.xpos >= 0 && temp.xpos < columnSize && temp.ypos >= 0 && temp.ypos < rowSize){
 		if(!visited[temp.ypos][temp.xpos])
    		return true;
	}
	return false;
}

void buildReachable(node start){
	/**
	* Builds a reachabilty matrix for the current state to determine the success of IDA*
	*/
	//printf("making reachable %d %d\n", start.xpos, start.ypos);
	reachable[start.ypos][start.xpos] = true;
	visited[start.ypos][start.xpos] = true;
	node succ[4];
	int n = find_successors(start,start,start,succ,0);
	for(int i=0;i<n;i++)
		buildReachable(succ[i]);
}

node makeEndReachable(node start, node end){
	/**
	* Changes the target to the nearest node to the previously determined target to ensure that goal node passed to IDA* is reachable.
	* @param start: The start state of the ghost.
	* @param end: The goal state of the ghost.
	*/
	while(!q.empty()){
		q.pop();
		//printf("makeEndReachable while loop 2\n");
	}
	int nreach = 0;
	for(int i=0;i<31;i++){
        for(int j=0;j<28;j++){
        	if(reachable[i][j])
        		nreach++;
        }
	}
	if(nreach < 2){
		node tmp;
		tmp.xpos = tmp.ypos = -1;
		return tmp;
	}
	q.push(end);
	visited[end.ypos][end.xpos] = true;
	while(!q.empty()){
		node curnode = q.front();
		if(reachable[curnode.ypos][curnode.xpos] && !(curnode.xpos==start.xpos && curnode.ypos == start.ypos)){
			return curnode;
		}
		//printf("makeEndReachable while loop. Curnode is %d %d\n", curnode.xpos, curnode.ypos);
		
		node temp;
		temp.xpos = curnode.xpos-1;
		temp.ypos = curnode.ypos;
		if (checkPosition(start, temp))
		{
			//printf("%d %d is pushing %d %d\n", curnode.xpos, curnode.ypos, temp.xpos, temp.ypos);
			visited[temp.ypos][temp.xpos] = true;
			q.push(temp);
		}
		temp.xpos = curnode.xpos+1;
		temp.ypos = curnode.ypos;
		if (checkPosition(start, temp))
		{
			//printf("%d %d is pushing %d %d\n", curnode.xpos, curnode.ypos, temp.xpos, temp.ypos);
			visited[temp.ypos][temp.xpos] = true;
			q.push(temp);
		}
		temp.xpos = curnode.xpos;
		temp.ypos = curnode.ypos-1;
		if (checkPosition(start, temp))
		{
			//printf("%d %d is pushing %d %d\n", curnode.xpos, curnode.ypos, temp.xpos, temp.ypos);
			visited[temp.ypos][temp.xpos] = true;
			q.push(temp);
		}
		temp.xpos = curnode.xpos;
		temp.ypos = curnode.ypos+1;
		if (checkPosition(start, temp))
		{
			//printf("%d %d is pushing %d %d\n", curnode.xpos, curnode.ypos, temp.xpos, temp.ypos);
			visited[temp.ypos][temp.xpos] = true;
			q.push(temp);
		}
		q.pop();
	}
}