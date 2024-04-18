
#include <stdio.h>
#include <kipr/wombat.h>
#include <math.h>
#include <stdbool.h>
#include "matrix.h"

#define PI 3.141592653589793
#define WINDOW_HEIGHT 241
#define WINDOW_WIDTH 119.38


struct bottom_left {
    float xn;
    float yn;
};
struct bottom_left convert_coords(float x, float y) {
    struct bottom_left hp;
    hp.xn = -x + WINDOW_WIDTH;
    hp.yn = -y + WINDOW_HEIGHT;
    return hp;
}
double* generate_arrow(double* buffer, float x1, float y1, float x2, float y2) { //just allocate
    //float y_t = convert_coords(x2,y2).yn;
    //double phi = acos(x2 / (sqrt((pow(x2,2.0) + pow(y2,2.0)))) );
    
    if(buffer != NULL) {
        buffer[0] = x1;
        buffer[1] = y1;
        buffer[2] = x2;
        buffer[3] = y2;
        return buffer;
    } else {
        double* failsafe_buf = (double*)malloc(sizeof(double)*4);
        failsafe_buf[0] = x1; failsafe_buf[1] = y1; failsafe_buf[2] = x2; failsafe_buf[3] = y2;
        return failsafe_buf;
    }
}
double* nonorigin_rotate(float angle, float x1, float y1, float curr_x, float curr_y) {
    // double transform[4];
    double* transform = (double*)malloc(sizeof(double)*4);
    generate_arrow(transform, x1, y1, curr_x, curr_y);
    //translate to origin
    transform[2] -= transform[0];
    transform[3] -= transform[1];
    //rotate (combine with wheel rotatoin)
    float new_x = transform[2]*cos(angle) - transform[3]*sin(angle);
    float new_y = transform[2]*sin(angle) + transform[3]*cos(angle);

    transform[2] = new_x + transform[0];
    transform[3] = new_y + transform[1];

    // generate_arrow(NULL, x1, y1, transform[2], transform[3]); //orientation vector

    return transform;
}
float spec_angle(double* v1, double* v2, float curr_x, float curr_y) {
    return acos(
        ( (*(v1+2)-curr_x)*(*(v2+2)-curr_x) + ((*(v1+3)-curr_y)*(*(v2+3)-curr_y)) ) / 
        ( sqrt(pow(*(v1+2)-curr_x,2)+pow(*(v1+3)-curr_y,2)) * sqrt( pow(*(v2+2)-curr_x,2) + pow(*(v2+3)-curr_y,2)) )
    );
}

struct rect { //robot is a rectangle bruh (everything is cm)
    float x;
    float y;
    int h;
    int w;
};




          


void turnLeft(double angle1){
    mav(2, 1000);
    mav(0, -1000);
    msleep((1010.0/90)*angle1);
    ao();
}

void turnRight(double angle2){
    mav(2, -1000);
    mav(0, 1000);
    msleep((970.0/90)*angle2);
    ao();
}
void move(double distance){
    mav(2, 1500);
    mav(0, 1500);
    // for(float i = 0; i <= 12*3.1415926; i += (3.1415926/24)) {
    // 	mav(l, round(-1200*cos(i)));
    //     mav(r, round(-1200*cos(i)));
    //     printf("%.3f", -1200*cos(i));
    //     msleep(distance/0.00675);
    // 	ao();
    // }
    msleep(distance/0.00675);
    ao();
}






int main(int argc, char* argv[]) {
   // uint32_t flags = 0; 
    static int angle=0;
    struct rect robot; robot.h = 44; robot.w = 21;
    //initial postition
    robot.x = 54;
    robot.y = 21.75;
    const float robo_y_adj = 21.5;

    bool exit = false;
    printf("ah yes, the path to the swamp has been found\n");
    while(!exit) {
        float curr_y = WINDOW_HEIGHT-(robot.y+(robot.h/2)); //center of square with respect to origin bl
        float curr_x = robot.x+(robot.w/2); //center of square 
        double* v1 = nonorigin_rotate((PI*angle)/180, curr_x,curr_y, curr_x,curr_y+robo_y_adj);
        double* v2 = generate_arrow(NULL,curr_x,curr_y,curr_x,100);

        float mfer = spec_angle(v1,v2,curr_x,curr_y);
        printf("angle: %f\r", mfer);
        if(*(v1+3) <= (curr_y+robo_y_adj) && *(v1+3) > curr_y) {
            printf("\ndo da clockwise\n");
        } else {
            printf("do counterclockwise\n");
        }

        //move(*(v2+3));
        turnLeft(mfer);



        free(v1);
        free(v2);
        //set frame rate

    }
    return 0;
}

//functions 
