%skeleton "lalr1.cc"
%require "3.8"
%debug
%defines
%define api.namespace { pint }
%define api.value.type variant
%define parse.assert

%code requires {
	namespace pint {
		class driver;
		class scanner;
	}
}

%parse-param { scanner &_scanner }
%parse-param { driver &_driver }

%code {
	#include <iostream>
	#include <cstdlib>
	#include <fstream>

	#include "driver.hh"

	#undef yylex
	#define yylex _scanner.yylex
}

%token
				END				0	"end of file"
				UNIT
				USES
				COMMA
				COMMENT
				IDENTIFIER
				SEMICOLON
;

%%

code
	: unit_stmt
	| header_stmt
;

unit_stmt
	: UNIT IDENTIFIER SEMICOLON {
		printf("Unit Statement");
	}
;

header_stmt
	:
	| comment
	| uses_stmt
;

comment
	: COMMENT {
		printf("Comment Found");
	}
;

uses_stmt
	: USES ilist SEMICOLON {
		printf("Use Statement");
	}
;

ilist
	: IDENTIFIER {

	}
	| ilist ',' IDENTIFIER {

	}
;

%%

void pint::parser::error(const location_type & loc, const std::string & message)
{
	std::cerr << "Error: " << message << " at " << loc << "\n";
}