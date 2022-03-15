#pragma once
#include <vector>
#include "csv.h"
#include "BMP.h"


struct knn_model
{
	std::vector<entry> entries;
	std::vector<std::string> categories;
	//std::vector<BGR_pixel> category_color;
};

void graph_model(BMP_image*, knn_model);
knn_model fit(train_test, int k, int cap = -1);

struct confusion_matrix
{
	int* data;
	std::vector<std::string> axis_labels;
	
	// x axis: original
	// y axis: predicted
	confusion_matrix(knn_model);
};

