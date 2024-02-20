%{
#include <malloc.h>
#include <ctype.h>
#include <math.h>
#include <string>
#include <stack>

#include "scanner.hh"
#include "location.hh"

using token = pint::parser::token;

#define ALLOC_SIZE 32
#define isodigit(x) ((x) >= '0' && (x) <= '7')
#define hextoint(x) (isdigit((x)) ? (x) - '0' : ((x) - 'A') + 10)

#undef  YY_DECL
#define YY_DECL int pint::scanner::yylex(pint::parser::semantic_type * const lval, pint::parser::location_type *location )
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
#define YY_NO_UNISTD_H

#define yyterminate() return(token::END)


%}

%option debug
%option nodefault
%option yyclass="pint::scanner"
%option noyywrap
%option c++

%x COMNT

DIGIT [0-9]
IDENT [a-zA-Z_][a-zA-Z0-9_]*
KWORD unit|uses|const|var|begin|end|if|then|else|return
CONTR ":"|";"|"("|")"|"."|","
OPERS "+"|"-"|"*"|"/"|"="
CMPOP "=="|">="|"<="|"<"|">"|"<>"
SPACE [ \t]+
NLINE [\n]

%%
%{
	yylval = lval;
%}

"{"             BEGIN(COMNT);
<COMNT>[^}\n]+
<COMNT>\n
<COMNT><<EOF>>  // pint::parser::error(loc, "EOF in comment");
<COMNT>"}"      BEGIN(INITIAL);

"//".* {

}

"'"|"\"" {
	 int inch, count, max_size, temp;
	 char *buffer;

	 buffer   = (char *) malloc(ALLOC_SIZE);
	 max_size = ALLOC_SIZE;
	 inch     = yyinput();
	 count    = 0;

	 while(inch != EOF && inch != yytext[0] && inch != '\n'){
		if(yytext[0] != '\'' && inch == '\\'){
			inch = yyinput();

			switch(inch) {
				case '\n': inch = yyinput(); break;
				case 'b' : inch = '\b';    break;
				case 't' : inch = '\t';    break;
				case 'n' : inch = '\n';    break;
				case 'v' : inch = '\v';    break;
				case 'f' : inch = '\f';    break;
				case 'r' : inch = '\r';    break;
				case 'X' :
				case 'x' : inch = yyinput();
					if(isxdigit(inch)) {
						temp = hextoint(toupper(inch));
						inch = yyinput();

						if(isxdigit(inch)){
							temp = (temp << 4) + hextoint(toupper(inch));
						} else {
							unput(inch);
						}

						inch = temp;
					} else {
						unput(inch);
						inch = 'x';
					}
					break;
				default:
					if(isodigit(inch)) {
						temp = inch - '0';
						inch = yyinput();
						if(isodigit(inch)) {
							temp = (temp << 3) + (inch - '0');
						} else {
							unput(inch);
							goto done;
						}

						inch = yyinput();

						if(isodigit(inch)){
							temp = (temp << 3) + (inch - '0');
						} else {
							unput(inch);
						}

						done:
							inch = temp;
					}
					break;
			}
		}

		buffer[count++] = inch;

		if(count >= max_size){
			buffer = (char *) realloc(buffer,max_size + ALLOC_SIZE);
			max_size += ALLOC_SIZE;
		}

		inch = yyinput();
	}

	if(inch == EOF || inch == '\n'){
		// pint::parser::syntax_error(loc, "Unterminated string.");
	}

	buffer[count] = '\0';
	printf("A string: \"%s\"\n",buffer);
	free(buffer);

	return token::COMMENT;
}

{DIGIT}+ {
	printf(yytext);
}

{DIGIT}+"."{DIGIT}* {
	printf(yytext);
}

{KWORD} {
	printf(yytext);

	if (yytext == "unit") return token::UNIT;
	if (yytext == "uses") return token::USES;
}

{IDENT} {
	printf(yytext);

	return token::IDENTIFIER;
}

{OPERS} {
	printf(yytext);
}

{CONTR} {
	printf(yytext);

	if (yytext == ";") return token::SEMICOLON;
	if (yytext == ",") return token::COMMA;
}

{NLINE} {
	printf(yytext);
}

{SPACE} {
	printf(yytext);
}

. {
	return *yytext;
}

<<EOF>> {
	return 0;
}

%%

