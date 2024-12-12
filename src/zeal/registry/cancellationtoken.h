/****************************************************************************
 * *
 ** Copyright (C) 2015 Artur Spychaj
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
 ** along with Zeal. If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef CANCELLATIONTOKEN_H
#define CANCELLATIONTOKEN_H

#include <qmetatype.h>

#include <atomic>
#include <memory>

namespace Zeal::Registry {

/*!
 * \brief Represents a token to manage cancellation state in asynchronous operations.
 *
 * The `CancellationToken` is used to check if an operation has been cancelled.
 * Instances of `CancellationToken` share the same state, so cancelling one token
 * affects all tokens that share ownership of the cancellation state.
 */
struct CancellationToken
{
public:
	/*!
	 * \brief Constructs a `CancellationToken` with an initial state of not cancelled.
	 */
	CancellationToken();

	/*!
	 * \brief Checks if the token has been cancelled.
	 * \return `true` if the token is in a cancelled state, `false` otherwise.
	 * \note This operation is thread-safe.
	 */
	[[nodiscard]] bool isCanceled() const;

	/*!
	 * \brief Sets the cancellation state to true.
	 * \note This operation is thread-safe and affects all shared instances of the token.
	 */
	void cancel();

private:
	/*!
	 * \brief Shared state indicating whether the token has been cancelled.
	 *
	 * This is shared across all instances of `CancellationToken` that originate
	 * from the same source. The use of `std::shared_ptr` ensures thread-safe
	 * reference counting, and `std::atomic<bool>` provides thread-safe access to
	 * the cancellation state.
	 */
	std::shared_ptr<std::atomic<bool>> m_cancelled;
};

}    // namespace Zeal::Registry

/*!
 * \brief Declares the `CancellationToken` type to the Qt meta-object system.
 *
 * This enables `CancellationToken` to be used in QVariant and other Qt meta-type-based
 * operations such as signal-slot communication.
 */
Q_DECLARE_METATYPE( Zeal::Registry::CancellationToken )

#endif	  // CANCELLATIONTOKEN_H
