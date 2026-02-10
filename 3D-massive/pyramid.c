#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>


float angle = 0.0f;

void drawPyramid() {
    glBegin(GL_TRIANGLES);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 0.0f,  1.0f,  0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f( 0.0f,  1.0f,  0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f( 0.0f,  1.0f,  0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f( 0.0f,  1.0f,  0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    }
    
    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        glTranslatef(0.0f, 0.0f, -6.0f);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glRotatef(angle * 0.5f, 1.0f, 0.0f, 0.0f);
        
        drawPyramid();
        
        glutSwapBuffers();
    }
    
    void update(int value) {
        angle += 2.0f;
        if (angle > 360) angle -= 360;
        
        glutPostRedisplay();
        glutTimerFunc(16, update, 0);
    }
    
    
