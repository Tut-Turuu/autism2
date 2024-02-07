#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <limits>
#include <iostream>


// bool is_equal(double x, double y) {
//     return std::fabs(x - y) < std::numeric_limits<double>::epsilon();
// }

inline int sign(double x) {
    if (x == 0.0) return 0;
    if (x > 0.0) return 1;
    return -1;
}

struct Vector2d {
    double x, y;

    Vector2d(double x, double y) : x(x), y(y) {}
 
    Vector2d normal() {
        return {y, -x};
    }

    double get_length() {
        return sqrt(pow(x, 2) + pow(y, 2));
    }
};

struct Point2d {
    double x;
    double y;
};

struct Line2d {
    double A;
    double B;
    double C;

    Line2d(Vector2d vec, Point2d p): A(vec.y), B(-vec.x), C(vec.y*p.x + vec.x*p.y) {}
};

// double distance(Line2d l, Point2d p) {
//     return fabs((l.A*p.x + l.B*p.y + l.C) / (sqrt(pow(l.A, 2) + pow(l.B, 2) + pow(l.C, 2))));
// }

struct Point {
    double x, y, z;
};

struct Vector3d {
    double x, y, z;

    Vector3d() = default;

    Vector3d(Point& a): x(a.x), y(a.y), z(a.z) {}

    Vector3d(double x, double y, double z): x(x), y(y), z(z) {}

    double get_length() {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    void set_length(double length) {
        double old_len = get_length();
        x = (x/old_len)*length;
        y = (y/old_len)*length;
        z = (z/old_len)*length;
    }
};



class Cube {

    struct Edge {

        bool is_visible;
        double A, B, C, D;
        Point a, b, c, d;
        sf::Color color;

        Edge() = default;

        Edge(Point a, Point b, Point c, Point(d)): a(a), b(b), c(c), d(d) {
            A = (b.y - a.y)*(c.z - a.z) - (b.z - a.z)*(c.y - a.y);
            B = (b.z - a.z)*(c.x - a.x) - (b.x - a.x)*(c.z - a.z);
            C = (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y);
            D = - a.x*A - a.y*B - a.z*C;
        }

        Edge(Point a, Point b, Point c, Point d, sf::Color orig_color, Vector3d light_vec): Edge(a, b, c, d)  {
            
            // is_visible = ((a.z + c.z) / 2.0 > 0);
            is_visible = true;


            // (cos(l, n) + 1) / 2
            // l - light vector
            // n - normal vector to edge
            double color_intensivity = ((light_vec.x*A + light_vec.y*B + light_vec.z*C) /
                                       (sqrt(pow(light_vec.x, 2) + pow(light_vec.y, 2) + pow(light_vec.z, 2))*sqrt(pow(A, 2) + pow(B, 2) + pow(C, 2))) + 1) / 2.0;

            color = sf::Color(orig_color.r*color_intensivity, orig_color.g*color_intensivity, orig_color.b*color_intensivity);
        }

        void draw(sf::RenderWindow& window, int x0, int y0) {
            if (!is_visible) return;

            sf::ConvexShape edge(4);
            edge.setPoint(0, sf::Vector2f(x0+a.x, y0+a.y));
            edge.setPoint(1, sf::Vector2f(x0+b.x, y0+b.y));
            edge.setPoint(2, sf::Vector2f(x0+c.x, y0+c.y));
            edge.setPoint(3, sf::Vector2f(x0+d.x, y0+d.y));

            edge.setFillColor(color);
            window.draw(edge);

        }
    };

    int x0;
    int y0;
    sf::Color color;
    double radius;
    Point a, b;
    Edge A, B, C, D, E, F;
    Vector3d light_vec;

public:


    void calculate_position() {
        Point f = {-a.x, -a.y, -a.x};
        Point e = {-b.x, -b.y, -b.x};

        Edge ed(a, b, e, f);

        Vector3d normal(ed.A, ed.B, ed.C);
        normal.set_length(radius * 0.81649);
        Point h = {(b.x + f.x) / 2 + normal.x, (b.y + f.y) / 2 + normal.y, (b.z + f.z) / 2 + normal.z};
        Point d = {(b.x + f.x) / 2 - normal.x, (b.y + f.y) / 2 - normal.y, (b.z + f.z) / 2 - normal.z};
        Point g = {(a.x + e.x) / 2 + normal.x, (a.y + e.y) / 2 + normal.y, (a.z + e.z) / 2 + normal.z};
        Point c = {(a.x + e.x) / 2 - normal.x, (a.y + e.y) / 2 - normal.y, (a.z + e.z) / 2 - normal.z};

        A = Edge(a, g, h, e, color, light_vec);
        B = Edge(a, b, d, c, color, light_vec);
        C = Edge(d, b, h, f, color, light_vec);
        D = Edge(c, e, f, d, color, light_vec);
        E = Edge(e, g, h, f, color, light_vec);
        F = Edge(a, c, e, g, color, light_vec);
    }

    Cube(int x0, int y0, Point a, Point b, sf::Color color) : a(a), b(b), x0(x0), y0(y0), color(color), radius(Vector3d(a).get_length()) {

        light_vec = Vector3d(-10, 0, 0);

        calculate_position();
    }

    void move(Vector2d direction) {

        if (direction.x == 0 || direction.y == 0) return;

        const double intensivity = 0.0000000001;

        std::cout << "direction: " << direction.x << ' ' << direction.y << '\n';

        double cosa = direction.x / sqrt(pow(direction.x, 2) + pow(direction.y, 2));
        double sina = direction.y / sqrt(pow(direction.x, 2) + pow(direction.y, 2));

        std::cout << "cossin: " << cosa << ' ' << sina << '\n';
        

        double new_ax = a.x * cosa + a.y * sina; // (a.x - sina/cosa * a.y) / (cosa - sina * sina / cosa);
        double new_ay = -a.x * sina + a.y * cosa; // (a.x - cosa/sina * a.y) / (sina - cosa * cosa / sina);
        double new_bx = b.x * cosa + b.y * sina; // (b.x - sina/cosa * b.y) / (cosa - sina * sina / cosa);
        double new_by = -b.x * sina + b.y * cosa; // (b.x - cosa/sina * b.y) / (sina - cosa * cosa / sina);

        double rotate_angle = sign(direction.x) * direction.get_length() * intensivity; // in radians

        double current_angle = sign(new_ax) * acos(a.z / sqrt(pow(a.z, 2) + pow(new_ax, 2)));

        std::cout << "curr_angle: " << current_angle << '\n';

        double rotate_radius = sqrt(pow(a.z, 2) + pow(new_ax, 2));

        new_ax = rotate_radius * cos(rotate_angle + current_angle);
        a.z = rotate_radius * sin(rotate_angle + current_angle);

        current_angle = sign(new_bx) * acos(b.z / sqrt(pow(b.z, 2) + pow(new_bx, 2)));

        rotate_radius = sqrt(pow(b.z, 2) + pow(new_bx, 2));

        new_bx = rotate_radius * cos(rotate_angle + current_angle);
        b.z = rotate_radius * sin(rotate_angle + current_angle);

        a.x = new_ax * cosa - new_ay * sina;
        a.y = new_ax * sina + new_ay * cosa;
        b.x = new_bx * cosa - new_by * sina;
        b.y = new_bx * sina + new_by * cosa;

        std::cout << "a: " << a.x << ' ' << a.y << ' ' << a.z << '\n';
        std::cout << "b: " << b.x << ' ' << b.y << ' ' << b.z  << '\n';

        calculate_position();

        // double nax = a.x * direction.x / sqrt(pow(direction.x, 2) + pow(direction.y, 2));
        // double nay = a.y * direction.x / sqrt(pow(direction.x, 2) + pow(direction.y, 2));
        // double nbx = b.x * direction.x / sqrt(pow(direction.x, 2) + pow(direction.y, 2));
        // double nby = b.y * direction.x / sqrt(pow(direction.x, 2) + pow(direction.y, 2));
        
    }

    void draw(sf::RenderWindow& window);

};