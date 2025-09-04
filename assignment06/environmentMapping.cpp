// Isabella Burgess - u1408202

#define GLUT_DISABLE_ATEXIT_HACK

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "include/cyTriMesh.h"
#include "include/cyMatrix.h"
#include "include/cyGL.h"
#include "include/lodepng.h"

using namespace std;

cy::TriMesh teapot;
string objFile;

cy::TriMesh box;

cy::GLSLProgram prog;
cy::GLSLProgram cubeProg;

cy::Matrix4f mvp;
cy::Matrix4f cubeMv;
cy::Matrix4f cubeMvp;


float rotX = -1.55;
float rotY = 0;
float zVar = 1;

float prevX = 0;
float prevY = 0;

float prevZ = -10;

int buttonPressed;

cy::Vec3f cameraPos = cy::Vec3f(0.0f, 0.0f, 3.0f);
cy::Vec3f cameraTarget = cy::Vec3f(0.0f, 0.0f, 3.0f);


GLuint vao1;
GLuint vao2;



void displayTeapot()
{
    float angle = 40*( M_PI/180.0 );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1);
        //Camera rotationlk,.;
       
        
        
    float directionX = cos((rotX)) * cos((rotY));
    float directionY = sin((rotY));
    float directionZ = sin((rotX)) * cos((rotY));

    cy::Vec3f cameraDirection = cy::Vec3f(directionX, directionY , directionZ );

   

       //Cube render
    glDisable(GL_DEPTH_TEST);

        cubeProg.Bind();
        glUseProgram(cubeProg.GetID());
        glBindVertexArray(vao2);

        //MV calculation
        cy::Matrix3f cubeRotMatrix = cy::Matrix3f::RotationXYZ(0, 0, 0);
        cy::Matrix4f cubeProjMatrix = cy::Matrix4f::Perspective(angle, float(1000)/float(750), 0.1, 1000.0f);

        cy::Vec3f cubeTransVector = cy::Vec3f(0, 0, 0);
        cy::Matrix4f cubeTransMatrix = cy::Matrix4f::Translation(cubeTransVector);

        cubeTransMatrix.SetIdentity();

        cy::Matrix4f cubeModel = (cubeTransMatrix * cubeRotMatrix);

        cy::Matrix4f cubeView;
        cubeView.SetIdentity();
    
            
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
    
        float currentFrame = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float cameraSpeed = 0.0005 * deltaTime;
    
        const float cubeRadius = 10.0f / zVar;


        cubeView = cy::Matrix4f::View(cameraDirection * cubeRadius, cy::Vec3f(0.0, 0, 0),  cy::Vec3f(0.0, 1.0, 0.0));  

        cubeProg["projection"] = cubeProjMatrix;
        cubeProg["view"] = cubeView;
        cubeProg["model"] = cubeModel;
        



        glDrawArrays(GL_TRIANGLES, 0, box.NF()*3);  

    //Render teapot
    prog.Bind();
    glUseProgram(prog.GetID());
    glBindVertexArray(vao1);

    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //MVP calculations

    cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(angle, float(1000)/float(750), 0.1, 1000.0f);
 
    cy::Vec3f transVector = cy::Vec3f(0, -5, 0);
    cy::Matrix4f model = cy::Matrix4f::Translation(transVector);

    
    cy::Matrix3f rotMatrix = cy::Matrix3f::RotationXYZ(11, 0, 0);

    model = model * rotMatrix;

    cy::Matrix4f view;

    view.SetIdentity();

    zVar = abs(zVar);

    const float radius = 50.0f / zVar;

    view = cy::Matrix4f::View(cameraDirection * radius, cy::Vec3f(0.0, 0, 0),  cy::Vec3f(0.0, 1.0, 0.0));  
    
    prog["view"] = view;
    prog["inverseView"] = -view;
    prog["model"] = model;
    prog["projection"] = projMatrix;
     
    
    // std::vector<unsigned char> image; //the raw pixels
    // unsigned width, height;

    // //decode
    // lodepng::decode(image, width, height, mapKD);

    // tex.SetImage(image.data(), 4, width, height);
    // tex.BuildMipmaps();
    // tex.Bind(GL_TEXTURE0);

    // prog["tex"] = GL_TEXTURE0;


    glDrawArrays(GL_TRIANGLES, 0, teapot.NF()*3);
            
    glutSwapBuffers();
 
}

void keyboardPress( unsigned char key, int x, int y)
{

    while (key == 27)
    {
        glutDestroyWindow(glutGetWindow());
        exit(0);
    }

}


void myIdle()
{
    glutPostRedisplay();
}


void leftClick(int x, int y){    
    //if the left button is X
    if(buttonPressed == 0){
        if(x > prevX)
        {
            rotX += 0.1;
        }

        if(x < prevX)
        {
            rotX -= 0.1;
        }

        if(y > prevY)
        {
            rotY += 0.1;
        }

        if(y < prevY)
        {
            rotY -= 0.1;
        }

        prevX = x;
        prevY = y;

    }

        //if the right button is clicked
        if(buttonPressed == 2)
        {

            if(y > prevZ)
            {
                zVar += 0.1;
            }

            if(y < prevZ)
            {
                zVar -= 0.1;
            }
            // printf("%f\n", zVar);

        prevZ = y;
    }
}


//
void buttonClick(int button, int state, int x, int y){
    buttonPressed = button;
    // printf("%d\n", buttonPressed);
}

int main (int argc, char** argv){


    // string objInput;
    // std::cin >> objInput;

    // objInput = "objFiles/" + objInput;
    
    // teapot.LoadFromFileObj(objInput.c_str()); 
    teapot.LoadFromFileObj("objFiles/teapot.obj"); 

    box.LoadFromFileObj("objFiles/box.obj");

    glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);

    glutInitWindowSize(1000, 750);

    glutCreateWindow("Assignment 05");


    glewInit();
    prog.BuildFiles("shaders/shader.vert", "shaders/shader.frag");

    
    prog.Bind();


    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);
    
     
    cy::Vec3f vertices [teapot.NF()*3];

    for(int i = 0; i < teapot.NF(); i++){
        cyTriMesh::TriFace currentFace = teapot.F(i);
        
        vertices [i*3 + 0] = teapot.V(currentFace.v[0]);
        vertices [i*3 + 1] = teapot.V(currentFace.v[1]);
        vertices [i*3 + 2] = teapot.V(currentFace.v[2]);
    } 

    cy::Vec3f normalArray [teapot.NF()*3];
    for(int i = 0; i < teapot.NF(); i++){
        cyTriMesh::TriFace currentFace = teapot.FN(i);
        
        normalArray [i*3 + 0] = teapot.VN(currentFace.v[0]);
        normalArray [i*3 + 1] = teapot.VN(currentFace.v[1]);
        normalArray [i*3 + 2] = teapot.VN(currentFace.v[2]);
    } 

    cy::Vec3f textureArray [teapot.NF()*3];
    for(int i = 0; i < teapot.NF(); i++){
        cyTriMesh::TriFace currentFace = teapot.FT(i);
        
        textureArray [i*3 + 0] = teapot.VT(currentFace.v[0]);
        textureArray [i*3 + 1] = teapot.VT(currentFace.v[1]);
        textureArray [i*3 + 2] = teapot.VT(currentFace.v[2]);
    } 

    

    //Buffers!!
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*teapot.NF() * 3,vertices , GL_STATIC_DRAW);
  
    GLuint pos = prog.AttribLocation ("pos");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*teapot.NF() * 3,normalArray , GL_STATIC_DRAW);

    GLuint normals = prog.AttribLocation ("normals");
    glEnableVertexAttribArray(normals);
    glVertexAttribPointer(normals, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint textureBuffer;
    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*teapot.NF()*3,textureArray , GL_STATIC_DRAW);

    GLuint txc = prog.AttribLocation ("txc");
    glEnableVertexAttribArray(txc);
    glVertexAttribPointer(txc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    cubeProg.Bind();
    cubeProg.BuildFiles("shaders/cubeShader.vert", "shaders/cubeShader.frag");
   
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
  

    cy::GLTextureCubeMap envmap;

    envmap.Initialize();

    vector<string> envMapTextures;
    envMapTextures.push_back("objFiles/cubeFiles/cubemap_posx.png");
    envMapTextures.push_back("objFiles/cubeFiles/cubemap_negx.png");
    envMapTextures.push_back("objFiles/cubeFiles/cubemap_posy.png");
    envMapTextures.push_back("objFiles/cubeFiles/cubemap_negy.png");
    envMapTextures.push_back("objFiles/cubeFiles/cubemap_posz.png");
    envMapTextures.push_back("objFiles/cubeFiles/cubemap_negz.png");
    

    unsigned char* data;
    for(unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;

        data = stbi_load(envMapTextures[i].c_str(), &width, &height, &nrChannels, 0);
        
        envmap.SetImageRGB((cy::GLTextureCubeMap::Side) i, data, 2048, 2048, 0);
    }

    envmap.BuildMipmaps();
    envmap.SetSeamless();

    envmap.Bind(GL_TEXTURE1);
    cubeProg["env"] = GL_TEXTURE1;

    cy::Vec3f cubeVertices [box.NF()*3];

    for(int i = 0; i < box.NF(); i++){
        cyTriMesh::TriFace currentFaceCube = box.F(i);
        
        cubeVertices [i*3 + 0] = box.V(currentFaceCube.v[0]);
        cubeVertices [i*3 + 1] = box.V(currentFaceCube.v[1]);
        cubeVertices [i*3 + 2] = box.V(currentFaceCube.v[2]);
    } 

    GLuint cubeBuffer;
    glGenBuffers(1, &cubeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*box.NF() * 3, cubeVertices , GL_STATIC_DRAW);
    
    GLuint posCube = cubeProg.AttribLocation ("posCube");
    glEnableVertexAttribArray(posCube);
    glVertexAttribPointer(posCube, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    
    GLuint textureCubeBuffer;
    glGenBuffers(1, &textureCubeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureCubeBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*box.NF() * 3, cubeVertices , GL_STATIC_DRAW);

    GLuint txcCube = cubeProg.AttribLocation ("txc");
    glEnableVertexAttribArray(txcCube);
    glVertexAttribPointer(txcCube, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glutMouseFunc(buttonClick);
    glutMotionFunc(leftClick);   

    glutDisplayFunc(displayTeapot);

    glutKeyboardFunc(keyboardPress);
    glutIdleFunc(myIdle);


    glutMainLoop();

    return 0;
}

