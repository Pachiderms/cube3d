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
# include <stdlib.h>
# include <fcntl.h>
# include "minilibx-linux/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <time.h>

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
# define yellow 0xFFFF00

# define mapWidth 24
# define mapHeight 24
# define screenWidth 640
# define screenHeight 480

# define texWidth 64
# define texHeight 64

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

void drawLine(int beginX, int beginY, int endX, int endY, int color)
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
    int w = screenWidth;
    int h = screenHeight;
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

        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);

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
            sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
        }
        if (rayDirY < 0){
            stepY = -1;
            sideDistY = (player.posY - mapY) * deltaDistY;
        }
        else{
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
        }

        while (hit == 0)
        {
            if (sideDistX < sideDistY){
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else{
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (worldMap[mapX][mapY] > 0)
                hit = 1;
        }

        if (side == 0)
            perpWallDist = sideDistX - deltaDistX;
        else
            perpWallDist = sideDistY - deltaDistY;

        // if (perpWallDist == 0)
        //     perpWallDist = 1;

        int lineHeight = (int)(h / perpWallDist);
        int drawStart = -lineHeight / 2 + h / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + h / 2;
        if (drawEnd >= h)
            drawEnd = h - 1;

        int color;
        switch(worldMap[mapX][mapY])
        {
            case 1:  color = red;  break;
            case 2:  color = green;  break;
            case 3:  color = blue;   break;
            case 4:  color = white;  break;
            default: color = yellow; break;
        }

        if (side == 1)
            color = color / 2;

        drawLine(x, drawStart, x, drawEnd, color);
    }

    mlx_clear_window(mlx, win);
}

int display()
{
    raycast();

    return 0;
}

int keyPress(int key)
{
    float rotSpeed = 0.1;
    if (key == KEY_ESC){
        mlx_destroy_window(mlx, win);
        return 0;
    }

    if (key == KEY_W || key == KEY_Z){
        if(worldMap[(int)(player.posX + player.dirX)][(int)(player.posY)] == 0) player.posX += player.dirX;
        if(worldMap[(int)(player.posX)][(int)(player.posY + player.dirY)] == 0) player.posY += player.dirY;
    }
    if(key == KEY_S){
        if(worldMap[(int)(player.posX - player.dirX)][(int)(player.posY)] == 0) player.posX -= player.dirX;
        if(worldMap[(int)(player.posX)][(int)(player.posY - player.dirY)] == 0) player.posY -= player.dirY;
    }
    if (key == KEY_A || key == KEY_Q)
        player.posY -= player.dirY * 1.5;
    if (key == KEY_D)
        player.posY += player.dirY * 1.5;
    if(key == KEY_RIGHT){
        double oldDirX = player.dirX;
        player.dirX = player.dirX * cos(-rotSpeed) - player.dirY * sin(-rotSpeed);
        player.dirY = oldDirX * sin(-rotSpeed) + player.dirY * cos(-rotSpeed);
        double oldPlaneX = player.planeX;
        player.planeX = player.planeX * cos(-rotSpeed) - player.planeY * sin(-rotSpeed);
        player.planeY = oldPlaneX * sin(-rotSpeed) + player.planeY * cos(-rotSpeed);
    }
    if(key == KEY_LEFT){
        double oldDirX = player.dirX;
        player.dirX = player.dirX * cos(rotSpeed) - player.dirY * sin(rotSpeed);
        player.dirY = oldDirX * sin(rotSpeed) + player.dirY * cos(rotSpeed);
        double oldPlaneX = player.planeX;
        player.planeX = player.planeX * cos(rotSpeed) - player.planeY * sin(rotSpeed);
        player.planeY = oldPlaneX * sin(rotSpeed) + player.planeY * cos(rotSpeed);
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
    player.planeY = 0.66;
    player.fov = (2 * atan(player.planeY/fabs(player.dirX))) * 180 / M_PI;
   
    mlx = mlx_init();
    win =  mlx_new_window(mlx, screenWidth, screenHeight, "test_raycasting");
    
    mlx_loop_hook(mlx, display, NULL);
    mlx_hook(win, KeyPress, KeyPressMask, keyPress, NULL);
    mlx_loop(mlx);

    return 0;
}