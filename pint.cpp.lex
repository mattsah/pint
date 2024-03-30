
BASE_TYPES ("integer" | "cardinal" | "real" | "char" | "string" | "boolean")
FORM_TYPES ("interface" | "implementation" | "class")
TMPL_TYPES ("type" | "label" | "record")

%%

\s+                           %empty
\/\/.*                        %empty
\{[^\}]*\}                    %empty
{BASE_TYPES}                  TYPE_BASE
(['"])(.*?[^\\])?(\\\\)*(\1)  STRING
true|false                    BOOLEAN
[0-9]+\.[0-9]+                REAL
[0-9]+                        INTEGER
[a-zA-Z_][a-zA-Z0-9_]*        IDENTIFIER
[;]                           SEMICOLON
\\                            SEPARATOR
\+                            ADD
\-                            SUB
\*                            MUL
\/                            DIV

