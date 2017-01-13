/*
Copyright (c) 2016-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <memory>
#include <vector>
#include <string>
#include <iostream>

#include "mysqlhs.hpp"

int main()
{
	//return test_();

	try
	{
		mysqlhs_init();
		atexit(mysqlhs_end);

		mysqlhs::connection c("127.0.0.1", 9999);
		mysqlhs::index movie(c, "test", "movie", "PRIMARY", "id,genre,title,view_count");

		bool ret = movie.open();
		if (!ret)
		{
			std::cout << "failed to open index\n";
			return 1;
		}

		std::vector<std::string> insert_data;
		//
			insert_data.push_back("0");
			insert_data.push_back("Sci-Fi");
			insert_data.push_back("Star wars");
		//

		for (int i = 0; i < 10; ++i)
		{
			ret = movie.insert(insert_data);
			if (!ret)
			{
				std::cout << "failed to insert\n";
			}
			std::cout << "insert : " << ret << "\n";
		}

		ret = movie.select_where_index('>', 0, 10, 0);
		if (!ret)
		{
			std::cout << "failed to select" << "\n";
		}
		std::cout << "select : " << ret << "\n";

		mysqlhs::result rs(movie);
		std::cout << "num_rows : " << rs.num_rows() << "\n";

		while (rs.fetch())
		{
			std::cout << rs.get<int32_t>("id") << "\n";
			std::cout << rs.get<std::string>("genre") << "\n";
			std::cout << rs.get<std::string>("title") << "\n";
			std::cout << rs.get<int32_t>("view_count") << "\n";
			std::cout << "\n";
		}

		std::vector<std::string> update_data;
		//
			update_data.push_back("11");
			update_data.push_back("Sci-Fi updated");
			update_data.push_back("Star wars updated");
			update_data.push_back("101");
		//

		ret = movie.update_where_index('=', 10, update_data);
		if (!ret)
		{
			std::cout << "failed to update" << "\n";
		}
		std::cout << "update : " << movie.conn().affected_rows() << "\n";

		ret = movie.delete_where_index('=', 9);
		if (!ret)
		{
			std::cout << "failed to delete\n";
		}
		std::cout << "delete : " << movie.conn().affected_rows() << "\n";
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cin.get();
	return 0;
}
