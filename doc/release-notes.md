EncoCoin Core *version* is now available from:  <https://github.com/encocoin-project/encocoin/releases>

This is a new revision version release, including various bug fixes and performance improvements, as well as updated translations.

Please report bugs using the issue tracker at github: <https://github.com/encocoin-project/encocoin/issues>


Recommended Update
==============



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

### Removed zerocoin GUI

Spending zXNK and getting zXNK balance information is no longer available in the graphical interface. The feature remains accessible through the RPC interface: `getzerocoinbalance`, `listmintedzerocoins`, `listzerocoinamounts`, `spendzerocoin`, `spendzerocoinmints`.


### Memory pool limiting

Previous versions of EncoCoin Core had their mempool limited by checking a transaction's fees against the node's minimum relay fee. There was no upper bound on the size of the mempool and attackers could send a large number of transactions paying just slighly more than the default minimum relay fee to crash nodes with relatively low RAM.

EncoCoin Core 3.1.0 will have a strict maximum size on the mempool. The default value is 300 MB and can be configured with the `-maxmempool` parameter. Whenever a transaction would cause the mempool to exceed its maximum size, the transaction that (along with in-mempool descendants) has the lowest total feerate (as a package) will be evicted and the node's effective minimum relay feerate will be increased to match this feerate plus the initial minimum relay feerate. The initial minimum relay feerate is set to 1000 satoshis per kB.

EncoCoin Core 3.1.0 also introduces new default policy limits on the length and size of unconfirmed transaction chains that are allowed in the mempool (generally limiting the length of unconfirmed chains to 25 transactions, with a total size of 101 KB). These limits can be overridden using command line arguments

### Benchmarking Framework

EncoCoin Core 3.1.0 backports  the internal benchmarking framework from Bitcoin Core, which can be used to benchmark cryptographic algorithms (e.g. SHA1, SHA256, SHA512, RIPEMD160, Poly1305, ChaCha20), Base58 encoding and decoding and thread queue. More tests are needed for script validation, coin selection and coins database, cuckoo cache, p2p throughtput

The binary file is compiled with encocoin-core, unless configured with `--disable-bench`.<br>
After compiling encocoin-core, the benchmarks can be run with:
```
src/bench/bench_encocoin
```
The output will be similar to:
```
#Benchmark,count,min(ns),max(ns),average(ns),min_cycles,max_cycles,average_cycles
Base58CheckEncode,131072,7697,8065,7785,20015,20971,20242
```

'label' and 'account' APIs for wallet
-------------------------------------

A new 'label' API has been introduced for the wallet. This is intended as a
replacement for the deprecated 'account' API. The 'account' can continue to
be used in v3.1 by starting encocoind with the '-deprecatedrpc=accounts'
argument, and will be fully removed in v5.0.

The label RPC methods mirror the account functionality, with the following functional differences:

- Labels can be set on any address, not just receiving addresses. This functionality was previously only available through the GUI.
- Labels can be deleted by reassigning all addresses using the `setlabel` RPC method.
- There isn't support for sending transactions _from_ a label, or for determining which label a transaction was sent from.
- Labels do not have a balance.

Here are the changes to RPC methods:

| Deprecated Method       | New Method            | Notes       |
| :---------------------- | :-------------------- | :-----------|
| `getaccount`            | `getaddressinfo`      | `getaddressinfo` returns a json object with address information instead of just the name of the account as a string. |
| `getaccountaddress`     | n/a                   | There is no replacement for `getaccountaddress` since labels do not have an associated receive address. |
| `getaddressesbyaccount` | `getaddressesbylabel` | `getaddressesbylabel` returns a json object with the addresses as keys, instead of a list of strings. |
| `getreceivedbyaccount`  | `getreceivedbylabel`  | _no change in behavior_ |
| `listaccounts`          | `listlabels`          | `listlabels` does not return a balance or accept `minconf` and `watchonly` arguments. |
| `listreceivedbyaccount` | `listreceivedbylabel` | Both methods return new `label` fields, along with `account` fields for backward compatibility. |
| `move`                  | n/a                   | _no replacement_ |
| `sendfrom`              | n/a                   | _no replacement_ |
| `setaccount`            | `setlabel`            | Both methods now: <ul><li>allow assigning labels to any address, instead of raising an error if the address is not receiving address.<li>delete the previous label associated with an address when the final address using that label is reassigned to a different label, instead of making an implicit `getaccountaddress` call to ensure the previous label still has a receiving address. |

| Changed Method         | Notes   |
| :--------------------- | :------ |
| `listunspent`          | Returns new `label` fields, along with `account` fields for backward compatibility if running with the `-deprecatedrpc=accounts` argument |
| `sendmany`             | The first parameter has been renamed to `dummy`, and must be set to an empty string, unless running with the `-deprecatedrpc=accounts` argument (in which case functionality is unchanged). |
| `listtransactions`     | The first parameter has been renamed to `dummy`, and must be set to the string `*`, unless running with the `-deprecatedrpc=accounts` argument (in which case functionality is unchanged). |
| `getbalance`           | `account`, `minconf` and `include_watchonly` parameters are deprecated, and can only be used if running with the `-deprecatedrpc=accounts` argument |
| `getcoldstakingbalance`| The `account` parameter is deprecated, and can only be used if running with the `-deprecatedrpc=accounts` argument (in which case functionality is unchanged) |
| `getdelegatedbalance`  | The `account` parameter is deprecated, and can only be used if running with the `-deprecatedrpc=accounts` argument (in which case functionality is unchanged) |

GUI Changes
----------

### Topbar navigation

- The "sync" button in the GUI topbar can be clicked to go directly to the Settings --> Information panel (where the current block number and hash is shown).

- The "connections" button in the GUI topbar can be clicked to open the network monitor dialog

Functional Changes
----------

### Stake-Split threshold

If the stake split is active (threshold > 0), then stake split threshold value must be greater than a minimum, set by default at 100 XNK. The minimum value can be changed using the `-minstakesplit` startup flag . A value `0` is still allowed, regardless of the minimum set, and, as before, can be used to disable the stake splitting functionality.

### Changed command-line options

- new command `-minstakesplit` to modify the minimum allowed for  the stake split threshold

- new commands `-maxmempool`, to customize  the memory pool size limit, and `-checkmempool=N`, to customize the frequency of the mempool check

- new commands `-limitancestorcount=N` and `limitancestorsize=N`, to limit the number and total size of all in-mempool ancestors for a transaction

- new commands `-limitdescendantcount=N` and `limitdescendantsize=N`, to limit the number and total size of all in-mempool descendants for a transaction

RPC Changes
------------

In addition to the afore mentioned 'label' and 'account' API changes, other RPC changes are as follows:

### Low-level API changes

- The `asm` property of each scriptSig now contains the decoded signature hash type for each signature that provides a valid defined hash type.<br>
The following items contain assembly representations of scriptSig signatures
and are affected by this change: RPC `getrawtransaction`, RPC `decoderawtransaction`, REST `/rest/tx/` (JSON format), REST `/rest/block/` (JSON format when including extended tx details), `encocoin-tx -json`

### Modified input/output for existing commands

- new "usage" field in the output of `getmempoolinfo`, displaying the total memory usage for the mempool

- new "upgrades" field in the output of `getblockchaininfo`, showing upcoming and active network upgrades

### Removed commands

- `masternodedebug`. Use `getmasternodestatus` instead.

### Newly introduced commands


*3.1.0* Change log
==============

Detailed release notes follow. This overview includes changes that affect behavior, not code moves, refactors and string updates. For convenience in locating the code changes and accompanying discussion, both the pull request and git merge commit are mentioned.

### Core Features

### Build System

### P2P Protocol and Network Code

### GUI

### RPC/REST

### Wallet

### Miscellaneous

## Credits

Thanks to everyone who directly contributed to this release:

As well as everyone that helped translating on [Transifex](https://www.transifex.com/projects/p/encocoin-project-translations/), the QA team during Testing and the Node hosts supporting our Testnet.
