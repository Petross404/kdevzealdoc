// /* This file is part of KDevelop
//   C opyright 2016 Anton Anik**in <anton.anikin@htower.ru>
//
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public
//   License as published by the Free Software Foundation; either
//   version 2 of the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//   General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; see the file COPYING.  If not, write to
//   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
//   Boston, MA 02110-1301, USA.
//   */
//
// #include "kdevzealdoc.h"
// #include "zealdocprovider.h"
// #include "zealdocconfigpage.h"
// #include "util.h"
// #include "debug.h"
//
// #include <interfaces/icore.h>
// #include <interfaces/idocumentationcontroller.h>
//
// #include <KPluginFactory>
// #include <KLocalizedString>
//
// // Factory registration for the ZealdocPlugin class using JSON configuration
// K_PLUGIN_FACTORY_WITH_JSON( ZealdocFactory, "kdevzealdoc.json", registerPlugin<ZealdocPlugin>(); )
//
// Q_LOGGING_CATEGORY(KDEV_PLUGINS_ZEALDOC, "my.zeal.plugin")
//
// using namespace KDevelop;
//
// ZealdocPlugin::ZealdocPlugin( QObject* parent, const QVariantList& )
// 	: KDevelop::IPlugin( QString::fromLocal8Bit( "kdevzealdoc" ), parent )
// {
// 	// Connect the signal changedProvidersList to the documentationController's slot
// 	connect( this,
// 	         &ZealdocPlugin::changedProvidersList,
// 	         KDevelop::ICore::self()->documentationController(),
// 	         &KDevelop::IDocumentationController::changedDocumentationProviders );
//
// 	// Reload documentation sets upon plugin initialization
// 	reloadDocsets();
// }
//
// // Destructor
// ZealdocPlugin::~ZealdocPlugin() = default;
//
// // Reloads documentation sets based on enabled docsets
// void ZealdocPlugin::reloadDocsets()
// {
// 	QStringList enabled = enabledDocsets(); // Retrieve enabled documentation sets
// 	QStringList loaded; // List of currently loaded docsets
// 	bool hasChanges = false; // Flag to track changes in providers list
//
// 	qCDebug( KDEV_PLUGINS_ZEALDOC ) << "Enabled docsets:" << enabled;
//
// 	// First, unload disabled docsets
// 	QMutableListIterator<ZealdocProvider*> i( m_providers );
//
// 	while ( i.hasNext() )
// 	{
// 		auto provider = i.next();
//
// 		if ( !enabled.contains( provider->name() ) )
// 		{
// 			qCDebug( KDEV_PLUGINS_ZEALDOC ) << "Removing provider for disabled docset:" << provider->name();
// 			i.remove(); // Remove provider from list
// 			hasChanges = true; // Indicate changes were made
// 		}
// 		else
// 		{
// 			loaded.append( provider->name() ); // Store name of loaded provider
// 		}
// 	}
//
// 	// Second, load new enabled docsets
// 	for ( const auto& docsetInformation : availableDocsets() )
// 	{
// 		qCDebug( KDEV_PLUGINS_ZEALDOC ) << "Checking docset:" << docsetInformation.title;
//
// 		if ( !enabled.contains( docsetInformation.title ) )
// 		{
// 			qCDebug( KDEV_PLUGINS_ZEALDOC ) << "Docset not enabled:" << docsetInformation.title;
// 			continue; // Skip docsets not enabled
// 		}
//
// 		if ( loaded.contains( docsetInformation.title ) )
// 		{
// 			qCDebug( KDEV_PLUGINS_ZEALDOC ) << "Docset already loaded:" << docsetInformation.title;
// 			continue; // Skip docsets already loaded
// 		}
//
// 		auto docset = new ZealdocProvider( docsetInformation.path, this );
//
// 		if ( !docset->isValid() )
// 		{
// 			qCWarning( KDEV_PLUGINS_ZEALDOC ) << "Invalid docset:" << docsetInformation.title;
// 			delete docset; // Clean up invalid docset objects
// 			continue;
// 		}
//
// 		qCDebug( KDEV_PLUGINS_ZEALDOC ) << "Adding docset provider:" << docsetInformation.title;
// 		m_providers.append( docset ); // Add valid docset provider to list
// 		hasChanges = true; // Indicate changes were made
// 	}
//
// 	if ( hasChanges )
// 	{
// 		qCDebug( KDEV_PLUGINS_ZEALDOC ) << "Providers list changed, emitting signal";
// 		emit changedProvidersList(); // Emit signal if providers list was modified
// 	}
// }
//
// QList<KDevelop::IDocumentationProvider*> ZealdocPlugin::providers()
// {
// 	QList<KDevelop::IDocumentationProvider*> result;
//
// 	for ( const auto p : m_providers )
// 	{
// 		result.append( p ); // Populate result with each provider
// 	}
//
// 	return result; // Return list of providers
// }
//
// // Returns the number of configuration pages provided by the plugin (always 1)
// int ZealdocPlugin::configPages() const
// {
// 	return 1;
// }
//
// // Returns the configuration page for the specified number (only one page supported)
// KDevelop::ConfigPage* ZealdocPlugin::configPage( int number, QWidget* parent )
// {
// 	if ( number )
// 	{
// 		return nullptr; // Return nullptr if number is not 0 (only one page supported)
// 	}
//
// 	return new ZealdocConfigPage( this, parent ); // Create and return configuration page
// }
//
// // Required for QObject class created from K_PLUGIN_FACTORY_WITH_JSON
// #include "zealdocplugin.moc"
// #include "moc_zealdocplugin.cpp"
