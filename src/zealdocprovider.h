/*!
 * \file zealdocprovider.h
 * \brief This file is part of KDevelop and provides the ZealdocProvider class definition.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; see the file COPYING. If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * \author Anton Anikin <anton.anikin@htower.ru>
 * \date 2016
 */

#pragma once

#include <interfaces/idocumentationprovider.h>
#include <interfaces/iplugin.h>

#include <QIcon>
#include <QStringListModel>
#include <QUrl>

/*!
 * \class ZealdocProvider
 * \brief The ZealdocProvider class provides documentation functionalities for KDevelop using Zeal docsets.
 *
 * This class implements the KDevelop::IDocumentationProvider interface to integrate with KDevelop's documentation system.
 */
class ZealdocProvider
	: public QObject
	, public KDevelop::IDocumentationProvider
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ZealdocProvider )
	Q_INTERFACES( KDevelop::IDocumentationProvider )

public:
	/*!
	 * \brief Constructs the ZealdocProvider with the specified docset path and parent object.
	 * \param docsetPath The path to the docset.
	 * \param parent The parent QObject.
	 */
	ZealdocProvider( const QString& docsetPath, QObject* parent );

	/*!
	 * \brief Destroys the ZealdocProvider.
	 */
	~ZealdocProvider() override;

	/*!
	 * \brief Checks if the documentation provider is valid.
	 * \return True if the provider is valid, false otherwise.
	 */
	bool isValid();

	/*!
	 * \brief Returns the icon representing the documentation provider.
	 * \return The icon.
	 */
	[[nodiscard]] QIcon icon() const override;

	/*!
	 * \brief Returns the name of the documentation provider.
	 * \return The name.
	 */
	[[nodiscard]] QString name() const override;

	/*!
	 * \brief Returns the home page of the documentation.
	 * \return The home page documentation pointer.
	 */
	[[nodiscard]] KDevelop::IDocumentation::Ptr homePage() const override;

	/*!
	 * \brief Returns the documentation for the specified URL.
	 * \param url The URL to get documentation for.
	 * \return The documentation pointer.
	 */
	[[nodiscard]] KDevelop::IDocumentation::Ptr documentation( const QUrl& url ) const override;

	/*!
	 * \brief Returns the documentation for the specified declaration.
	 * \param dec The declaration to get documentation for.
	 * \return The documentation pointer.
	 */
	[[nodiscard]] KDevelop::IDocumentation::Ptr documentationForDeclaration(
		KDevelop::Declaration* dec ) const override;

	/*!
	 * \brief Returns the documentation for the specified index.
	 * \param index The index to get documentation for.
	 * \return The documentation pointer.
	 */
	[[nodiscard]] KDevelop::IDocumentation::Ptr documentationForIndex(
		const QModelIndex& index ) const override;

	/*!
	 * \brief Returns the documentation for the specified token.
	 * \param token The token to get documentation for.
	 * \return The documentation pointer.
	 */
	[[nodiscard]] KDevelop::IDocumentation::Ptr documentationForToken( const QString& token ) const;

	/*!
	 * \brief Returns the index model for the documentation.
	 * \return The index model pointer.
	 */
	[[nodiscard]] QAbstractListModel* indexModel() const override;

	/*!
	 * \brief Returns the list of token groups.
	 * \return The list of token groups.
	 */
	[[nodiscard]] QStringList tokenGroups() const;

	/*!
	 * \brief Returns the icon for the specified group.
	 * \param group The group to get the icon for.
	 * \return The icon.
	 */
	[[nodiscard]] QIcon groupIcon( const QString& group );

	/*!
	 * \brief Returns the list of tokens for the specified group.
	 * \param group The group to get tokens for.
	 * \return The list of tokens.
	 */
	[[nodiscard]] QStringList groupTokens( const QString& group ) const;

private:
	bool		  m_isValid; /**< Indicates whether the provider is valid. */
	QString		  m_name;    /**< The name of the provider. */
	QIcon		  m_icon;    /**< The icon of the provider. */
	QStringListModel* m_model;   /**< The index model. */
	QStringList	  m_tokenGroups;	  /**< The list of token groups. */
	QMap<QString, QStringList> m_groupTokens; /**< The map of group tokens. */
	QMap<QString, QUrl>	   m_tokenUrls;	  /**< The map of token URLs. */
};
