#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <GL/gl.h>
#include "pac_1.h"
#include "reader.h"
#include "settings.h"
#include "ghosts.h"
#include "motion.h"
#include "gameinput.h"
#include "idaStar.h"
#include "ghostAI.h"
#include "pacAI.h"
#include "minimax.h"
#include "minimax2.h"

#define YLIM (float)55.0 				//Y Limit for the board size
#define XLIM (float)(aspectRatio*YLIM)	//X Limit for the board size

int winsizex,winsizey;					//Size of the window (Resolution)
double aspectRatio; //aspect ratio : will be updated by handleResize, whnever window is resized
int gridDivisions=15; //for development purposes; no. of divisions in the reference grid in X direction.
static GLint fogMode;
ObjModel minion(minionObjFile,0.2);

void init(int argc, char** argv)
{
	/**
	* This function initializes the graphics properties of the whole game
	*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);		//Double Buffering - RGBAlpha - 3D - Ani-Aliasing
	glutInitWindowSize(winsizex, winsizey); 											//Window size
	glutCreateWindow("PAC-MINION"); 														//Create a window
	glutFullScreen();																	//Start with full screen mode
	glutIgnoreKeyRepeat(1);																//Ignoring key repaeat, Since we have key states
	glClearDepth(1);			
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);												//Initial color flush
	glEnable(GL_DEPTH_TEST); 															//Make sure 3D drawing works when one object is in front of another
	glEnable(GL_LIGHTING);																//Enable Lighting
	glEnable(GL_LIGHT0);																//Light 0 is enabled
	glShadeModel (GL_SMOOTH);	
	glEnable(GL_COLOR_MATERIAL);														//Enabling coloring in 3d materials
	glEnable(GL_BLEND); 																//Enable the OpenGL Blending functionality  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);									//Function to be used for blending the graphical objects 
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);									//Setting the light position
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);									//Setting the ambient light intensity
	minion.genList();																	//Generating the drawing list of thw minion
	
}

void agentsSet(){
	/**
	* This function sets the initial position of the agents including PAC
	*/
    pacRow=7;
    pacCol=13; 

    inky.setGhostMode(0);
    pinky.setGhostMode(0);
    blinky.setGhostMode(0);
    clyde.setGhostMode(0);

    blinky.row=19;
    blinky.col=14;

    inky.row=16;
    inky.col=9;

    pinky.row=16;
    pinky.col=18;

    clyde.row=13;
    clyde.col=14;
    
}

void agentsInit(){
	/**
	* computes the exact position of the agent using the row and col position in the agent set position
	*/
	agentsSet();
    pacx = gridCellSide * (pacCol);
    pacy = gridCellSide * (pacRow);
    pacCol = divRound(pacx,gridCellSide);
    pacRow = divRound(pacy,gridCellSide);
    blinky.init();
    inky.init();
    pinky.init();
    clyde.init();
}

//Called when the window is resized

void handleResize(int w, int h) 
{
	/**
	* Handles the propertites of the objects when the window is resized
	*/
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	aspectRatio = (double)w / (double)h;
	winsizex = w;
	winsizey = h;
	camera();
	gluPerspective(60.0,  
				   aspectRatio,
				   1.0,				   
				   200.0);										//The visible volume is frustrum of a cone with 60 degrees
	if(aspectRatio > 28.0/31.0)
	    gridCellSide = YLIM*2/31.0;
	else 
        gridCellSide = XLIM*2/28.0;
    pacMoveDist = gridCellSide/8;
    // printf("Grid Cell Side = %f\n", gridCellSide);  
    pacx = gridCellSide * (pacCol);
    pacy = gridCellSide * (pacRow);
    gridHeightby2 = 31.0 * 0.5 * gridCellSide;
    gridWidthby2 = 28.0 * 0.5 * gridCellSide;
    pacCol = divRound(pacx,gridCellSide);
    pacRow = divRound(pacy,gridCellSide);
    blinky.init();
    inky.init();
    pinky.init();
    clyde.init();
    // printf("%d %d\n%f %f\n", pacRow,pacCol,pacx,pacy);
    
}

void showString(char s[128], int size=18){
	/**
	* Prints the given string in the Graphic window using Bit-Map array
	*/
	void * font;
	if(size<=18)
		font = GLUT_BITMAP_9_BY_15; //GLUT_BITMAP_HELVETICA_18;
	else
		font = GLUT_BITMAP_TIMES_ROMAN_24;

    for (int i = 0; s[i]!='\0'; ++i)
    {
        char c = s[i]; 
        glutBitmapCharacter(font, c);
    }
}

void text(){
	/**
	* Draws the text which guides the gameplay using the showString function
	*/
	glMatrixMode( GL_PROJECTION ) ;
	glPushMatrix() ; // save
	glLoadIdentity();// and clear
	glMatrixMode( GL_MODELVIEW ) ;
	glPushMatrix() ;
	glLoadIdentity() ;

	glDisable( GL_DEPTH_TEST ) ;
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f,1.0f);
    glRasterPos2f(-0.8, 0);
    char s[128];
    sprintf(s,"Score : %d",score);
    showString(s);

    glRasterPos2f(-0.8, -0.06);
    sprintf(s,"Deaths : %d",deathCount);
    showString(s);

    if(nfood<=0){
    	glRasterPos2f(-0.9, -0.12);
    	sprintf(s,"Game over. g to restart");
    	showString(s);
	}
	else if(paused){
		glRasterPos2f(-0.9,-0.18);
		sprintf(s,"Game paused. p to resume");
		showString(s);
	}
	if(pacCurDir==100 || pacCurDir==101 || pacCurDir==102 || pacCurDir==103){
		glEnable(GL_LIGHTING);
	    glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on

		glMatrixMode( GL_PROJECTION ) ;
		glPopMatrix() ; // revert back to the matrix I had before.
		glMatrixMode( GL_MODELVIEW ) ;
		glPopMatrix() ;
		return;
	}
	float top = 0.7;
	glRasterPos2f(0.5,top);
	sprintf(s,"PAC MAN");
	showString(s,24);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"Objective: Finish the food");
	showString(s);
	top-=0.05;
	glRasterPos2f(0.5,top);
	sprintf(s,"           (White dots)");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"Stay away from the ghosts.");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"Eat the power food");
	showString(s);
	top-=0.05;

	glRasterPos2f(0.5,top);
	sprintf(s,"   (green) to eat ghosts.");
	showString(s);
	top-=0.05;

	glRasterPos2f(0.5,top);
	sprintf(s,"     (lasts for a short time)");
	showString(s);
	top-=0.2;

	glRasterPos2f(0.5,top);
	sprintf(s,"Arrow keys: Move");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"W S A D: Move Camera");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"Shift+(W/S): Zoom in/out");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"Mouse click and drag: Look around");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"r: Reset camera position");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"c: Change camera mode");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"p: Pause");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"g: Restart game.");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"1,2,3: PAC controlled by Computer");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"0: Switch back to manual PAC");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"t: Toggle anti-aliasing");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"m: Toggle minion");
	showString(s);
	top-=0.05;

	glRasterPos2f(0.5,top);
	sprintf(s,"(May improve performance)");
	showString(s);
	top-=0.06;

	glRasterPos2f(0.5,top);
	sprintf(s,"Esc: Exit");
	showString(s);
	top-=0.06;

    glEnable(GL_LIGHTING);
    glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on

	glMatrixMode( GL_PROJECTION ) ;
	glPopMatrix() ; // revert back to the matrix I had before.
	glMatrixMode( GL_MODELVIEW ) ;
	glPopMatrix() ;
}


void draw()
{
	/**
	* Draws the 3D scene within the view volume
	*/
	int i,j;
	nfood=0;
	
	prevFrameTime = curFrameTime;
	curFrameTime = glutGet(GLUT_ELAPSED_TIME);								//Determines the time gap between two continuous frame to enable frame independent animation
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); 											//Switch to the drawing perspective
	
	glLoadIdentity(); 														//Reset the drawing perspective
	glPushMatrix();
	camera();
	//glTranslatef(0.0f,0.0f,-100.0f);
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	//Set material for pac and draw the pac
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = {50.0};
	
	glTranslatef(-gridWidthby2, -gridHeightby2,-100.0f);
	
	glPushMatrix();
	glTranslatef(-gridCellSide/2,-gridCellSide/2,-gridCellSide/2);
	glColor4f(0.3,0.34,0.32,1.0);
	glBegin(GL_QUADS);
		glVertex3f(-gridCellSide,-gridCellSide,0);
		glVertex3f(-gridCellSide,gridHeightby2*2+gridCellSide,0);
		glVertex3f(gridWidthby2*2+gridCellSide,gridHeightby2*2+gridCellSide,0);
		glVertex3f(gridWidthby2*2+gridCellSide,0-gridCellSide,0);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	
	/*
	In the following lines of code, we translate the PAC, and rotate it according to the direction of the PAC
	*/
	glTranslatef(pacx,pacy,0);

	if(!minionEnabled){
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glColor4f(0.75f,0.75f,0.0f,1.0f);
		glutSolidSphere(PACSIZE,50,50);
	}
	else{
		glDisable(GL_COLOR_MATERIAL);
		glRotatef(90.0f,1.0,0.0,0.0);
		if(pacCurDir == GLUT_KEY_LEFT) {
			if(pacAngle <= 270 - angleChange || pacAngle >= 270 + angleChange){
				if(pacAngle<=90 || pacAngle>270){
					pacAngle-=angleChange;
					if(pacAngle<0)
						pacAngle+=360;
				}
				else
					pacAngle = (pacAngle+angleChange)%360;
			}
			glRotatef((float)pacAngle,0.0,1.0,0.0);
		}
		else if(pacCurDir == GLUT_KEY_RIGHT){
			if(pacAngle <= 90 - angleChange || pacAngle >= 90 + angleChange){
				if(pacAngle<90 || pacAngle>=270){
					pacAngle = (pacAngle+angleChange)%360;
				}
				else
					pacAngle = (pacAngle-angleChange);
					if(pacAngle<0)
							pacAngle+=360;
			}
			glRotatef((float)pacAngle,0.0,1.0,0.0);
		}
		else if(pacCurDir == GLUT_KEY_UP){
			if(pacAngle <= 180 - angleChange || pacAngle >= 180 + angleChange){
				if(pacAngle<=359 && pacAngle>180){
					pacAngle = (pacAngle-angleChange);
					if(pacAngle<0)
							pacAngle+=360;
				}
				else
					pacAngle = (pacAngle+angleChange)%360;
			}
			glRotatef((float)pacAngle,0.0,1.0,0.0);
		}
		else if(pacCurDir == GLUT_KEY_DOWN){
			if(pacAngle <= 0 - angleChange || pacAngle >= 0 + angleChange){
				if(pacAngle<=180 && pacAngle>0){
					pacAngle = (pacAngle-angleChange);
					if(pacAngle<0)
							pacAngle+=360;
				}
				else
					pacAngle = (pacAngle+angleChange)%360;
			}
			glRotatef((float)pacAngle,0.0,1.0,0.0);
		}
		minion.draw();	//Draws the minion
		glEnable(GL_COLOR_MATERIAL);
	}								
	glPopMatrix();
	
	//Drawing ghosts
	if(!paused)
		blinky.move();
	if(Ghost::getGhostMode() == 2)
		glColor4f(0.0,0.0,1.0,0.8);
	else
		glColor4fv(blinky.color);
	glTranslatef(blinky.getX(),blinky.getY(),0.0f);
	glutSolidSphere(PACSIZE,5,5);
	glTranslatef(-blinky.getX(),-blinky.getY(),0.0f);
	
	if(!paused)
		inky.move();
	if(Ghost::getGhostMode() == 2)
		glColor4f(0.0,0.0,1.0,0.8);
	else
		glColor4fv(inky.color);
	glTranslatef(inky.getX(),inky.getY(),0.0f);
	glutSolidSphere(PACSIZE,5,5);
	glTranslatef(-inky.getX(),-inky.getY(),0.0f);

	if(!paused)
		pinky.move();
	if(Ghost::getGhostMode() == 2)
		glColor4f(0.0,0.0,1.0,0.8);
	else
		glColor4fv(pinky.color);
	glTranslatef(pinky.getX(),pinky.getY(),0.0f);
	glutSolidSphere(PACSIZE,5,5);
	glTranslatef(-pinky.getX(),-pinky.getY(),0.0f);

	if(!paused)
		clyde.move();
	if(Ghost::getGhostMode() == 2)
		glColor4f(0.0,0.0,1.0,0.8);
	else
		glColor4fv(clyde.color);
	glTranslatef(clyde.getX(),clyde.getY(),0.0f);
	glutSolidSphere(PACSIZE,5,5);
	glTranslatef(-clyde.getX(),-clyde.getY(),0.0f);

	GLfloat mat_specular2[] = {0.2,0.2,0.2,0.7};
	mat_shininess[0] = 100.0;
	GLfloat mat_diffuse[] = {0.2,0.17,0.73,1.0};
	GLfloat mat_ambient[] = {0.1,0.07,0.63,1.0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	//Drawing the maze
	GLfloat mazeColor[] = {0.05f,0.07f,0.53f,1.0f};
	for(i=0;i<31;i++){
        
        for(j=0;j<28;j++){
            if(maze[i][j]==1){
                glColor4fv(mazeColor);
                glutSolidCube(gridCellSide);
            }
            else if(maze[i][j]==2){
                glColor4f(1.0f,0.8f,0.85f,0.9f);
                glutSolidSphere(0.5,10,10);
                nfood++;
            }
            else if(maze[i][j]==3){
                glColor4f(0.6f,1.0f,0.7f,0.75f);
                glutSolidSphere(1.2,20,20);
                nfood++;
            }
            glTranslatef(gridCellSide,0.0f,0.0f);
        }
        glTranslatef(-28*gridCellSide,gridCellSide,0.0f);
    }
    glPopMatrix();
    //staticElementsDraw();
    text();
	glutSwapBuffers(); //Send scene to the screen to be shown
}

int main(int argc, char** argv) {

	/**
	* Main function for the whole PAC Game
	*/

	curLevel[0] = '1';
	curLevel[1] = '\0';
	loadMaze();
	loadConfig();
	srand(time(NULL));
	winsizex=config->resx,winsizey=config->resy;

	//Pac initialization
	agentsSet();

	cameraSetting.posx = 0.0;
	cameraSetting.posy = 0.0;
	cameraSetting.posz = 0.0;
	cameraSetting.dirx = 0.0;
	cameraSetting.diry = 0.0;
	cameraSetting.dirz = -1.0;
	cameraSetting.upx = 0.0;
	cameraSetting.upy = 1.0;
	cameraSetting.upz = 0.0;
	cameraSetting.cameraMode = 0;
	prevMouseX = INT_MAX;
	prevMouseY = INT_MAX;
	unbuildPacAIMaze();
	pacControlAgent = 0;

	
    init(argc, argv); //Initialize rendering
	
	//Set functions for glutMainLoop to call
	glutDisplayFunc(draw);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(keyListen);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouse);
	//glutIdleFunc(motion);
	//glutIdleFunc(draw);
	startTime = prevFrameTime = curFrameTime = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(ANIM, motion, 0);
	glutMainLoop(); //Start the main loop. glutMainLoop doesn't return.
	return 0; //This line is never reached
}

