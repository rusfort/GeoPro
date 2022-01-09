#ifndef GEOMETRY_MAIN_H
#define GEOMETRY_MAIN_H

enum class DrStyle{
    None,
    Light,
    Bold
};

class GOBJ{
private:
    bool visible = true;
public:
    inline bool is_visible() const{
        return visible;
    }
    inline void hide(){
        visible = false;
    }
    inline void make_visible(){
        visible = true;
    }
};

class Point : public GOBJ{
public:
    double x;
    double y;
private:
    DrStyle style;
public:
    Point(double x0, double y0, DrStyle st0 = DrStyle::None);
    ~Point();
    inline void change_style(DrStyle st){
        style = st;
    }
};

class Line : public GOBJ{
public:
    double x1;
    double y1;
    double x2;
    double y2;
private:
    DrStyle style;
public:
    Line(double x01, double y01, double x02, double y02, DrStyle st0 = DrStyle::None);
    ~Line();
    inline void change_style(DrStyle st){
        style = st;
    }
};

#endif // GEOMETRY_MAIN_H
