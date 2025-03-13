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

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

double radToDeg(double rad) {return (rad* 180 / M_PI);}

float dist(double rayDirX, double rayDirY)
{
    return (sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY)));
}

typedef struct s_player
{
    double posX;
    double posY;

    double dirX;
    double dirY;

    double planeX;
    double planeY;

    double fov;
} t_player;

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
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

void    raycast()
{
    int done = 0;
    int w = mapWidth;
    for (int x=0; x < w; x++)
    {
        double rx = player.posX;
        double ry = player.posY;

        double cameraX = 2 * x / (double)w - 1;
        double rayDirX = player.dirX + player.planeX * cameraX;
        double rayDirY = player.dirY + player.planeY * cameraX;

        int mapX = (int)player.posX;
        int mapY = (int)player.posY;

        double sideDistX;
        double sideDistY;

        double deltaDistX = abs(1 / rayDirX);
        double deltaDistY = abs(1 / rayDirY);
        double perpWallDist;

        int stepX;
        int stepY;

        int hit = 0;
        int side;

        if (rayDirX < 0){
            stepX = -1;
            sideDistX = (player.posX - mapX) * deltaDistX;
        }
        else{
            stepX = 1;
            sideDistX = (mapX + 1) * deltaDistX;
        }
        if (rayDirY < 0){
            sideDistX = (player.posY - mapY) * deltaDistY;
            stepY = -1;
        }
        else{
            stepY = 1;
            sideDistX = (mapY + 1) * deltaDistY;
        }

        while (condition)
        {
            /* code */
        }
    }    
}

int display()
{
    raycast();
    //render_player();
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
    player.posX = 22;
    player.posY = 12;
    player.dirX = -1;
    player.dirY = 0;
    player.planeX = 0;
    player.planeY = 0.6;
    player.fov = 2 * atan(player.planeY/player.dirX);
   
    mlx = mlx_init();
    win =  mlx_new_window(mlx, screenWidth, screenHeight, "test_raycasting");

    
    mlx_loop_hook(mlx, display, NULL);
    mlx_hook(win, KeyPress, KeyPressMask, keyPress, NULL);
    mlx_loop(mlx);

    return 0;
}