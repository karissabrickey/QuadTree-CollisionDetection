
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>


class GameObject
{
public:
GameObject(Surface & surface)
    : surface(surface)
    {
        x = rand() % W;
        y = rand() % H;
           
       
        radius = 3; 
        dx = 3;
        dy = 3;
        color1 = true;

        // Color 1 (true)
        // RED = 255, 0, 0                   PURPLE = 87, 0, 128
        // MAGENTA = 255, 0, 255             TEAL = 0, 247, 247
        // BLUE = 0, 0, 255                  ORANGE = 255, 89, 0
        // GREEN = 0, 255, 0
        r1 = 255;
        g1 = 89;
        b1 = 0;

        // Color 2 (false)
        r2 = 87;
        g2 = 0;
        b2 = 128;
    }

    void draw()
    {
        if(color1) surface.put_circle(x, y, radius, r1, g1, b1);
        else surface.put_circle(x, y, radius, r2, g2, b2);
    }

    void move()
    {   
        x += dx;
        if (x < 0)
        {
            x = 0;
            dx = -dx;
        }
        else if (x + radius * 2 > W - 1)
        {
            x = W - 1 - radius * 2;
            dx = -dx;
        }
        
        y += dy;
        if (y < 0)
        {
            y = 0;
            dy = -dy;
        }
        else if (y + radius * 2 > H - 1)
        {
            y = H - 1 - radius * 2;
            dy = -dy;
        }
    }
    
    bool get_color()
    {
        return color1;
    }

    void toggle_color()
    {
        color1 = (color1 ? false : true);
    }

    void toggle_direction()
    {
        dx = -dx;
        dy = -dy;
    }
    
    int get_x()
    {
        return x;
    }

    int get_y()
    {
        return y;
    }

    int get_radius()
    {
        return radius;
    }
    
private:
    int x, y, radius, dx, dy, r1, g1, b1, r2, g2, b2;
    bool color1;
    Surface & surface;
};


#endif

