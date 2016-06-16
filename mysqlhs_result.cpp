
#include "mysqlhs.hpp"

namespace mysqlhs
{

	result::result(index* idx)
		: idx_(idx), i(2), columns_(idx->columns())
	{
		boost::split(data_, idx->conn()->raw_data(), boost::is_any_of("\t"));

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

} // namespace mysqlhs
