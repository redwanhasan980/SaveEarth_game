
#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_image.h>
using namespace std;
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
bool is_collision = false;
bool start = false;
  bool is_genarate = true;
int radius = 30;
bool do_text=0;
int  t1 = 0,t2=0;
int ini[20] = {0};
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *textTexture;
TTF_Font *font;
  SDL_Surface *textSurface;
   SDL_Rect textRect;
   bool maingame=false;
bool gamerun = false;

int circle2_x = SCREEN_WIDTH / 2;
int circle2_y = radius;
int when_collid_c1y = 0;
int when_collid_c1x = 0;
int c1x[25] = {(-1) * radius};
SDL_Color color;
struct Particle
{
    int x, y;
    int speed;
};
struct star
{
    int x, y;
};

vector<Particle> particless;
vector<star> stars;
int num_particle = 200;
bool initializeWindow(void)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Error : SDL initialization failed." << endl
             << "SDL Error: " << SDL_GetError();
        return false;
    }
    window = SDL_CreateWindow("Save EARTH from Asteroid",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL)
    {
        cout << "Error: Failed to open window" << endl
             << "SDL Error: " << SDL_GetError();
        return false;
    }
    if (TTF_Init() != 0)
    {
        SDL_Log("TTF_Init Error: %s", TTF_GetError());
        SDL_Quit();
    }


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        cout << "Error: Failed to open renderer" << endl
             << "SDL Error: " << SDL_GetError();
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    return true;
}

void input(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (SDLK_RIGHT == event.key.keysym.sym)
                circle2_x += 5;
            else if (SDLK_LEFT == event.key.keysym.sym)
                circle2_x -= 5;
            else if (SDLK_UP == event.key.keysym.sym)
                circle2_y -= 5;
            else if (SDLK_DOWN == event.key.keysym.sym)
                circle2_y += 5;
        }
        if (event.type == SDL_KEYUP)
        {
            if (SDLK_f == event.key.keysym.sym)
            {
                    start = true,do_text=0;
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                     SDL_RenderClear(renderer);
            }
                
             else if (SDLK_r == event.key.keysym.sym)
             {
                 maingame=0,start = true,do_text=0;
                 circle2_x = SCREEN_WIDTH / 2;
                 circle2_y = radius;
                 for(int i=0;i<25;i++)
                   c1x[i]=(-1) * radius;
                   for(int i=0;i<20;i++)
                   ini[i]=0;
                   is_collision=false;
                  t1=0;t2=0;
                  particless.clear();
                   is_genarate = true;

                   
             }
                
            else if (SDLK_ESCAPE == event.key.keysym.sym)
                gamerun = false;
        }
        else if (event.type == SDL_QUIT)
        {
            gamerun = false;
            break;
        }
    }
}

void drawCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int i = x - radius; i <= x + radius; i++)
    {
        for (int j = y - radius; j <= y + radius; j++)
        {
            int distance = sqrt((i - x) * (i - x) + (j - y) * (j - y));
            if (distance <= radius)
            {
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
    SDL_RenderPresent(renderer);
}
void measure_collision(int xaxis, int y_axis_of1c)
{
    int distance;

    distance = (((c1x[xaxis] - circle2_x) * (c1x[xaxis] - circle2_x)) + ((y_axis_of1c - circle2_y) * (y_axis_of1c - circle2_y)));
    if (distance <= (50) * (50))
    {
        is_collision = true;
        when_collid_c1y = y_axis_of1c;
        when_collid_c1x = xaxis;
    }
}
void circle1(int x, int y, int i)
{
    color = {139, 69, 19};
    drawCircle(renderer, c1x[i], y, radius, color);
    c1x[i] += x;
    if (c1x[i] >= SCREEN_WIDTH + radius)
        c1x[i] = (-1) * radius;

    // SDL_Delay(1);
}
void circle2()
{
    color = {30, 144, 255};
    drawCircle(renderer, circle2_x, circle2_y, 20, color);
}
void clear_renderer(int x_coordinate, int y_coordinate, int radius)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = x_coordinate - radius - 5; i <= x_coordinate + radius + 5; i++)
    {
        for (int j = y_coordinate - radius - 5; j <= y_coordinate + radius + 5; j++)
        {
            int distance = sqrt((i - x_coordinate) * (i - x_coordinate) + (j - y_coordinate) * (j - y_coordinate));
            if (distance > radius)
            {
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
    SDL_RenderPresent(renderer);
}
void genarate_particle(int x, int y, int radius)
{
    for (int i = x - radius; i <= x + radius; i += 1)
    {
        for (int j = y + radius; j >= y - radius; j -= 1)
        {
            int distance = sqrt((i - x) * (i - x) + (j - y) * (j - y));
            if (distance <= radius)
            {

                Particle particle;
                particle.x = i;
                particle.y = j;
                particle.speed = rand() % 10 + 5;
                particless.push_back(particle);
            }
        }
    }
}

void genarate_stars()
{
    stars.clear();
    for (int i = 0; i < 200; i++)
    {
        star stare;
        stare.x = rand() % SCREEN_WIDTH;
        stare.y = rand() % SCREEN_HEIGHT;
        stars.push_back(stare);
    }
}
void draw_moving_circle(int speed, int y, int i)
{
    circle1(speed, y, i);
    clear_renderer(c1x[i], y, radius);
    measure_collision(i, y);
}
void drawText(char s[], char ss[], int x, int y, int size, SDL_Color textColor)
{
    
     font = TTF_OpenFont(ss, size);
    if (!font)
    {
        SDL_Log("TTF_OpenFont Error: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

    textSurface = TTF_RenderText_Solid(font, s, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
   
    textRect.x = x;
    textRect.y = y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

void destroyWindow(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int main(int argc, char *argv[])
{

  
    gamerun = initializeWindow();
    int time = 0;
    
    int first = 0;
    while (gamerun)
    {
        input();

        if (first == 0 && start == false&&do_text==0)
        {

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            drawText("SAVE EARTH", "zebulon/Zebulon Bold Italic.otf", 220, 200, 90, {255, 0, 0});
            drawText("To Start press F", "font/GothamMedium.ttf", 400, 350, 50, {255, 255, 255});
            drawText("To Quit press ESC", "font/GothamMedium.ttf", 400, 450, 50, {255, 255, 255});
            drawText("REDO GAMING", "font/SerpentineBoldItalic.ttf", 800, 600, 40, {255, 0, 0});
            drawText("©2023 REDO GAMING. All rights reserved.", "font/GothamMedium.ttf", 800, 650, 15, {255, 255, 255});
            SDL_RenderPresent(renderer);
            do_text=1;
            first=1;
        }
        else if( start == false&&do_text==1)
        SDL_RenderPresent(renderer);
        else if(start==true&&do_text==0&&maingame==0)
        {
            
             SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                drawText("SAVE THE EARTH !", "zebulon/Zebulon Bold Italic.otf", 100, 100, 70, {255, 255, 255});
                drawText("GO DOWN WITHOUT COLLIDING", "zebulon/Zebulon Bold Italic.otf", 70, 250, 50, {255, 255, 255});
                 drawText("WITH ASTEROID", "zebulon/Zebulon Bold Italic.otf", 70, 350, 50, {255, 255, 255});
                 drawText("Press F to start", "font/GothamMedium.ttf", 400, 500, 50, {255, 0, 0}); 
                  SDL_RenderPresent(renderer);
                do_text=1;
                maingame=1;
        }
         else if(do_text==1)
         {
           
           SDL_RenderPresent(renderer);
                
         }
       
        else if (start == true)
        {
            first=1;
             
            if (t1 > 1000)
                t1 = 0;

            if (t1 == 0 || t1 == 200 || t1 == 400 || t1 == 600 || t1 == 800)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                genarate_stars();
                for (auto &star : stars)
                {

                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawPoint(renderer, star.x, star.y);
                }
            }
            t1++;

            if (is_collision == false)
            {

                draw_moving_circle(1, 30, 20);
                draw_moving_circle(2, 30, 19);
                if (t1 >= 350 || ini[2] == 1)
                {
                    draw_moving_circle(1, 100, 18);
                    ini[2] = 1;
                }
                draw_moving_circle(2, 170, 17);
                if (t1 >= 200 || ini[0] == 1)
                {
                    draw_moving_circle(3, 170, 16);
                    ini[0] = 1;
                }
                draw_moving_circle(4, 290, 15);
                if (t1 >= 300 || ini[1] == 1)
                {
                    draw_moving_circle(2, 290, 14);
                    ini[1] = 1;
                }
                draw_moving_circle(1, 290, 13);
                draw_moving_circle(2, 360, 12);
                if (t1 >= 700 || ini[3] == 1)
                {
                    draw_moving_circle(2, 430, 11);
                    ini[3] = 1;
                }
                draw_moving_circle(3, 430, 10);
                draw_moving_circle(5, 430, 9);
                draw_moving_circle(5, 530, 8);
                draw_moving_circle(2, 530, 7);
                if (t1 >= 370 || ini[4] == 1)
                {
                    draw_moving_circle(3, 530, 6);
                    ini[4] = 1;
                }
                draw_moving_circle(2, 620, 5);
                if (t1 >= 520 || ini[6] == 1)
                {
                    draw_moving_circle(3, 620, 4);
                    ini[6] = 1;
                }
                draw_moving_circle(4, 620, 3);
                if (t1 >= 430 || ini[5] == 1)
                {
                    draw_moving_circle(5, 620, 2);
                    ini[5] = 1;
                }

                circle2();

                clear_renderer(circle2_x, circle2_y, 20);

                SDL_Delay(3);
            }
           //cout<<do_text<<endl;
            if (is_collision == true || circle2_y > 720)
            {
                start = false;
            }
        }
        else if (start == false)
        {    
            if (time >= (4 + (8 * when_collid_c1x)) && do_text==0)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                drawText("GAME OVER", "zebulon/Zebulon Bold Italic.otf", 220, 200, 100, {255, 0, 0});
                 drawText("To restart press r", "font/GothamMedium.ttf", 400, 350, 50, {255, 255, 255});
                drawText("To Quit press ESC", "font/GothamMedium.ttf", 400, 450, 50, {255, 255, 255});
                drawText("REDO GAMING", "font/SerpentineBoldItalic.ttf", 800, 600, 40, {255, 0, 0});
                drawText("©2023 REDO GAMING. All rights reserved.", "font/GothamMedium.ttf", 800, 650, 15, {255, 255, 255});
                drawText("Developed by: Redwan Hasan", "font/GothamMedium.ttf", 800, 680, 20, {255, 255, 255});
                SDL_RenderPresent(renderer);
               do_text =1;
               time=0;
            }
            else if (circle2_y > 715&&do_text==0)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                drawText("CONGRATULATION !!!", "zebulon/Zebulon Bold Italic.otf", 100, 100, 80, {255, 255, 255});
                drawText("YOU HAVE SAVED THE EARTH.", "zebulon/Zebulon Bold Italic.otf", 100, 250, 50, {255, 255, 255});
                drawText("YOU ARE A HERO.", "zebulon/Zebulon Bold Italic.otf", 100, 350, 80, {255, 255, 255});
                 drawText("To Quit press ESC", "font/GothamMedium.ttf", 400, 500, 50, {255, 255, 255});
                drawText("REDO GAMING", "font/SerpentineBoldItalic.ttf", 800, 600, 40, {255, 0, 0});
                drawText("©2023 REDO GAMING. All rights reserved.", "font/GothamMedium.ttf", 800, 650, 15, {255, 255, 255});
                drawText("Developed by: Redwan Hasan", "font/GothamMedium.ttf", 800, 680, 20, {255, 255, 255});
                SDL_RenderPresent(renderer);
                do_text=1;
            }
            else if(do_text==1)
            SDL_RenderPresent(renderer);
            
            else
            {
                if (is_genarate == true)
                {
                    genarate_particle(c1x[when_collid_c1x], when_collid_c1y, radius);
                    genarate_particle(circle2_x, circle2_y, 20);
                    is_genarate = false;
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                for (auto &particle : particless)
                {
                    if (particle.y < SCREEN_HEIGHT)
                        particle.y += particle.speed;
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawPoint(renderer, particle.x, particle.y);
                }
                time++;

                SDL_RenderPresent(renderer);
                SDL_Delay(15);
            }
        }
    }

    destroyWindow();
    return 0;
}