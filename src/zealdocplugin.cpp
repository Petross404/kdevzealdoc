/* This file is part of KDevelop
  C opyright 2016 Anton Anik**in <anton.anikin@htower.ru>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
  */

#include "zealdocplugin.h"
#include "zealdocprovider.h"
#include "zealdocconfigpage.h"
#include "util.h"
#include "debug.h"

#include <interfaces/icore.h>
#include <interfaces/idocumentationcontroller.h>

#include <KPluginFactory>
#include <KLocalizedString>

K_PLUGIN_FACTORY_WITH_JSON( ZealdocFactory, "kdevzealdoc.json", registerPlugin<ZealdocPlugin>(); )

using namespace KDevelop;

ZealdocPlugin::ZealdocPlugin( QObject* parent, const QVariantList& )
	: KDevelop::IPlugin( QString::fromLocal8Bit( "kdevzealdoc" ), parent )
{
	//KDEV_USE_EXTENSION_INTERFACE( KDevelop::IDocumentationProviderProvider )

	connect( this,
	         &ZealdocPlugin::changedProvidersList,
	         KDevelop::ICore::self()->documentationController(),
	         &KDevelop::IDocumentationController::changedDocumentationProviders );

	reloadDocsets();
}

ZealdocPlugin::~ZealdocPlugin() = default;

void ZealdocPlugin::reloadDocsets()
{
	QStringList enabled = enabledDocsets();
	QStringList loaded;
	bool hasChanges = false;

	// First, unload disabled docsets
	QMutableListIterator<ZealdocProvider*> i( m_providers );

	while ( i.hasNext() )
	{
		auto provider = i.next();

		if ( !enabled.contains( provider->name() ) )
		{
			i.remove();
			hasChanges = true;
		}
		else
		{
			loaded << provider->name();
		}
	}

	// Second, load new enabled docsets
	for ( const auto& docsetInformation : availableDocsets() )
	{
		if ( !enabled.contains( docsetInformation.title ) )
		{
			continue;
		}

		if ( loaded.contains( docsetInformation.title ) )
		{
			continue;
		}

		auto docset = new ZealdocProvider( docsetInformation.path, this );

		if ( !docset->isValid() )
		{
			delete docset;
			continue;
		}

		m_providers << docset;
		hasChanges = true;
	}

	if ( !hasChanges )
	{
		emit changedProvidersList();
	}
}

QList<KDevelop::IDocumentationProvider*> ZealdocPlugin::providers()
{
	QList<KDevelop::IDocumentationProvider*> result;

	for ( const auto p : m_providers )
	{
		result << p;
	}

	return result;
}

int ZealdocPlugin::configPages() const
{
	return 1;
}

KDevelop::ConfigPage* ZealdocPlugin::configPage( int number, QWidget* parent )
{
	if ( number )
	{
		return nullptr;
	}

	return new ZealdocConfigPage( this, parent );
}

#include "zealdocplugin.moc"
