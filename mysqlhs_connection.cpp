
#include "mysqlhs.hpp"

namespace mysqlhs
{

	connection::connection(const std::string& host, const std::string& port)
		: socket_(io_service_), req_stream(&req_buf), res_stream(&res_buf), index_id_(0), query_type_(query_type::none_)
	{
		try
		{
			boost::asio::ip::tcp::resolver resolver_(io_service_);
			boost::asio::ip::tcp::resolver::query query_(host, port);
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver_.resolve(query_);

			boost::asio::connect(socket_, endpoint_iterator);
		}
		catch (...)
		{
			throw;
		}
	}

	connection::~connection()
	{
		socket_.close();
	}

	void connection::clear_()
	{
		oss.str(std::string());
		oss.clear();
	}

	bool connection::query(query_type type_)
	{
		query_type_ = type_;

		req_stream << oss.str();
		boost::asio::write(socket_, req_buf);

		boost::asio::read(socket_, res_buf, boost::asio::transfer_at_least(1), error);
		if (error)
		{
			raw_data_ = "1\n";
			return false;
		}

		clear_();
		oss << &res_buf;
		raw_data_ = oss.str();

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

		if (raw_data_.size() < 2)
		{
			return -1;
		}

		return boost::lexical_cast<int>(raw_data_[raw_data_.size() - 2]);
	}

} // namespace mysqlhs
