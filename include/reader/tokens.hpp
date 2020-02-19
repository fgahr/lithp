#ifndef _LITHP_READER_TOKENS_H_
#define _LITHP_READER_TOKENS_H_

#define SPACE ' '
#define TAB '\t'
#define NEWLINE '\n'

#ifdef LITHP_READER_STRING_TOKENS
#define LPAREN "("
#define RPAREN ")"
#define LBRACK "["
#define RBRACK "]"
#define LBRACE "{"
#define RBRACE "}"
#define SQUOTE "'"
#define DQUOTE "\""
#define QQUOTE "`"
#define QCOMMA ","
#else // LITHP_READER_STRING_TOKENS
#define LPAREN '('
#define RPAREN ')'
#define LBRACK '['
#define RBRACK ']'
#define LBRACE '{'
#define RBRACE '}'
#define SQUOTE '\''
#define DQUOTE '"'
#define QQUOTE '`'
#define QCOMMA ','
#endif // LITHP_READER_STRING_TOKENS

#endif // _LITHP_READER_TOKENS_H_
