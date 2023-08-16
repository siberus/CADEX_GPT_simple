#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// Base class for 3D curves
class Curve3D {
public:
    virtual ~Curve3D() {}
    virtual double radius() const = 0;
    virtual void pointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const = 0;
};

// Circle class
class Circle : public Curve3D {
private:
    double _radius;

public:
    Circle(double radius) : _radius(radius) {
        if (_radius <= 0) {
            throw std::invalid_argument("Radius must be positive.");
        }
    }

    double radius() const override {
        return _radius;
    }

    void pointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const override {
        x = _radius * cos(t);
        y = _radius * sin(t);
        z = 0.0;
        dx = -_radius * sin(t);
        dy = _radius * cos(t);
        dz = 0.0;
    }
};

// Ellipse class
class Ellipse : public Curve3D {
private:
    double _radiusX;
    double _radiusY;

public:
    Ellipse(double radiusX, double radiusY) : _radiusX(radiusX), _radiusY(radiusY) {
        if (_radiusX <= 0 || _radiusY <= 0) {
            throw std::invalid_argument("Radius must be positive.");
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

// Helix class
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
    // Populate a container with random curves
    std::vector<Curve3D*> curves;
    curves.push_back(new Circle(2.0));
    curves.push_back(new Ellipse(3.0, 2.0));
    curves.push_back(new Helix(1.0, 0.5));

    // Print coordinates and derivatives at t = PI/4
    double t = M_PI / 4.0;
    for (const Curve3D* curve : curves) {
        double x, y, z, dx, dy, dz;
        curve->pointAndDerivative(t, x, y, z, dx, dy, dz);
        std::cout << "Curve type: " << typeid(*curve).name() << std::endl;
        std::cout << "Point at t = PI/4: (" << x << ", " << y << ", " << z << ")" << std::endl;
        std::cout << "Derivative at t = PI/4: (" << dx << ", " << dy << ", " << dz << ")" << std::endl;
        std::cout << std::endl;
    }

    // Populate a second container with circles from the first container
    std::vector<Circle*> circles;
    for (Curve3D* curve : curves) {
        if (dynamic_cast<Circle*>(curve)) {
            circles.push_back(static_cast<Circle*>(curve));
        }
    }

    // Sort the second container based on radii
    std::sort(circles.begin(), circles.end(), [](const Circle* c1, const Circle* c2) {
        return c1->radius() < c2->radius();
    });

    // Calculate the total sum of radii
    double totalRadiusSum = 0.0;
    for (const Circle* circle : circles) {
        totalRadiusSum += circle->radius();
    }

    // Clean up memory
    for (Curve3D* curve : curves) {
        delete curve;
    }

    return 0;
}
