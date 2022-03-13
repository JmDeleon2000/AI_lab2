#include "csv.h"
#include <iostream>
#include "knn.h"
#include "bmp.h"




int main() 
{
	using namespace std;
	csv* x = new csv("Oranges vs Grapefruit.csv", ',');
	int i = 0;
	while (i < x->col_count)
	{
		cout << x->col_names[i] << " ";
		i++;
	}
	cout << "\n";

	train_test a = split(x, 0.7f);

	cout << a.train->begin << " " << a.train->end << "\n";
	cout << a.test->begin << " " << a.test->end << "\n";
	BMP_image* frameBuffer = new BMP_image(1920, 1080);

	BMP_image::output_BMP(frameBuffer, "out.bmp");
}