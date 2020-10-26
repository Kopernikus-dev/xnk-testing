// Copyright (c) 2019-2020 The PIVX developers
// Copyright (c) 2020 The EncoCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EncoCoin_CORE_NEW_GUI_PRUNNABLE_H
#define EncoCoin_CORE_NEW_GUI_PRUNNABLE_H

class Runnable {
public:
    virtual void run(int type) = 0;
    virtual void onError(QString error, int type) = 0;
};

#endif //EncoCoin_CORE_NEW_GUI_PRUNNABLE_H
