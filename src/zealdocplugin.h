// /* This file is part of KDevelop
//  * * Copyright 2016 Anton Anikin <anton.anikin@htower.ru>
//  *
//  * This program is free software; you can redistribute it and/or
//  * modify it under the terms of the GNU General Public
//  * License as published by the Free Software Foundation; either
//  * version 2 of the License, or (at your option) any later version.
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//  * General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with this program; see the file COPYING.  If not, write to
//  * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
//  * Boston, MA 02110-1301, USA.
//  */
//
// #pragma once
//
// #include <interfaces/iplugin.h>
// #include <interfaces/idocumentationproviderprovider.h>
//
// #include <QObject>
//
// class ZealdocProvider;
//
// /*!
//  * \class ZealdocPlugin
//  * \brief The ZealdocPlugin class implements a KDevelop plugin that integrates Zeal documentation.
//  *
//  * This class provides functionality to load and manage Zeal docsets, and offers a configuration interface
//  * for managing the docset paths and enabled docsets.
//  */
// class ZealdocPlugin : public KDevelop::IPlugin, public KDevelop::IDocumentationProviderProvider
// {
//         Q_OBJECT
//         Q_INTERFACES(KDevelop::IDocumentationProviderProvider)
//
// public:
//         /*!
//          * \brief Constructs the ZealdocPlugin with the specified parent and arguments.
//          * \param parent The parent object.
//          * \param args The list of arguments.
//          */
//         ZealdocPlugin(QObject* parent, const QVariantList& args);
//
//         /*!
//          * \brief Destructor for the ZealdocPlugin.
//          */
//         ~ZealdocPlugin() override;
//
//         /*!
//          * \brief Returns the list of documentation providers.
//          * \return The list of documentation providers.
//          */
//         QList<KDevelop::IDocumentationProvider*> providers() override;
//
//         /*!
//          * \brief Returns the number of configuration pages provided by this plugin.
//          * \return The number of configuration pages.
//          */
//         int configPages() const override;
//
//         /*!
//          * \brief Returns the configuration page at the specified index.
//          * \param number The index of the configuration page.
//          * \param parent The parent widget.
//          * \return The configuration page at the specified index.
//          */
//         KDevelop::ConfigPage* configPage(int number, QWidget* parent) override;
//
//         /*!
//          * \brief Reloads the Zeal docsets based on the current configuration.
//          */
//         void reloadDocsets();
//
// Q_SIGNALS:
//         /*!
//          * \brief Emitted when the list of documentation providers changes.
//          */
//         void changedProvidersList() override;
//
// private:
//         QList<ZealdocProvider*> m_providers; /*!< The list of documentation providers. */
// };
