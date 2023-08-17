#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>

class Point 
{
private:
    double m_x = 0.0;
    double m_y = 0.0;
    double m_z = 0.0;

public:
    Point(double x, double y, double z)
         : m_x(x), m_y(y), m_z(z) {}

    friend std::ostream& operator<<(std::ostream &out, const Point &p)
    {
        out << "Point(" << p.m_x << ", " << p.m_y << ", " << p.m_z << ")"; 
        return out;
    }
    
    float getX() const { return m_x; } 
    float getY() const { return m_y; } 
    float getZ() const { return m_z; } 
};

class Curve 
{
private:
/*      double m_centerX;
    double m_centerY;
    double m_centerZ;
*/
    Point m_center;
public:
    /* Curve(double x = 0.0, double y = 0.0, double z = 0.0) 
        : m_centerX(x), m_centerY(y), m_centerZ(z) {}
    
    float getCenterX() const { return m_centerX; } 
    float getCenterY() const { return m_centerY; } 
    float getCenterZ() const { return m_centerZ; } 
 */
    Curve(const Point &p1) 
        : m_center (p1) {}
        
    virtual ~Curve() {}
    //virtual double radius() const = 0;
    virtual void pointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const = 0;
};

class Circle : public Curve 
{
    private:
        double m_radius;

    public:
        Circle(double radius, double x = 0.0, double y = 0.0, double z = 0.0) 
            : Curve(x, y, z),  
                 m_radius(radius) 
        {
            if (m_radius <= 0) {
                throw std::invalid_argument("Radius must be positive.");
            }
        }

       // double getRadius() const { return m_radius; }

       /* void pointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const  
        {
            x = m_radius * cos(t);
            y = m_radius * sin(t);
            z = 0.0;
            dx = -m_radius * sin(t);
            dy = m_radius * cos(t);
            dz = 0.0;
        }*/

        void Circle::pointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const 
        {
            x = getCenterX() + m_radius * cos(t);
            y = getCenterY() + m_radius * sin(t);
            z = getCenterZ();
            dx = -m_radius * sin(t);
            dy = m_radius * cos(t);
            dz = 0.0;
        }

};

class Ellipse : public Curve3D {
private:
    double _radiusX;
    double _radiusY;

public:
    Ellipse(double radiusX, double radiusY) : _radiusX(radiusX), _radiusY(radiusY) {
        if (_radiusX <= 0 || _radiusY <= 0) {
            throw std::invalid_argument("Radii must be positive.");
        }
    }

    double radius() const override {
        return (_radiusX + _radiusY) / 2.0;
    }

    void pointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const override {
        x = _radiusX * cos(t);
        y = _radiusY * sin(t);
        z = 0.0;
        dx = -_radiusX * sin(t);
        dy = _radiusY * cos(t);
        dz = 0.0;
    }
};

class Helix : public Curve3D {
private:
    double _radius;
    double _step;

public:
    Helix(double radius, double step) : _radius(radius), _step(step) {
        if (_radius <= 0 || _step <= 0) {
            throw std::invalid_argument("Radius and step must be positive.");
        }
    }

    double radius() const override {
        return _radius;
    }

    void pointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const override {
        x = _radius * cos(t);
        y = _radius * sin(t);
        z = t * _step;
        dx = -_radius * sin(t);
        dy = _radius * cos(t);
        dz = _step;
    }
};

int main() {
    std::vector<std::unique_ptr<Curve3D>> curves;
    curves.push_back(std::make_unique<Circle>(2.0));
    curves.push_back(std::make_unique<Ellipse>(3.0, 2.0));
    curves.push_back(std::make_unique<Helix>(1.0, 0.5));

    double t = M_PI / 4.0;
    for (const auto& curve : curves) {
        double x, y, z, dx, dy, dz;
        curve->pointAndDerivative(t, x, y, z, dx, dy, dz);
        std::cout << "Curve type: " << typeid(*curve).name() << std::endl;
        std::cout << "Point at t = PI/4: (" << x << ", " << y << ", " << z << ")" << std::endl;
        std::cout << "Derivative at t = PI/4: (" << dx << ", " << dy << ", " << dz << ")" << std::endl;
        std::cout << std::endl;
    }

    std::vector<Circle*> circles;
    for (const auto& curve : curves) {
        if (auto circle = dynamic_cast<Circle*>(curve.get())) {
            circles.push_back(circle);
        }
    }

    std::sort(circles.begin(), circles.end(), [](const Circle* c1, const Circle* c2) {
        return c1->radius() < c2->radius();
    });

    double totalRadiusSum = 0.0;
    for (const Circle* circle : circles) {
        totalRadiusSum += circle->radius();
    }

    return 0;
}
