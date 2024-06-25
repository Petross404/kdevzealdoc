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

#include "zealdocprovider.h"
#include "zealdocumentation.h"
#include "debug.h"

#include "registry/docset.h"

#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/declaration.h>

#include <KPluginFactory>

#include <QRegularExpression>
#include <QStringList>

using namespace KDevelop;

ZealdocProvider::ZealdocProvider( const QString& docsetPath, QObject* parent )
	: QObject( parent )
{
	Zeal::Registry::Docset ds( docsetPath );

	m_isValid = ds.isValid();

	if ( !isValid() )
	{
		return;
	}

	m_name = ds.title();
	m_icon = ds.icon();

	QStringList allTokens;

	auto tokenGroups = ds.symbolCounts();
	QMapIterator<QString, int> i( tokenGroups );

	while ( i.hasNext() )
	{
		i.next();

		QString groupName = i.key();
		auto groupTokens = ds.symbols( groupName );

		if ( !groupTokens.isEmpty() )
		{
			m_tokenGroups << groupName;
		}

		QMapIterator<QString, QUrl> j( groupTokens );

		while ( j.hasNext() )
		{
			j.next();

			QString token = j.key();
			QUrl url = j.value();

			m_tokenUrls[token] = url;
			m_groupTokens[groupName] << token;
		}
	}

	QMapIterator<QString, QUrl> k( m_tokenUrls );

	while ( k.hasNext() )
	{
		k.next();
		allTokens << k.key();
	}

	m_model = new QStringListModel( this );
	m_model->setStringList( allTokens );
}

ZealdocProvider::~ZealdocProvider() = default;

bool ZealdocProvider::isValid()
{
	return m_isValid;
}

QIcon ZealdocProvider::icon() const
{
	return m_icon;
}

QString ZealdocProvider::name() const
{
	return m_name;
}

IDocumentation::Ptr ZealdocProvider::homePage() const
{
	ZealDocumentation::m_provider = const_cast<ZealdocProvider*>( this );
	return IDocumentation::Ptr( new ZealDocumentationHome );
}

KDevelop::IDocumentation::Ptr ZealdocProvider::documentation( const QUrl& url ) const
{
	auto token = m_tokenUrls.key( url );
	return documentationForToken( token );
}

IDocumentation::Ptr ZealdocProvider::documentationForDeclaration( Declaration* dec ) const
{
	if ( dec )
	{
		static const IndexedString qmlJs( "QML/JS" );
		QString token;

		{
			DUChainReadLocker lock;
			token = dec->qualifiedIdentifier().toString( RemoveTemplateInformation );

			if ( dec->topContext()->parsingEnvironmentFile()->language() == qmlJs && !token.isEmpty() )
			{
				token = QLatin1String( "QML." ) + token;
			}
		}

		return documentationForToken( token );
	}

	return {};
}

IDocumentation::Ptr ZealdocProvider::documentationForIndex( const QModelIndex& index ) const
{
	return documentationForToken( index.data( Qt::DisplayRole ).toString() );
}

KDevelop::IDocumentation::Ptr ZealdocProvider::documentationForToken( const QString& token ) const
{
	if ( !token.isEmpty() )
	{
		const QUrl url = m_tokenUrls[token];

		if ( url.isValid() )
		{
			ZealDocumentation::m_provider = const_cast<ZealdocProvider*>( this );
			return IDocumentation::Ptr( new ZealDocumentation( token, url ) );
		}
	}

	return {};
}

QAbstractListModel* ZealdocProvider::indexModel() const
{
	return m_model;
}

QStringList ZealdocProvider::tokenGroups() const
{
	return m_tokenGroups;
}

QIcon ZealdocProvider::groupIcon( const QString& group )
{
	return QIcon::fromTheme( QStringLiteral( "zealdoc_%1" ).arg( group.toLower() ) );
}

QStringList ZealdocProvider::groupTokens( const QString& group ) const
{
	return m_groupTokens[group];
}
