#include <iostream>
#include <cmath>

class Circle {
	public:
	double radius;
	
	Circle(double rr){
		radius = rr;
	}
	
	void findArea() {
		std::cout << "Area of circle is " << (3.14 * radius * radius) << std::endl;
	}
	void perimeter() {
		std::cout << "Perimeter of cicle is " << (2.0 * 3.14 * radius) << std::endl;
	}
};

int main(){
	Circle c(20);
	
	c.findArea();
	c.perimeter();
}
