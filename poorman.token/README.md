# poorman.token
EOS Token Contract with "Burning" and "Signup" built in (for making the token RAM cheap)

This token uses the standard eosio.token contract with two modifications.

## The poorman.token test is available on the mainnet

Everyone feel free to signup for the "Free airdrop test" of the poormantoken

You can go this at https://eostoolkit.io/airgrab

### To signup for an airdrop:
`cleos push action poormantoken signup '{"owner":"iwanttokens1","quantity":"0.0000 POOR"}' -p iwanttokens1@active`

The signup function allows an account to create a balance entry using their own personal ram.

They signup restricts the quantity to 0 however you could configure this in the contract code to allow a "signup bonus".

You would have to add some controls to prevent double signups though, so 0 quantity signup is safest.

### To burn tokens run the command:
`cleos push action poormantoken burn '{"from":"tokensowner1","quantity":"1.0000 POOR","memo":"Lets remove POOR supply!"}' -p tokensowner1@active`

The burn function burns the token from the "from account" and also reduces the supply.

The burn function makes sure you can't burn more tokens than supply.
