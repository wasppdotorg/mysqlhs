
#include "mysqlhs.hpp"

namespace mysqlhs
{

	result::result(index* idx)
		: idx_(idx), i(1), data_size_(0), column_size_(0), columns_(idx->columns())
	{
		boost::split(data_, idx->conn()->raw_data(), boost::is_any_of("\t"));
		data_size_ = data_.size();

		column_size_ = boost::lexical_cast<std::size_t>(data_[1]);
	}

	result::~result()
	{

	}

	void result::test()
	{
		for (std::size_t j = 0; j < columns_.size(); ++j)
		{
			//std::cout << columns_[j] << std::endl;
		}

		for (; i < data_.size(); ++i)
		{
			std::cout << "i:" << i << std::endl;

			std::cout << data_[i];
			if (i % column_size_ == 1)
			{
				break;
			}
		}
	}

	bool result::fetch()
	{
		row_.clear();

		std::cout << i << std::endl;
		if (i > data_size_)
		{
			return false;
		}

		std::cout << column_size_ << std::endl;
		for (std::size_t j; j < column_size_; ++j)
		{
			std::cout << columns_[j] << std::endl;
			row_.insert(std::make_pair(columns_[j], data_[++i]));
		}

		return true;
	}

} // namespace mysqlhs
