


struct Material{
    char name[128];
    GLfloat matAmbient[4],matDiffuse[4],matSpecular[4];
    GLfloat specularCoefficient[1], transparency;
};

class ObjModel{
    char fileName[50];
    Material materials[50];
    int matCount;
    GLuint drawList;
    float scaleFactor;
    GLfloat vertices[25000][3];
    GLfloat uvs[25000][2];
    GLfloat normals[25000][3];
public:
    void draw(){
        glCallList(drawList);
    }

    ObjModel(char fn[], float scaleFactor){
        matCount = -1;
        strcpy(fileName,fn);
        FILE * file = fopen(fileName, "r");
        if( file == NULL ){
            printf("Impossible to open the file - %s\n", fileName);
            return;
        }
        int vcount = 0, ucount = 0, ncount = 0;
        while( 1 ){
            
            char lineHeader[128];
            
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            if ( strcmp( lineHeader, "v" ) == 0 ){
                GLfloat vertex[3];
                fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2] );
                vertices[vcount][0] = vertex[0]*scaleFactor;
                vertices[vcount][1] = vertex[1]*scaleFactor;
                vertices[vcount][2] = vertex[2]*scaleFactor;
                vcount++;
            }
            else if ( strcmp( lineHeader, "vt" ) == 0 ){
                GLfloat uv[2];
                fscanf(file, "%f %f\n", &uv[0], &uv[1] );
                uvs[ucount][0] = uv[0];
                uvs[ucount][1] = uv[1];
                ucount++;
            }
            else if ( strcmp( lineHeader, "vn" ) == 0 ){
                GLfloat normal[3];
                fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
                normals[ncount][0] = normal[0];
                normals[ncount][1] = normal[1];
                normals[ncount][2] = normal[2];
                ncount++;
            }
            else if ( strcmp( lineHeader, "mtllib" ) == 0 ){
                char mtlFile[128];
                fscanf(file, "%s\n", mtlFile);
                getMaterials(mtlFile);
            }
        }
        printf("Initialization of %s successful\n", fileName);
    }

    void getMaterials(char mtlFile[128]){
        FILE * file = fopen(mtlFile, "r");
        if( file == NULL ){
            printf("Impossible to open the file - %s\n", fileName);
            return;
        }

        while( 1 ){
            
            char lineHeader[128];
            
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            if(strcmp(lineHeader,"newmtl") == 0){
                matCount++;
                fscanf(file,"%s\n", materials[matCount].name);
            }
            else if(strcmp(lineHeader,"Ns") == 0){
                fscanf(file,"%f\n", &(materials[matCount].specularCoefficient[0]));
            }
            else if(strcmp(lineHeader,"Ka") == 0){
                fscanf(file,"%f %f %f\n", &(materials[matCount].matAmbient[0]), &(materials[matCount].matAmbient[1]), &(materials[matCount].matAmbient[2]));
                materials[matCount].matAmbient[3] = 1.0;
            }
            else if(strcmp(lineHeader,"Kd") == 0){
                fscanf(file,"%f %f %f\n", &(materials[matCount].matDiffuse[0]), &(materials[matCount].matDiffuse[1]), &(materials[matCount].matDiffuse[2]));
                materials[matCount].matDiffuse[3] = 1.0;
            }
            else if(strcmp(lineHeader,"Ks") == 0){
                fscanf(file,"%f %f %f\n", &(materials[matCount].matSpecular[0]), &(materials[matCount].matSpecular[1]), &(materials[matCount].matSpecular[2]));
                materials[matCount].matSpecular[3] = 1.0;
            }
            else if(strcmp(lineHeader,"d") == 0){
                fscanf(file,"%f\n", &(materials[matCount].transparency));
            }
        }
    }

    Material getMaterialbyName(char mname[128]){
        for (int i = 0; i <= matCount; ++i)
        {
            if( strcmp(mname, materials[i].name) == 0 )
                return materials[i];
        }
    }

    void genList(){
        drawList = glGenLists(1);
        glNewList(drawList, GL_COMPILE);
        
        FILE * file = fopen(fileName, "r");
        if( file == NULL ){
            printf("Impossible to open the file - %s\n", fileName);
            return;
        }
        

        while(1)
        {
            char lineHeader[128];
            
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.
            if ( strcmp( lineHeader, "f" ) == 0 ){

                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return;
                }
                
            
                glBegin(GL_TRIANGLES);
                {
                    glNormal3fv(normals[normalIndex[0] - 1]);
                    glVertex3fv(vertices[vertexIndex[0] - 1]);

                    glNormal3fv(normals[normalIndex[1] - 1]);
                    glVertex3fv(vertices[vertexIndex[1] - 1]);

                    glNormal3fv(normals[normalIndex[2] - 1]);
                    glVertex3fv(vertices[vertexIndex[2] - 1]);
                }
                glEnd();
            }
            else if(strcmp(lineHeader, "usemtl")==0){
                char mname[128];
                fscanf(file, "%s\n", mname);
                Material mat = getMaterialbyName(mname);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.matAmbient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.matDiffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.matSpecular);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat.specularCoefficient);
            }
        }
        glEndList();
    }
};
