#pragma once

#include <cstdio>
#include <memory>

struct FileDeleter final
{
	void operator()(FILE* file)
	{
		fclose(file);
	}
};

using file_ptr = std::unique_ptr<FILE, FileDeleter>;
