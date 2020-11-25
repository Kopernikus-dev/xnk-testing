// Copyright (c) 2017-2020 The EncoCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZXNKCONTROLDIALOG_H
#define ZXNKCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "zxnk/zerocoin.h"

class CZerocoinMint;
class WalletModel;

namespace Ui {
class ZXnkControlDialog;
}

class CZXnkControlWidgetItem : public QTreeWidgetItem
{
public:
    explicit CZXnkControlWidgetItem(QTreeWidget *parent, int type = Type) : QTreeWidgetItem(parent, type) {}
    explicit CZXnkControlWidgetItem(int type = Type) : QTreeWidgetItem(type) {}
    explicit CZXnkControlWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent, type) {}

    bool operator<(const QTreeWidgetItem &other) const;
};

class ZXnkControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZXnkControlDialog(QWidget *parent);
    ~ZXnkControlDialog();

    void setModel(WalletModel* model);

    static std::set<std::string> setSelectedMints;
    static std::set<CMintMeta> setMints;
    static std::vector<CMintMeta> GetSelectedMints();

private:
    Ui::ZXnkControlDialog *ui;
    WalletModel* model;

    void updateList();
    void updateLabels();

    enum {
        COLUMN_CHECKBOX,
        COLUMN_DENOMINATION,
        COLUMN_PUBCOIN,
        COLUMN_VERSION,
        COLUMN_CONFIRMATIONS,
        COLUMN_ISSPENDABLE
    };
    friend class CZXnkControlWidgetItem;

private Q_SLOTS:
    void updateSelection(QTreeWidgetItem* item, int column);
    void ButtonAllClicked();
};

#endif // ZXNKCONTROLDIALOG_H
