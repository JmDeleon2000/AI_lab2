#include "knn.h"
#include <algorithm>



entry tested;

bool euclidean_comparator(const entry& a, const entry& b)
{
	int i = 0;
	float a_sqr_dist = 0, b_sqr_dist = 0;
	float dif_a, dif_b;
	while (i < a.numeric.size())
	{
		dif_a = a.numeric[i] - tested.numeric[i];
		dif_b = b.numeric[i] - tested.numeric[i];
		a_sqr_dist += dif_a * dif_a;
		b_sqr_dist += dif_b * dif_b;
		i++;
	}

	return a_sqr_dist < b_sqr_dist;
}

knn_model fit(train_test data, int k, int cap) 
{
	using namespace std;
	if (cap == -1)
		cap = data.test->end;
	

	knn_model out = knn_model();
	int i = data.test->begin;
	int j;
	int l;

	struct cat_count 
	{
		int count = 0;
		string category;
	};
	vector<cat_count> categories;

	// TODO remove hardcode
	cat_count grapefruit;
	grapefruit.category = "grapefruit";
	cat_count orange;
	orange.category = "orange";
	categories.push_back(grapefruit);
	categories.push_back(orange);

	entry element;
	while (i < cap)
	{
		tested = data.test->data[i];
		element = tested;

		std::sort(data.train->data.begin(), 
			data.train->data.end(),
			euclidean_comparator);

		categories[0].count = 0;
		categories[1].count = 0;
		j = 0;
		while (j < k)
		{
			l = 0;
			while (l < categories.size())
			{
				if (l < categories.size() && 
					data.train->data[j].categoric[0] == categories[l].category)
				{
					categories[l].count += 1;
					break;
				}
				l++;
			}
			j++;
		}
		element.numeric = data.test->data[i].numeric;

		sort(categories.begin(), categories.end(),
			[](const cat_count& a, const cat_count& b) 
			{
				return a.count > b.count;
			});

		element.categoric.push_back(categories[0].category);

		out.entries.push_back(element);
		i++;
	}
	out.categories.push_back(categories[0].category);
	out.categories.push_back(categories[1].category);

	return out;
}

confusion_matrix::confusion_matrix(knn_model model)
{
	axis_labels = model.categories;
	int i = 0;
	int axis_size = axis_labels.size();
	data = new int[axis_size * axis_size];

	while (i < axis_size * axis_size)
	{
		data[i] = 0;
		i++;
	}

	i = 0;
	entry current;
	int j, a1 = 0, a2 = 0;
	while (i < model.entries.size())
	{
		current = model.entries[i];
		j = 0;
		while (j < axis_size)
		{
			if (current.categoric[0] == axis_labels[j])
				a1 = j;
			if (current.categoric[1] == axis_labels[j])
				a2 = j;
			j++;
		}
		data[a1 + a2 * axis_size] += 1;
		i++;
	}
}


void graph_model(BMP_image* frbuff, knn_model model) 
{
	using namespace std;
	int i = 0;
	struct point 
	{
		float x = 0;
		float y = 0;
		bool orange;
	};
	vector<point> points;
	
	// TODO remove harcoded colors
	int x_min = INT_MAX, x_max = INT_MIN;
	int y_min = INT_MAX, y_max = INT_MIN;
	while (i < model.entries.size())
	{
		point p;
		p.x = model.entries[i].numeric[0];
		p.x += model.entries[i].numeric[1];
		p.y = model.entries[i].numeric[2];
		p.y += model.entries[i].numeric[3];
		p.y += model.entries[i].numeric[4];
		p.orange = model.entries[i].categoric[1] == "orange";
		points.push_back(p);
		if (x_min > p.x)
			x_min = p.x;
		if (x_max < p.x)
			x_max = p.x;
		if (y_min > p.y)
			y_min = p.y;
		if (y_max < p.y)
			y_max = p.y;
		i++;
	}
	x_max -= x_min;
	y_max -= y_min;


	i = 0;
	while (i < points.size())
	{
		points[i].x -= x_min;
		points[i].x /= x_max;
		points[i].x *= frbuff->width - 1;
		
		points[i].y -= y_min;
		points[i].y /= y_max;
		points[i].y *= frbuff->height - 1;


		if (points[i].orange)
		{
			frbuff->image[(int)points[i].y][(int)points[i].x].b = 0;
			frbuff->image[(int)points[i].y][(int)points[i].x].g = 175;
			frbuff->image[(int)points[i].y][(int)points[i].x].r = 255;
		}
		else 
		{
			frbuff->image[(int)points[i].y][(int)points[i].x].b = 168;
			frbuff->image[(int)points[i].y][(int)points[i].x].g = 0;
			frbuff->image[(int)points[i].y][(int)points[i].x].r = 121;
		}
		i++;
	}
}