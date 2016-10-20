/*
Copyright (c) 2016-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <sstream>
#include <algorithm>
#include <iterator>

#include "mysqlhs.hpp"

namespace mysqlhs
{

	index::index(connection& conn, const std::string& db, const std::string& table, const std::string& index_name, const std::string& columns)
		: conn_(conn), index_id_(conn_.new_index_id())
	{
		conn_.clear();
		conn_ << "P\t" << index_id_ << "\t" << db << "\t" << table << "\t" << index_name << "\t" << columns << "\n";

		conn_.split(columns, ',', columns_);
	}

	index::~index()
	{

	}

	bool index::open()
	{
		if (!conn_.query(query_type::none_) || !conn_.is_okay())
		{
			return false;
		}

		return true;
	}

	bool index::insert(const std::vector<std::string>& params)
	{
		conn_.clear();
		conn_ << index_id_ << "\t+\t" << params.size();

		for (auto& p : params)
		{
			conn_ << "\t" << p;
		}
		conn_ << "\n";

		if (!conn_.query(query_type::insert_) || !conn_.is_okay())
		{
			return false;
		}

		return true;
	}

} // namespace mysqlhs
