#!/usr/bin/env python3
# Copyright (c) 2018 The Zcash developers
# Copyright (c) 2020 The PIVX developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or https://www.opensource.org/licenses/mit-license.php .

from test_framework.test_framework import EncoCoinTestFramework
from test_framework.util import (
    assert_equal,
    sync_mempools,
    get_coinstake_address
)

from decimal import Decimal

# Test wallet behaviour with Sapling addresses
class WalletSaplingTest(EncoCoinTestFramework):

    def set_test_params(self):
        self.num_nodes = 4
        self.setup_clean_chain = True
        saplingUpgrade = ['-nuparams=v5_dummy:1']
        self.extra_args = [saplingUpgrade, saplingUpgrade, saplingUpgrade, saplingUpgrade]

    def check_tx_priority(self, txids):
        sync_mempools(self.nodes)
        mempool = self.nodes[0].getrawmempool(True)
        for txid in txids:
            assert(Decimal(mempool[txid]['startingpriority']) == Decimal('1E+25'))

    def run_test(self):
        self.log.info("Mining 120 blocks...")
        self.nodes[0].generate(120)
        self.sync_all()
        # Sanity-check the test harness
        assert_equal([x.getblockcount() for x in self.nodes], [120] * self.num_nodes)

        taddr1 = self.nodes[1].getnewaddress()
        saplingAddr0 = self.nodes[0].getnewshieldedaddress()
        saplingAddr1 = self.nodes[1].getnewshieldedaddress()

        # Verify addresses
        assert(saplingAddr0 in self.nodes[0].listshieldedaddresses())
        assert(saplingAddr1 in self.nodes[1].listshieldedaddresses())
        assert_equal(self.nodes[0].validateaddress(saplingAddr0)['type'], 'sapling')
        assert_equal(self.nodes[0].validateaddress(saplingAddr1)['type'], 'sapling')

        # Verify balance
        assert_equal(self.nodes[0].getshieldedbalance(saplingAddr0), Decimal('0'))
        assert_equal(self.nodes[1].getshieldedbalance(saplingAddr1), Decimal('0'))
        assert_equal(self.nodes[1].getreceivedbyaddress(taddr1), Decimal('0'))

        # Fixed fee
        fee = 1

        # Node 0 shields some funds
        # taddr -> Sapling
        self.log.info("TX 1: shield funds from specified transparent address.")
        recipients = [{"address": saplingAddr0, "amount": Decimal('10')}]
        mytxid1 = self.nodes[0].shielded_sendmany(get_coinstake_address(self.nodes[0]), recipients, 1, fee)

        # shield more funds automatically selecting the transparent inputs
        self.log.info("TX 2: shield funds from any transparent address.")
        mytxid2 = self.nodes[0].shielded_sendmany("from_transparent", recipients, 1, fee)

        # shield more funds creating and then sending a raw transaction
        self.log.info("TX 3: shield funds creating and sending raw transaction.")
        tx_json = self.nodes[0].raw_shielded_sendmany("from_transparent", recipients, 1, fee)
        mytxid3 = self.nodes[0].sendrawtransaction(tx_json["hex"])

        # Verify priority of tx is INF_PRIORITY, defined as 1E+25 (10000000000000000000000000)
        self.check_tx_priority([mytxid1, mytxid2, mytxid3])
        self.log.info("Priority for tx1, tx2 and tx3 checks out")

        self.nodes[2].generate(1)
        self.sync_all()

        # Verify balance
        assert_equal(self.nodes[0].getshieldedbalance(saplingAddr0), Decimal('30'))
        assert_equal(self.nodes[1].getshieldedbalance(saplingAddr1), Decimal('0'))
        assert_equal(self.nodes[1].getreceivedbyaddress(taddr1), Decimal('0'))
        self.log.info("Balances check out")

        # Node 0 sends some shielded funds to node 1
        # Sapling -> Sapling
        #         -> Sapling (change)
        self.log.info("TX 4: shielded transaction from specified sapling address.")
        recipients4 = [{"address": saplingAddr1, "amount": Decimal('10')}]
        mytxid4 = self.nodes[0].shielded_sendmany(saplingAddr0, recipients4, 1, fee)
        self.check_tx_priority([mytxid4])

        self.nodes[2].generate(1)
        self.sync_all()

        # Send more shielded funds (this time with automatic selection of the source)
        self.log.info("TX 5: shielded transaction from any sapling address.")
        recipients5 = [{"address": saplingAddr1, "amount": Decimal('5')}]
        mytxid5 = self.nodes[0].shielded_sendmany("from_shielded", recipients5, 1, fee)
        self.check_tx_priority([mytxid5])

        self.nodes[2].generate(1)
        self.sync_all()

        # Send more shielded funds (with create + send raw transaction)
        self.log.info("TX 6: shielded raw transaction.")
        tx_json = self.nodes[0].raw_shielded_sendmany("from_shielded", recipients5, 1, fee)
        mytxid6 = self.nodes[0].sendrawtransaction(tx_json["hex"])
        self.check_tx_priority([mytxid6])

        self.nodes[2].generate(1)
        self.sync_all()

        # Verify balance
        assert_equal(self.nodes[0].getshieldedbalance(saplingAddr0), Decimal('7'))   # 30 received - (20 sent + 3 fee)
        assert_equal(self.nodes[1].getshieldedbalance(saplingAddr1), Decimal('20'))  # 20 received
        assert_equal(self.nodes[1].getreceivedbyaddress(taddr1), Decimal('0'))
        self.log.info("Balances check out")

        # Node 1 sends some shielded funds to node 0, as well as unshielding
        # Sapling -> Sapling
        #         -> taddr
        #         -> Sapling (change)
        self.log.info("TX 7: deshield funds from specified sapling address.")
        recipients7 = [{"address": saplingAddr0, "amount": Decimal('8')}]
        recipients7.append({"address": taddr1, "amount": Decimal('10')})
        mytxid7 = self.nodes[1].shielded_sendmany(saplingAddr1, recipients7, 1, fee)
        self.check_tx_priority([mytxid7])

        self.nodes[2].generate(1)
        self.sync_all()

        # Verify balance
        assert_equal(self.nodes[0].getshieldedbalance(saplingAddr0), Decimal('15'))  # 7 prev balance + 8 received
        assert_equal(self.nodes[1].getshieldedbalance(saplingAddr1), Decimal('1'))   # 20 prev balance - (18 sent + 1 fee)
        assert_equal(self.nodes[1].getreceivedbyaddress(taddr1), Decimal('10'))
        self.log.info("Balances check out")

        # Verify existence of Sapling related JSON fields
        resp = self.nodes[0].getrawtransaction(mytxid7, 1)
        assert_equal(Decimal(resp['valueBalance']), Decimal('11.00'))    # 20 shielded input - 8 shielded spend - 1 change
        assert_equal(len(resp['vShieldedSpend']), 3)
        assert_equal(len(resp['vShieldedOutput']), 2)
        assert('bindingSig' in resp)
        shieldedSpend = resp['vShieldedSpend'][0]
        assert('cv' in shieldedSpend)
        assert('anchor' in shieldedSpend)
        assert('nullifier' in shieldedSpend)
        assert('rk' in shieldedSpend)
        assert('proof' in shieldedSpend)
        assert('spendAuthSig' in shieldedSpend)
        shieldedOutput = resp['vShieldedOutput'][0]
        assert('cv' in shieldedOutput)
        assert('cmu' in shieldedOutput)
        assert('ephemeralKey' in shieldedOutput)
        assert('encCiphertext' in shieldedOutput)
        assert('outCiphertext' in shieldedOutput)
        assert('proof' in shieldedOutput)
        self.log.info("Raw transaction decoding checks out")

        # Verify importing a spending key will update the nullifiers and witnesses correctly
        self.log.info("Checking exporting/importing a spending key...")
        sk0 = self.nodes[0].exportsaplingkey(saplingAddr0)
        saplingAddrInfo0 = self.nodes[2].importsaplingkey(sk0, "yes")
        assert_equal(saplingAddrInfo0["address"], saplingAddr0)
        assert_equal(self.nodes[2].getshieldedbalance(saplingAddrInfo0["address"]), Decimal('15'))
        sk1 = self.nodes[1].exportsaplingkey(saplingAddr1)
        saplingAddrInfo1 = self.nodes[2].importsaplingkey(sk1, "yes")
        assert_equal(saplingAddrInfo1["address"], saplingAddr1)
        assert_equal(self.nodes[2].getshieldedbalance(saplingAddrInfo1["address"]), Decimal('1'))

        # Verify importing a viewing key will update the nullifiers and witnesses correctly
        self.log.info("Checking exporting/importing a viewing key...")
        extfvk0 = self.nodes[0].exportsaplingviewingkey(saplingAddr0)
        saplingAddrInfo0 = self.nodes[3].importsaplingviewingkey(extfvk0, "yes")
        assert_equal(saplingAddrInfo0["address"], saplingAddr0)
        assert_equal(Decimal(self.nodes[3].getshieldedbalance(saplingAddrInfo0["address"], 1, True)), Decimal('15'))
        extfvk1 = self.nodes[1].exportsaplingviewingkey(saplingAddr1)
        saplingAddrInfo1 = self.nodes[3].importsaplingviewingkey(extfvk1, "yes")
        assert_equal(saplingAddrInfo1["address"], saplingAddr1)
        assert_equal(self.nodes[3].getshieldedbalance(saplingAddrInfo1["address"], 1, True), Decimal('1'))
        # no balance in the wallet
        assert_equal(self.nodes[3].getshieldedbalance(), Decimal('0'))
        # watch only balance
        assert_equal(self.nodes[3].getshieldedbalance("*", 1, True), Decimal('16.00'))

        self.log.info("All good.")

if __name__ == '__main__':
    WalletSaplingTest().main()
