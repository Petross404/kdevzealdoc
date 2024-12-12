/****************************************************************************
 ** *
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

#include "cancellationtoken.h"

using namespace Zeal::Registry;

CancellationToken::CancellationToken()
	: m_cancelled{ std::make_shared<std::atomic<bool>>( false ) }
{}

void CancellationToken::cancel()
{
	m_cancelled->store( true, std::memory_order::relaxed );
}

bool CancellationToken::isCanceled() const
{
	return m_cancelled->load( std::memory_order::relaxed );
}
