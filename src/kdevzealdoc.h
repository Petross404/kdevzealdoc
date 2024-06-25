/* This file is part of KDevelop
 * * Copyright 2016 Anton Anikin <anton.anikin@htower.ru>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#include <interfaces/iplugin.h>
#include <interfaces/idocumentationproviderprovider.h>

#include <QObject>

class ZealdocProvider;

/*!
 * \brief The ZealdocPlugin class represents the main plugin class for the Zeal integration in KDevelop.
 *
 * This class implements the KDevelop::IPlugin interface to integrate with KDevelop's plugin system
 * and KDevelop::IDocumentationProviderProvider to provide documentation providers.
 */
class ZealdocPlugin : public KDevelop::IPlugin, public KDevelop::IDocumentationProviderProvider
{
        Q_OBJECT
        Q_INTERFACES(KDevelop::IDocumentationProviderProvider)

public:
        /*!
         * \brief Constructs a ZealdocPlugin object.
         * \param parent The parent object.
         * \param args List of arguments passed during initialization.
         */
        ZealdocPlugin(QObject* parent, const QVariantList& args);

        /*!
         * \brief Destroys the ZealdocPlugin object.
         */
        ~ZealdocPlugin() override;

        /*!
         * \brief Returns the list of documentation providers.
         * \return List of documentation providers.
         */
        QList<KDevelop::IDocumentationProvider*> providers() override;

        /*!
         * \brief Returns the number of configuration pages provided by the plugin.
         * \return Number of configuration pages.
         */
        [[nodiscard]] int configPages() const override;

        /*!
         * \brief Returns a configuration page for the specified number.
         * \param number The index of the configuration page to retrieve.
         * \param parent The parent widget for the configuration page.
         * \return Configuration page object.
         */
        KDevelop::ConfigPage* configPage(int number, QWidget* parent) override;

        /*!
         * \brief Reloads the documentation sets and updates the providers.
         */
        void reloadDocsets();

Q_SIGNALS:
        /*!
         * \brief Signal emitted when the list of documentation providers changes.
         */
        void changedProvidersList() override;

private:
        QList<ZealdocProvider*> m_providers; /*!< List of documentation providers managed by the plugin. */
};
