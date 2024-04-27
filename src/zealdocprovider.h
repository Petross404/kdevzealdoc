/* This file is part of KDevelop
 * Copyright 2016 Anton Anikin <anton.anikin@htower.ru>
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
#include <interfaces/idocumentationprovider.h>

#include <QIcon>
#include <QUrl>
#include <QStringListModel>

class ZealdocProvider : public QObject, public KDevelop::IDocumentationProvider
{
        Q_OBJECT
        Q_INTERFACES ( KDevelop::IDocumentationProvider )

public:
        ZealdocProvider ( const QString& docsetPath, QObject* parent );
        ~ZealdocProvider() override;

        bool isValid();

        QIcon icon() const override;
        QString name() const override;

        KDevelop::IDocumentation::Ptr homePage() const override;

        KDevelop::IDocumentation::Ptr documentation ( const QUrl& url ) const override;
        KDevelop::IDocumentation::Ptr documentationForDeclaration ( KDevelop::Declaration* dec ) const override;
        KDevelop::IDocumentation::Ptr documentationForIndex ( const QModelIndex& index ) const override;
        KDevelop::IDocumentation::Ptr documentationForToken ( const QString& token ) const;

        QAbstractListModel* indexModel() const override;

        QStringList tokenGroups() const;
        QIcon groupIcon ( const QString& group );
        QStringList groupTokens ( const QString& group ) const;
        /*
        Q_SIGNALS:
                void addHistory(const KDevelop::IDocumentation::Ptr& doc) const;*/

private:
        bool m_isValid;
        QString m_name;
        QIcon m_icon;

        QStringListModel* m_model;

        QStringList m_tokenGroups;
        QMap< QString, QStringList > m_groupTokens;

        QMap< QString, QUrl > m_tokenUrls;
};
