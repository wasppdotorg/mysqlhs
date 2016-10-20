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

std::cout << "_" << oss.str() << "_" << std::endl;
		mysqlhs_execute(mysqlhs_, oss.str().c_str());
		if (mysqlhs_->result != MYSQL_HS_OK)
		{
			raw_data_ = "1\n";
			return false;
		}

		clear();
		raw_data_ = mysqlhs_->data;
		std::cout << raw_data_ << std::endl;

		return true;
	}

	bool connection::is_okay()
	{
		return (raw_data_[0] == '0');
	}

	void connection::split(const std::string &s, char c, std::vector<std::string>& v)
	{
		std::stringstream ss;
		ss.str(s);

		std::string item;
		while (std::getline(ss, item, c))
		{
			v.push_back(item);
		}
	}

	const std::string& connection::raw_data()
	{
		return raw_data_;
	}

	uint64_t connection::affected_rows()
	{
		if (query_type_ != query_type::update_ && query_type_ != query_type::delete_)
		{
			return -1;
		}

		if (raw_data_.size() < 1)
		{
			return -1;
		}

		auto found = raw_data_.find_last_of("\t");
		return strtoull(raw_data_.substr(found + 1).c_str(), nullptr, 0);
	}

} // namespace mysqlhs
