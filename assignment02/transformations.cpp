// Isabella Burgess - u1408202

#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "include/cyTriMesh.h"
#include "include/cyMatrix.h"
#include "include/cyGL.h"


cy::TriMesh teapot;
bool success = teapot.LoadFromFileObj("teapot.obj");
    cy::GLSLProgram prog;

cy::Matrix4f mvp;


float rotX1 = 0;
float rotY = 0;
float zVar = -47;

float prevX = 0;
float prevY = 0;

float prevZ = 0;

int buttonPressed;

void myDisplay()
{
   

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5, 0.5, 0.5, 1);

    glDrawArrays(GL_POINTS, 0, teapot.NV());
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
    
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*teapot.NV(),&teapot.V(0), GL_STATIC_DRAW);

    GLuint pos = prog.AttribLocation ("pos");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(keyboardPress);
     
  
    glutIdleFunc(myIdle);

    
    glutMainLoop();
    return 0;
}

