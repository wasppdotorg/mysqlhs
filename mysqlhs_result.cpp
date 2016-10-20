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

	result::result(index& idx)
		: i(2), columns_(idx.columns())
	{
		idx.conn().split(idx.conn().raw_data(), '\t', data_);

		for (auto& c : columns_)
		{
			row_.insert(std::make_pair(c, ""));
		}
	}

	result::~result()
	{

	}

	std::size_t result::num_rows()
	{
		return (data_.size() - 2) / columns_.size();
	}

	bool result::fetch()
	{
		if (i >= data_.size())
		{
			return false;
		}

		for (std::size_t j = 0; j < columns_.size(); ++i, ++j)
		{
			row_[columns_[j]] = data_[i];
		}

		return true;
	}
	
	void result::set_value(const std::string& column, uint8_t& value)
	{
		value = atoi(row_[column].c_str());
	}

	void result::set_value(const std::string& column, int16_t& value)
	{
		value = atoi(row_[column].c_str());
	}

	void result::set_value(const std::string& column, uint16_t& value)
	{
		value = atoi(row_[column].c_str());
	}

	void result::set_value(const std::string& column, int32_t& value)
	{
		value = strtol(row_[column].c_str(), nullptr, 0);
	}

	void result::set_value(const std::string& column, uint32_t& value)
	{
		value = strtoul(row_[column].c_str(), nullptr, 0);
	}

	void result::set_value(const std::string& column, int64_t& value)
	{
		value = strtoll(row_[column].c_str(), nullptr, 0);
	}

	void result::set_value(const std::string& column, uint64_t& value)
	{
		value = strtoull(row_[column].c_str(), nullptr, 0);
	}

	void result::set_value(const std::string& column, float& value)
	{
		value = strtof(row_[column].c_str(), nullptr);
	}

	void result::set_value(const std::string& column, double& value)
	{
		value = strtod(row_[column].c_str(), nullptr);
	}

	void result::set_value(const std::string& column, std::string& value)
	{
		value = row_[column];
	}

} // namespace mysqlhs
