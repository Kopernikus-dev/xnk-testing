// Copyright (c) 2020 The PIVX developers
// Copyright (c) 2020 The EncoCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.
#include "sapling/sapling_operation.h"

#include "net.h" // for g_connman
#include "policy/policy.h" // for GetDustThreshold
#include "sapling/key_io_sapling.h"
#include "utilmoneystr.h"        // for FormatMoney

struct TxValues
{
    CAmount transInTotal{0};
    CAmount shieldedInTotal{0};
    CAmount transOutTotal{0};
    CAmount shieldedOutTotal{0};
    CAmount target{0};
};

OperationResult SaplingOperation::checkTxValues(TxValues& txValues, bool isFromtAddress, bool isFromShielded)
{
    assert(!isFromtAddress || txValues.shieldedInTotal == 0);
    assert(!isFromShielded || txValues.transInTotal == 0);

    if (isFromtAddress && (txValues.transInTotal < txValues.target)) {
        return errorOut(strprintf("Insufficient transparent funds, have %s, need %s",
                                  FormatMoney(txValues.transInTotal), FormatMoney(txValues.target)));
    }

    if (isFromShielded && (txValues.shieldedInTotal < txValues.target)) {
        return errorOut(strprintf("Insufficient shielded funds, have %s, need %s",
                                  FormatMoney(txValues.shieldedInTotal), FormatMoney(txValues.target)));
    }
    return OperationResult(true);
}

OperationResult loadKeysFromShieldedFrom(const libzcash::SaplingPaymentAddress &addr,
                                         libzcash::SaplingExpandedSpendingKey& expskOut,
                                         uint256& ovkOut)
{
    // Get spending key for address
    libzcash::SaplingExtendedSpendingKey sk;
    if (!pwalletMain->GetSaplingExtendedSpendingKey(addr, sk)) {
        return errorOut("Spending key not in the wallet");
    }
    expskOut = sk.expsk;
    ovkOut = expskOut.full_viewing_key().ovk;
    return OperationResult(true);
}

TxValues calculateTarget(const std::vector<SendManyRecipient>& recipients, const CAmount& fee)
{
    TxValues txValues;
    for (const SendManyRecipient &t : recipients) {
        if (t.IsTransparent())
            txValues.transOutTotal += t.transparentRecipient->nValue;
        else
            txValues.shieldedOutTotal += t.shieldedRecipient->amount;
    }
    txValues.target = txValues.shieldedOutTotal + txValues.transOutTotal + fee;
    return txValues;
}

OperationResult SaplingOperation::build()
{

    bool isFromtAddress = fromAddress.isFromTAddress();
    bool isFromShielded = fromAddress.isFromSapAddress();

    if (!isFromtAddress && !isFromShielded) {
        isFromtAddress = selectFromtaddrs;
        isFromShielded = selectFromShield;

        // It needs to have a from.
        if (!isFromtAddress && !isFromShielded) {
            return errorOut("From address parameter missing");
        }

        // Cannot be from both
        if (isFromtAddress && isFromShielded) {
            return errorOut("From address type cannot be shielded and transparent");
        }
    }

    if (recipients.empty()) {
        return errorOut("No recipients");
    }

    if (isFromShielded && mindepth == 0) {
        return errorOut("Minconf cannot be zero when sending from shielded address");
    }

    CAmount nFeeRet = (fee > 0 ? fee : minRelayTxFee.GetFeePerK());
    int tries = 0;
    while (true) {
        // First calculate target values
        TxValues txValues = calculateTarget(recipients, nFeeRet);
        OperationResult result(false);
        // Necessary keys
        libzcash::SaplingExpandedSpendingKey expsk;
        uint256 ovk;
        if (isFromShielded) {
            // Try to get the sk and ovk if we know the address from, if we don't know it then this will be loaded in loadUnspentNotes
            // using the sk of the first note input of the transaction.
            if (fromAddress.isFromSapAddress()) {
                // Get spending key for address
                auto loadKeyRes = loadKeysFromShieldedFrom(fromAddress.fromSapAddr.get(), expsk, ovk);
                if (!loadKeyRes) return loadKeyRes;
            }

            // Load and select notes to spend
            if (!(result = loadUnspentNotes(txValues, expsk, ovk))) {
                return result;
            }
        } else {
            // Sending from a t-address, which we don't have an ovk for. Instead,
            // generate a common one from the HD seed. This ensures the data is
            // recoverable, while keeping it logically separate from the ZIP 32
            // Sapling key hierarchy, which the user might not be using.
            ovk = pwalletMain->GetSaplingScriptPubKeyMan()->getCommonOVKFromSeed();
        }

        // Add outputs
        for (const SendManyRecipient &t : recipients) {
            if (t.IsTransparent()) {
                txBuilder.AddTransparentOutput(*t.transparentRecipient);
            } else {
                const auto& address = t.shieldedRecipient->address;
                const CAmount& amount = t.shieldedRecipient->amount;
                const std::string& memo = t.shieldedRecipient->memo;
                assert(IsValidPaymentAddress(address));
                std::array<unsigned char, ZC_MEMO_SIZE> vMemo = {};
                if (!(result = GetMemoFromString(memo, vMemo)))
                    return result;
                txBuilder.AddSaplingOutput(ovk, address, amount, vMemo);
            }
        }

        // If from address is a taddr, select UTXOs to spend
        // note: when spending coinbase utxos, you can only specify a single shielded addr as the change must go somewhere
        // and if there are multiple shielded addrs, we don't know where to send it.
        if (isFromtAddress && !(result = loadUtxos(txValues))) {
            return result;
        }

        const auto& retCalc = checkTxValues(txValues, isFromtAddress, isFromShielded);
        if (!retCalc) return retCalc;

        // Set change address if we are using transparent funds
        if (isFromtAddress) {
            if (!tkeyChange) {
                tkeyChange = new CReserveKey(pwalletMain);
            }
            CPubKey vchPubKey;
            if (!tkeyChange->GetReservedKey(vchPubKey, true)) {
                return errorOut("Could not generate a taddr to use as a change address");
            }
            CTxDestination changeAddr = vchPubKey.GetID();
            txBuilder.SendChangeTo(changeAddr);
        }

        // Build the transaction
        txBuilder.SetFee(nFeeRet);
        TransactionBuilderResult txResult = txBuilder.Build();
        auto opTx = txResult.GetTx();

        // Check existent tx
        if (!opTx) {
            return errorOut("Failed to build transaction: " + txResult.GetError());
        }
        finalTx = *opTx;

        // Now check fee
        bool isShielded = finalTx.IsShieldedTx();
        const CAmount& nFeeNeeded = isShielded ? GetShieldedTxMinFee(finalTx) :
                                                 GetMinRelayFee(finalTx.GetTotalSize(), false);
        if (nFeeNeeded <= nFeeRet) {
            // Check that the fee is not too high.
            CAmount nMaxFee = nFeeNeeded * (isShielded ? 100 : 10000);
            if (nFeeRet > nMaxFee) {
                return errorOut(strprintf("The transaction fee is too high: %s > %s", FormatMoney(nFeeRet), FormatMoney(100 * nFeeNeeded)));
            }
            // Done, enough fee included
            LogPrint(BCLog::SAPLING, "%s: spending %s to send %s with fee %s (min required %s)\n", __func__ , FormatMoney(txValues.target),
                    FormatMoney(txValues.shieldedOutTotal + txValues.transOutTotal), FormatMoney(nFeeRet), FormatMoney(nFeeNeeded));
            LogPrint(BCLog::SAPLING, "%s: transparent input: %s (to choose from)\n", __func__ , FormatMoney(txValues.transInTotal));
            LogPrint(BCLog::SAPLING, "%s: private input: %s (to choose from)\n", __func__ , FormatMoney(txValues.shieldedInTotal));
            LogPrint(BCLog::SAPLING, "%s: transparent output: %s\n", __func__ , FormatMoney(txValues.transOutTotal));
            LogPrint(BCLog::SAPLING, "%s: private output: %s\n", __func__ , FormatMoney(txValues.shieldedOutTotal));
            break;
        }
        if (fee > 0 && nFeeNeeded > fee) {
            // User selected fee is not enough
            return errorOut(strprintf("Fee set (%s) too low. Must be at least %s", FormatMoney(fee), FormatMoney(nFeeNeeded)));
        }
        // If we can't get the optimal fee after 100 tries, give up.
        if (++tries > 100) {
            return errorOut("Unable to compute optimal fee. Set manually.");
        }
        // include more fee and try again
        LogPrint(BCLog::SAPLING, "%s: incrementing fee: %s --> %s\n", __func__ , FormatMoney(nFeeRet), FormatMoney(nFeeNeeded));
        clearTx();
        nFeeRet = nFeeNeeded;
    }
    // Done
    fee = nFeeRet;
    return OperationResult(true);
}

OperationResult SaplingOperation::send(std::string& retTxHash)
{
    CWalletTx wtx(pwalletMain, finalTx);
    const CWallet::CommitResult& res = pwalletMain->CommitTransaction(wtx, tkeyChange, g_connman.get());
    if (res.status != CWallet::CommitStatus::OK) {
        return errorOut(res.ToString());
    }

    retTxHash = finalTx.GetHash().ToString();
    return OperationResult(true);
}

OperationResult SaplingOperation::buildAndSend(std::string& retTxHash)
{
    OperationResult res = build();
    return (res) ? send(retTxHash) : res;
}

void SaplingOperation::setFromAddress(const CTxDestination& _dest)
{
    fromAddress = FromAddress(_dest);
}

void SaplingOperation::setFromAddress(const libzcash::SaplingPaymentAddress& _payment)
{
    fromAddress = FromAddress(_payment);
}

OperationResult SaplingOperation::loadUtxos(TxValues& txValues)
{
    std::set<CTxDestination> destinations;
    if (fromAddress.isFromTAddress()) destinations.insert(fromAddress.fromTaddr);
    CWallet::AvailableCoinsFilter coinsFilter(false,
                                              false,
                                              ALL_COINS,
                                              true,
                                              true,
                                              &destinations,
                                              mindepth);
    if (!pwalletMain->AvailableCoins(&transInputs, nullptr, coinsFilter)) {
        return errorOut("Insufficient funds, no available UTXO to spend");
    }

    // sort in ascending order, so smaller utxos appear first
    std::sort(transInputs.begin(), transInputs.end(), [](const COutput& i, const COutput& j) -> bool {
        return i.Value() < j.Value();
    });

    // Final step, append utxo to the transaction

    // Get dust threshold
    CAmount dustThreshold = GetDustThreshold(minRelayTxFee);
    CAmount dustChange = -1;

    CAmount selectedUTXOAmount = 0;
    std::vector<COutput> selectedTInputs;
    for (const COutput& t : transInputs) {
        const auto& outPoint = t.tx->vout[t.i];
        selectedUTXOAmount += outPoint.nValue;
        selectedTInputs.emplace_back(t);
        if (selectedUTXOAmount >= txValues.target) {
            // Select another utxo if there is change less than the dust threshold.
            dustChange = selectedUTXOAmount - txValues.target;
            if (dustChange == 0 || dustChange >= dustThreshold) {
                break;
            }
        }
    }

    // Not enough funds
    if (selectedUTXOAmount < txValues.target) {
                return errorOut(strprintf("Insufficient transparent funds, have %s, need %s",
                                  FormatMoney(selectedUTXOAmount), FormatMoney(txValues.target)));
    }

    // If there is transparent change, is it valid or is it dust?
    if (dustChange < dustThreshold && dustChange != 0) {
        return errorOut(strprintf("Insufficient transparent funds, have %s, need %s more to avoid creating invalid change output %s (dust threshold is %s)",
                                  FormatMoney(selectedUTXOAmount), FormatMoney(dustThreshold - dustChange), FormatMoney(dustChange), FormatMoney(dustThreshold)));
    }

    transInputs = selectedTInputs;
    txValues.transInTotal = selectedUTXOAmount;

    // update the transaction with these inputs
    for (const auto& t : transInputs) {
        const auto& outPoint = t.tx->vout[t.i];
        txBuilder.AddTransparentInput(COutPoint(t.tx->GetHash(), t.i), outPoint.scriptPubKey, outPoint.nValue);
    }

    return OperationResult(true);
}

OperationResult SaplingOperation::loadUnspentNotes(TxValues& txValues,
                                                   libzcash::SaplingExpandedSpendingKey& expsk,
                                                   uint256& ovk)
{
    shieldedInputs.clear();
    pwalletMain->GetSaplingScriptPubKeyMan()->GetFilteredNotes(shieldedInputs, fromAddress.fromSapAddr, mindepth);

    for (const auto& entry : shieldedInputs) {
        std::string data(entry.memo.begin(), entry.memo.end());
        LogPrint(BCLog::SAPLING,"%s: found unspent Sapling note (txid=%s, vShieldedSpend=%d, amount=%s, memo=%s)\n",
                 __func__ ,
                 entry.op.hash.ToString().substr(0, 10),
                 entry.op.n,
                 FormatMoney(entry.note.value()),
                 HexStr(data).substr(0, 10));
    }

    if (shieldedInputs.empty()) {
        return errorOut("Insufficient funds, no available notes to spend");
    }

    // sort in descending order, so big notes appear first
    std::sort(shieldedInputs.begin(), shieldedInputs.end(),
              [](const SaplingNoteEntry& i, const SaplingNoteEntry& j) -> bool {
                  return i.note.value() > j.note.value();
              });

    // Now select the notes that we are going to use.
    std::vector<SaplingOutPoint> ops;
    std::vector<libzcash::SaplingNote> notes;
    txValues.shieldedInTotal = 0;
    CAmount dustThreshold = GetShieldedDustThreshold(minRelayTxFee);
    CAmount dustChange = -1;
    for (const auto& t : shieldedInputs) {
        // if null, load the first input sk
        if (expsk.IsNull()) {
            auto resLoadKeys = loadKeysFromShieldedFrom(t.address, expsk, ovk);
            if (!resLoadKeys) return resLoadKeys;
        }
        ops.emplace_back(t.op);
        notes.emplace_back(t.note);
        txValues.shieldedInTotal += t.note.value();
        if (txValues.shieldedInTotal >= txValues.target) {
            // Select another note if there is change less than the dust threshold.
            dustChange = txValues.shieldedInTotal - txValues.target;
            if (dustChange == 0 || dustChange >= dustThreshold) {
                break;
            }
        }
    }

    // Not enough funds
    if (txValues.shieldedInTotal < txValues.target) {
                return errorOut(strprintf("Insufficient shielded funds, have %s, need %s",
                                  FormatMoney(txValues.shieldedInTotal), FormatMoney(txValues.target)));
    }

    // Fetch Sapling anchor and witnesses
    uint256 anchor;
    std::vector<boost::optional<SaplingWitness>> witnesses;
    pwalletMain->GetSaplingScriptPubKeyMan()->GetSaplingNoteWitnesses(ops, witnesses, anchor);

    // Add Sapling spends
    for (size_t i = 0; i < notes.size(); i++) {
        if (!witnesses[i]) {
            return errorOut("Missing witness for Sapling note");
        }
        txBuilder.AddSaplingSpend(expsk, notes[i], anchor, witnesses[i].get());
    }

    return OperationResult(true);
}

OperationResult GetMemoFromString(const std::string& s, std::array<unsigned char, ZC_MEMO_SIZE>& memoRet)
{
    memoRet.fill(0x00);
    // default memo (no_memo), see section 5.5 of the protocol spec
    if (s.empty()) {
        memoRet[0] = 0xF6;
        return OperationResult(true);
    }
    // non-empty memo
    std::vector<unsigned char> rawMemo(s.begin(), s.end());
    const size_t sizeMemo = rawMemo.size();
    if (sizeMemo > ZC_MEMO_SIZE) {
        return errorOut(strprintf("Memo size of %d is too big, maximum allowed is %d", sizeMemo, ZC_MEMO_SIZE));
    }
    // copy vector into array
    for (unsigned int i = 0; i < sizeMemo; i++) {
        memoRet[i] = rawMemo[i];
    }
    return OperationResult(true);
}
