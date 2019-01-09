// Karissa Brickey
// 350 final project
// 11-20-18
// Collision detection with quad trees. 
// PRESS SPACEBAR while program is running to perform "standard" O(n^2) collision detection.
// Let up on spacebar to return to improved quadtree collision detection.

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"
#include "GameObject.h"
#include "QuadtreeNode.h"

/****************************************************************************/
const int NUM_CIRCLES = 1000;


void draw_boundaries(QuadtreeNode * p)
{
    p->draw_boundaries();
    if(p->is_leaf()) return;
    else
    {
        for(int i = 0; i < 4; ++i)
        {
            draw_boundaries(p->children(i));
        }
    }
}

void draw_game_objects(std::vector< GameObject * > & circles)
{
    for(int i = 0; i < NUM_CIRCLES; ++i)
    {
        circles[i]->draw();
    }
}

bool are_colliding(std::vector< GameObject * > circles, int i, int j)
{
    int x_diff = abs(circles[i]->get_x() - circles[j]->get_x());
    int y_diff = abs(circles[i]->get_y() - circles[j]->get_y());
    int distance = sqrt(pow(x_diff, 2) + pow(y_diff, 2));
    int radius = circles[i]->get_radius();
                
    if(i != j && distance <= radius * 2) return true;
    else return false;
}

void make_gameobject_changes(GameObject * & p, GameObject * & q)
{
    q->toggle_color();
    p->toggle_color();                    
    q->toggle_direction();
    p->toggle_direction();
    q->move();
    p->move();  
}

void get_parent_collisions(QuadtreeNode * p)
{
    QuadtreeNode * q = p->parent();
    for(int i = 0; i < q->num_circles(); ++i)
    {
        for(int j = 0; j < p->num_circles(); ++j)
        {
            int x_diff = abs(q->circles(i)->get_x() -
                             p->circles(j)->get_x());
            int y_diff = abs(q->circles(i)->get_y() -
                             p->circles(j)->get_y());
            int distance = sqrt(pow(x_diff, 2) + pow(y_diff, 2));
            int radius = q->circles(i)->get_radius();
                
            if(distance <= radius * 2)
            {
                make_gameobject_changes(q->circles(i), p->circles(j));   
            }
        }
    }
}

void get_quadtree_collisions(QuadtreeNode * p)
{
    if(p->is_leaf())
    {
        for(int i = 0; i < p->num_circles(); ++i)
        {
            for(int j = 0; j < p->num_circles(); ++j)
            {   
                int x_diff = abs(p->circles(i)->get_x() -
                                 p->circles(j)->get_x());
                int y_diff = abs(p->circles(i)->get_y() -
                                 p->circles(j)->get_y());
                int distance = sqrt(pow(x_diff, 2) + pow(y_diff, 2));
                int radius = p->circles(i)->get_radius();
                
                if(i != j && distance <= radius * 2)
                {
                    make_gameobject_changes(p->circles(i), p->circles(j));
                }
            }
            if(p->parent_contains_objects())
            {
                get_parent_collisions(p);
            }
        }
        return;
    }
    else
    {
        for(int i = 0; i < 4; ++i)
        {
            get_quadtree_collisions(p->children(i));
        }
    }
}

void get_collisions(std::vector< GameObject * > & circles, bool use_quadtrees,
                     QuadtreeNode * root)
{
    if(use_quadtrees)
    {
        get_quadtree_collisions(root);   
    }
    else
    {
        for(int i = 0; i < NUM_CIRCLES; ++i)
        {
            for(int j = 0; j < NUM_CIRCLES; ++j)
            {
                if(are_colliding(circles, i, j))
                {
                    make_gameobject_changes(circles[i], circles[j]);
                }
            }
        }
    }
}


/****************************************************************************/
int main(int argc, char* argv[])
{
    Surface surface(W, H);
    Event event;
    bool use_quadtrees = true;

    std::vector< GameObject * > circles;
    for(int i = 0; i < NUM_CIRCLES; ++i)
    {
        GameObject * p = new GameObject(surface);
        circles.push_back(p);
    }

    while(1)
    {
        // hold space to run without using quadtrees- let go to use quadtrees
        if (event.poll() && event.type() == QUIT) break;
        else if (event.type() == KEYDOWN)
        {
            use_quadtrees = false;
        }
        else if (event.type() == KEYUP)
        {
            use_quadtrees = true;
        }
        
        //move circles
        for(int i = 0; i < circles.size(); ++i)
        {
            circles[i]->move();
        }

        // create new quad tree at each iteration
        QuadtreeNode * root = new QuadtreeNode(0, 0, W - 1, H - 1, circles,
                                               surface, NULL);
        root->create_quadtree(); 

        // collision detection
        get_collisions(circles, use_quadtrees, root);
        
        //Moving and drawing
        surface.lock();
        surface.fill(BLACK);
        draw_game_objects(circles);
        if(use_quadtrees) draw_boundaries(root);
        surface.unlock();
        surface.flip();
        delay(10);
    }

    return 0;
}

