#pragma once
#include <vector>
#include <string>


struct entry 
{
	std::vector<float> numeric;
	std::vector<std::string> categoric;
};

class csv 
{
public:
	std::vector<std::string> col_names;
	int col_count = 0;
	long int begin = 0;
	long int end = 0;
	std::vector<entry> data;
	uint8_t* type_table;

public:
	csv(const char*, char);
	csv(csv*, int);
	template<typename T>
	std::vector<T> getVec(const int&);
};

//Bitmasks for column type identification could be replaced by an enum
//This aproach allows to use bithacking if needed
//TODO change values for flags the user may combine with | to determinate type coercion behaviour
//That's a broken dream
#define CSV_STRING	0x01
#define CSV_FLOAT	0x02

struct train_test
{
	csv* train;
	csv* test;
};

train_test split(csv*, float);