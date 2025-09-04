// Isabella Burgess - u1408202

#define GLUT_DISABLE_ATEXIT_HACK

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "include/cyTriMesh.h"
#include "include/cyMatrix.h"
#include "include/cyGL.h"

using namespace std;

cy::TriMesh teapot;
bool success = teapot.LoadFromFileObj("teapot.obj"); 
cy::GLSLProgram prog;

cy::Matrix4f mvp;
int triangleCount = 0;


float rotX1 = 0;
float rotY = 8;
float zVar = -47;

float prevX = 0;
float prevY = 0;

float prevZ = -10;

int buttonPressed;

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5, 0.5, 0.5, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    //create array of the vertecies? then use the faces to look at that array
    //god i hope this makes sense to me in the morning my brain is so very fried
    //might have to rebind the array to the buffer
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
            rotX1 += 0.1;
        }

        if(x < prevX)
        {
            rotX1 -= 0.1;
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

       // printf("Y is %f\n", rotY);
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

    float angle = 40*( M_PI/180.0 );
    

    cy::Matrix3f rotMatrix = cy::Matrix3f::RotationXYZ(rotY, rotX1, 0);
    cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(angle, float(1000)/float(750), 0.1, 1000.0f);

    cy::Vec3f transVector = cy::Vec3f(0, 0, zVar);
    cy::Matrix4f transMatrix = cy::Matrix4f::Translation(transVector);

    mvp = (projMatrix * transMatrix * rotMatrix);
    
    prog["mvp"] = mvp;
}

//
void buttonClick(int button, int state, int x, int y){
    buttonPressed = button;
    // printf("%d\n", buttonPressed);
}


int main (int argc, char** argv){

    glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);

    glutInitWindowSize(1000, 750);

    glutCreateWindow("Assignment 02");


    glewInit();
    prog.BuildFiles("shader.vert", "shader.frag");

    glutMouseFunc(buttonClick);
    glutMotionFunc(leftClick);

    prog.Bind();
   

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
     
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


    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(keyboardPress);
    
  
    glutIdleFunc(myIdle);

    
    glutMainLoop();

    return 0;
}

