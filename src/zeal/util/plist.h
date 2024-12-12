/****************************************************************************
 * *             *
 ** Copyright (C) 2015-2016 Oleg Shparber
 ** Contact: https://go.zealdocs.org/l/contact
 **
 ** This file is part of Zeal.
 **
 ** Zeal is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** Zeal is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with Zeal. If not, see <https://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef PLIST_H
#define PLIST_H

#include <QHash>
#include <QVariant>

namespace Zeal { namespace Util {

/*!
 * \class Plist
 * \brief A class for parsing and representing property lists (Plists).
 *
 * This class extends QHash<QString, QVariant> to provide functionality
 * for reading and storing data from Plist files (used in macOS/iOS for structured data storage).
 */
class Plist: public QHash<QString, QVariant>
{
public:
	Plist();

	/*!
	 * \brief Reads a Plist file and populates the hash with its key-value pairs.
	 *
	 * \param fileName The path to the Plist file to read.
	 * \return True if the file was successfully read, false otherwise.
	 */
	bool read( const QString& fileName );

	/*!
	 * \brief Checks if an error occurred during the last read operation.
	 *
	 * \return True if there was an error, false otherwise.
	 */
	[[nodiscard]] bool hasError() const;

private:
	bool m_hasError = false;
};

}}    // namespace Zeal::Util

#endif	  // PLIST_H
