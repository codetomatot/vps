#include <stdio.h>
#include <kipr/wombat.h>
#include <math.h>
#include <stdbool.h>
#include "matrix.h"
#define PI 3.141592653589793
#define WINDOW_HEIGHT 241
#define WINDOW_WIDTH 119.38
int s = 0;


int rot1 = 0;
int rot2 = 0;
struct bottom_left {
    float xn;
    float yn;
};
struct rect { //robot is a rectangle bruh (everything is cm)
    float x;
    float y;
    int h;
    int w;
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
float spec_angle(double* v1, double* v2, float curr_x, float curr_y) { //{x1,y1,x2,y2}, {x1,y1,x2,y2}
    return acos(
        ( (*(v1+2)-curr_x)*(*(v2+2)-curr_x) + ((*(v1+3)-curr_y)*(*(v2+3)-curr_y)) ) / 
        ( sqrt(pow(*(v1+2)-curr_x,2)+pow(*(v1+3)-curr_y,2)) * sqrt( pow(*(v2+2)-curr_x,2) + pow(*(v2+3)-curr_y,2)) )
    );
}






          


void turn_Left(double angle2){
    float angle_r = (100000)*(angle2);
    printf("%f\n", angle_r);
    while(rot1<(angle_r)){ //2*PI*100000
        mav(3, -1000);
        mav(1, 1000);
    	
        
    	ao();
        msleep(1);
        
        rot1 += 1000;
    }
}

void turn_Right(float angle2){
    float angle_r = (100000)*(angle2);
    printf("%f\n", angle_r);
    while(rot1<(angle_r)){ //2*PI*100000
        mav(3, 1200);
        mav(1, -1200);
    	
        
    	ao();
        msleep(1);
        
        rot1 += 1000;
    }
}
void r_move(float distance){
    mav(3, -1500);
    mav(1, -1500);
    msleep(distance/0.017145); //previous mode 0.00675
    ao();
}
void moveBack(float d){
    mav(3, 1500);
    mav(1, 1500);
    msleep(d/0.017145); //previous mode 0.00675
    ao();
}
void openSorter(){
    set_servo_position(0, 1941);
}
void closeSorter(){
    set_servo_position(0, 1100);
}



int main() {
   // uint32_t flags = 0; 
    float angle=0;
    struct rect robot; 
	robot.h = 44; 
	robot.w = 21;
    //initial postition
    robot.x = 42.5;
    robot.y = WINDOW_HEIGHT-(37);
    const float robo_y_adj = 21.5;
    printf("ah yes, the path to the swamp has been found\n");
    //strat
  	float init[2] = {robot.x+(robot.w/2), 18};
    const float target_coords[6][3] = {
        {42.5,WINDOW_HEIGHT-(115),0}, //turns left initial
        {57, WINDOW_HEIGHT-(115), 1}, //turns right to get next rock
        {90, WINDOW_HEIGHT-(115), 0}, //adjust course 
        {90, WINDOW_HEIGHT-(130), 0},
        {42.5, WINDOW_HEIGHT-(130), 0},
        {40, WINDOW_HEIGHT-(200), 1}
    };
    enable_servos();
    set_servo_position(3, 0);
    for(int i = 0; i < sizeof(target_coords)/sizeof(target_coords[0]); i++) {
        printf("%f, %f\n\n", init[0], init[1]);
        double *v1 = nonorigin_rotate(angle, init[0],init[1], init[0],init[1]+robo_y_adj);
        double *v2 = generate_arrow(NULL,init[0],init[1],target_coords[i][0],target_coords[i][1]);
        
    	float shrek = spec_angle(v1, v2, init[0],init[1]);
        float distance_metric =sqrt( pow((target_coords[i][0] - init[0]), 2) + pow( (target_coords[i][1] - init[1]), 2) );


        printf("\nshrek angle: %f\n", shrek);
        if(target_coords[i][2] == 1) {
        	turn_Right(shrek);
            //angle += shrek;
        } else if(target_coords[i][2] == 0) {
            if(i == 5) {
            	turn_Left(2*shrek );
            }else {
        	turn_Left(shrek);}
        }
        r_move(distance_metric);
        //update init

        init[0] = target_coords[i][0];
        init[1] = target_coords[i][1];
        
     //   angle += shrek;
    }
    ao();
    return 0;
}
