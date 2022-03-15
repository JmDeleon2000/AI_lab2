#include "csv.h"
#include <iostream>
#include "knn.h"
#include "bmp.h"
#include <algorithm>




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

	cout << a.train->data.size() << " " ;
	cout << a.test->data.size() << "\n";

	knn_model model = fit(a, 101);
	confusion_matrix* matrix = new confusion_matrix(model);
	cout << "confusion matrix:\n";
	cout << matrix->data[0] << " " << matrix->data[1] << "\n";
	cout << matrix->data[2] << " " << matrix->data[3] << "\n";

	BMP_image* frameBuffer = new BMP_image(128, 128);
	graph_model(frameBuffer, model);
	BMP_image::output_BMP(frameBuffer, "out.bmp");
}