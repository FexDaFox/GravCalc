#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <cstdio>
using namespace std;
#define GC 5

int die(SDL_Window *window,SDL_Renderer *renderer){ //cleans up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;}
void setat(SDL_Renderer *renderer,int x,int y,int r,int g,int b){ //draws points
    SDL_SetRenderDrawColor(renderer,r,g,b,0);
    SDL_RenderDrawPoint(renderer,300+x,300-y);}
void savescreen(SDL_Renderer *renderer){ //save window image
    SDL_Surface *shot = SDL_CreateRGBSurface(0,600,600,32,0x00ff0000,0x0000ff00,0x000000ff,0xff000000);
    SDL_RenderReadPixels(renderer,NULL,SDL_PIXELFORMAT_ARGB8888,shot->pixels,shot->pitch);
    char buff[50];
    sprintf(buff,"gravcalc %d.bmp",time(0));
    SDL_SaveBMP(shot,buff);
    SDL_FreeSurface(shot);}

struct prt{
    float x;
    float y;
    float m=1;};

int main(){
srand(time(0));
float torad = 3.141592653589793/180;
//initialize sdl window
SDL_Init(SDL_INIT_VIDEO);
SDL_Window *window = SDL_CreateWindow("GravCalc",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,600,600,SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
SDL_SetRenderDrawColor(renderer,0,0,0,0);
SDL_RenderClear(renderer);
SDL_Event event;
//define object
vector<prt> points;
prt point;
//----Reserved for point creation----//
point.m = 5;
for(int x=0;x<100;x++) for(int y=0;y<100;y++){
    point.x=x; point.y=y;
    points.push_back(point);
}
for(int x=100;x<120;x++) for(int y=100;y<120;y++){
    point.x=x; point.y=y;
    points.push_back(point);
}
//-----------------------------------//
//center object
float xc=0,yc=0,m=0;
for(int i=0;i<points.size();i++){
    point = points.at(i);
    xc += point.x*point.m;
    yc += point.y*point.m;
    m += point.m;}
xc /= m; yc /= m;
for(int i=0;i<points.size();i++){
    points.at(i).x -= xc;
    points.at(i).y -= yc;}
for(int i=0;i<points.size();i++) setat(renderer,points.at(i).x,points.at(i).y,255,255,255); //display points
//do physics
float px,py,forcex,forcey,dist,force,force2,angg;
char winname[50];
for(px=-300;px<301;px++){
    while(SDL_PollEvent(&event)){ //handle input events
        if(event.type == SDL_QUIT) return die(window,renderer);}
    for(py=-300;py<301;py++){
        force2=0; //forcex=0; forcey=0;
        for(int i=0;i<points.size();i++){
            point = points.at(i);
            //calculate distance
            dist = sqrt(pow(point.x-px,2)+pow(point.y-py,2));
            if(dist == 0) dist=0.0001;
            //calculate force
            force = (GC*point.m)/dist;
            force2 += force;
            //split force into 2d vectors
            //angg = atan2(py-point.y,px-point.x);
            //forcex += abs(force*cos(angg));
            //forcey += abs(force*sin(angg));
        }
        setat(renderer,px,py,force2,force2,force2);
    }
    SDL_RenderPresent(renderer);
    sprintf(winname,"GravCalc %d",int(px+300)/6);
    SDL_SetWindowTitle(window,winname);
}
savescreen(renderer); //save screen
for(int i=0;i<points.size();i++) setat(renderer,points.at(i).x,points.at(i).y,255,255,255); //display points
SDL_Delay(2000);
savescreen(renderer);
//keep screen alive and display image
while(true){
    while(SDL_PollEvent(&event)){ //handle input events
        if(event.type == SDL_QUIT) return die(window,renderer);
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) return die(window,renderer); }
    SDL_Delay(100);
}

}
