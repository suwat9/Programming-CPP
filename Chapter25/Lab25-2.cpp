#include <iostream>
#include <cmath>

class Base_class {
	public:
	int base, height;
	int width, length;
	double radius;
	
	void findArea() {
		std::cout << "findArea() in Base class" << std::endl;
	}
	void perimeter() {
		std::cout << "perimeter() in Base class" << std::endl;
	}
};
class Triangle : public Base_class {
	public:
		void findArea() {
			std::cout << "Area of Triangle is " << (0.5 * base * height) << std::endl;
		}
		void perimeter() {
			double c= std::sqrt((base*base) + (height*height));
			std::cout << "Perimeter of triangle is " << (base + height + c) << std::endl;
		}
};
class Rectangle : public Base_class {
	
};
class Circle : public Base_class {
	
};

int main() {
	
}
