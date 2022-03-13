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

knn_model fit(train_test data, int k) 
{
	using namespace std;
	knn_model out;
	int i = data.test->begin;
	int j;
	int l;

	struct cat_count 
	{
		int count = 0;
		string category;
	};
	vector<cat_count> categories;

	entry element;
	while (i < data.test->end)
	{
		std::sort(data.train->data.begin(), 
			data.train->data.end(),
			euclidean_comparator);

		j = 0;
		while (j < k)
		{
			l = 0;
			while (l <= categories.size())
			{
				if (data.train->data[j].categoric[0] == categories[l].category)
				{
					categories[l].count += 1;
					break;
				}
				if (l == categories.size())
				{
					cat_count new_category;
					new_category.category = data.train->data[j].categoric[0];
					categories.push_back(new_category);
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

		element.categoric[0] = categories[0].category;

		out.entries.push_back(element);
		i++;
	}
}