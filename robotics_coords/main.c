#include <stdio.h>
#include <math.h>
#include <stdbool.h>
//#include <kipr/wombat.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "matrix.h"


#define PI 3.141592653589793
#define WINDOW_HEIGHT 500
#define WINDOWN_WIDTH 700



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
int g[3] =      {0x00,0xff,0x00};
int p[3] =      {0xff,0xc0,0xcb};
int orange[3] = {0xff,0xa5,0x00};

void render_color(SDL_Renderer* r, int* c) {
    SDL_SetRenderDrawColor(r, c[0], c[1], c[2], 255);
}

void create_rect(SDL_Renderer* r, int xpos, int ypos, int w, int h) {
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

double* generate_arrow(SDL_Renderer* r, double* buffer,float x1, float y1, float x2, float y2, bool crt, int* color) {
    float y_t = convert_coords(x2,y2).yn;
    int c = 20; //arbitrarily set magnitude of a line
    double phi = acos(x2 / (sqrt((pow(x2,2.0) + pow(y2,2.0)))) );
    float left = PI + (phi/2.0);
    float right = PI + (3.0*phi/2.0);
    

    render_color(r, color);
    SDL_RenderDrawLine(r,x1,convert_coords(x1,y1).yn,x2,y_t);

    if(crt) {
        SDL_SetRenderDrawColor(r, 255,0,0,255);
        // render_color(r, color);
        SDL_RenderDrawLine(r,x2, y_t, (x2 + (c*cos(left))), (y_t - (c*sin(left))));
        SDL_RenderDrawLine(r,x2,y_t, x2 + (c*cos(right)), y_t - (c*sin(right)));
    }
    
    if(buffer != NULL) {
        buffer[0] = x1;
        buffer[1] = y1;
        buffer[2] = x2;
        buffer[3] = y2;
    }

    return buffer;
}
void nonorigin_rotate(SDL_Renderer* render, float angle, float x1, float y1, float curr_x, float curr_y) {
    double transform[4] = {0};
    generate_arrow(render, transform, x1, y1, curr_x, curr_y, true, g);
    //translate to origin
    transform[2] -= transform[0];
    transform[3] -= transform[1];
    //rotate
    float new_x = transform[2]*cos(angle) - transform[3]*sin(angle);
    float new_y = transform[2]*sin(angle) + transform[3]*cos(angle);

    transform[2] = new_x + transform[0];
    transform[3] = new_y + transform[1];

    generate_arrow(render, NULL, x1, y1, transform[2], transform[3], false, g);
}

void clear_scheme(SDL_Renderer* render) {
    SDL_SetRenderDrawColor(render, 0, 0,0, 255);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);
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
    SDL_Window* win = SDL_CreateWindow("Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWN_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL); //turns out top left corner is origin (0,0)
    SDL_Renderer* render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(render,0,0,0,255);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);

    SDL_Surface* surface = SDL_LoadBMP("shrek.bmp");
    SDL_Color white = {.r=255,.b=255,.g=255};
    if(!surface)
        printf("not getting surface??");
    SDL_Texture* txt = SDL_CreateTextureFromSurface(render, surface);
    if(!txt)
        printf("erorr gettting texture");

    SDL_Rect rect = {.x=20,.y=460,.h=40,.w=40};
    SDL_Rect msgrect = {.x=20,.y=50,.h=30,.w=120};
    
    SDL_RenderPresent(render);

    //setup points for targets
    // generate_point(render, 350, 250, 8);

    static int angle=0;

    bool exit = false;
    printf("ah yes, the path to the swamp has been found\n");
    while(!exit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                exit = true;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_UP) {
                    rect.y -= 2;
                }
                if(event.key.keysym.sym == SDLK_DOWN) {
                    rect.y += 2;
                }
                if(event.key.keysym.sym == SDLK_LEFT) {
                    rect.x -= 2;
                }
                if(event.key.keysym.sym == SDLK_RIGHT) {
                    rect.x += 2;
                }
                break;
            case SDL_KEYUP:
                break;
            default:
                break;
            }
        }
        float curr_y = WINDOW_HEIGHT-(rect.y+(rect.h/2)); //center of square with respect to origin bl
        float curr_x = rect.x+(rect.w/2); //center of square 

        printf("distance to target 1: %f where currx is %f and curry is %f\n", (sqrt(pow(660-curr_x,2)+pow(240-curr_y,2))), curr_x, curr_y);
        //below works
        // SDL_SetRenderDrawColor(render,0x00,0x00,0x00,0xff);
        // SDL_RenderClear(render);
        // SDL_RenderCopyEx(render, txt, NULL, &rect, -angle, NULL, SDL_FLIP_NONE);
        // nonorigin_rotate(render, (PI*angle)/180, curr_x,curr_y, curr_x,curr_y+60);
        // SDL_RenderPresent(render);
        // angle+=30;
        // SDL_Delay(1000/30);
        SDL_SetRenderDrawColor(render,0,0,0,255);
        SDL_RenderClear(render);
        //initials
        SDL_RenderCopy(render, txt, NULL, &rect);
        
        
        //targets for example
        create_rect(render, 650, 250, 20,20);
        create_rect(render, 650, 450, 20,20);
        create_rect(render, 650, 50, 20,20);

        //draw optimal path vectors
        generate_arrow(render,NULL,curr_x,curr_y,650+(10),250-(10),true,g);
        generate_arrow(render,NULL,curr_x,curr_y,650+10,450-10,true,g);
        generate_arrow(render,NULL,curr_x,curr_y,650+10,50-10,true,g);

        SDL_RenderPresent(render);

        SDL_Delay(1000/30);

    }
    // free(m1); //uncomment when running matrices computation
    SDL_DestroyTexture(txt);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

/*
important dimensional information:

width: 47.124 something
length: 92.25 something


*/