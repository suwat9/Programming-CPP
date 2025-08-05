
#include <iostream>
#include <cmath>

class Base_class {
	public:
	int base, height;
	int width, length;
	double radius;
	int depth;
	
	void findArea() {
		std::cout << "findArea() in Base class" << std::endl;
	}
	void perimeter() {
		std::cout << "perimeter() in Base class" << std::endl;
	}
};
class Triangle : public Base_class {
	public:	
	Triangle(){
		base = 10; height = 2;
	}
	void findArea() {
		std::cout << "Area of Triangle is " << (0.5 * base * height) << std::endl;
	}
	void perimeter() {
		double c= std::sqrt((base*base) + (height*height));
		std::cout << "Perimeter of triangle is " << (base + height + c) << std::endl;
	}
};
class Rectangle : public Base_class {
	public:	
	Rectangle(){
		width = 5; length = 10;
	}
	void findArea(){
		std::cout << "Area of rectangle is " << (width * length) << std::endl;
	}
	void perimeter(){
		std::cout << "Perimeter of rectangle is " << (2*(width+length)) << std::endl;
	}
};
class Circle : public Base_class {
	public:	
	Circle(double rr){
		radius = rr;
	}
	void findArea() {
		std::cout << "Area of circle is " << (M_PI * radius * radius) << std::endl;
	}
	void perimeter() {
		std::cout << "Perimeter of cicle is " << (2.0 * M_PI * radius) << std::endl;
	}
};

class DTriangle : public Triangle {
	public:	
	DTriangle(){
		base = 10; height = 2; depth = 3;
	}
	void findArea() {
		std::cout << "Area of 3D Triangle is " << (0.5 * base * height * depth) << std::endl;
	}
};
int main() {
	Triangle tr;
	tr.findArea();
	tr.perimeter();
	
	Rectangle rec;
	rec.findArea();
	rec.perimeter();
	
	Circle cr(10);
	cr.findArea();
	cr.perimeter();
	
	DTriangle tr3;
	tr3.findArea();
	tr3.perimeter();

	return 0;
}
