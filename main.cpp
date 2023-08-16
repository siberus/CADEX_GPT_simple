#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>

class Curve3D {
public:
    virtual ~Curve3D() {}
    virtual double radius() const = 0;
    virtual void pointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const = 0;
};

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
