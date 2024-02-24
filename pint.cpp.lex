KEYWORDS unit|uses|const|var|begin|end|if|then|else|return

%%

\s+							%empty
\{[^\}]*\}					%empty
\/\/.*\n					%empty
unit						UNIT
uses						USES
const						CONST
var							VAR
begin						BEGIN
if							IF
then						THEN
else						ELSE
case						CASE
is							IS
end							END
constructor					CONSTRUCTOR
function					FUNCTION
true|false					BOOL
[0-9]						INT
[0-9]+\.[0-9]+				FLOAT
['"](.*?[^\\])?(\\\\)*['"]	STRING
[a-zA-Z_][a-zA-Z0-9_]*		IDENTIFIER
\;							SEMICOLON
\\							SEPARATOR