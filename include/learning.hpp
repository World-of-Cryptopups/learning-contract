#include <eosio/eosio.hpp>
using namespace eosio;

CONTRACT learning : public contract {
   public:
      using contract::contract;

      ACTION hi( name nm );


      /*
       * Allows `issuer` account to create a token in supply of `maximum_supply`. If validation is successful, a new entry in statstable for token symbol gets created.
       *
       * @param issuer - the accoint theat creates the token
       * @param maximum_supply - the maximum supply set for the token created
       *
       * @pre Token symbol has to be valid
       * @pre Token symbol must not be already created
       * @pre maximum_supply has to be smaller than the maximum supply allowed by the system: 1^62 - 1
       * @pre Maximum supply must be positive
       * */
      ACTION create(name issuer, asset maximum_supply);

      /*
       * This action issues to `to` account a `quantity` of tokens.
       *
       * @param to - the account to issue tokens to, it must be the same as the issuer
       * @param quantity - the amount of tokens to be issued
       * @param memo - the memo string that accompanies the token issue transaction
       * */
      ACTION issue(name to, asset quantity, const string& memo);

      /*
       * The opposite for create action, if all validation succeed, it debits the statstable.supply amount.
       *
       * @param quantity - the quantity of tokens to retire
       * @param memo - the memo string to accompany the transaction
       * */
      ACTION retire(asset quantity, const string& memo);

      /*
       * Allows `from` account to transfer to `to` account the `quantity` tokens.
       * ONe account is debited and the other is credited with quantity tokens.
       *
       * @param from - the account to transfer from
       * @param to - the account to be transferred to,
       * @param quantity - the quantity of tokens to be transferred,
       * @param memo - the memo string to accompany the transaction
       * */
      ACTION transfer(name from, name to, asset quantity, const string& memo);

      /*
       * Allows `ram_payer` to create an account `owner` with zero balance for token `symbol` at the expense of `ram_payer`.
       *
       * @param owner - the account to be created
       * @param symbol - the token to be payed with by `ram_payer`
       * @param ram_payer - the account that supports the cost of this action
       *
       * More information can be read [here](https://github.com/EOSIO/eosio.contracts/issues/62)
       * and [here](https://github.com/EOSIO/eosio.contracts/issues/61)
       * */
      ACTION open(name owner, symbol symbol, name ram_payer);


      /*
       * Action is the opposite for open, it closes the account `owner` for token `symbol`
       *
       * @param owner - the owner account to execute the close action for.
       * @param symbol - the symbol of the token to execute the close action for
       *
       * @pre The pair of owner plus symbol has to exist otherwise no action is executed,
       * @pre If the pair of owner plus symbol exists, the balance has to be zero.
       * */
      ACTION close(name owner, symbol symbol);


      static asset get_supply(const name& token_contract_account, const symbol_code& sym_code) 
      {
      	stats statstable( token_contract_account, sym_code.raw() );
       	const auto& st = statstable.get( sym_code.raw(), "invalid supply symbol code" );
	      return st.supply;
      }

      static asset get_balance(const name& token_contract_account, const name& owner, const symbol_code& sym_code)
      {
      	accounts accountstable( token_contract_account, owner.value );
	      const auto& ac = accountstable.get( sym_code.raw(), "no balance with specified symbol" );
	      return ac.balance;
      }



      using hi_action = action_wrapper<"hi"_n, &learning::hi>;
      using create_action = action_wrapper<"create"_n, &learning::create>;
      using issue_action = action_wrapper<"issue"_n, &learning::issue>;
      using retire_action = action_wrapper<"retire"_n, &learning::retire>;
      using transfer_action = action_wrapper<"transfer"_n, &learning::transfer>;
      using open_action = action_wrapper<"open"_n, &learning::open>;
      using close_action = action_wrapper<"close"_n, &learning::close>;


   private:
      TABLE account {
      	asset balance;

	uint64_t primary_key() const { return balance.symbol.code().raw(); }
      };

      TABLE currency_stats {
      	asset       supply;
	      asset       max_supply;
	      name        issuer;

	uint64_t primary_key() const { return supply.symbol.code().raw(); }
      };

      typedef eosio::multi_index< "accounts"_n, account > accounts;
      typedef eosio::multi_index< "stat"_n, currency_stats > stats;


      void sub_balance( const name& owner, const asset& value );
      void add_balance( const name& owner, const asset& value, const name& ram_payer );

};
