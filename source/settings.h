
struct CameraSetting{
	float posx,posy,posz; //camera position
	float dirx,diry,dirz; //direction to which camera is pointing
	float upx,upy,upz; //Up vector of the camera
	int cameraMode; //0 - Default   1 - From below the maze    2 - Third Person(race Like)
};

struct CameraSetting cameraSetting;

struct Config{
	int resx , resy; //window resolution
	char strafeLeft, strafeRight, camFront, camBack, camUp, camDown, rotCC, rotC, camLeft, camRight,
	toggleAntiAlias,resetCamera;
	int antiAlias;
};

struct Config *config;

void loadConfig(){
	FILE *cf = fopen("config.txt","r");
	static struct Config c;
	if(cf==NULL){
		config = &c;
		printf("No config file found. Reverting to defaults. Run config.exe to generate a config file.\n");
		config->resx = 1366;
		config->resy = 768;
		config->strafeLeft = 'a';
		config->strafeRight = 'd';
		config->camFront = 'W';
		config->camBack = 'S';
		config->camUp = 'w';
		config->camDown = 's';
		config->rotCC = 'e';
		config->rotC = 'q';
		config->camLeft = 'A';
		config->camRight = 'D';
		config->antiAlias = 1;
		config->toggleAntiAlias = 't';
		config->resetCamera = 'r';
		return;
	}
	fread((void*)config, sizeof(struct Config),1,cf);
}
