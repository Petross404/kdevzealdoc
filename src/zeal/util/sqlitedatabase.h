/****************************************************************************
 * *                                *
 ** Copyright (C) 2016 Jerzy Kozera
 ** Contact: https://go.zealdocs.org/l/contact
 **
 ** This file is part of Zeal.
 **
 ** Zeal is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** Zeal is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with Zeal. If not, see <https://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef ZEAL_UTIL_SQLITEDATABASE_H
#define ZEAL_UTIL_SQLITEDATABASE_H

#include <sqlite3.h>

#include <QStringList>
#include <QVariant>
#include <memory>

/*!
 * \brief Custom deleter for sqlite3.
 *
 * This struct is used as a custom deleter for `std::unique_ptr<sqlite3>`.
 * It ensures that the `sqlite3_close` function is called when the `std::unique_ptr<sqlite3>`
 * goes out of scope, properly closing the SQLite database connection.
 */
struct SQLite3Deleter
{
	/*!
	 * \brief Deletes the sqlite3 database handle.
	 *
	 * This function is invoked when the `std::unique_ptr<sqlite3>` is destroyed.
	 * It checks if the database handle is valid and if so, calls `sqlite3_close`
	 * to close the database connection and release any associated resources.
	 *
	 * \param db A pointer to the sqlite3 database handle to close.
	 */
	void operator()( sqlite3* db ) const
	{
		if ( db ) { sqlite3_close( db ); }
	}
};

/*!
 * \brief Custom deleter for sqlite3_stmt.
 *
 * This struct is used as a custom deleter for `std::unique_ptr<sqlite3_stmt>`.
 * It ensures that the `sqlite3_finalize` function is called when the `std::unique_ptr<sqlite3_stmt>`
 * goes out of scope, properly finalizing the SQLite prepared statement.
 */
struct SQLite3StmtDeleter
{
	/*!
	 * \brief Finalizes the sqlite3_stmt prepared statement.
	 *
	 * This function is invoked when the `std::unique_ptr<sqlite3_stmt>` is destroyed.
	 * It checks if the statement pointer is valid and if so, calls `sqlite3_finalize`
	 * to release resources associated with the prepared statement.
	 *
	 * \param stmt A pointer to the sqlite3_stmt prepared statement to finalize.
	 */
	void operator()( sqlite3_stmt* stmt )
	{
		if ( stmt ) { sqlite3_finalize( stmt ); }
	}
};

namespace Zeal { namespace Util {

/*!
 * \class SQLiteDatabase
 * \brief Encapsulates SQLite database management and querying in a Qt-compatible way.
 *
 * This class provides methods for opening, querying, and retrieving results from SQLite
 * databases while ensuring thread safety using SQLite's mutexes.
 */

/*!
 * \brief Constructor that initializes and attempts to open an SQLite database.
 *
 * \param path The file path to the SQLite database file.
 */
class SQLiteDatabase
{
public:
	explicit SQLiteDatabase( const QString& path );

	/*!
	 * \brief Destructor that finalizes the current statement and closes the database connection.
	 */
	virtual ~SQLiteDatabase();

	/*!
	 * \brief Checks if the database is currently open.
	 *
	 * \return True if the database is open, false otherwise.
	 */
	[[nodiscard]] bool isOpen() const;

	/*!
	 * \brief Retrieves the names of all tables in the database.
	 *
	 * \return A QStringList containing the names of all tables.
	 */
	QStringList tables();

	/*!
	 * \brief Executes an SQL query.
	 *
	 * \param queryStr The SQL query string to execute.
	 * \return True if the query was executed successfully, false otherwise.
	 */
	bool execute( const QString& queryStr );

	/*!
	 * \brief Advances to the next row in the result set of the current query.
	 *
	 * \return True if there is another row, false otherwise.
	 */
	bool next();

	/*!
	 * \brief Retrieves the value of a column in the current row of the result set.
	 *
	 * \param index The column index to retrieve.
	 * \return A QVariant containing the value of the column.
	 */
	[[nodiscard]] QVariant value( int index ) const;

	/*!
	 * \brief Retrieves the last error message from SQLite.
	 *
	 * \return A QString containing the error message.
	 */
	[[nodiscard]] QString lastError() const;

	/*!
	 * \brief Retrieves the raw SQLite database handle.
	 *
	 * \return A pointer to the SQLite database.
	 */
	[[nodiscard]] sqlite3* handle() const;

private:
	/*!
	 * \brief Closes the database connection.
	 */
	void close();
	/*!
	 * \brief Finalizes the current prepared statement.
	 */
	void finalize();
	/*!
	 * \brief Updates the last error message from SQLite.
	 */
	void updateLastError();

	std::unique_ptr<sqlite3, SQLite3Deleter>               m_db;
	std::unique_ptr<sqlite3_stmt, SQLite3StmtDeleter>      m_stmt;

	QString m_lastError;
};

}}    // namespace Zeal::Util

#endif	  // ZEAL_UTIL_SQLITEDATABASE_H
