#include <fstream>
#include "csv.h"
#include <stdexcept>
#include <utility>

#define debug 0
#if debug
#include <iostream>
#endif

//impure function
//writes the content of line to target
inline void line_to_vec(std::vector<std::string>* target, std::string line, char separator);



csv::csv(const char* filename, char separator)
{
	using namespace std;
	fstream fileStream;
	string readBuffer;

	fileStream.open(filename, ios::in);
	if (!fileStream.is_open())
		throw "Couldn't open .csv";

	getline(fileStream, readBuffer);
	line_to_vec(&col_names, readBuffer, separator);
	col_count = col_names.size();
	int i = 0;
	vector<string> last_line_vec;
	getline(fileStream, readBuffer);
	line_to_vec(&last_line_vec, readBuffer, separator);
	vector<float> numeric_element;
	vector<string> categoric_element;
	entry element;
	
	while (i < col_count)
	{
		try
		{
			float x;
			x = stof(last_line_vec[i]);
			numeric_element.push_back(x);
			type_table[i] = CSV_FLOAT;
		}
		catch (invalid_argument const& ex)
		{
			categoric_element.push_back(last_line_vec[i]);
			type_table[i] = CSV_STRING;
		}
#if debug
		cout << (int)type_table[i] << "\n";
#endif
		i++;
	}

	element.numeric = numeric_element;
	element.categoric = categoric_element;
	data.push_back(element);

#if debug
	i = 0;
	while (i < col_count)
	{
		switch (type_table[i])
		{
		case CSV_FLOAT:
			cout << (*((vector<float>*)data[i]))[0] << "\n";
			break;
		case CSV_STRING:
			cout << (*((vector<string>*)data[i]))[0] << "\n";
			break;
		case CSV_INT:
			cout << (*((vector<int>*)data[i]))[0] << "\n";
			break;
		default:
			return;
		}
		i++;
	}
#endif
	while (getline(fileStream, readBuffer))
	{
		numeric_element.clear();
		categoric_element.clear();
		line_to_vec(&last_line_vec, readBuffer, separator);
		i = 0;
		while (i < col_count)
		{
			switch (type_table[i])
			{
			case CSV_FLOAT:
				float x;
				x = stof(last_line_vec[i]);
				numeric_element.push_back(x);
				break;
			case CSV_STRING:
				categoric_element.push_back(last_line_vec[i]);
				break;
			default:
				throw "Undetermined type";
			}
			i++;
		}
		element.numeric = numeric_element;
		element.categoric = categoric_element;
		data.push_back(element);
	}

	end = data.size();
}

csv::csv(csv* base, int size) 
{
	using namespace std;
	col_count = base->col_count;
	col_names = base->col_names;
	begin = 0;
	end = 0;
	int rng;
	vector<entry>::iterator pos;

	while (end < size)
	{
		rng = rand() % base->data.size();
		data.push_back(base->data[rng]);


		pos = base->data.begin();
		pos += rng;
		base->data.erase(pos);
	}
	
}

train_test split(csv* a, float frac) 
{
	train_test out;
	out.train = new csv(a, (int)((float)(out.train->end) * frac));
	out.test = new csv(a, 1-(int)((float)(out.train->end) * frac));
	return out;
}





template <typename T>
std::vector<T> csv::getVec(const int& i) 
{
	return *((std::vector<T>*)data[i]);
}


inline void line_to_vec(std::vector<std::string>* target, std::string line, char separator)
{
	using namespace std;
	int i;
	string substr;
	target->clear();

	i = line.find(separator);
	while (i != -1 && !line.empty())
	{
		substr = line.substr(0, i);
#if debug
		cout << " line decode: " << substr << "\n";
#endif
		target->push_back(substr);
		line.erase(0, i + 1);
		i = line.find(separator);
	}
	if (!line.empty())
		target->push_back(line);
}


