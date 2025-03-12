/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:08:36 by marvin            #+#    #+#             */
/*   Updated: 2025/03/10 15:08:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include "minilibx-linux/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>

//M_PI pour pi

# define KEY_Z				122
# define KEY_Q				113
# define KEY_W				119
# define KEY_A				97
# define KEY_S				115
# define KEY_D				100

# define KEY_UP  			65362
# define KEY_LEFT  			65361
# define KEY_RIGHT 			65363
# define KEY_DOWN  			65364

# define KEY_ESC  			65307

# define GREEN				"\033[0;32m"
# define RED 				"\033[1;31m"
# define GREY 				"\033[0;90m"
# define CYAN				"\033[1;96m"
# define RESET 				"\033[0m"

# define white 0xFFFFFF
# define red   0x00FF0000
# define green 0x0000FF00
# define blue  0x000000FF
# define black 000000

#define PI2 M_PI/2
# define PI3 3*M_PI/2
# define DEG_TO_RAD 0.0174533

float dist(float ax, float ay, float bx, float by, float ang)
{
    return (sqrt((bx-ax) * (bx-ax) + (by-ay) * (by-ay)));
}

typedef struct s_player
{
    double x;
    double y;

    double dx;
    double dy;

    double pa;
} t_player;

int mapx=8;
int mapy=8;
int maps=64;

int map[] =
{
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 1, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};

void *mlx;
void *win;

t_player player;

void drawRect(int x, int y, int color, int size)
{
    int i = 0;
    int j;
    while (i<mapy*size)
    {
        j = 0;
        while (j<mapx*size)
        {
            mlx_pixel_put(mlx, win, x+j++, y+i, color);
        }
        i++;
    }
}

void drawLine(double beginX, double beginY, double endX, double endY, int color)
{
    double deltaX = endX - beginX;
    double deltaY = endY - beginY;

    double pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));

    deltaX /= pixels;
    deltaY /= pixels;

    double pixelsX = beginX;
    double pixelsY = beginY;
 
    for(int i=0; i<pixels; i++){
        mlx_pixel_put(mlx, win, pixelsX, pixelsY, color);

        pixelsX+=deltaX;
        pixelsY+=deltaY;
        pixels--;
    }
}

void drawRay()
{
    int r;
    int mx;
    int my;
    int mp;
    int dof=0;
    float rx;
    float ry;
    float ra;
    float xo;
    float yo;
    float disT;

    ra = -player.pa-DEG_TO_RAD*30;
    if (ra < 0)
        ra += 2*M_PI;
    if (ra > 2*M_PI)
        ra -= 2*M_PI;
    for(r=0; r<60;r++)
    {
        dof = 0;
        float disH=1000000;
        float hx = player.x;
        float hy = player.y;
        float aTan=-1/tan(ra);
        if (ra > M_PI) //looking down
        {
            ry = (int)player.y - 0.0001;
            rx = (player.y - ry) * aTan + player.x;
            yo = -64;
            xo = -yo * aTan;
        }
        if (ra < M_PI) //looking up
        {
            ry = (int)player.y + 64;
            rx = (player.y - ry) * aTan + player.x;
            yo = 64;
            xo = -yo * aTan;
        }
        if (ra == 0 || ra == M_PI) /// looking left or right
        {
            rx = player.x;
            ry = player.y;
            dof = 8;
        }
        while (dof < 8)
        {
            mx = (int) (rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapx + mx;
            if (mp > 0 && mp < mapx * mapy && map[mp] == 1)
            {
                hx = rx;
                hy = ry;
                disH = dist(player.x, player.y, hx, hy, ra);
                dof = 8;
            }
            else // next step adding offset on x and y
            {
                rx += xo;
                ry += yo;
                dof++;
            }
        }
        //drawLine(player.x, player.y, rx, ry, red);

        dof = 0;
        float disV=1000000;
        float vx = player.x;
        float vy = player.y;
        float nTan=-tan(ra);
        if (ra > PI2 && ra < PI3) //looking left
        {
            rx = (int)player.x - 0.0001;
            ry = (player.x - rx) * nTan + player.y;
            xo = -64;
            yo = -xo * nTan;
        }
        if (ra > PI2 || ra < PI3) //looking right
        {
            rx = (int)player.x + 64;
            ry = (player.x - rx) * nTan + player.y;
            xo = 64;
            yo = -xo * nTan;
        }
        if (ra == 0 || ra == M_PI) /// looking left or right
        {
            rx = player.x;
            ry = player.y;
            dof = 8;
        }
        while (dof < 8)
        {
            mx = (int) (rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapx + mx;
            if (mp > 0 && mp < mapx * mapy && map[mp] == 1)
            {
                vx = rx;
                vy = ry;
                disV = dist(player.x, player.y, vx, vy, ra);
                dof = 8;
            }
            else // next step adding offset on x and y
            {
                rx += xo;
                ry += yo;
                dof++;
            }
        }
        if (disV < disH)
        {
            disT = disV;
            rx = vx;
            ry = vy;
        }
        if (disH > disV)
        {
            disT = disH;
            rx = hx;
            ry = hy;
        }
        //drawLine(player.x, player.y, rx, ry, green);
        float ca = -player.pa - ra; // fisheye fix
        if (ca < 0)
            ca += 2*M_PI;
        if (ca > 2*M_PI)
            ca -= 2*M_PI;
        float lineH = (maps*320/disT); // clalc line Height
        if (lineH>320)
            lineH=320;
        float lineO = 160 - lineH/2; // calc line offset
        drawRect(r*8+530, lineH+lineO, red, 8);
        ra += DEG_TO_RAD;
        if (ra < 0)
        ra += 2*M_PI;
        if (ra > 2*M_PI)
            ra -= 2*M_PI;
    }
}

void drawMap2D()
{
    int x, y, xo, yo;
    int color;

    for(y=0;y<mapy;y++)
    {
        for(x=0;x<mapx;x++)
        {
            if (map[y*mapx+x]==1) {color = white;}
            else {color = black;}
            xo = x*maps;
            yo = y*maps;
            drawRect(xo, yo, color, 8);
        }
    }
    
}

void render_player()
{
    drawRect(player.x, player.y, red, 2);
    drawRay();
}

int display()
{
    drawMap2D();
    render_player();

    return 0;
}

int keyPress(int key)
{
    if (key == KEY_ESC){
        mlx_destroy_window(mlx, win);
        return 0;
    }

    mlx_pixel_put(mlx, win, player.x, player.y, black);
    if (key == KEY_W || key == KEY_Z){
        player.x-=player.dy;
        player.y-=player.dx;
    }else if(key == KEY_D){
        player.pa-=0.1;
        if (player.pa<0)
        {
            player.pa+=2*M_PI;
        }
        player.dx=cos(player.pa)*5;
        player.dy=sin(player.pa)*5;
    }else if(key == KEY_S){
        player.x+=player.dy;
        player.y+=player.dx;
    }else if(key == KEY_Q || key == KEY_A){
        player.pa+=0.1;
        if (player.pa>2*M_PI)
        {
            player.pa-=2*M_PI;
        }
        player.dx=cos(player.pa)*5;
        player.dy=sin(player.pa)*5;
    }
    return 0;
}

int main(int argc, char **argv)
{
    player.x = 300;
    player.y = 300;
    player.pa = PI2;
    player.dx=cos(player.pa)*5;
    player.dy=sin(player.pa)*5;
   
    mlx = mlx_init();
    win =  mlx_new_window(mlx, 1024, 512, "test_raycasting");

    
    mlx_loop_hook(mlx, display, NULL);
    mlx_hook(win, KeyPress, KeyPressMask, keyPress, NULL);
    mlx_loop(mlx);

    return 0;
}