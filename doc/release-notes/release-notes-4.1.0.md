EncoCoin Core version *3.0.1* is now available from:XNK

This is a new major version release, including various bug fixes and performance improvements, as well as updated translations.

Please report bugs using the issue tracker at github: 


Recommended Update
==============

This version is an optional, but recommended, update for all users and services.

How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely shut down (which might take a few minutes for older versions), then run the installer (on Windows) or just copy over /Applications/EncoCoin-Qt (on Mac) or encocoind/encocoin-qt (on Linux).


Compatibility
==============

EncoCoin Core is extensively tested on multiple operating systems using the Linux kernel, macOS 10.10+, and Windows 7 and later.

Microsoft ended support for Windows XP on [April 8th, 2014](https://www.microsoft.com/en-us/WindowsForBusiness/end-of-xp-support), No attempt is made to prevent installing or running the software on Windows XP, you can still do so at your own risk but be aware that there are known instabilities and issues. Please do not report issues about Windows XP to the issue tracker.

Apple released it's last Mountain Lion update August 13, 2015, and officially ended support on [December 14, 2015](http://news.fnal.gov/2015/10/mac-os-x-mountain-lion-10-8-end-of-life-december-14/). EncoCoin Core software starting with v3.2.0 will no longer run on MacOS versions prior to Yosemite (10.10). Please do not report issues about MacOS versions prior to Yosemite to the issue tracker.

EncoCoin Core should also work on most other Unix-like systems but is not frequently tested on them.


Notable Changes
==============

Hierarchical Deterministic Wallet (HD Wallet)
----------

Wallets under a tree derivation structure in which keypairs are generated deterministically from a single seed, which can be shared partially or entirely with different systems, each with or without the ability to spend coins, [BIP32](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki).

Enabling major improvements over the keystore management, the EncoCoin wallet doesn't require regular backups as before, keys are following a deterministic creation path that can be verified at any time (before HD Wallet, every keypair was randomly created and added to the keypool, forcing the user to backup the wallet every certain amount of time or could end up loosing coins forever if the latest `wallet.dat` was not being used).
As well as new possibilities like the account extended public key that enables deterministic public keys creation without the private keys requisite inside the wallet (A good use case could be online stores generating fresh addresses).

This work includes a customization/extension to the [BIP44](https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki) standard. We have included an unique staking keys derivation path which introduced the deterministic generation/recovery of staking addresses.

### HD Wallet FAQ

 - How do i upgrade to HD Wallet?

    GUI:
    1) A dialog will appear on every wallet startup notifying you that you are running a pre-HD wallet and letting you upgrade it from there.
    2) If you haven't upgraded your wallet, the topbar (bar with icons that appears at the top of your wallet) will have an "HD" icon. Click it and the upgrade dialog will be launched.

    RPC:
    1) If your wallet is unlocked, use the `-upgradewallet` flag at startup and will automatically upgrade your wallet.
    2) If your wallet is encrypted, use the `upgradewallet` rpc command. It will upgrade your wallet to the latest wallet version.

 - How do i know if i'm already running an HD Wallet?

    1) GUI: Go to settings, press on the Debug option, then Information.
    2) RPC: call `getwalletinfo`, the `walletversion` field must be `169900` (HD Wallet Feature).

Boosted wallet performance
----------

This release introduces a number of upgrades and improvements that greatly increase the wallet performance. RAM usage is reduced by ~64% and wallet's initial sync is ~42% faster.

Drop Windows 32-bit Binaries
----------

We are no longer shipping pre-compiled 32-bit Windows binaries as the demand for such binaries has been observed to be non-existent. While it may still be possible to self-compile for 32-bit Windows (x86), no efforts henceforth will be made to maintain that compatibility.

Any users currently running a 32-bit Windows OS should seek to upgrade to a 64-bit Windows OS in order to continue using the Core wallet now and in the future.

Removal of Partially translated locales
----------

From version 3.0.1 and onward, releases will no longer include any translation for languages that are not at least 80% translated.

MultiSend Disabled
----------

The MultiSend wallet feature has been effectively disabled as it's flow is clunky, out of date, and in need of a full re-code and review.

This has no effect on user funds.

GUI Changes
----------

### Keyboard navigation

Dialogs can now be accepted with the `ENTER` (`RETURN`) key, and dismissed with the `ESC` key .

### Address sorting

Address sorting in "My Addresses" / "Contacts" / "Cold Staking" can now be customized, setting it either by label (default), by address, or by creation date, ascending (default) or descending order.
Addresses in the dropdown of the "Send Transaction" and "Send Delegation" widgets are now automatically sorted by label with ascending order.

### Custom Fee

The custom fee selected when sending a transaction is now saved in the wallet database and persisted across multiple sends and wallet's restarts. The fee is now also validated against the maximum value (10000 times `minRelayTxFee`) and minimum value (`minTxFee`).

### Include delegations in send

The send and cold-staking page present a checkbox to make the automatic input selection algorithm include delegated (P2CS) utxos if needed. The option is unchecked by default.

### Optional Hiding of Staking Charts

The staking charts can now be hidden at startup (with a flag `--hidecharts`) or at runtime with a checkbox in settings --> options --> display.

### Context Lock/Unlock

Present the unlock dialog directly (instead of an error message), whenever an action on encrypted/locked wallet requires full unlock.<br>

Restore the previous locking state ("locked" or "locked for staking only") when the action is completed. 

### External Change Address Warning

A new warning/confirmation dialog is displayed if a custom change address is not part of the wallet.

### Cold Stake delegations marked in Coin Control

The Coin Control window now includes an icon next to the select checkbox when the UTXO is a Cold Stake delegation. This shares the space with the locked UTXO indicator icon, and locked UTXO's take priority in their icon display.

### Hide zXNK balance info as needed

When the wallet contains no zXNK, the zXNK balance details will be hidden, reducing visual clutter.

### CSV Exporting

Transaction and address data can now be quickly exported to a CSV file from the Settings area of the GUI.

Transaction output format is comma separated with header row as follows:
```
"Confirmed","Date","Type","Label","Address","Amount (XNK)","ID"
```

Address output format is comma separated with header row as follows:
```
"Label","Address","Type"
```


Functional Changes
----------

### zXNK Backup Removed

Automatic zXNK backup has been disabled. Thus, the following configuration options have been removed  (either as entries in the encocoin.conf file or as startup flags):
- `autozxnkbackup`
- `backupzxnk`
- `zxnkbackuppath`

### Stake-Split threshold

The stake split threshold is no longer required to be integer. It can be a fractional amount. A threshold value of 0 disables the stake-split functionality.

The default value for the stake-split threshold has been lowered from 2000 XNK, down  to 500 XNK.

### Changed command-line options

- `-debuglogfile=<file>` can be used to specify an alternative debug logging file. This can be an absolute path or a path relative to the data directory
- `-debugexclude=<category>` can be used to specify which debug categories to not log, useful when pairing with the `-debug=<exclude>` option.
- The `-reservebalance` configuration/startup option has been removed.


Dependencies
------------

The minimum required version of QT has been increased from 5.0 to 5.5.1  provides 5.9.7)


RPC Changes
------------

### Modified input/output for existing commands

- "CoinStake" JSON object in `getblock` output is removed, and replaced with the strings "stakeModifier" and "hashProofOfStake"
- "obfcompat" JSON field in `getmasternodecount` output is removed as it is/was redundant with the `enabled` field.
- "moneysupply" and "zxnkSupply" attributes in `getblock` output are removed.
- "isPublicSpend" boolean (optional) input parameter is removed from the following commands:
  - `createrawzerocoinspend`
  - `spendzerocoin`
  - `spendzerocoinmints`
  - `spendrawzerocoin`

  These commands are now able to create only *public* spends (private spends were already enabled only on regtest).

- "mintchange" and "minimizechange" boolean input parameters are removed from the following commands:
  - `spendzerocoin`

  Mints are disabled, therefore it is no longer possible to mint the change of a zerocoin spend. The change is minimized by default.

- `setstakesplitthreshold` now accepts decimal amounts. If the provided value is `0`, split staking gets disabled. `getstakesplitthreshold` returns a double.

- `dumpwallet` no longer allows overwriting files. This is a security measure
   as well as prevents dangerous user mistakes.

- The output of `getstakingstatus` was reworked. It now shows the following information:
  ```
  {
     "staking_status": true|false,       (boolean) whether the wallet is staking or not
     "staking_enabled": true|false,      (boolean) whether staking is enabled/disabled in encocoin.conf
     "coldstaking_enabled": true|false,  (boolean) whether cold-staking is enabled/disabled in encocoin.conf
     "haveconnections": true|false,      (boolean) whether network connections are present
     "mnsync": true|false,               (boolean) whether masternode data is synced
     "walletunlocked": true|false,       (boolean) whether the wallet is unlocked
     "stakeablecoins": n,                (numeric) number of stakeable UTXOs
     "stakingbalance": d,                (numeric) XNK value of the stakeable coins (minus reserve balance, if any)
     "stakesplitthreshold": d,           (numeric) value of the current threshold for stake split
     "lastattempt_age": n,               (numeric) seconds since last stake attempt
     "lastattempt_depth": n,             (numeric) depth of the block on top of which the last stake attempt was made
     "lastattempt_hash": xxx,            (hex string) hash of the block on top of which the last stake attempt was made
     "lastattempt_coins": n,             (numeric) number of stakeable coins available during last stake attempt
     "lastattempt_tries": n,             (numeric) number of stakeable coins checked during last stake attempt
   }
   ```

### Removed commands

The following commands have been removed from the RPC interface:
- `createrawzerocoinstake`
- `getmintsinblocks`
- `reservebalance`
- `getpoolinfo`

### Newly introduced commands

The following new commands have been added to the RPC interface:
- `logging` Gets and sets the logging configuration.

  When called without an argument, returns the list of categories that are currently being debug logged.<br>
  When called with arguments, adds or removes categories from debug logging.<br>
  E.g. `logging "[\"all\"]" "[\"http\"]""`


*3.0.1* Change log
==============

Detailed release notes follow. For convenience in locating the code changes and accompanying discussion, both the pull request and git merge commit are mentioned.
