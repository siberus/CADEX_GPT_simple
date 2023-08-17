#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <ctime> // для time() 
#include <cstdlib> // для rand() и srand() 



class Point 
{
private:
    double m_x = 0.0;
    double m_y = 0.0;
    double m_z = 0.0;

public:
    Point() {}
    Point(double x, double y, double z)
         : m_x(x), m_y(y), m_z(z) {}

    friend std::ostream& operator<<(std::ostream &out, const Point &p)
    {
        out << "x=" << p.m_x << ", y=" << p.m_y << ", z=" << p.m_z << " "; 
        return out;
    }
    
    float getX() const { return m_x; } 
    float getY() const { return m_y; } 
    float getZ() const { return m_z; } 
};

class Curve 
{
private:
    Point m_center;
protected:
    std::string m_type; 
    Curve(std::string type, const Point &p1) 
        : m_type(type), m_center (p1) {}
public:
    virtual ~Curve() {}
    //virtual double radius() const = 0;
    Point getCenter() const { return m_center; }
    std::string getType() { return m_type;}
    virtual void pointAndDerivative(double t, Point &p, Point &dp) const = 0;
};

class Circle : public Curve 
{
private:
    double m_radius;

public:
    Circle(double radius, const Point &center)
        : Curve("Circle", center), m_radius(radius) {
        if (m_radius <= 0.0) {
            throw std::invalid_argument("Radius must be positive.");
        }
    }

    void pointAndDerivative(double t, Point &p, Point &dp) const override {
        double x = getCenter().getX() + radius() * cos(t);
        double y = getCenter().getY() + radius() * sin(t);
        double z = getCenter().getZ();
        double dx = -radius() * sin(t);
        double dy = radius() * cos(t);
        double dz = 0.0;

        p = Point(x, y, z);
        dp = Point(dx, dy, dz);
    }

    double radius() const {
        return m_radius;
    }
};

class Ellipse : public Curve {
private:
    double m_radiusMin;
    double m_radiusMax;

public:
    Ellipse(double radiusMin, double radiusMax, const Point &center)
        : Curve("Ellipse", center), m_radiusMin(radiusMin), m_radiusMax(radiusMax) 
        {
            if (m_radiusMin <= 0.0 || m_radiusMax <= 0.0) {
                throw std::invalid_argument("Radii must be positive.");
        }
    }

    void pointAndDerivative(double t, Point &p, Point &dp) const override 
    {
        
        double paramT = atan2(m_radiusMax * sin(t), m_radiusMin * cos(t));
            if (t < 0.0) { t += 2 * M_PI; }
        double x = getCenter().getX() + m_radiusMax * cos(paramT);
        double y = getCenter().getY() + m_radiusMin * sin(paramT);
        double z = getCenter().getZ();
        double dx_dt = -m_radiusMax * sin(paramT);
        double dy_dt = m_radiusMin * cos(paramT);
        double dz_dt = 0.0;
        
        p = Point(x, y, z);
        dp = Point(dx_dt, dy_dt, dz_dt);
    }
};
   
class Helix : public Curve {
private:
    double m_radius;
    double m_step;

public:
    Helix(double radius, double step, const Point &center)
        : Curve("Helix", center), m_radius(radius), m_step(step) {
        if (m_radius <= 0.0 || m_step <= 0.0) {
            throw std::invalid_argument("Radius and step must be positive.");
        }
    }

    void pointAndDerivative(double t, Point &p, Point &dp) const override {
        double x = getCenter().getX() + m_radius * cos(t);
        double y = getCenter().getY() + m_radius * sin(t);
        double z = getCenter().getZ() + (t / 2 * M_PI) * m_step;
        double dx = -m_radius * sin(t);
        double dy = m_radius * cos(t);
        double dz = m_step;

        p = Point(x, y, z);
        dp = Point(dx, dy, dz);
    }
};

double getRandomNumber(int min, int max) 
{ 
 static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
 // Равномерно распределяем генерацию случайного числа в диапазоне значений
 return (rand() * fraction * (max - min + 1) + min); 
}

int main() 
{
    srand(static_cast<unsigned int>(time(0))); // устанавливаем значение

    //Populate a container (e.g. vector or list) of objects of these types created in random manner with random parameters.
    std::vector<std::unique_ptr<Curve>> curves;

    for (int i = 0; i < 10; ++i) {
        double x = getRandomNumber(0, 10);
        double y = getRandomNumber(0, 10);
        double z = 0;
        double radius =  getRandomNumber(0, 10);
        double radiusMin =  getRandomNumber(0, 9);
        double radiusMax =  getRandomNumber(radiusMin, 10);
        double step = getRandomNumber(0, 3);;

        std::unique_ptr<Curve> curve;
        if (static_cast<int>(getRandomNumber(1, 3))  == 1) 
        {
            curve = std::make_unique<Circle>(radius, Point(x, y, z));
        } else if (static_cast<int>(getRandomNumber(1, 3))  == 2) 
        {
            curve = std::make_unique<Ellipse>(radiusMin, radiusMax, Point(x, y, z));
        } else 
        {
            curve = std::make_unique<Helix>(radius, step, Point(x, y, z));
        }

        curves.push_back(std::move(curve));
    }

    // Print coordinates and derivatives of all curves at t = PI/4
    int i = 1;
    for (const auto &curve : curves) 
    {
        Point p, dp;
        curve->pointAndDerivative(M_PI / 4, p, dp);
        std::cout << i++ << std::endl;
        std::cout << "Curve Type: " << (*curve).getType() << std::endl;
        std::cout << "Point: " << p << std::endl;
        std::cout << "Derivative: " << dp << std::endl;
    }

    //Populate a second container that would contain only circles from the first container.
    std::vector<Circle *> circles;
    
    for (const auto &curve : curves) {
        if (typeid(*curve) == typeid(Circle)) {
            circles.push_back(static_cast<Circle *>(curve.get()));
        }
    }

    i = 1;
    for (const auto &circle : circles) 
    {
        std::cout << i++ << std::endl;
        std::cout << "Curve Type: " << (*circle).getType() <<  " Radius: " << (*circle).radius() << std::endl;
    }

    //Sort the second container in the ascending order of circles’ radii. T
    std::sort(circles.begin(), circles.end(), [](const Circle *a, const Circle *b) {
        return a->radius() < b->radius();
    });

    i = 1;
    for (const auto &circle : circles) 
    {
        std::cout << i++ << std::endl;
        std::cout << "Curve Type: " << (*circle).getType() <<  " Radius: " << (*circle).radius() << std::endl;
    }


    //Compute the total sum of radii of all curves in the second container.
    double totalRadii = 0.0;
    for (const Circle *circle : circles) {
        totalRadii += circle->radius();
    }

    std::cout << "Total Sum of Radii: " << totalRadii << std::endl;

    return 0;
}