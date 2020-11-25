EncoCoin Core integration/staging repository
=====================================

[![Build Status](https://travis-ci.org/EncoCoin-Project/EncoCoin.svg?branch=master)](https://travis-ci.org/EncoCoin-Project/EncoCoin) [![GitHub version](https://badge.fury.io/gh/EncoCoin-Project%2FEncoCoin.svg)](https://badge.fury.io/gh/EncoCoin-Project%2FEncoCoin)

EncoCoin is an open source crypto-currency focused on fast private transactions using the Zerocoin protocol, with low transaction fees & environmental footprint.  It utilizes the first ever anonymous proof of stake protocol, called zPoS, combined with regular PoS and masternodes for securing its network. zPoS incentivises using the privacy features available in EncoCoin by granting a higher block reward for zPoS over regular PoS and masternodes. In practice EncoCoin has between 4 to 10 times higher use of it's privacy features in comparison to other coins that combine public and private transactions. This is thanks to innovations like zPoS and integrating the Zerocoin protocol into light/mobile wallets, allowing for a complete and lightweight privacy protocol that can be used on the go. As well as security improvements like [deterministic generation of zXNK for easy backups.](https://www.reddit.com/r/encocoin/comments/8gbjf7/how_to_use_deterministic_zerocoin_generation/)
The goal of EncoCoin is to achieve a decentralized sustainable crypto currency with near instant full-time private transactions, fair governance and community intelligence.
- Anonymized transactions & consensus using the [_Zerocoin Protocol_](http://www.encocoin.org/zpiv) and [zPoS](https://encocoin.org/zpos/).
- light/mobile wallet privacy using the [Zerocoin Light Node Protocol](https://encocoin.org/wp-content/uploads/2018/11/Zerocoin_Light_Node_Protocol.pdf)
- Fast transactions featuring guaranteed zero confirmation transactions, we call it _SwiftX_.
- Decentralized blockchain voting utilizing Masternode technology to form a DAO. The blockchain will distribute monthly treasury funds based on successful proposals submitted by the community and voted on by the DAO.

More information at [encocoin.net](http://www.encocoin.net) Visit our ANN thread at [BitcoinTalk](https://bitcointalk.org/index.php?topic=5028776.0). Join the community at [PIVX Discord](https://discord.gg/JFnYdyv).

### Coin Specs
<table>
<tr><td>Algorithm</td><td>Quark</td></tr>
<tr><td>Block type</td><td>PoS/MN</tr>
<tr><td>Block Time</td><td>60 Seconds</td></tr>
<tr><td>Coin Ticker</td><td>XNK</td></tr>
<tr><td>P2P Port</td><td>43013</td></tr>
<tr><td>RPC Port</td><td>43014</td></tr>
<tr><td>Address Prefixes</td><td>4 or 5</td></tr>
<tr><td>Total Coin Supply</td><td>60000000</td></tr>
<tr><td>Premine</td><td>6%</td></tr>
<tr><td>MN confirmations</td>15 blocks<td></td></tr>
<tr><td>TXN confirmations</td>6 blocks<td></td></tr>
</table>

MN Rewards needs 100(+ 1) confirmations to mature
Staking begins with 200(+ 1) confirmations


### Reward Distribution

<table>
<th>Phase</th><th>Block Height</th><th>MN Collateral</th><th>Block Reward</th><th>MN Reward</th><th>Stake Reward</th>
<tr><td>Phase 1</td><td>0-500</td><td>0</td><td><0.5/td><td>0</td><td>0</td></tr>
<tr><td>Phase 2</td><td>501-2000</td><td>9000</td><td>0.1</td><td>0.0999</td><td>0.0001</td></tr>
<tr><td>Phase 3</td><td>2001-10000</td><td><15000/td><td>0.15</td><td>0.1499</td><td>0.0001</td></tr>
<tr><td>Phase 4</td><td>10001-20000</td><td>20000</td><td>0.3</td><td>0.2999</td><td>0.0001</td></tr>
<tr><td>Phase 5</td><td>20001-30000</td><td>21000</td><td>0.6</td><td>0.5999</td><td>0.0001</td></tr>
<tr><td>Phase 6</td><td>30001-40000</td><td>22000</td><td>1.2</td><td>1.1999</td><td>0.0001</td></tr>
<tr><td>Phase 7</td><td>40001-50000</td><td>25000</td><td>1.5</td><td>1.4999</td><td>0.0001</td></tr>
<tr><td>Phase 8</td><td>50001-70000</td><td>28000</td><td>3.0</td><td>2.9999</td><td>0.0001</td></tr>
<tr><td>Phase 9</td><td>70001-80000</td><td>30000</td><td>6.0</td><td>5.9999</td><td>0.0001</td></tr>
<tr><td>Phase 10</td><td>80001-90000</td><td>35000</td><td>7.0</td><td>6.9999</td><td>0.0001</td></tr>
<tr><td>Phase 11</td><td>90001-100000></td><td>36000</td><td>7.5</td><td>7.4999</td><td>0.0001</td></tr>
<tr><td>Phase 12</td><td>100001-120000</td><td>40000</td><td>15.0</td><td>14.9999</td><td>0.0001</td></tr>
<tr><td>Phase 13</td><td>120001-140000</td><td>45000</td><td>16.5</td><td>16.4999</td><td>0.0001</td></tr>
<tr><td>Phase 14</td><td>140001-150000</td><td>46000</td><td>17.0</td><td>16.9999</td><td>0.0001</td></tr>
<tr><td>Phase 15</td><td>150001-∞</td><td>50000</td><td>20.0</td><td>19.995</td><td>0.005</td></tr>
</table>

## Masternode Setup

If you like to create a Masternode, [click here] (https://github.com/Encocoin/xnk-mn-install) for more Informations