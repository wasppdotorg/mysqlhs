
#include <memory>
#include <vector>
#include <string>
#include <iostream>

#include "mysqlhs.hpp"

int main()
{
	mysqlhs::connection c("127.0.0.1", "9999");

	mysqlhs::index idx(&c, "test", "movie", "PRIMARY", "id,genre,title,view_count");
	if (!idx.open_())
	{
		std::cout << "failed to open index" << std::endl;
		return 1;
	}

	std::vector<std::string> insert_data;
	{
		insert_data.push_back("0");
		insert_data.push_back("Sci-Fi");
		insert_data.push_back("Star wars");
	}

	bool result = false;
	for (int i = 0; i < 10; ++i)
	{
		result = idx.insert_(insert_data);
		if (!result)
		{
			std::cout << "failed to insert" << std::endl;
		}
		std::cout << "insert : " << idx.raw_data() << std::endl;
		std::cout << "-----------------------" << std::endl;
	}

	result = idx.select_where_index('>', 0, 10, 0);
	if (!result)
	{
		std::cout << "failed to select" << std::endl;
	}
	std::cout << "select : " << idx.raw_data() << std::endl;
	std::cout << "-----------------------" << std::endl;

	std::vector<std::string> update_data;
	{
		update_data.push_back("11");
		update_data.push_back("Sci-Fi updated");
		update_data.push_back("Star wars updated");
		update_data.push_back("101");
	}

	idx.update_where_index('=', 10, update_data);
	if (!result)
	{
		std::cout << "failed to update" << std::endl;
	}
	std::cout << "update : " << idx.affected_rows() << std::endl;
	std::cout << "-----------------------" << std::endl;

	idx.delete_where_index('=', 9);
	if (!result)
	{
		std::cout << "failed to delete" << std::endl;
	}
	std::cout << "delete : " << idx.affected_rows() << std::endl;
	std::cout << "-----------------------" << std::endl;

    return 0;
}
