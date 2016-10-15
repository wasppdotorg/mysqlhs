/*
Copyright (c) 2016-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include "mysqlhs.hpp"

namespace mysqlhs
{

	connection::connection(const std::string& host, int port)
		: index_id_(-1), query_type_(query_type::none_)
	{
		mysqlhs_ = mysqlhs_connect(host.c_str(), port);
		if (mysqlhs_ == NULL || mysqlhs_->result != MYSQL_HS_OK)
		{
			throw std::runtime_error("mysqlhs_connect failed");
		}
	}

	connection::~connection()
	{
		mysqlhs_close(mysqlhs_);
	}

	void connection::clear()
	{
		oss.str(std::string());
		oss.clear();
	}

	bool connection::query(query_type type_)
	{
		query_type_ = type_;

		mysqlhs_execute(mysqlhs_, oss.str().c_str());
		if (mysqlhs_->result != MYSQL_HS_OK)
		{
			raw_data_ = "1\n";
			return false;
		}

		clear();
		raw_data_ = mysqlhs_->data;;
		raw_data_.pop_back(); // trim

		return true;
	}

	bool connection::is_okay()
	{
		return (raw_data_[0] == '0');
	}

	const std::string& connection::raw_data()
	{
		return raw_data_;
	}

	int connection::affected_rows()
	{
		if (query_type_ != query_type::update_ && query_type_ != query_type::delete_)
		{
			return -1;
		}

		if (raw_data_.size() < 1)
		{
			return -1;
		}

		return boost::lexical_cast<int>(raw_data_[raw_data_.size() - 1]);
	}

} // namespace mysqlhs
