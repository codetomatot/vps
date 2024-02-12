#include <stdio.h>
#include <math.h>
#include <stdbool.h>
//#include <kipr/wombat.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "matrix.h"

#define PI 3.141592653589793
#define WINDOW_HEIGHT 800
#define WINDOWN_WIDTH 800

struct bottom_left {
    float xn;
    float yn;
};
struct bottom_left convert_coords(float x, float y) {
    struct bottom_left hp;
    hp.xn = -x + WINDOWN_WIDTH;
    hp.yn = -y + WINDOW_HEIGHT;
    return hp;
}

char optlookup[2] = "ri";

void render_color(SDL_Renderer* r, int* c) {
    SDL_SetRenderDrawColor(r, c[0], c[1], c[2], 255);
}

void create_border(SDL_Renderer* r, int xpos, int ypos, int w, int h) {
    SDL_Rect border;
    border.x = xpos;
    border.y = ypos;
    border.w = w;
    border.h = h;

    SDL_SetRenderDrawColor(r, 255,255,255,255);
    SDL_RenderDrawRect(r, &border);
    SDL_RenderFillRect(r, &border);
    SDL_SetRenderDrawColor(r, 0,0,0,255);
}

void generate_point(SDL_Renderer* r, int32_t x_c, int32_t y_c, int32_t radius) { //dont make radius const
    SDL_SetRenderDrawColor(r, 255,0,0,(0xff)-1);
    //important information: SDL does not provide automatic way to draw a circle or any curves
    //use the midpoint algorithm:
    const int32_t d = radius * 2;
    int32_t x = radius - 1;
    int32_t y = 0;
    int32_t dx = 1;
    int32_t dy = 1;
    int32_t error = (dx - (radius << 1)); //radius is 32bit -> represents in binary and shift bit left

    while(x >= y) {
        SDL_RenderDrawPoint(r, x_c + x, y_c + y);
        SDL_RenderDrawPoint(r, x_c + y, y_c + x);
        SDL_RenderDrawPoint(r, x_c - y, y_c + x);
        SDL_RenderDrawPoint(r, x_c - x, y_c + y);
        SDL_RenderDrawPoint(r, x_c - x, y_c - y);
        SDL_RenderDrawPoint(r, x_c - y, y_c - x);
        SDL_RenderDrawPoint(r, x_c + y, y_c - x);
        SDL_RenderDrawPoint(r, x_c + x, y_c - y);
        if(error <= 0) {
            y++;
            error += dy;
            dy += 2;
        }
        if(error > 0) {
            x--;
            dx += 2;
            error += (dx - d);
        }
    }
    //i know it looks ugly but truly this algorithm works in about 500ms
}

double generate_arrow(SDL_Renderer* r, float x1, float y1, float x2, float y2, bool crt, int* color) {
    float y_t = convert_coords(x2,y2).yn;
    int c = 20; //arbitrarily set magnitude of a line
    double phi = acos(x2 / (sqrt((pow(x2,2.0) + pow(y2,2.0)))) );
    float left = PI + (phi/2.0);
    float right = PI + (3.0*phi/2.0);
    
    // printf("phi is %f \n", phi);

    // SDL_SetRenderDrawColor(r, 0,255,0,255);
    render_color(r, color);
    SDL_RenderDrawLine(r,x1,convert_coords(x1,y1).yn,x2,y_t);
    // SDL_SetRenderDrawColor(r, 0,0,255,255);
    if(crt) {
        SDL_SetRenderDrawColor(r, 255,0,0,255);
        // render_color(r, color);
        SDL_RenderDrawLine(r,x2, y_t, (x2 + (c*cos(left))), (y_t - (c*sin(left))));
        SDL_RenderDrawLine(r,x2,y_t, x2 + (c*cos(right)), y_t - (c*sin(right)));
    }

    return phi;
}

void clear_scheme(SDL_Renderer* render) {
    SDL_SetRenderDrawColor(render, 0, 0,0, 255);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);
}

void move(float* x, float* y, float v) { //buffer, 0,0,10
    *x += v; *y += v;
    // buffer[0] = *x;
    // buffer[1] = *y;
}

int main(int argc, char* argv[]) {
    uint32_t flags = 0;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("eror init sdl");
        exit(1);
    }
    SDL_Window* win = SDL_CreateWindow("Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0); //turns out top left corner is origin (0,0)
    SDL_Renderer* render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(render,0,0,0,255);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);

    SDL_Surface* surface = SDL_LoadBMP("image.bmp");
    if(!surface)
        printf("not getting surface??");
    SDL_Texture* txt = SDL_CreateTextureFromSurface(render, surface);
    if(!txt)
        printf("erorr gettting texture");

    //matrix stuff 
    // float angle = PI;
    // struct matrix* m1 = new_matrix(2, 2, &(optlookup[0]), &angle);
    // struct matrix* m2 = new_matrix(2, 2, &(optlookup[1]), NULL);
    // confirm_transform2d(m1, m2);

    /*
    setup initial coordinate orientation relative of robot
    */
    SDL_Rect rect = {.x=200,.y=200,.h=40,.w=40};

    int g[3] =      {0x00,0xff,0x00};
    int p[3] =      {0xff,0xc0,0xcb};
    int orange[3] = {0xff,0xa5,0x00};

    SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(render);
    SDL_RenderCopyEx(render,txt,NULL,&rect,90,NULL,SDL_FLIP_NONE);
    // SDL_RenderCopy(render, txt, NULL, &rect);
    generate_arrow(render, 0,0,rect.x+(rect.w/2),(WINDOW_HEIGHT-(rect.y+(rect.h/2))), true, g);
    generate_arrow(render, rect.x+(rect.w/2), (WINDOW_HEIGHT-(rect.y+(rect.h/2))), rect.x+(rect.w/2), WINDOW_HEIGHT-(rect.y+(rect.h/2))+60, false, p); //y-axis
    generate_arrow(render, rect.x+(rect.w/2), (WINDOW_HEIGHT-(rect.y+(rect.h/2))), rect.x+60, (WINDOW_HEIGHT-(rect.y+(rect.h/2))), false, orange); //x-axis

    SDL_RenderPresent(render);

    static int angle=0;
    bool exit = false;
    while(!exit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                exit = true;
                break;
            
            default:
                break;
            }
        }
        
        // angle++;
        // SDL_Delay(1000/30);
    }
    // free(m1); //uncomment when running matrices computation
    SDL_DestroyTexture(txt);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    return 0;
}