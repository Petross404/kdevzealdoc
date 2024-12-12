/* This file is part of KDevelop
 **  Copyright 2016 Anton Anikin <anton.anikin@htower.ru>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "util.h"

#include <util/path.h>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QString>

#include "debug.h"
#include "registry/docset.h"

const KConfigGroup zealdocConfig()
{
	const KConfigGroup cfg_gr{ KSharedConfig::openConfig(), QStringLiteral( "Zealdoc" ) };
	return cfg_gr;
}

QString defaultDocsetsPath()
{
	QDir baseDataDir{ QStandardPaths::writableLocation( QStandardPaths::DataLocation ) };

	if ( !QCoreApplication::applicationName().isEmpty() ) { baseDataDir.cdUp(); }

	if ( !QCoreApplication::organizationName().isEmpty() ) { baseDataDir.cdUp(); }

	const QString docsetsPath{ baseDataDir.canonicalPath()
				   + QStringLiteral( "/Zeal/Zeal/docsets/" ) };
	qDebug() << "docsetsPath" << docsetsPath;

	if ( QDir{ docsetsPath }.exists() )
	{
		return QDir{ docsetsPath }.canonicalPath();
	}

	return QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
}

QString docsetsPath()
{
	return zealdocConfig().readEntry( QStringLiteral( "DocsetsPath" ),
					  defaultDocsetsPath() );
}

QStringList enabledDocsets()
{
	return zealdocConfig().readEntry( QStringLiteral( "EnabledDocsets" ), QStringList{} );
}

QList<DocsetInformation> availableDocsets( const QString& docsetsPath )
{
	QList<DocsetInformation> docsets;

	// Ensure correct file path concatenation using QDir::filePath
	const QDir	  docsetDir{ docsetsPath };
	const QStringList docsetFsNames{
		docsetDir.entryList( QDir::Dirs | QDir::NoDot | QDir::NoDotDot ) };

	// Reserve space in the list for efficiency if known in advance
	docsets.reserve( docsetFsNames.size() );

	// Iterate over each directory (docset)
	for ( const auto& docsetFsName : docsetFsNames )
	{
		const Zeal::Registry::Docset ds{ docsetDir.filePath( docsetFsName ) };

		// Skip invalid docsets...
		if ( !ds.isValid() ) { continue; }
		// ... and store the valid docsets
		docsets << DocsetInformation{ ds.path(), ds.title(), ds.icon(), ds.isValid() };
	}
	return docsets;
}
