/*
Copyright (c) 2016-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef mysqlhs_hpp
#define mysqlhs_hpp

#include <cstdint>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

#include "mysqlhs.h"

namespace mysqlhs
{

	enum class query_type { none_ = 0, select_ = 1, insert_ = 2, update_ = 3, delete_ = 4 };

	class connection
	{
	public:
		connection(const std::string& host, int port);
		~connection();

		int new_index_id() { return ++index_id_; }
		void clear();

		template<typename T>
		std::ostringstream& operator<<(T v)
		{
			oss << v;
			return oss;
		}

		bool query(query_type type_);
		bool is_okay();

		void split(const std::string &s, char c, std::vector<std::string>& v);

		const std::string& raw_data();
		uint64_t affected_rows();

	private:
		mysqlhs_context* mysqlhs_;

		int index_id_;
		query_type query_type_;
		std::string raw_data_;

		std::ostringstream oss;

	};

	/*
	CREATE TABLE movie (
		id int not null auto_increment primary key,
		genre varchar(20) not null,
		title varchar(100) not null,
		view_count int default 0,
		key(genre)
	) engine innodb;
	*/

	class index
	{
	public:
		index(connection& conn, const std::string& db, const std::string& table, const std::string& index_name, const std::string& columns);
		~index();

		connection& conn() { return conn_; }
		const std::vector<std::string>& columns() { return columns_; }

		bool open();

		//idx,cond,vlen,val,lmt,offs
		// "1	=	1	10	1	0\n"
		template<typename T>
		bool select_where_index(char condition, T value, int limit = 1, int offset = 0)
		{
			conn_.clear();
			conn_ << index_id_ << "\t" << condition << "\t1\t" << value << "\t" << limit << "\t" << offset << "\n";

			if (!conn_.query(query_type::select_) || !conn_.is_okay())
			{
				return false;
			}

			return true;
		}

		//idx,cmd,vlen,	v1,	v2,		v3
		// "1	+	3	0	Sci-Fi	Star wars\n"
		bool insert(const std::vector<std::string>& params);

		//idx,cond,vlen,val,lmt,off,cmd,v1, v2,		v3,			v4
		// "1	=	1	10	1	0	U	11	Sci-Fi	Star Wars	101\n"
		template<typename T>
		bool update_where_index(char condition, T value, const std::vector<std::string>& params)
		{
			conn_.clear();
			conn_ << index_id_ << "\t" << condition << "\t1\t" << value << "\t1\t0\tU";

			for (auto& p : params)
			{
				conn_ << "\t" << p;
			}
			conn_ << "\n";

			if (!conn_.query(query_type::update_) || !conn_.is_okay())
			{
				return false;
			}

			return true;
		}

		//idx,cond,vlen,val,lmt,off,cmd
		// "1	=	1	9	1	0	D\n"
		template<typename T>
		bool delete_where_index(char condition, T value)
		{
			conn_.clear();
			conn_ << index_id_ << "\t" << condition << "\t1\t" << value << "\t1\t0\tD\n";

			if (!conn_.query(query_type::delete_) || !conn_.is_okay())
			{
				return false;
			}

			return true;
		}

	private:
		connection& conn_;
		std::size_t index_id_;
		std::vector<std::string> columns_;

	};

	class result
	{
	public:
		result(index& idx);
		~result();

		std::size_t num_rows();
		bool fetch();

		template<typename T>
		T get(const std::string& column)
		{
			T value;
			set_value(column, value);

			return value;
		}

	private:
		void set_value(const std::string& column, uint8_t& value);

		void set_value(const std::string& column, int16_t& value);
		void set_value(const std::string& column, uint16_t& value);

		void set_value(const std::string& column, int32_t& value);
		void set_value(const std::string& column, uint32_t& value);

		void set_value(const std::string& column, int64_t& value);
		void set_value(const std::string& column, uint64_t& value);

		void set_value(const std::string& column, float& value);
		void set_value(const std::string& column, double& value);

		void set_value(const std::string& column, std::string& value);

		std::size_t i;
		std::vector<std::string> data_;
		const std::vector<std::string>& columns_;
		std::unordered_map<std::string, std::string> row_;

	};

} // mysqlhs

#endif // mysqlhs_hpp
