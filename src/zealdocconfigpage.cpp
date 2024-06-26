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

#include "zealdocconfigpage.h"
#include "ui_zealdocconfigpage.h"

#include "kdevzealdoc.h"
#include "util.h"
#include "debug.h"

#include <KLineEdit>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KMessageWidget>

ZealdocConfigPage::ZealdocConfigPage( KDevelop::IPlugin* plugin, QWidget* parent )
	: ConfigPage( plugin, nullptr, parent )
	, ui( std::make_unique<Ui::ZealdocConfigPage>() )
	, m_plugin( dynamic_cast<ZealdocPlugin*>( plugin ) )
{
	ui->setupUi( this );

	ui->kcfg_docsetsPath->setText( docsetsPath() );
	ui->kcfg_docsetsPath->lineEdit()->setReadOnly( true );

	if ( docsetsPath().isEmpty() )
	{
		auto* w = new KMessageWidget( i18n( "Have you installed Zeal and/or any docsets?" ), this );
		w->show();
	}

	reloadDocsets( docsetsPath() );

	connect( ui->docsetsList, &QListWidget::itemChanged, this, [this]( QListWidgetItem* ) { emit changed(); } );

	connect( ui->kcfg_docsetsPath, &KUrlRequester::textChanged,
	this, [this]( const QString & path ) { reloadDocsets( path ); } );
}

ZealdocConfigPage::~ZealdocConfigPage() = default;

void ZealdocConfigPage::reloadDocsets( const QString& path )
{
	auto enabled = enabledDocsets();

	ui->docsetsList->clear();

	for ( const auto& docsetInformation : availableDocsets( path ) )
	{
		if ( !docsetInformation.isValid )
		{
			continue;
		}

		auto item = new QListWidgetItem( ui->docsetsList );
		item->setText( docsetInformation.title );
		item->setIcon( docsetInformation.icon );

		if ( enabled.contains( docsetInformation.title ) )
		{
			item->setCheckState( Qt::Checked );
		}
		else
		{
			item->setCheckState( Qt::Unchecked );
		}
	}
}

KDevelop::ConfigPage::ConfigPageType ZealdocConfigPage::configPageType() const
{
	return KDevelop::ConfigPage::DefaultConfigPage;
}

QString ZealdocConfigPage::name() const
{
	return i18n( "Zeal" );
}

QString ZealdocConfigPage::fullName() const
{
	return i18n( "Configure Zeal documentation settings" );
}

QIcon ZealdocConfigPage::icon() const
{
	return QIcon::fromTheme( QStringLiteral( "zeal" ) );
}

void ZealdocConfigPage::apply()
{
	QStringList enabled;

	for ( int i = 0; i < ui->docsetsList->count(); i++ )
	{
		auto item = ui->docsetsList->item( i );

		if ( item->checkState() == Qt::Checked )
		{
			enabled << item->text();
		}
	}

	KConfigGroup config{ KSharedConfig::openConfig(),"Zealdoc" };
	config.writeEntry( QStringLiteral( "DocsetsPath" ), ui->kcfg_docsetsPath->text() );
	config.writeEntry( QStringLiteral( "EnabledDocsets" ), enabled );

	m_plugin->reloadDocsets();
}

void ZealdocConfigPage::defaults()
{
	QSignalBlocker blocker( this );
	ui->kcfg_docsetsPath->setText( defaultDocsetsPath() );

	for ( int i = 0; i < ui->docsetsList->count(); i++ )
	{
		ui->docsetsList->item( i )->setCheckState( Qt::Unchecked );
	}
}

void ZealdocConfigPage::reset()
{
}
