/****************************************************************************
 * * *
 ** Copyright (C) 2015-2016 Oleg Shparber
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

#include "plist.h"

#include <QFile>
#include <QXmlStreamReader>

using namespace Zeal::Util;

Plist::Plist()
	: QHash<QString, QVariant>()
	, m_hasError( false )	 // Initialize the base QHash and error state
{}

bool Plist::read( const QString& fileName )
{
	// Open the Plist file for reading
	const std::unique_ptr<QFile> file{ std::make_unique<QFile>( fileName ) };

	if ( !file->open( QIODevice::ReadOnly ) )
	{
		// Log an error or report failure
		// TODO: Report/log error properly
		m_hasError = true;
		return false;
	}

	// Set up the XML reader
	QXmlStreamReader xml( file.get() );

	// Read the XML file
	while ( !xml.atEnd() )
	{
		const QXmlStreamReader::TokenType token = xml.readNext();

		// Skip unnecessary tokens
		if ( token == QXmlStreamReader::StartDocument
		     || token != QXmlStreamReader::StartElement )
			continue;

		// Process only elements named "key"
		if ( xml.name() != QLatin1String( "key" ) )
			continue;    // Skip unknown or irrelevant tags

		// Read the key name
		const QString key = xml.readElementText();

		// Skip whitespace characters between tags
		while ( xml.readNext() == QXmlStreamReader::Characters );

		// Ensure the next token is a StartElement
		if ( xml.tokenType() != QXmlStreamReader::StartElement ) continue;

		QVariant value;

		// Determine the type of the value and read it
		if ( xml.name() == QLatin1String( "string" ) )
			value = xml.readElementText();
		else if ( xml.name() == QLatin1String( "true" ) )
			value = true;
		else if ( xml.name() == QLatin1String( "false" ) )
			value = false;
		else
			continue;    // Skip unknown or unsupported types

		// Insert the key-value pair into the hash
		insert( key, value );
	}

	// Return the error state
	return !m_hasError;
}

bool Plist::hasError() const { return m_hasError; }
