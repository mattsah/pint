
%%

\s+                           %empty
\{[^\}]*\}                    %empty
\/\/.*\n                      %empty
[0-9]+\.[0-9]+                REAL
[0-9]+                        INTEGER
true|false                    BOOLEAN
(['"])(.*?[^\\])?(\\\\)*(\1)  STRING
[a-zA-Z_][a-zA-Z0-9_]*        IDENTIFIER
[:]                           COLON
[;]                           SEMICOLON
\\                            SEPARATOR
