# Assignment 2: Robotic Body and Camera movement




![My GIF](/animatedGIF.gif)

# Camera Movements


## Camera rotation arround vertical axis to the left and right


```cpp
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

void Left()
{
    rotatePoint(up, (float)22/7/20, eye);

    limitRightLeft += 1;

}

void Right()
{

    rotatePoint(up, -(float)22/7/20, eye);
    limitRightLeft -= 1;
}
```



 ## Rotation in Up and Down around Horizontal Axis
 using mouse or up and down arrows

```cpp
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
void Up()
{

    double horizontal[] = {0, 0, 0};

    crossProduct(eye, up, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, 3.14/20, eye);
    rotatePoint(horizontal, 3.14/20, up);
    limitUpDown += 1;

}

void Down()
{

    double horizontal[] = {0, 0, 0};
    crossProduct(eye, up, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, -3.14/20, eye);
    rotatePoint(horizontal, -3.14/20, up);
    limitUpDown -= 1;

}

```
## Moving Forward and Backwards
using right and left arrows cause limited movement for object forward and backward.

```cpp
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
```
 ##  *reset camera* : <kbd>r</kbd>

```cpp
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
```

## Special Keys

```cpp
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
```

<br/>


# Keyboard body joints' movements 
* *Right leg* : <kbd>k</kbd> <kbd>K</kbd> flexion & extention
* *Right leg*: <kbd>j</kbd> <kbd>J</kbd>  abduction & adduction
* *left leg*: <kbd>l</kbd> <kbd>L</kbd> flexion & extention
* *left leg*  : <kbd>p</kbd> <kbd>P</kbd> abduction & adduction
* *left knee* : <kbd>y</kbd> <kbd>Y</kbd>
* *Right knee* : <kbd>t</kbd> <kbd>T</kbd>
* *Right shoulder* : <kbd>s</kbd> <kbd>S</kbd> abduction & adduction
* *left shoulder* : <kbd>d</kbd> <kbd>D</kbd> abduction & adduction
* *Right elbow* : <kbd>e</kbd> <kbd>E</kbd>
* *left elbow* : <kbd>n</kbd> <kbd>N</kbd>
* *left shoulder rotation* : <kbd>z</kbd> <kbd>Z</kbd>
* *Right shoulder rotation* : <kbd>x</kbd> <kbd>X</kbd>
* *Right hand fingers lower* : <kbd>q</kbd> <kbd>Q</kbd>
* *Right hand fingers upper* : <kbd>W</kbd> <kbd>w</kbd>
* *left hand fingers lower* : <kbd>I</kbd> <kbd>i</kbd>
* *left hand fingers upper* :<kbd>o</kbd><kbd>O</kbd>

# Robotic-Body-With-Camera-Movements
