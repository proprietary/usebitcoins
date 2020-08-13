======
hdkeys
======

This is a library to generate Bitcoin addresses in a heirarcical
deterministic manner as outlined in `BIP-32
<https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki>`. The
user need only provide an extended public key to generate a basically
infinite tree of public keys (2^31 per node). The user's private key,
which can remain completely offline, is able to generate all of the
addresses that the extended public key would produce.

Almost all of the code here is ripped out of Bitcoin Core, the
canonical implementatation of Bitcoin originally developed by Satoshi.
