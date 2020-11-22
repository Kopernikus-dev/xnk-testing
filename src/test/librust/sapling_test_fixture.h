// Copyright (c) 2020 The PIVX developers
// Copyright (c) 2020 The EncoCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.
#ifndef EncoCoin_SAPLING_TEST_FIXTURE_H
#define EncoCoin_SAPLING_TEST_FIXTURE_H

#include "test/test_encocoin.h"

/**
 * Testing setup that configures a complete environment for Sapling testing.
 */
struct SaplingTestingSetup : public TestingSetup {
    SaplingTestingSetup();
    ~SaplingTestingSetup();
};


#endif //EncoCoin_SAPLING_TEST_FIXTURE_H
