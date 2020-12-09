// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
// Copyright (c) 2020 The EncoCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "util.h"
#include "utilstrencodings.h"

#include <boost/assign/list_of.hpp>

#include <assert.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(std::make_shared<const CTransaction>(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The thick foliage and intertwined vines made the hike nearly impossible.";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256S("0x00000ae5d77e5eb3010a53e731688304019cdada4fee60dfa6b5bc424d87b2a1"))
    (500, uint256S("0x0000042ff28adc30c4ebe9faa16e8a014bd9e327866413e0e536687e7c789c65")) // genesis
    (2001, uint256S("0x034f2fbbdb4654d8796f2dc30826605861616ffe5880c6f228f7f0d115255e8e")) //new collatral
    (10001, uint256S("0x8cdd558c63de28aaf6dcb40b6409968081a004348aa7ef1daf53412ffa7c9161")) //new collatral
    (16373, uint256S("0x3a066d10c877c40f9b27fde2c51363074e54429abc1ac414f80c3fff4f8ab4ab")) // ntworksplit here? peer asking for block bed2e290edbe8c0f36ff3817a64d660c45d1873a4764bc50a2b459381844843b
    (20001, uint256S("0xf39f388b55aa469a3d8dd7582fdc1271b80743aa0b72b63b1518776ebfcf09aa")) //new collatral
    (30001, uint256S("0x697b2ab9411fa49f20d62bb25ee2fd6e09f5a08871c8e13d380c9b495e9dd9e8")) //new collatral
    (40001, uint256S("0xaa5db40ccfc839bea65c08268dd242e8508ef559b272e17729d20a75fe7f2478")) //new collatral
    (50001, uint256S("0xbd13a59a139f094aad587f191dd050b4f0853c78c88a704adeaf3fbe321608a7")) //new collatral
    (70001, uint256S("0x89b21413ba25ff759535b27324f185cf0005faf99a9bd66325dedf7e5c6ff506")) //new collatral
    (80001, uint256S("0xa2e800989a13a7f6a291a1477b57237d32520aadc1fec546d3b5506aac6e8ab8")) //new collatral
    (90001, uint256S("0x078b598b43257ee0b7e33b63458eb39623554b0130a4affbcefe4860e0072f62")) //new collatral
    (100001, uint256S("0xe0578f9a19bca0bf16f19bb9e84a28230ff505707a07fef1b69f9706af7addf3")) //new collatral
    (120001, uint256S("0x03dd81f003126e362559822d78c4d6c4fa17d19cb82aa2890bfbf5a1f8131a8d")) //new collatral
    (140001, uint256S("0x3ae4595300544e43c07b7e07de3d7103a25e392ed76ce3bc7fb8afd428107c20")) //new collatral
    (150001, uint256S("0xd2c77bec94601ef35e978774c89a3213df61bba2b27d4cfa2b4744a7842ec0a6")) //new collatral
    (182881, uint256S("0xb638593545e9c0d6460d2eab9c5ff8cef133baa95e803a0c306fc9193a571185")) // networksplit here? wallet break by Crypos and Midas
    (183002, uint256S("0x5e29fcf20d2efbcb8bc1ab7a41e87425f515d4c67277ea04f4e8b8d781701be9")) // or here Wallet was stuck by user
    (200000, uint256S("0x90784a8df844a7aa590f4ce86004ab56da0aea466bb02f4c896f1c4d6614d65b")); // additional Checkpoint
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1601693520, // * UNIX timestamp of last checkpoint block
    430162,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    3000           // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256S("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256S("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        genesis = CreateGenesisBlock(1588787853, 39886, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("00000ae5d77e5eb3010a53e731688304019cdada4fee60dfa6b5bc424d87b2a1"));
        assert(genesis.hashMerkleRoot == uint256S("be95faff7110778c545e219557a8520c5a2a973c9622b0c1ef6d4be24a234bbc"));

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~UINT256_ZERO >> 20;				// EncoCoin starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 43200;      				 // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;    				  // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;					// num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 60000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;						// 1 hour
        consensus.nStakeMinDepth = 200;
        consensus.nTargetTimespan = 40 * 60;					// 40 minutes
        consensus.nTargetTimespanV2 = 30 * 60;					// 30 minutes
        consensus.nTargetSpacing = 1 * 60;						// 1 minute
        consensus.nTimeSlotLength = 15;							// 15 seconds

        // spork keys
        consensus.strSporkPubKey = "0485126a36740117077d8c3b7a503a79b6d696d0626c1d929b4242344fe3bf7dde8377dc19679466e28fffd1aad4e6b2b3ee74de1d96de5fdb5a4bdd5ba3dd6ebe";
        consensus.strSporkPubKeyOld = "045cf0ccc3f5d859efd52234ec8f0a03f325ca05e5c153b7612a5b77b47646f1416ccffbd57228b9c0bedcfd2e08b5e215a990b3c1268885f7291eec05808bd288";
        consensus.nTime_EnforceNewSporkKey = 1566860400; 		   //!> Sporks signed after Monday, August 26, 2019 11:00:00 PM GMT must use the new spork key
        consensus.nTime_RejectOldSporkKey = 4070908800;  		   //!> Fully reject old spork key after Thursday, September 26, 2019 11:00:00 PM GMT

        // height-based activations
        consensus.height_last_ZC_AccumCheckpoint = 502;
        consensus.height_last_ZC_WrappedSerials = 15000000;
        consensus.height_start_InvalidUTXOsCheck = 999999999; 					//Start enforcing the invalid UTXO's
        consensus.height_start_ZC_InvalidSerials = 999999999;					//First block that bad serials emerged
        consensus.height_start_ZC_SerialRangeCheck = 840;						//Enforce serial range starting this block
        consensus.height_ZC_RecalcAccumulators = 999999999;						//Trigger a recalculation of accumulators

        // validation by-pass
        consensus.nEncoCoinBadBlockTime = 1471401614;						    // Skip nBit validation of Block 259201 per PR #915
        consensus.nEncoCoinBadBlockBits = 0x00;									// Skip nBit validation of Block 259201 per PR #915

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;							    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;        							    // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 4070908800;							        // October 17, 2017 4:30:00 AM

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 501;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 495;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 999999999;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 999999999;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 503;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 15000000;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 501;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 530;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock =
                uint256S("0x");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock =
                uint256S("0x");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock =
                uint256S("0xc2563e46a176297f088b89fb55cf3e1fe66a9afb0651f23b114f99b12010c2b0");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock =
                uint256S("0x");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock =
                uint256S("0x9900804cd430082d408530938d0fd3c074896981ee7f216a308f346631377e42");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock =
                uint256S("0x46acb0b0a7303835911115e617f366e02a3d9b2fe7f441df1e45f1644fb08aea");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0x2c;
        pchMessageStart[2] = 0x9b;
        pchMessageStart[3] = 0xcf;
        nDefaultPort = 43013;

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("45.77.111.105", "45.77.111.105", true);
        vSeeds.emplace_back("173.199.114.235", "173.199.114.235", true);
        vSeeds.emplace_back("45.63.11.20", "45.63.11.20", true);
        vSeeds.emplace_back("149.28.227.4", "149.28.227.4", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 9);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 93);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 63);     // starting with 'S'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ps";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviews";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "pivks";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-main";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviews";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

//        genesis = CreateGenesisBlock(1454124731, 2402015, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
//        assert(consensus.hashGenesisBlock == uint256S("0x0000084fa44e813d62c60d1443dd86164d7cea08e14420353c6c9c5e8c9a743e"));
//        assert(genesis.hashMerkleRoot == uint256S("be95faff7110778c545e219557a8520c5a2a973c9622b0c1ef6d4be24a234bbc"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;				// EncoCoin starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         			// approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      			// (only 8-blocks window for finalization on testnet)
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        			// num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5; 			// at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;						// 1 hour
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;					// 40 minutes
        consensus.nTargetTimespanV2 = 30 * 60;					// 30 minutes
        consensus.nTargetSpacing = 1 * 60;						// 1 minute
        consensus.nTimeSlotLength = 15;							// 15 seconds

        // spork keys
        consensus.strSporkPubKey = "0485126a36740117077d8c3b7a503a79b6d696d0626c1d929b4242344fe3bf7dde8377dc19679466e28fffd1aad4e6b2b3ee74de1d96de5fdb5a4bdd5ba3dd6ebe";
        consensus.strSporkPubKeyOld = "04A8B319388C0F8588D238B9941DC26B26D3F9465266B368A051C5C100F79306A557780101FE2192FE170D7E6DEFDCBEE4C8D533396389C0DAFFDBC842B002243C";
        consensus.nTime_EnforceNewSporkKey = 1566860400;    //!> August 26, 2019 11:00:00 PM GMT
        consensus.nTime_RejectOldSporkKey = 1569538800;     //!> September 26, 2019 11:00:00 PM GMT

        // height based activations
        consensus.height_last_ZC_AccumCheckpoint = 1106090;
        consensus.height_last_ZC_WrappedSerials = -1;
        consensus.height_start_InvalidUTXOsCheck = 999999999;
        consensus.height_start_ZC_InvalidSerials = 999999999;
        consensus.height_start_ZC_SerialRangeCheck = 1;			//Enforce serial range starting this block
        consensus.height_ZC_RecalcAccumulators = 999999999;

        // validation by-pass
        consensus.nEncoCoinBadBlockTime = 1489001494;			// Skip nBit validation of Block 201 per PR #915
        consensus.nEncoCoinBadBlockBits = 0x1e0a20bd;			// Skip nBit validation of Block 201 per PR #915

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;   				 // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;           				 // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1501776000;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 51197;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 201576;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 444020;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 851019;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 1106100;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 1214000;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 1347000;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 1873550;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock =
                uint256S("0x258c489f42f03cb97db2255e47938da4083eee4e242853c2d48bae2b1d0110a6");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock =
                uint256S("0xfcc6a4c1da22e4db2ada87d257d6eef5e6922347ca1bb7879edfee27d24f64b5");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock =
                uint256S("0xc54b3e7e8b710e4075da1806adf2d508ae722627d5bcc43f594cf64d5eef8b30");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock =
                uint256S("0x1822577176173752aea33d1f60607cefe9e0b1c54ebaa77eb40201a385506199");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock =
                uint256S("0x30c173ffc09a13f288bf6e828216107037ce5b79536b1cebd750a014f4939882");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0x45;
        pchMessageStart[1] = 0x76;
        pchMessageStart[2] = 0x65;
        pchMessageStart[3] = 0xba;
        nDefaultPort = 51474;

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("fuzzbawls.pw", "encocoin-testnet.seed.fuzzbawls.pw", true);
        vSeeds.emplace_back("fuzzbawls.pw", "encocoin-testnet.seed2.fuzzbawls.pw", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet encocoin addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet encocoin script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet encocoin BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet encocoin BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet encocoin BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ptestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "pivktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-test";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviewtestsapling";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";

//        genesis = CreateGenesisBlock(1454124731, 2402015, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
//        assert(consensus.hashGenesisBlock == uint256S("0x0000084fa44e813d62c60d1443dd86164d7cea08e14420353c6c9c5e8c9a743e"));
//        assert(genesis.hashMerkleRoot == uint256S("be95faff7110778c545e219557a8520c5a2a973c9622b0c1ef6d4be24a234bbc"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;				// EncoCoin starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         			// approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      			// (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        			// num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  		// at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 2;
        consensus.nTargetTimespan = 40 * 60;					// 40 minutes
        consensus.nTargetTimespanV2 = 30 * 60;					// 30 minutes
        consensus.nTargetSpacing = 1 * 60;						// 1 minute
        consensus.nTimeSlotLength = 15;							// 15 seconds

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "043969b1b0e6f327de37f297a015d37e2235eaaeeb3933deecd8162c075cee0207b13537618bde640879606001a8136091c62ec272dd0133424a178704e6e75bb7";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height based activations
        consensus.height_last_ZC_AccumCheckpoint = 310;     // no checkpoints on regtest
        consensus.height_last_ZC_WrappedSerials = -1;
        consensus.height_start_InvalidUTXOsCheck = 999999999;
        consensus.height_start_ZC_InvalidSerials = 999999999;
        consensus.height_start_ZC_SerialRangeCheck = 300;
        consensus.height_ZC_RecalcAccumulators = 999999999;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 10;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 10;
        consensus.ZC_TimeStart = 0;                 // not implemented on regtest

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 251;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 251;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 300;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 300;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 400;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 251;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight 			= 300;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nDefaultPort = 51476;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
