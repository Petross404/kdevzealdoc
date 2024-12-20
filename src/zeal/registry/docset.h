
/****************************************************************************
 * *              *
 ** Copyright (C) 2015-2016 Oleg Shparber
 ** Copyright (C) 2013-2014 Jerzy Kozera
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

#ifndef DOCSET_H
#define DOCSET_H

#include <util/sqlitedatabase.h>

#include <QIcon>
#include <QMap>
#include <QMetaObject>
#include <QUrl>
#include <memory>

namespace Zeal {

// namespace Util
// {
// class SQLiteDatabase;
// }

namespace Registry {

struct CancellationToken;
struct SearchResult;

/*!
 * \brief Represents the concept of docsets and their interaction with KDevelop.
 *
 * Docsets are structured archives of documentation files used by tools like
 * Dash, Zeal, or other documentation browsers. They typically include:
 *
 * - **Documentation Content**: Often in HTML format, organized for offline browsing.
 * - **Metadata Database**: Usually a SQLite database (`docSet.dsidx`), which
 *   indexes all symbols (functions, classes, methods, etc.) for fast searching.
 * - **Resources**: Additional assets like CSS, JS, or images.
 *
 * \section interaction_with_kdevelop Interaction with KDevelop
 * If your plugin integrates with KDevelop, its role is likely to:
 *
 * - Locate installed docsets.
 * - Parse and index their contents.
 * - Allow users to search the docsets via a UI in KDevelop.
 * - Render the relevant documentation in a view (using a web rendering engine, for example).
 */

class Docset
{
public:
	explicit Docset( const QString& path );
	~Docset();

	Docset( const Docset& dc ) = delete;
	Docset( Docset&& dc )	   = delete;

	Docset& operator=( const Docset& dc ) = delete;
	Docset& operator=( Docset&& dc )      = delete;

	bool isValid() const;

	QString	    name() const;
	QString	    title() const;
	QStringList keywords() const;

	QString version() const;
	QString revision() const;

	QString path() const;
	QString documentPath() const;
	QIcon	icon() const;
	QIcon	symbolTypeIcon( const QString& symbolType ) const;
	QUrl	indexFileUrl() const;

	QMap<QString, int> symbolCounts() const;
	int		   symbolCount( const QString& symbolType ) const;

	const QMap<QString, QUrl>& symbols( const QString& symbolType ) const;

	QList<SearchResult> search( const QString& query, const CancellationToken& token ) const;
	QList<SearchResult> relatedLinks( const QUrl& url ) const;

	// FIXME: This is an ugly workaround before we have a proper docset sources implementation
	bool hasUpdate = false;

private:
	enum class Type { Invalid, Dash, ZDash };

	void loadMetadata();
	void countSymbols();
	void loadSymbols( const QString& symbolType ) const;
	void loadSymbols( const QString& symbolType, const QString& symbolString ) const;
	void createIndex();
	QUrl createPageUrl( const QString& path, const QString& fragment = QString{} ) const;

	static QString parseSymbolType( const QString& str );

	QString	     m_name;
	QString	     m_title;
	QStringList  m_keywords;
	QString	     m_version;
	QString	     m_revision;
	Docset::Type m_type = Type::Invalid;
	QString	     m_path;
	QString	     m_documentPath;	// FIXME add to github master
	QIcon	     m_icon;

	QUrl m_indexFileUrl;

	QMultiMap<QString, QString>			m_symbolStrings;
	QMap<QString, int>				m_symbolCounts;
	mutable QMap<QString, QMultiMap<QString, QUrl>> m_symbols;
	std::unique_ptr<Util::SQLiteDatabase>		m_db = nullptr;
};

}    // namespace Registry
}    // namespace Zeal

#endif	  // DOCSET_H
