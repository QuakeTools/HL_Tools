#pragma once

#include <string>

namespace models
{
class Model
{
public:
	virtual ~Model() = 0 {}

	std::string FileName;
};
}
