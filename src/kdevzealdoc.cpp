/* This file is part of KDevelop
 *  C opyright 2016 Anton Anik**in <anton.anikin@htower.ru>
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

#include "kdevzealdoc.h"

#include <interfaces/icore.h>
#include <interfaces/idocumentationcontroller.h>

#include <KLocalizedString>
#include <KPluginFactory>

#include "debug.h"
#include "util.h"
#include "zealdocconfigpage.h"
#include "zealdocprovider.h"

// Factory registration for the ZealdocPlugin class using JSON configuration
K_PLUGIN_FACTORY_WITH_JSON( ZealdocFactory, "kdevzealdoc.json", registerPlugin<ZealdocPlugin>(); )

ZealdocPlugin::ZealdocPlugin( QObject* parent, const QVariantList& )
	: KDevelop::IPlugin( QString::fromLocal8Bit( "kdevzealdoc" ), parent )
{
	// Connect the signal changedProvidersList to the documentationController's slot
	connect( this,
		 &ZealdocPlugin::changedProvidersList,
		 KDevelop::ICore::self()->documentationController(),
		 &KDevelop::IDocumentationController::changedDocumentationProviders );

	// Reload documentation sets upon plugin initialization
	reloadDocsets();
}

// Destructor
ZealdocPlugin::~ZealdocPlugin() = default;

// Reloads documentation sets based on enabled docsets
void ZealdocPlugin::reloadDocsets()
{
	const QStringList enabled{ enabledDocsets() };	  // Retrieve enabled documentation sets
	QStringList loaded;		   // List of currently loaded docsets
	bool	    hasChanges = false;	   // Flag to track changes in providers list

	// First, unload disabled docsets
	QMutableListIterator<ZealdocProvider*> i( m_providers );

	while ( i.hasNext() )
	{
		ZealdocProvider* provider{ i.next() };

		if ( !enabled.contains( provider->name() ) )
		{
			i.remove();	      // Remove provider from list
			hasChanges = true;    // Indicate changes were made
		}
		else
		{
			loaded << provider->name();    // Store name of loaded provider
		}
	}

	// Second, load new enabled docsets
	for ( const auto& docsetInformation : availableDocsets() )
	{
		if ( !enabled.contains( docsetInformation.title ) )
		{
			continue;    // Skip docsets not enabled
		}

		if ( loaded.contains( docsetInformation.title ) )
		{
			continue;    // Skip docsets already loaded
		}

		auto docset = new ZealdocProvider( docsetInformation.path, this );

		if ( !docset->isValid() )
		{
			delete docset;	  // Clean up invalid docset objects
			continue;
		}

		m_providers << docset;	  // Add valid docset provider to list
		hasChanges = true;	  // Indicate changes were made
	}

	if ( !hasChanges )
	{
		emit changedProvidersList();	// Emit signal if providers list was modified
	}
}

// Returns list of documentation providers managed by the plugin
QList<KDevelop::IDocumentationProvider*> ZealdocPlugin::providers()
{
	QList<KDevelop::IDocumentationProvider*> result;

	for ( const auto p : m_providers )
	{
		result << p;	// Populate result with each provider
	}

	return result;	  // Return list of providers
}

// Returns the number of configuration pages provided by the plugin (always 1)
int ZealdocPlugin::configPages() const { return 1; }

// Returns the configuration page for the specified number (only one page supported)
KDevelop::ConfigPage* ZealdocPlugin::configPage( int number, QWidget* parent )
{
	if ( number )
	{
		return nullptr;	   // Return nullptr if number is not 0 (only one page supported)
	}

	return new ZealdocConfigPage{ this, parent };	 // Create and return configuration page
}

// Required for QObject class created from K_PLUGIN_FACTORY_WITH_JSON
#include "kdevzealdoc.moc"
#include "moc_kdevzealdoc.cpp"
