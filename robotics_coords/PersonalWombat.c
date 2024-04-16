#include <stdio.h>
#include <kipr/wombat.h>
#include <math.h>


//function declaration:
//the values for the distance are all converted to inches from some n ammount of ticks
void turnLeft(double angle1);  //takes in the angle to figure out how much degrees to turn to the left
void turnRight(double angle2);  //takes in the angle to figure out how much degrees to turn to the right
void move(double distance);

int l = 2;
int r = 0;
int rockPosition = 1938;
int puffBallPosition = 1154;

int main (){
    move(25.1);
    //printf("%.2f", cos(21));
    return 0;
}

//functions 
void move(double distance){
    //mav(l, 1200*sin(21));
    //mav(r, 1200*cos(21));
    for(float i = 0; i <= 12*3.1415926; i += (3.1415926/24)) {
    	mav(l, round(-1200*cos(i)));
        mav(r, round(-1200*cos(i)));
        printf("%.3f", -1200*cos(i));
        msleep(distance/0.00675);
    	ao();
    }
    //msleep(distance/0.00675);
    //ao();
}
          


void turnLeft(double angle1){
    mav(l, 1000);
    mav(r, -1000);
    msleep((1010.0/90)*angle1);
    ao();
}

void turnRight(double angle2){
    mav(l, -1000);
    mav(r, 1000);
    msleep((970.0/90)*angle2);
    ao();
}
    
