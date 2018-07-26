#pragma once
#include <eosiolib/core_symbol.hpp>
#include <eosiolib/serialize.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/system.h>
#include <tuple>
#include <limits>

namespace eosio {

   static constexpr uint64_t string_to_symbol( uint8_t precision, const char* str ) {
      uint32_t len = 0;
      while( str[len] ) ++len;

      uint64_t result = 0;
      for( uint32_t i = 0; i < len; ++i ) {
         if( str[i] < 'A' || str[i] > 'Z' ) {
            /// ERRORS?
         } else {
            result |= (uint64_t(str[i]) << (8*(1+i)));
         }
      }

      result |= uint64_t(precision);
      return result;
   }

   #define S(P,X) ::eosio::string_to_symbol(P,#X)

   typedef uint64_t symbol_name;

   // is_valid_symbol(symbol명) : 심볼명 유효성 검사
   static constexpr bool is_valid_symbol( symbol_name sym ) {
      sym >>= 8;
      for( int i = 0; i < 7; ++i ) {
         char c = (char)(sym & 0xff);

         // A ~ Z 이내의 symbol을 사용하지 않으면 false
         if( !('A' <= c && c <= 'Z')  ) return false;
         sym >>= 8;
         if( !(sym & 0xff) ) {
            do {
              sym >>= 8;
              if( (sym & 0xff) ) return false;
              ++i;
            } while( i < 7 );
         }
      }
      return true;
   }

   static constexpr uint32_t symbol_name_length( symbol_name tmp ) {
      tmp >>= 8; /// skip precision
      uint32_t length = 0;
      while( tmp & 0xff && length <= 7) {
         ++length;
         tmp >>= 8;
      }

      return length;
   }

   struct symbol_type {

      // value 선언(symbol_name, unit64_t타입)
      symbol_name value;

      // symbol_type 생성자?
      symbol_type() { }
      // symbol_type 오버로딩?
      symbol_type(symbol_name s): value(s) { }

      // 심볼명 유효성 검사
      bool     is_valid()const  { return is_valid_symbol( value ); }
      uint64_t precision()const { return value & 0xff; }
      uint64_t name()const      { return value >> 8;   }
      uint32_t name_length()const { return symbol_name_length( value ); }
      /// 심볼명 유효성 검사

      // symbol_name() : 심볼명 반환
      operator symbol_name()const { return value; }

      // print() : 메세지 프린트(뭔지모름)
      void print(bool show_precision=true)const {
         if( show_precision ){
            ::eosio::print(precision());
            prints(",");
         }

         auto sym = value;
         sym >>= 8;
         for( int i = 0; i < 7; ++i ) {
            char c = (char)(sym & 0xff);
            if( !c ) return;
            prints_l(&c, 1 );
            sym >>= 8;
      }

    }
      EOSLIB_SERIALIZE( symbol_type, (value) )
   };

   struct extended_symbol : public symbol_type
   {
      extended_symbol( symbol_name s = 0, account_name c = 0 ):symbol_type{s},contract(c){}

      account_name contract;

      void print()const {
         symbol_type::print();
         prints("@");
         printn( contract );
      }

      friend bool operator == ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.value, a.contract ) == std::tie( b.value, b.contract );
      }
      friend bool operator != ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.value, a.contract ) != std::tie( b.value, b.contract );
      }
      EOSLIB_SERIALIZE( extended_symbol, (value)(contract) )
   };

} /// namespace eosio
