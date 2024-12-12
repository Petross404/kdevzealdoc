/****************************************************************************
 * * *
 ** Copyright (C) 2024 Petross404
 ** Contact: petross404@gmail.com
 **
 ** This file is part of KDevZealDoc.
 **
 ** KDevZealDoc is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** KDevZealDoc is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with KDevZealDoc. If not, see <https://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef MACROS_HPP
#define MACROS_HPP

#include <QString>
#include <source_location>

/*!
 * \brief Provides macros for detailed assertion logging with source location.
 *
 * This header defines macros that can be used to enhance assertions with
 * detailed source location information (file name, function name, and line number).
 * The macros rely on `std::source_location` which is available in C++20 and later.
 */

/*!
 * \brief Macro that returns the current source location in a human-readable format.
 *
 * \returns A QString containing the current file name, function name, and line number.
 */
#define location()                                                     \
	QString{ std::source_location::current().file_name() } + " - " \
		+ std::source_location::current().function_name()      \
		+ " - Line: " + QString::number( std::source_location::current().line() )

/*!
 * \brief Custom assertion macro with detailed source location.
 *
 * This macro works like `Q_ASSERT_X` but includes a custom message and
 * provides detailed information about the source location where the assertion failed.
 * It uses `qFatal` to output the failure reason with the location information.
 *
 * \param condition The condition to check.
 * \param location The source location string.
 * \param message A custom error message to be printed on failure.
 */
#define Q_ASSERT_X_CUSTOM( condition, location, message ) \
	if ( !( condition ) )                             \
	{                                                 \
		qFatal( "Assertion failed at %s: %s",     \
			qPrintable( location ),           \
			qPrintable( message ) );          \
	}

#endif	  // MACROS_HPP
