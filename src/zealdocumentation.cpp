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

#include "zealdocumentation.h"
#include "zealdocprovider.h"

#include <interfaces/icore.h>
#include <interfaces/idocumentationcontroller.h>
#include <documentation/standarddocumentationview.h>

#include <KLocalizedString>

#include <QTreeView>

using namespace KDevelop;

ZealdocProvider* ZealDocumentation::m_provider = nullptr;

ZealDocumentation::ZealDocumentation( const QString& name, const QUrl& url )
	: m_name( name )
	, m_url( url )
{
}

QWidget* ZealDocumentation::documentationWidget( DocumentationFindWidget* findWidget, QWidget* parent )
{
	auto view = new StandardDocumentationView( findWidget, parent );
	view->load( m_url );

	return view;
}

QString ZealDocumentation::name() const
{
	return m_name;
}

QString ZealDocumentation::description() const
{
	return m_name;
}

IDocumentationProvider* ZealDocumentation::provider() const
{
	return m_provider;
}

// =================================================================================================

ZealDocumentationHome::ZealDocumentationHome()
{
}

QString ZealDocumentationHome::name() const
{
	return i18n( "%1 Content Page", ZealDocumentation::m_provider->name() );
}

QString ZealDocumentationHome::description() const
{
	return name();
}

QWidget* ZealDocumentationHome::documentationWidget( DocumentationFindWidget* findWidget, QWidget* parent )
{
	Q_UNUSED( findWidget );

	QTreeView* contents = new QTreeView( parent );
	auto model = new ZealContentsModel( contents );

	QObject::connect( contents, &QTreeView::clicked, model, &ZealContentsModel::showItem );

	contents->setHeaderHidden( true );
	contents->setModel( model );

	return contents;
}

IDocumentationProvider* ZealDocumentationHome::provider() const
{
	return ZealDocumentation::m_provider;
}

// =================================================================================================

ZealContentsModel::ZealContentsModel( QObject* parent )
	: QAbstractItemModel( parent )
{
}

int ZealContentsModel::rowCount( const QModelIndex& parent ) const
{
	if ( !parent.isValid() )
	{
		return ZealDocumentation::m_provider->tokenGroups().size();
	}

	if ( int( parent.internalId() ) < 0 )
	{
		QString groupName = parent.data( Qt::DisplayRole ).toString();
		int groupSize = ZealDocumentation::m_provider->groupTokens( groupName ).size();

		return groupSize;
	}

	return 0;
}

int ZealContentsModel::columnCount( const QModelIndex& ) const
{
	return 1;
}

QModelIndex ZealContentsModel::parent( const QModelIndex& child ) const
{
	if ( child.isValid() && child.column() == 0 && int( child.internalId() ) >= 0 )
	{
		return createIndex( child.internalId(), 0, -1 );
	}

	return QModelIndex();
}

QModelIndex ZealContentsModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( row < 0 || column != 0 )
	{
		return QModelIndex();
	}

	if ( !parent.isValid() && row >= rowCount() )
	{
		return QModelIndex();
	}

	return createIndex( row, column, int( parent.isValid() ? parent.row() : -1 ) );
}

QVariant ZealContentsModel::data( const QModelIndex& index, int role ) const
{
	if ( index.isValid() )
	{
		int internal( index.internalId() );
		QString groupName;

		if ( internal < 0 )
		{
			groupName = ZealDocumentation::m_provider->tokenGroups().at( index.row() );
		}
		else
		{
			groupName = index.parent().data( Qt::DisplayRole ).toString();
		}

		if ( role == Qt::DisplayRole )
		{
			if ( internal < 0 )
			{
				return groupName;
			}

			return ZealDocumentation::m_provider->groupTokens( groupName ).at( index.row() );
		}

		if ( role == Qt::DecorationRole )
		{
			return ZealDocumentation::m_provider->groupIcon( groupName );
		}
	}

	return QVariant();
}

void ZealContentsModel::showItem( const QModelIndex& idx )
{
	if ( idx.isValid() && int( idx.internalId() ) >= 0 )
	{
		auto doc = ZealDocumentation::m_provider->documentationForToken( idx.data( Qt::DisplayRole ).toString() );
		ICore::self()->documentationController()->showDocumentation( doc );
	}
}
