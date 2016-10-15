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
	try
	{
		mysqlhs::connection c("127.0.0.1", 9999);
		mysqlhs::index movie(&c, "test", "movie", "PRIMARY", "id,genre,title,view_count");

		bool ret = movie.open();
		if (!ret)
		{
			std::cout << "failed to open index" << std::endl;
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
				std::cout << "failed to insert" << std::endl;
			}
			std::cout << "insert : " << ret << std::endl;
		}

		ret = movie.select_where_index('>', 0, 10, 0);
		if (!ret)
		{
			std::cout << "failed to select" << std::endl;
		}
		std::cout << "select : " << ret << std::endl;

		mysqlhs::result rs(&movie);
		std::cout << "num_rows : " << rs.num_rows() << std::endl;

		while (rs.fetch())
		{
			std::cout << rs.get<int>("id") << std::endl;
			std::cout << rs.get_str("genre") << std::endl;
			std::cout << rs.get_str("title") << std::endl;
			std::cout << rs.get<int>("view_count") << std::endl;
			std::cout << std::endl;
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
			std::cout << "failed to update" << std::endl;
		}
		std::cout << "update : " << movie.conn()->affected_rows() << std::endl;

		ret = movie.delete_where_index('=', 9);
		if (!ret)
		{
			std::cout << "failed to delete" << std::endl;
		}
		std::cout << "delete : " << movie.conn()->affected_rows() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.get();
	return 0;
}
