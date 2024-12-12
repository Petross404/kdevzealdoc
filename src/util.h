/* This file is part of KDevelop
 *  Copyright 2016 Anton Anikin <anton.anikin@htower.ru> *
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

#include <QIcon>
#include <QList>
#include <QStringList>

/*!
 * \struct DocsetInformation
 * \brief Contains information about a documentation set.
 *
 * This structure holds the path, title, icon, and validity status of a documentation set.
 */
struct DocsetInformation
{
	QString path;  /*!< The file path to the documentation set. */
	QString title; /*!< The title of the documentation set. */
	QIcon	icon;  /*!< The icon representing the documentation set. */
	bool isValid;  /*!< A flag indicating whether the documentation set is valid. */
};

/*!
 * \brief Returns the default path where documentation sets are stored.
 * \return The default path for documentation sets.
 */
QString defaultDocsetsPath();

/*!
 * \brief Returns the current path where documentation sets are stored.
 * \return The current path for documentation sets.
 */
QString docsetsPath();

/*!
 * \brief Returns a list of enabled documentation sets.
 * \return A list of titles of enabled documentation sets.
 */
QStringList enabledDocsets();

/*!
 * \brief Returns a list of available documentation sets.
 * \param docsetsPath The path to search for documentation sets. Defaults to the current documentation path.
 * \return A list of DocsetInformation structures representing the available documentation sets.
 */
QList<DocsetInformation> availableDocsets( const QString& docsetsPath = docsetsPath() );
