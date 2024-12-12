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

#include <interfaces/configpage.h>

#include <memory>

namespace Ui {
class ZealdocConfigPage;
}

class ZealdocPlugin;

/*!
 * \class ZealdocConfigPage
 * \brief The ZealdocConfigPage class provides a configuration page for the Zeal documentation plugin in KDevelop.
 *
 * This class implements the configuration interface for the Zeal documentation plugin, allowing users to
 * configure the path to Zeal docsets and manage which docsets are enabled.
 */
class ZealdocConfigPage: public KDevelop::ConfigPage
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ZealdocConfigPage )

public:
	/*!
	 * \brief Constructs the ZealdocConfigPage with the specified plugin and parent widget.
	 * \param plugin The plugin associated with this configuration page.
	 * \param parent The parent widget.
	 */
	ZealdocConfigPage( KDevelop::IPlugin* plugin, QWidget* parent );

	/*!
	 * \brief Destructor for the ZealdocConfigPage.
	 */
	~ZealdocConfigPage() override;

	/*!
	 * \brief Returns the type of the configuration page.
	 * \return The configuration page type.
	 */
	[[nodiscard]] KDevelop::ConfigPage::ConfigPageType configPageType() const override;

	/*!
	 * \brief Returns the name of the configuration page.
	 * \return The name of the page.
	 */
	[[nodiscard]] QString name() const override;

	/*!
	 * \brief Returns the full name of the configuration page.
	 * \return The full name of the page.
	 */
	[[nodiscard]] QString fullName() const override;

	/*!
	 * \brief Returns the icon representing the configuration page.
	 * \return The icon.
	 */
	[[nodiscard]] QIcon icon() const override;

	/*!
	 * \brief Applies the configuration changes made by the user.
	 */
	void apply() override;

	/*!
	 * \brief Resets the configuration page to the default settings.
	 */
	void defaults() override;

	/*!
	 * \brief Resets the configuration page to the current settings.
	 */
	void reset() override;

private:
	/*!
	 * \brief Reloads the docsets from the specified path.
	 * \param path The path to the docsets.
	 */
	void reloadDocsets( const QString& path );

	std::unique_ptr<Ui::ZealdocConfigPage> m_ui; /*!< The user interface for the configuration page. */
	ZealdocPlugin* m_plugin; /*!< The plugin associated with this configuration page. */
};
