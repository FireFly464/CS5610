#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>


void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 1, 1);

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

int main (int argc, char** argv){
    glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);

    glutInitWindowSize(500, 500);
   // glutInitWindowPosition(200,200);
    glutCreateWindow("My Window");
 
    glewInit();
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);

    glutKeyboardFunc(keyboardPress);



    glutMainLoop();
    return 0;
}


