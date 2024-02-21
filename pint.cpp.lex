DIGIT [0-9]
STRNG ['"](.*?[^\\])?(\\\\)*['"]
KWORD unit|uses|const|var|begin|end|if|then|else|return
CONTR ":"|";"|"("|")"|"."|","
OPERS "+"|"-"|"*"|"/"|"="
CMPOP "=="|">="|"<="|"<"|">"|"<>"
SPACE [ \t]+
NLINE [\n]

%%

\s+						%empty
[0-9]					INT
[0-9]+\.[0-9]+			FLOAT
[a-zA-Z_][a-zA-Z0-9_]*	SYMBOL
{STRING}				STRING
{OPERS}					OP
{CMPOP}					CMP
{CONTR}					CTRL