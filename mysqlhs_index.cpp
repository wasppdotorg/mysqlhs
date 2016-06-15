
#include <boost/algorithm/string/join.hpp>

#include "mysqlhs_index.hpp"

namespace mysqlhs
{

	class connection;

	index::index(connection* conn, const std::string& db, const std::string& table, const std::string& index_name, const std::string& columns)
		: conn_(conn), index_id_(conn_->new_index_id())
	{
		conn_->clear_();
		*conn_ << "P\t" << index_id_ << "\t" << db << "\t" << table << "\t" << index_name << "\t" << columns << "\n";
	}

	bool index::open_()
	{
		if (!conn_->query(query_type::none_) || !conn_->is_okay())
		{
			return false;
		}

		return true;
	}

	bool index::insert_(const std::vector<std::string>& params)
	{
		conn_->clear_();
		*conn_ << index_id_ << "\t+\t" << params.size() << "\t" << boost::algorithm::join(params, "\t") << "\n";

		if (!conn_->query(query_type::insert_) || !conn_->is_okay())
		{
			return false;
		}

		return true;
	}

} // namespace mysqlhs
