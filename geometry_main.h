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

class STYLE{
protected:
    DrStyle style;
public:
    STYLE(DrStyle st0);
    ~STYLE();
    inline void change_style(DrStyle st){
        style = st;
    }
    inline DrStyle get_style(DrStyle st) const{
        return st;
    }
};

class Point : public GOBJ, public STYLE{
public:
    double x;
    double y;
public:
    Point(double x0, double y0, DrStyle st0 = DrStyle::None);
    ~Point();
};

class Line : public GOBJ, public STYLE{
public:
    Point p1;
    Point p2;
public:
    Line(double x01, double y01, double x02, double y02, DrStyle st0 = DrStyle::None);
    ~Line();
};

#endif // GEOMETRY_MAIN_H
