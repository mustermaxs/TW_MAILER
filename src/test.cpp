#include <iostream>

// Abstract class (interface)
class IShape {
public:
    virtual double getArea() const = 0; // Pure virtual function
    virtual void display() const = 0;   // Pure virtual function
    virtual ~IShape() {} // Virtual destructor to make sure derived classes' destructors are called
};

// Concrete class implementing the interface
class Circle : public IShape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    // Implementing the pure virtual functions
    double getArea() const override {
        return 3.14159 * radius * radius;
    }

    void display() const override {
        std::cout << "Circle with radius " << radius << std::endl;
    }
};

// Another concrete class implementing the interface
class Rectangle : public IShape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    // Implementing the pure virtual functions
    double getArea() const override {
        return width * height;
    }

    void display() const override {
        std::cout << "Rectangle with width " << width << " and height " << height << std::endl;
    }

    void existsonlyhere() {...};
};

int main() {
    // Create objects of classes implementing the interface
    Circle circle(5.0);
    Rectangle rectangle(4.0, 6.0);

    // Use the objects through the interface
    IShape* shape1 = &circle;
    IShape* shape2 = &rectangle;

    // Call interface methods
    shape1->display();
    std::cout << "Area: " << shape1->getArea() << std::endl;

    shape2->display();
    std::cout << "Area: " << shape2->getArea() << std::endl;

    return 0;
}
