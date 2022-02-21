// $ gcc -o source Source.cpp -lGL -lGLU -lglut -lm
// $ ./source

#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

int moving, startx, starty;

static int elbow[] = { 0, 0 };
static int shoulder[] = { 0, 0 };
static int shoulder_Z[] = { 0, 0 };
static int finger_up[] = { 0, 0 };
static int finger_base[] = { 0, 0 };
static int legR[] ={0,0};
static int legL[] ={0,0};
static int knee[]={0 ,0};



GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */
double eye[] = { 0, 0, 2 };
double center[] = { 0, 0, 1 };
double up[] = { 0, 1, 0 };
double speed = 0.2;
int pressed = 0;
int limitForwardBackward = 0;
int limitUpDown = 0;
int limitRightLeft = 0;
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(65.0, (GLfloat)1024 / (GLfloat)869, 1.0, 60.0);
}
void crossProduct(double a[], double b[], double c[])
{
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
    double norm;
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[])
{

    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2] * p[1] + a[1] * p[2];
    temp[1] = a[2] * p[0] - a[0] * p[2];
    temp[2] = -a[1] * p[0] + a[0] * p[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1 - cos(theta))*(a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
    temp[1] += (1 - cos(theta))*(a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
    temp[2] += (1 - cos(theta))*(a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

    temp[0] += cos(theta)*p[0];
    temp[1] += cos(theta)*p[1];
    temp[2] += cos(theta)*p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];

}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

void Left()
{


    // implement camera rotation arround vertical window screen axis to the left
    // used by mouse and left arrow
    rotatePoint(up, (float)22/7/20, eye);

    limitRightLeft += 1;

}

void Right()
{

    // implement camera rotation arround vertical window screen axis to the right
    // used by mouse and right arrow
    rotatePoint(up, -(float)22/7/20, eye);
    limitRightLeft -= 1;
}

void Up()
{
    // implement camera rotation arround horizontal window screen axis +ve
    // used by up arrow
    double horizontal[] = {0, 0, 0};

    crossProduct(eye, up, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, 3.14/20, eye);
    rotatePoint(horizontal, 3.14/20, up);
    limitUpDown += 1;

}

void Down()
{
    // implement camera rotation arround horizontal window screen axis
    // used by down arrow
    double horizontal[] = {0, 0, 0};
    crossProduct(eye, up, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, -3.14/20, eye);
    rotatePoint(horizontal, -3.14/20, up);
    limitUpDown -= 1;

}

void moveForward()
{
    double direction[] = {0, 0, 0};
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    eye[0] -= direction[0] * speed;
    eye[1] -= direction[1] * speed;
    eye[2] -= direction[2] * speed;
    center[0] -= direction[0] * speed;
    center[1] -= direction[1] * speed;
    center[2] -= direction[2] * speed;

    limitForwardBackward += 1;
}

void moveBack()
{

    double direction[] = {0, 0, 0};
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;
    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;

    limitForwardBackward -= 1;
}


void display(void)
{

   glClear(GL_COLOR_BUFFER_BIT);



    glPushMatrix();
    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              up[0], up[1], up[2]);
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);


    // START OF TRUNK
    glPushMatrix();
    glTranslatef(0.0, 0.25, 0.0);
    glScalef(1, 0.5, 0.5);
    glutWireCube(1.0);
    glPopMatrix();
    // END OF TRUNK

    // START OF STOMACH
    glPushMatrix();
    glTranslatef(0.0, 1.25, 0.0);
    glScalef(1, 1.5, 0.5);
    glutWireCube(1.0);
    glPopMatrix();
    // END OF STOMACH

    // START OF RIGHT LEG
    glPushMatrix();

    glTranslatef(0.1, 0.0, 0.0);
    glRotatef((GLfloat)legR[0], 0.0, 0.0, 1.0);
    glTranslatef(-0.1, -0.25, 0.0);

    glRotatef((GLfloat)legL[0], 1.0, 0.0, 0.0);
    glTranslatef(0.3, -0.5, 0.0);

    glPushMatrix();
    glScalef (0.4, 1.5, 0.5);
    glutWireCube(1.0);
    glPopMatrix();


    glTranslatef(0.0, -0.75, 0.0);
    glRotatef(GLfloat (knee[0]), 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glPushMatrix();
    glScalef (0.4, 1.0, 0.5);
    glutWireCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -0.625, 0.0);
    glPushMatrix();
    glScalef (0.5, 0.25, 0.9);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();
    // END OF RIGHT LEG

    //START OF LEFT LEG
    glPushMatrix();

    glTranslatef(-0.1, 0.0, 0.0);
    glRotatef((GLfloat)legR[1], 0.0, 0.0, 1.0);
    glTranslatef(0.1, -0.25, 0.0);

    glRotatef((GLfloat)legL[1], 1.0, 0.0, 0.0);
    glTranslatef(-0.29, -0.5, 0.0);

    glPushMatrix();
    glScalef (0.4, 1.5, 0.5);
    glutWireCube(1.0);
    glPopMatrix();


    glTranslatef(0.0, -0.75, 0.0);
    glRotatef(GLfloat (knee[1]), 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glPushMatrix();
    glScalef (0.4, 1.0, 0.5);
    glutWireCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -0.625, 0.0);
    glPushMatrix();
    glScalef (0.5, 0.25, 0.9);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    //END OF LEFT LEG
    


    // START OF HEAD
    glPushMatrix();
    glTranslatef(0.0, 2.5, 0.0);
    glutWireSphere(0.35, 15, 15);
    glPopMatrix();
    // END OF HEAD

    // START OF RIGHT ARM

    glPushMatrix();

    glTranslatef(0.88, 1.85, 0);

    glRotatef((GLfloat)shoulder_Z[0], 1.0, 0.0, 0.0);


    glTranslatef(-0.375, 0, 0);
    glRotatef(shoulder[0], 0.0, 0.0, 1.0);
    glTranslatef(0.375, 0, 0);

    
    glPushMatrix();
    glScalef(0.75, 0.3, 0.3);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslatef(0.75, 0, 0);

    glTranslatef(-0.375, 0, 0.0);
    glRotatef((GLfloat)elbow[0], 0.0, 0.0, 1.0);
    glTranslatef(0.375, 0, 0.0);
    glPushMatrix();
    glScalef(0.75, 0.3, 0.3);
    glutWireCube(1.0);
    glPopMatrix();

    

    // START OF FINGER 1
    glPushMatrix();

    glTranslatef(0.45, 0.05, 0.1125);
    glTranslatef(-0.1, 0, 0);
    glRotatef(finger_base[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(finger_up[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 1


    // START OF FINGER 2
    glPushMatrix();

    glTranslatef(0.45, 0.05, 0.0375);
    glTranslatef(-0.1, 0, 0);
    glRotatef(finger_base[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(finger_up[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 2

    // START OF FINGER 3
    glPushMatrix();

    glTranslatef(0.45, 0.05, -0.0375);

    glTranslatef(-0.1, 0, 0);
    glRotatef(finger_base[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(finger_up[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 3


    // START OF FINGER 4
    glPushMatrix();

    glTranslatef(0.45, 0.05, -0.1125);
    glTranslatef(-0.1, 0, 0);
    glRotatef(finger_base[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(finger_up[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 4


    // START OF FINGER 5
    glPushMatrix();
    glTranslatef(0.475, -0.1, 0);
    glTranslatef(-0.1, 0, 0);
    glRotatef(-finger_base[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(-finger_up[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 5

    glPopMatrix();
    // END OF RIGHT ARM


    // START OF LEFT ARM

    glPushMatrix();



    glTranslatef(-0.88, 1.85, 0);
    glRotatef((GLfloat)shoulder_Z[1], 1.0, 0.0, 0.0);
    glTranslatef(0.375, 0, 0);
    glRotatef(shoulder[1], 0.0, 0.0, 1.0);
    glTranslatef(-0.375, 0, 0);

    
    glPushMatrix();
    glScalef(0.75, 0.3, 0.3);
    glutWireCube(1.0);
    glPopMatrix();



    glTranslatef(-0.75, 0, 0);

    glTranslatef(0.375, 0, 0.0);
    glRotatef((GLfloat)elbow[1], 0.0, 0.0, 1.0);
    glTranslatef(-0.375, 0, 0.0);
    glPushMatrix();
    glScalef(0.75, 0.3, 0.3);
    glutWireCube(1.0);
    glPopMatrix();

   

    // START OF FINGER 1
    glPushMatrix();

    glTranslatef(-0.475, 0.05, 0.1125);
    glTranslatef(0.1, 0, 0);
    glRotatef(finger_base[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(finger_up[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 1


    // START OF FINGER 2
    glPushMatrix();

    glTranslatef(-0.475, 0.05, 0.0375);
    glTranslatef(0.1, 0, 0);
    glRotatef(finger_base[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(finger_up[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 2

    // START OF FINGER 3
    glPushMatrix();

    glTranslatef(-0.475, 0.05, -0.0375);
    glTranslatef(0.1, 0, 0);
    glRotatef(finger_base[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(finger_up[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 3


    // START OF FINGER 4
    glPushMatrix();

    glTranslatef(-0.475, 0.05, -0.1125);
    glTranslatef(0.1, 0, 0);
    glRotatef(finger_base[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(finger_up[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 4


    // START OF FINGER 5
    glPushMatrix();

    glTranslatef(-0.475, -0.1, 0);
    glTranslatef(0.1, 0, 0);
    glRotatef(-finger_base[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(-finger_up[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutWireCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 5

    glPopMatrix();
    // END OF LEFT ARM

    glPopMatrix();
    glutSwapBuffers();

}
void reset()
{
	double e[] = {0.0, 0.0, 5.0};
	double c[] = {0.0, 0.0, 0.0};
	double u[] = {0.0, 1.0, 0.0};
	
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            if(pressed==0) {
                if (limitForwardBackward < 10) {
                    moveForward();
                }
            }
            else {
                if (limitRightLeft < 10) {
                    Left();
                }
            }
            break;
        case GLUT_KEY_RIGHT:
            if(pressed==0) {
                if (limitForwardBackward > -10){
                    moveBack();
                }
            }
            else {
                if (limitRightLeft > -10) {
                    Right();
                }
            }
            break;
        case GLUT_KEY_UP:
            if(limitUpDown<20)
            {
                Up();
            }
            break;
        case GLUT_KEY_DOWN:
            if(limitUpDown>-20){
                Down();}
            break;
    }

    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key) {


    //LEGS MOVMENT
    case 'k':
    if(legL[1] >= -70){
        legL[1] = (legL[1]-5) %360;
        glutPostRedisplay();
    }
    break;
    case 'K':
    if(legL[1] <= 70){
        legL[1] = (legL[1]+5) %360;
        glutPostRedisplay();
    }
    break;
    case 'j':
    if(legR[1]>= -70){
        legR[1]  = (legR[1]-5) %360;
        glutPostRedisplay();
    }
    break;
    case 'J':
    if(legR[1]<= 2){
        legR[1]  = (legR[1]+5) %360;
        glutPostRedisplay();
    }
    break;


    case 'l':
    if(legL[0] >= -70){
        legL[0] = (legL[0]-5) %360;
        glutPostRedisplay();
    }
    break;
    case 'L':
    if(legL[0] <= 70){
        legL[0] = (legL[0]+5) %360;
        glutPostRedisplay();
    }
    break;
    case 'P':
    if(legR[0]>= -2){
        legR[0]  = (legR[0]-5) %360;
        glutPostRedisplay();
    }
    break;
    case 'p':
    if(legR[0]<= 70){
        legR[0]  = (legR[0]+5) %360;
        glutPostRedisplay();
    }
    break;
    //KNEE MOVMENTS
    case 'y':
    if(knee[0] <=70){
        knee[0] = (knee[0]+5) %360;
        glutPostRedisplay();
    }
    break;
    case 'Y':
    if(knee[0] >=-15){
        knee[0] = (knee[0]-5) %360;
        glutPostRedisplay();
    }
    break;
    case 't':
    if(knee[1] <=70){
        knee[1] = (knee[1]+5) %360;
        glutPostRedisplay();
    }
    break;
    case 'T':
    if(knee[1] >=-15){
        knee[1] = (knee[1]-5) %360;
        glutPostRedisplay();
    }
    break;

       // shoulder MOVEMENT
    case 's':
        if (shoulder[1] < 90) {
            shoulder[1] = (shoulder[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'S':
        if (shoulder[1] > -90) {
            shoulder[1] = (shoulder[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'D':
        if (shoulder[0] < 90) {
            shoulder[0] = (shoulder[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'd':
        if (shoulder[0] > -90) {
            shoulder[0] = (shoulder[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;


        // ELBOWS MOVEMENT
    case 'e':
        if (elbow[1] < 135) {
            elbow[1] = (elbow[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'E':
        if (elbow[1] > -10) {
            elbow[1] = (elbow[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'N':
        if (elbow[0] < 10) {
            elbow[0] = (elbow[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'n':
        if (elbow[0] > -135) {
            elbow[0] = (elbow[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'z':
        if (shoulder_Z[0] < 90) {
            shoulder_Z[0] = (shoulder_Z[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'Z':
        if (shoulder_Z[0] > -90) {
            shoulder_Z[0] = (shoulder_Z[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'x':
        if (shoulder_Z[1] < 90) {
            shoulder_Z[1] = (shoulder_Z[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'X':
        if (shoulder_Z[1] > -90) {
            shoulder_Z[1] = (shoulder_Z[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    //finger_up movement
    case 'q':
        if (finger_base[1] < 20) {
            finger_base[1] = (finger_base[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'Q':
        if (finger_base[1] > -20) {
            finger_base[1] = (finger_base[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'w':
        if (finger_up[1] < 20) {
            finger_up[1] = (finger_up[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'W':
        if (finger_up[1] > -20) {
            finger_up[1] = (finger_up[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'i':
        if (finger_base[0] < 20) {
            finger_base[0] = (finger_base[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'I':
        if (finger_base[0] > -20) {
            finger_base[0] = (finger_base[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'o':
        if (finger_up[0] < 20) {
            finger_up[0] = (finger_up[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'O':
        if (finger_up[0] > -20) {
            finger_up[0] = (finger_up[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'r':

        eye[0] = 0;
        eye[1] = 0;
        eye[2] = 2;
        center[0] = 0;
        center[1] = 0;
        center[2] = 1;
        up[0] = 0;
        up[1] = 1;
        up[2] = 0;
        angle = 0;
        angle2 = 0;
        limitRightLeft = 0;
        limitForwardBackward = 0;
        limitUpDown = 0;
        glutPostRedisplay();
        break;
      
		break;
    
    case 27:
        exit(0);
        
        break;
    default:
        break;
    }
       
}



static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            pressed = 1;
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
            pressed = 0;
        }
    }
}


static void motion(int x, int y)
{
    if (moving) {
        angle = angle + (x - startx);
        angle2 = angle2 + (y - starty);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}