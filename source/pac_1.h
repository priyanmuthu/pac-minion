
/***
* To be used with pac_1.c
* Miscellaneous user defined functions and global variables are placed in this file
*/

#define PACSIZE 1.395542 //1.395542
#define ANIM 16

float pacx=0, pacy=0;
int pacRow,pacCol;
int pacCurDir; 
float pacMoveDist; //stored because of repeated use
int startTime, prevFrameTime, curFrameTime,pacStartTime = -1; //for framerate independent animation

int maze[31][28]; //Maze value key: 1-Filled 0-Empty 2-Food 3-PowerUp. Positions of ghost box and gate is fixed and will be hardcoded.
bool boolmaze[31][28];
int pacAIMaze[31][28];
char curLevel[3];
float gridCellSide;
double gridHeightby2, gridWidthby2;
int keystates[256];
int prevMouseX, prevMouseY;
int pacNextDir; //specifies next direction change of pac
float ghostSpeedPercent = 0.8;
int pacControlAgent; //0 - user 	 1 - AI(IDA*) 	 2 - AI(Minimax)    3 - AI(Modified Minimax)
int panicStartTime, panicDuration = 6000;
int chaseScatterDuration[4] = {2000,25000,45000,50000};
int npanic = 0;
int score = 0;
bool paused = false;
int nfood=212, deathCount = 0;
char minionObjFile[25] = "minion.obj";
int pacAngle=0, angleChange=10;     //the angle at which pac is facing wrt down direction
bool minionEnabled = true;

using namespace std;

struct node
{
	int xpos,ypos;
};

int divRound(double a, double b){
	double c = a/b;
	if(((int)(c*10))%10 >= 5)
		c+=1;
	return (int)c;
}

double absm(double a){
    /**
    * returns Absoulute Modulus of a
    */
	if(a<0)
		return -1.0 * a;
	else
		return a;
}

void loadMaze() { 
    /**
    * Function to load the maze from a text file maze<curLevel>.txt
    */
    FILE *mz;
    int i,j;
    int err=0;
    char filename[10] = "maze";
    strcat(filename,curLevel);
    strcat(filename,".txt");
    // printf("Opening %s\n",filename);
    mz = fopen(filename,"r");
    if(mz==NULL){
        err = 1;
        printf("File not found.");
    }
    else {
        for(i=0;i<31;i++){
            for(j=0;j<28;j++){
                /*if(eof(mz)){
                    //printf("EOF at i=%d and j=%d\n",i,j);
                    err=1;
                }*/
                fscanf(mz,"%d",&maze[30-i][j]);
                
                if(maze[30-i][j]<0 || maze[30-i][j]>3)
                    err = 1;

                boolmaze[30-i][j] = (maze[30-i][j]==1);
            }
        }
        for(i=12;i<15;i++){
        	for (j = 0; j < 5; ++j)
        	{
        		boolmaze[i][j] = 1;
        		boolmaze[i][27-j] = 1;
        	}
        }
        for(i=18;i<21;i++){
        	for (j = 0; j < 5; ++j)
        	{
        		boolmaze[i][j] = 1;
        		boolmaze[i][27-j] = 1;
        	}
        }
        boolmaze[14][13] = 0;
        boolmaze[14][14] = 0;

        /*for(i=0;i<31;i++){
            for(j=0;j<28;j++){
            	printf("%d ",boolmaze[i][j]);
            }
           	printf("\n");
        }*/
    }
    
    if(err){
        printf("Error reading from file or file contents corrupted. Exiting.");
        system("pause");
        exit(0);
    }
}