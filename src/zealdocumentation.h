/* This file is part of KDevelop
 * *  Copyright 2016 Anton Anikin <anton.anikin@htower.ru>
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

#pragma once

#include <interfaces/idocumentation.h>

#include <QAbstractItemModel>
#include <QUrl>

class ZealdocProvider;

/*!
 * \class ZealDocumentation
 * \brief Represents a documentation entry in Zeal.
 *
 * This class is responsible for displaying a documentation page for a specific
 * entry in Zeal.
 */
class ZealDocumentation: public KDevelop::IDocumentation
{
	Q_OBJECT

public:
	/*!
	 * \brief Constructs a ZealDocumentation object with the specified name and URL.
	 * \param name The name of the documentation entry.
	 * \param url The URL of the documentation entry.
	 */
	ZealDocumentation( const QString& name, const QUrl& url );

	/*!
	 * \brief Returns the name of the documentation entry.
	 * \return The name of the documentation entry.
	 */
	QString name() const override;

	/*!
	 * \brief Returns a description of the documentation entry.
	 * \return The description of the documentation entry.
	 */
	QString description() const override;

	/*!
	 * \brief Creates and returns a widget to display the documentation.
	 * \param findWidget The widget used for finding text within the documentation.
	 * \param parent The parent widget.
	 * \return A widget to display the documentation.
	 */
	QWidget* documentationWidget( KDevelop::DocumentationFindWidget* findWidget,
				      QWidget* parent = nullptr ) override;

	/*!
	 * \brief Returns the documentation provider for this entry.
	 * \return The documentation provider.
	 */
	KDevelop::IDocumentationProvider* provider() const override;

	static ZealdocProvider* m_provider; /*!< The documentation provider for Zeal. */

private:
	QString m_name; /*!< The name of the documentation entry. */
	QUrl	m_url;	/*!< The URL of the documentation entry. */
};

/*!
 * \class ZealDocumentationHome
 * \brief Represents the home page of Zeal documentation.
 *
 * This class provides the main content page for the Zeal documentation.
 */
class ZealDocumentationHome: public KDevelop::IDocumentation
{
	Q_OBJECT

public:
	/*!
	 * \brief Constructs a ZealDocumentationHome object.
	 */
	ZealDocumentationHome();

	/*!
	 * \brief Returns the name of the home page.
	 * \return The name of the home page.
	 */
	QString name() const override;

	/*!
	 * \brief Returns a description of the home page.
	 * \return The description of the home page.
	 */
	QString description() const override;

	/*!
	 * \brief Creates and returns a widget to display the home page content.
	 * \param findWidget The widget used for finding text within the documentation.
	 * \param parent The parent widget.
	 * \return A widget to display the home page content.
	 */
	QWidget* documentationWidget( KDevelop::DocumentationFindWidget* findWidget,
				      QWidget* parent = nullptr ) override;

	/*!
	 * \brief Returns the documentation provider for the home page.
	 * \return The documentation provider.
	 */
	KDevelop::IDocumentationProvider* provider() const override;
};

/*!
 * \class ZealContentsModel
 * \brief Provides the model for the content tree view in Zeal documentation.
 *
 * This class is responsible for managing the data and structure of the content
 * tree view used in the Zeal documentation.
 */
class ZealContentsModel: public QAbstractItemModel
{
	Q_OBJECT

public:
	/*!
	 * \brief Constructs a ZealContentsModel object with the specified parent.
	 * \param parent The parent object.
	 */
	ZealContentsModel( QObject* parent );

	/*!
	 * \brief Returns the number of rows under the given parent.
	 * \param parent The parent index.
	 * \return The number of rows under the given parent.
	 */
	[[nodiscard]] int rowCount( const QModelIndex& parent = QModelIndex() ) const override;

	/*!
	 * \brief Returns the number of columns for the children of the given parent.
	 * \param parent The parent index.
	 * \return The number of columns for the children.
	 */
	[[nodiscard]] int columnCount( const QModelIndex& parent = QModelIndex() ) const override;

	/*!
	 * \brief Returns the parent index of the given child index.
	 * \param child The child index.
	 * \return The parent index of the given child index.
	 */
	[[nodiscard]] QModelIndex parent( const QModelIndex& child ) const override;

	/*!
	 * \brief Returns the index of the item in the specified row and column under the given parent.
	 * \param row The row number.
	 * \param column The column number.
	 * \param parent The parent index.
	 * \return The index of the item.
	 */
	[[nodiscard]] QModelIndex index( int row,
					 int column,
					 const QModelIndex& parent = QModelIndex() ) const override;

	/*!
	 * \brief Returns the data stored under the given role for the item referred to by the index.
	 * \param index The index of the item.
	 * \param role The role for which the data is requested.
	 * \return The data stored under the given role.
	 */
	[[nodiscard]] QVariant data( const QModelIndex& index, int role ) const override;

public Q_SLOTS:
	/*!
	 * \brief Shows the item referred to by the index.
	 * \param idx The index of the item to show.
	 */
	void showItem( const QModelIndex& idx );
};
