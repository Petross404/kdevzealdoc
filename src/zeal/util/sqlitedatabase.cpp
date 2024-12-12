/****************************************************************************
 * * *
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

#include "sqlitedatabase.h"

#include "macros.hpp"

Zeal::Util::SQLiteDatabase::SQLiteDatabase( const QString& path )
	: m_db{ nullptr, SQLite3Deleter{} }
	, m_stmt{ nullptr, SQLite3StmtDeleter{} }
{
	if ( sqlite3_initialize() != SQLITE_OK ) return;    // Initialize SQLite library

	sqlite3* raw_db{ m_db.get() };
	if ( sqlite3_open16( path.constData(), &raw_db ) != SQLITE_OK )
	{
		updateLastError();    // Store the error message
		close();	      // Ensure resources are cleaned up
		sqlite3_close( raw_db );
	}

	m_db.reset( raw_db );
}

Zeal::Util::SQLiteDatabase::~SQLiteDatabase()
{
	finalize();    // Clean up prepared statements
	close();       // Close the database connection
}

bool Zeal::Util::SQLiteDatabase::isOpen() const { return m_db != nullptr; }

QStringList Zeal::Util::SQLiteDatabase::tables()
{
	Q_ASSERT_X_CUSTOM( !m_stmt, location(), "Null pointer" );

	QStringList res;

	if ( !isOpen() ) { return res; }

	const QLatin1String sql{
		"SELECT name FROM sqlite_master WHERE type='table' UNION ALL "
		"SELECT name FROM sqlite_temp_master WHERE type='table'" };

	if ( !sql.isEmpty() && execute( sql ) )
	{
		while ( next() ) { res.append( value( 0 ).toString() ); }
	}

	return res;
}

bool Zeal::Util::SQLiteDatabase::execute( const QString& queryStr )
{
	if ( !m_db ) return false;

	if ( m_stmt ) { finalize(); }	 // Finalize any active statement

	m_lastError.clear();

	sqlite3_mutex_enter( sqlite3_db_mutex( m_db.get() ) );	  // Lock the database for thread safety
	const void*   pzTail = nullptr;
	sqlite3_stmt* raw{ m_stmt.get() };
	const int     res{ sqlite3_prepare16_v2( m_db.get(),
						 queryStr.constData(),
						 ( queryStr.size() + 1 ) * sizeof( QChar ),
						 &raw,
						 &pzTail ) };

	m_stmt.reset( raw );
	sqlite3_mutex_leave( sqlite3_db_mutex( m_db.get() ) );	  // Unlock the database

	if ( res != SQLITE_OK )
	{
		updateLastError();    // Store the error
		finalize();
		sqlite3_finalize( raw );
		return false;
	}
	else if ( pzTail
		  && !QString( reinterpret_cast<const QChar*>( pzTail ) ).trimmed().isEmpty() )
	{
		// Ensure no multiple statements are executed
		updateLastError();
		finalize();
		return false;
	}

	return true;
}

bool Zeal::Util::SQLiteDatabase::next()
{
	if ( !m_stmt ) return false;

	sqlite3_mutex_enter( sqlite3_db_mutex( m_db.get() ) );	  // Lock the database
	const int res = sqlite3_step( m_stmt.get() );
	sqlite3_mutex_leave( sqlite3_db_mutex( m_db.get() ) );	  // Unlock the database

	switch ( res )
	{
		case SQLITE_ROW: return true;	 // Row available
		case SQLITE_DONE:		 // Query completed
		case SQLITE_CONSTRAINT:		 // Constraint violation
		case SQLITE_ERROR:		 // General error
		case SQLITE_MISUSE:		 // Misuse of API
		case SQLITE_BUSY:		 // Database is busy
		default: updateLastError();	 // Handle any errors
	}

	return false;
}

QVariant Zeal::Util::SQLiteDatabase::value( int index ) const
{
	Q_ASSERT_X( index >= 0, "SQLiteDatabase::value", "Index must be non-negative." );

	if ( index >= sqlite3_data_count( m_stmt.get() ) )
	{
		return {};    // Invalid index
	}

	sqlite3_mutex_enter( sqlite3_db_mutex( m_db.get() ) );	  // Lock the database
	const int type = sqlite3_column_type( m_stmt.get(), index );

	QVariant ret;

	switch ( type )
	{
		case SQLITE_INTEGER:
			ret = sqlite3_column_int64( m_stmt.get(), index );
			break;

		case SQLITE_NULL:
			ret = QVariant{ QVariant::String };    // Represent NULL as empty string
			break;

		default:
			ret = QString( reinterpret_cast<const QChar*>(
					       sqlite3_column_text16( m_stmt.get(), index ) ),
				       sqlite3_column_bytes16( m_stmt.get(), index )
					       / sizeof( QChar ) );
			break;
	}

	sqlite3_mutex_leave( sqlite3_db_mutex( m_db.get() ) );	  // Unlock the database
	return ret;
}

QString Zeal::Util::SQLiteDatabase::lastError() const { return m_lastError; }

void Zeal::Util::SQLiteDatabase::close()
{
	sqlite3_close( m_db.get() );
	m_db = nullptr;
}

void Zeal::Util::SQLiteDatabase::finalize()
{
	sqlite3_finalize( m_stmt.get() );
	m_stmt = nullptr;
}

void Zeal::Util::SQLiteDatabase::updateLastError()
{
	if ( !m_db ) return;

	m_lastError =
		QString( reinterpret_cast<const QChar*>( sqlite3_errmsg16( m_db.get() ) ) );
}

sqlite3* Zeal::Util::SQLiteDatabase::handle() const { return m_db.get(); }
