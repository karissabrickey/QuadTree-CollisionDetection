#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <iostream>
#include <vector>
#include "GameObject.h"
#include "Surface.h"

const int threshold = 5;
const int maxdepth = 5;

class QuadtreeNode
{
public:
QuadtreeNode(int xmin, int ymin, int xmax, int ymax,
             std::vector< GameObject * > & circles, Surface & surface,
             QuadtreeNode * parent=NULL)
    : xmin_(xmin), ymin_(ymin), xmax_(xmax), ymax_(ymax), parent_(parent),
      surface_(surface)
    {
        for(int i = 0; i < circles.size(); ++i)
        {
            circles_.push_back(circles[i]);
        }
    }
    
    void create_quadtree()
    {
        recursive_create(this);
    }

    void recursive_create(QuadtreeNode * p)
    {   
        if(p->num_circles() > threshold && p->depth() < maxdepth)
        {
            p->create_children();
            p->distribute_children(p);
    
            for(int i = 0; i < 4; ++i)
            {
                recursive_create(p->children(i));
            }      
        }
        else return;
    }

    void create_children()
    {
        children_.push_back(new QuadtreeNode(xmin_, ymin_, (xmin_ + xmax_) / 2,
                                             (ymin_ + ymax_) / 2, circles_, surface_,
                                             this));
        children_.push_back(new QuadtreeNode((xmin_ + xmax_) / 2, ymin_, xmax_,
                                             (ymin_ + ymax_) / 2, circles_, surface_,
                                             this));
        children_.push_back(new QuadtreeNode(xmin_, (ymin_ + ymax_) / 2,
                                        (xmin_ + xmax_) / 2, ymax_,
                                             circles_, surface_, this));
        children_.push_back(new QuadtreeNode((xmin_ + xmax_) / 2,
                                        (ymin_ + ymax_) / 2, xmax_, ymax_,
                                             circles_, surface_, this));   
        children_[0]->clear_circles();
        children_[1]->clear_circles();
        children_[2]->clear_circles();
        children_[3]->clear_circles();
    }

    
    void distribute_children(QuadtreeNode * p)
    {
        std::vector< GameObject * > temp;        
        for(int i = p->num_circles() - 1; i > -1; --i)
        {
            GameObject * x = p->circles(i);
            p->pop_back_circle();

            if(x->get_x() > p->children(0)->xmin() && 
               x->get_x() < p->children(0)->xmax() &&
               x->get_y() > p->children(0)->ymin() &&
               x->get_y() < p->children(0)->ymax())
            {
                p->children(0)->insert_circle(x);
            }
            else if(x->get_x() > p->children(1)->xmin() &&
                    x->get_x() < p->children(1)->xmax() &&
                    x->get_y() > p->children(1)->ymin() &&
                    x->get_y() < p->children(1)->ymax())
            {       
                p->children(1)->insert_circle(x);
            }
            else if(x->get_x() > p->children(2)->xmin() &&
                    x->get_x() < p->children(2)->xmax() &&
                    x->get_y() > p->children(2)->ymin() &&
                    x->get_y() < p->children(2)->ymax())
            {       
                p->children(2)->insert_circle(x);
            }
            else if(x->get_x() > p->children(3)->xmin() &&
                    x->get_x() < p->children(3)->xmax() &&
                    x->get_y() > p->children(3)->ymin() &&
                    x->get_y() < p->children(3)->ymax())
            {   
                p->children(3)->insert_circle(x);
            }
            else temp.push_back(x);
        }

        // put cirlces that stay in parent back into parent from temp
        for(int i = 0; i < temp.size(); ++i)
        {
            p->insert_circle(temp[i]);
        }
    }

    
     void draw_boundaries()
    {
        // White = 255, 255, 255, green = 0, 255, 0
        surface_.put_line(xmin_, ymin_, xmax_, ymin_, 255, 255, 255);
        surface_.put_line(xmin_, ymax_, xmax_, ymax_, 255, 255, 255);
        surface_.put_line(xmin_, ymin_, xmin_, ymax_, 255, 255, 255);
        surface_.put_line(xmax_, ymin_, xmax_, ymax_, 255, 255, 255);
    }

     
    // Getters/ Setters
    const int xmin() const { return xmin_; }
    const int ymin() const { return ymin_; }
    const int xmax() const { return xmax_; }
    const int ymax() const { return ymax_; }
    const int num_circles() const { return circles_.size(); }
    const int num_children() const { return children_.size(); }
    const QuadtreeNode * const parent() const { return parent_; }
    std::vector< GameObject * > circles() { return circles_; }
    QuadtreeNode *& parent() { return parent_; }


    // Booleans
    const bool is_root() const { return (parent_ == NULL); }
    const bool is_leaf() const
    {
        return (children_.size() > 0 ? false : true);
    }
    const bool has_children() const { return !this->is_leaf(); }
    const bool parent_contains_objects() const
    {
        if(!this->is_root() && parent_->contains_objects()) return true;
        return false;
    }
    const bool contains_objects() const
    {
        if(circles_[0] != NULL && circles_[0] != 0) return true;
        return false;
    }
    

    // Others
    QuadtreeNode * root()
    {
        QuadtreeNode * p = this;
        while(p->parent() != NULL)
        {
            p = p->parent();
        }
        return p;
    }

    int depth()
    {
        int count = 0;
        find_depth(count, this);
        return count;
    }
    
    void find_depth(int & count, const QuadtreeNode * const p)
    {
        if(p->parent() == NULL) return;
        else
        {
            ++count;
            return find_depth(count, p->parent());
        }
    }


    // return pointer to Game Objects
    const GameObject * circles(int index) const // rvalue cannot be changed
    {
        if(circles_.size() <= index) return NULL;
        else return circles_[index];
    }
    
    GameObject *& circles(int index) //lvalue can be changed
    {
        if(circles_.size() <= index) circles_.resize(index + 1);
        return circles_[index];
    }

        
    // return pointer to children
    const QuadtreeNode * children(int index) const // rvalue cannot be changed
    {
        if(children_.size() <= index) return NULL;
        else return children_[index];
    }
    
    QuadtreeNode *& children(int index) //lvalue can be changed
    {
        if(children_.size() <= index) children_.resize(index + 1);
        return children_[index];
    }

    
    // Circle-related
    void insert_circle(GameObject * c)
    {
        circles_.push_back(c);
    }
    void remove_circle(const int index)
    {
        circles_.erase(circles_.begin()+index);
    }
    void clear_circles()
    {
        circles_.clear();
    }
    void pop_back_circle()
    {
        circles_.pop_back();
    }

    
    // Child-related
    void insert_child(const int index, const int xmin, const int ymin,
                      const int xmax, const int ymax)
    {
        QuadtreeNode * p = new QuadtreeNode(xmin, ymin, xmax, ymax,
                                            circles_, surface_, this);
        if(this->children(index) != 0 || this->children(index) != NULL
            || index < 0 || index > 3)
        {
            std::cout << "Invalid insertion of child" << std::endl;
        }
        else if(index > children_.size() - 1)
        {
            for(int i = children_.size() - 1; i < index; ++i)
            {
                this->children(i) = NULL;
            }
            this->children(index) = p;
        }
        else
        {
            this->children(index) = p;
        }
    }
    void remove_child(const int index) // remove the subtree at child i.
    {
        remove_subsequent(this->children(index));
        this->children(index) = NULL;
    }
    void remove_subsequent(const QuadtreeNode * p)
    {
        if(p->is_leaf())
        {
            delete p; return;
        }
        else
        {
            for(int i = 0; i < p->num_children(); ++i)
            {
                remove_subsequent(p->children(i));
            }
            delete p;
        }
    }
    
    
    // delete entire tree
    ~QuadtreeNode()
    {
        delete_quadtree(this);
    }
    
    void delete_quadtree(QuadtreeNode * p)
    {
        if(p->is_leaf())
        {
            delete p; return;
        }
        else
        {
            for(int i = 0; i < p->num_children(); ++i)
            {
                delete_quadtree(p->children(i));
            }
            delete p;
        }
    }

    
private:
    int xmin_, ymin_, xmax_, ymax_;
    Surface & surface_;
    QuadtreeNode * parent_; // parent node
    std::vector< GameObject * > circles_; // keys
    std::vector< QuadtreeNode * > children_; // max 4 children
};


std::ostream & operator<< (std::ostream & cout, QuadtreeNode * p)
{
    std::cout << "here";
    std::cout << "< xmin: " << p->xmin() << " xmax: " << p->xmax()
              << " ymin: " << p->ymin() << " ymax: " << p->ymax()
              << " num circles: " << p->num_circles() << " num children: "
              << p->num_children() << " >" << std::endl;
   
    return cout;
}

std::ostream & operator<< (std::ostream & cout, QuadtreeNode & p)
{
    std::cout << "< xmin: " << p.xmin() << " xmax: " << p.xmax()
              << " ymin: " << p.ymin() << " ymax: " << p.ymax()
              << " num circles: " << p.num_circles() << " num children: "
              << p.num_children() << " >" << std::endl;
   
    return cout;
}

#endif

