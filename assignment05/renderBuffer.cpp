// Isabella Burgess - u1408202

#define GLUT_DISABLE_ATEXIT_HACK

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

cy::GLSLProgram prog;
cy::GLSLProgram planeProg;

cy::Matrix4f mvp;
cy::Matrix4f planeMvp;

int triangleCount = 0;

float rotX1 = -1.55;
float rotY1 = 0;
float zVar1 = 1;

float prevX1 = 0;
float prevY1 = 0;

float prevZ1 = -10;

float rotX2 = 0;
float rotY2 = 0;
float zVar2 = -2;

float prevX2 = 0;
float prevY2 = 0;

float prevZ2 = 0;

int buttonPressed;
cy::Vec3f cameraPos = cy::Vec3f(0.0f, 0.0f, 3.0f);
cy::Vec3f cameraTarget = cy::Vec3f(0.0f, 0.0f, 3.0f);

cy::Vec3f square[] = {
    {-0.5f, -0.5f, 0.0f}, 
    {0.5f, -0.5f, 0.0f}, 
    {-0.5f, 0.5f, 0.0f}, 

    {0.5f, 0.5f, 0.0f},   
    {0.5f, -0.5f, 0.0f}, 
    {-0.5f, 0.5f, 0.0f}};

cy::Vec2f squareUV[] = {{0, 0}, 
                        {1, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 1}};

// cy::Vec3f square[] = {
//     {-1.0f, -1.0f, 0.0f}, 
//     {1.0f, -1.0f, 0.0f}, 
//     {-1.0f, 1.0f, 0.0f}, 
//     {1.0f, 1.0f, 0.0f},   
//     {1.0f, -1.0f, 0.0f}, 
//     {-1.0f, 1.0f, 0.0f}};

cy::Vec3f squareTextureCoords[] = {
    {0.0f, 0.0f, 0.0f}, 
    {1.0f, 0.0f, 0.0f}, 
    {0.0f, 1.0f, 0.0f}, 
    {1.0f, 1.0f, 0.0f},   
    {1.0f, 0.0f, 0.0f}, 
    {0.0f, 1.0f, 0.0f}};    
    
GLuint vao1;
GLuint vao2;

cy::GLRenderTexture2D renderBuffer;



void displayTeapot()
{
    //Render teapot
    prog.Bind();
    glUseProgram(prog.GetID());
    glBindVertexArray(vao1);

    
    float angle = 40*( M_PI/180.0 );
    
    //MVP calculations
            
    float directionX = cos((rotX1)) * cos((rotY1));
    float directionY = sin((rotY1));
    float directionZ = sin((rotX1)) * cos((rotY1));

    cy::Vec3f cameraDirection = cy::Vec3f(directionX, directionY , directionZ );

    cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(angle, float(1000)/float(750), 0.1, 1000.0f);
 
    cy::Vec3f transVector = cy::Vec3f(0, -5, 0);
    cy::Matrix4f model = cy::Matrix4f::Translation(transVector);

    
    cy::Matrix3f rotMatrix = cy::Matrix3f::RotationXYZ(11, 0, 0);

    model = model * rotMatrix;

    cy::Matrix4f view;

    view.SetIdentity();

    zVar1 = abs(zVar1);

    const float radius = 50.0f / zVar1;

    view = cy::Matrix4f::View(cameraDirection * radius, cy::Vec3f(0.0, 0, 0),  cy::Vec3f(0.0, 1.0, 0.0));  
    
    prog["view"] = view;
    prog["model"] = model;
    prog["projection"] = projMatrix;
     
    renderBuffer.Initialize(true, 3, 1000, 1000);
    
    renderBuffer.Bind();
        //Texture stuff!
        cyGLTexture2D tex;
        tex.Initialize();

        cy::TriMesh::Mtl material = teapot.M(0);
        string mapKD = material.map_Kd.data;

        std::vector<unsigned char> image; //the raw pixels
        unsigned width, height;

        //decode
        lodepng::decode(image, width, height, mapKD);

        tex.SetImage(image.data(), 4, width, height);
        tex.BuildMipmaps();
        tex.Bind(GL_TEXTURE0);

        prog["tex"] = GL_TEXTURE0;


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1);


        glDrawArrays(GL_TRIANGLES, 0, teapot.NF()*3);
            
        renderBuffer.BuildTextureMipmaps();
        renderBuffer.BindTexture(GL_TEXTURE1);

        planeProg["renderBuffer"] = GL_TEXTURE1;
        
    renderBuffer.Unbind();

    //Plane render
    planeProg.Bind();
    glUseProgram(planeProg.GetID());
    glBindVertexArray(vao2);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5, 0.5, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //MVP calculation
    cy::Matrix3f planeRotMatrix = cy::Matrix3f::RotationXYZ(rotY2, rotX2, 0);
    cy::Matrix4f planeProjMatrix = cy::Matrix4f::Perspective(angle, float(1000)/float(750), 0.1, 1000.0f);

    cy::Vec3f planeTransVector = cy::Vec3f(0, 0, zVar2);
    cy::Matrix4f planeTransMatrix = cy::Matrix4f::Translation(planeTransVector);

    planeMvp = (planeProjMatrix * planeTransMatrix * planeRotMatrix);
    
    planeProg["mvpPlane"] = planeMvp;

    glDrawArrays(GL_POLYGON , 0, 6);
  

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
    // printf("Alt is %d\n", altPressed);
    
    if(glutGetModifiers() == GLUT_ACTIVE_ALT){
        if(buttonPressed == 0){
            if(x > prevX2)
            {
                rotX2 += 0.1;
            }

            if(x < prevX2)
            {
                rotX2 -= 0.1;
            }

            if(y > prevY2)
            {
                rotY2 += 0.1;
            }

            if(y < prevY2)
            {
                rotY2 -= 0.1;
            }

            prevX2 = x;
            prevY2 = y;

        // printf("Y is %f\n", rotY);
        }

        //if the right button is clicked
        if(buttonPressed == 2)
        {
            if(y > prevZ2)
            {
                zVar2 += 0.1;
            }

            if(y < prevZ2)
            {
                zVar2 -= 0.1;
            }
            // printf("%f\n", zVar);

            prevZ2 = y;
        }
    }

    else{
        //if the left button is X
        if(buttonPressed == 0){
            if(x > prevX1)
            {
                rotX1 += 0.1;
            }

            if(x < prevX1)
            {
                rotX1 -= 0.1;
            }

            if(y > prevY1)
            {
                rotY1 += 0.1;
            }

            if(y < prevY1)
            {
                rotY1 -= 0.1;
            }

            prevX1 = x;
            prevY1 = y;

        // printf("Y is %f\n", rotY);
        }

        //if the right button is clicked
        if(buttonPressed == 2)
        {

            if(y > prevZ1)
            {
                zVar1 += 0.1;
            }

            if(y < prevZ1)
            {
                zVar1 -= 0.1;
            }
            // printf("%f\n", zVar);

            prevZ1 = y;
        }
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
    // teapot.LoadFromFileObj(objInput.c_str()); 

    teapot.LoadFromFileObj("teapot.obj"); 

    glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);

    glutInitWindowSize(1000, 750);

    glutCreateWindow("Assignment 05");


    glewInit();
    prog.BuildFiles("shaders/shader.vert", "shaders/shader.frag");

    
    prog.Bind();


    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);
    
    vector<cy::Vec3f> faces;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*teapot.NF() * 3,textureArray , GL_STATIC_DRAW);

    GLuint txc = prog.AttribLocation ("txc");
    glEnableVertexAttribArray(txc);
    glVertexAttribPointer(txc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    planeProg.Bind();
    planeProg.BuildFiles("shaders/shader2.vert", "shaders/shader2.frag");

    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);


    GLuint squareBuffer;
    glGenBuffers(1, &squareBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, squareBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*6 * 3, square, GL_STATIC_DRAW);
    
    GLuint posSquare = planeProg.AttribLocation ("posSquare");
    glEnableVertexAttribArray(posSquare);
    glVertexAttribPointer(posSquare, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    
    GLuint textureSquareBuffer;
    glGenBuffers(1, &textureSquareBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureSquareBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*6 *3, squareTextureCoords , GL_STATIC_DRAW);

    GLuint txcSquare = planeProg.AttribLocation ("txcSquare");
    glEnableVertexAttribArray(txcSquare);
    glVertexAttribPointer(txcSquare, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glutMouseFunc(buttonClick);
    glutMotionFunc(leftClick);

    
    glutDisplayFunc(displayTeapot);

    glutKeyboardFunc(keyboardPress);
    glutIdleFunc(myIdle);


    glutMainLoop();

    return 0;
}

