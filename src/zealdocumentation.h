/* This file is part of KDevelop
 *  * Copyright 2016 Anton Anikin <anton.anikin@htower.ru>
 *  *
 *  * This program is free software; you can redistribute it and/or
 *  * modify it under the terms of the GNU General Public
 *  * License as published by the Free Software Foundation; either
 *  * version 2 of the License, or (at your option) any later version.
 *  *
 *  * This program is distributed in the hope that it will be useful,
 *  * but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  * General Public License for more details.
 *  *
 *  * You should have received a copy of the GNU General Public License
 *  * along with this program; see the file COPYING.  If not, write to
 *  * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  * Boston, MA 02110-1301, USA.
 *  */

#pragma once

#include <interfaces/idocumentation.h>

#include <QUrl>
#include <QAbstractItemModel>

class ZealdocProvider;

class ZealDocumentation : public KDevelop::IDocumentation
{
        Q_OBJECT

public:
        ZealDocumentation ( const QString& name, const QUrl& url );

        QString name() const override;
        QString description() const override;

        QWidget* documentationWidget ( KDevelop::DocumentationFindWidget* findWidget, QWidget* parent = nullptr ) override;

        KDevelop::IDocumentationProvider* provider() const override;

        static ZealdocProvider* m_provider;

private:
        QString m_name;
        QUrl m_url;
};

class ZealDocumentationHome : public KDevelop::IDocumentation
{
        Q_OBJECT

public:
        ZealDocumentationHome();

        QString name() const override;
        QString description() const override;

        QWidget* documentationWidget ( KDevelop::DocumentationFindWidget* findWidget, QWidget* parent = nullptr ) override;

        KDevelop::IDocumentationProvider* provider() const override;
};

class ZealContentsModel : public QAbstractItemModel
{
        Q_OBJECT

public:
        ZealContentsModel ( QObject* parent );

        int rowCount ( const QModelIndex& parent = QModelIndex() ) const override;
        int columnCount ( const QModelIndex& ) const override;

        QModelIndex parent ( const QModelIndex& child ) const override;
        QModelIndex index ( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
        QVariant data ( const QModelIndex& index, int role ) const override;

public Q_SLOTS:
        void showItem ( const QModelIndex& idx );
};

