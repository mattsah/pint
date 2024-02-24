/**
 * LR parser for C++ generated by the Syntax tool.
 *
 * https://www.npmjs.com/package/syntax-cli
 *
 *   npm install -g syntax-cli
 *
 *   syntax-cli --help
 *
 * To regenerate run:
 *
 *   syntax-cli \
 *     --grammar ~/path-to-grammar-file \
 *     --mode <parsing-mode> \
 *     --output ~/ParserClassName.h
 */
#ifndef __Syntax_LR_Parser_h
#define __Syntax_LR_Parser_h

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"

#include <assert.h>
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

// ------------------------------------
// Module include prologue.
//
// Should include at least value/result type:
//
// type Value = <...>;
//
// Or struct Value { ... };
//
// Can also include parsing hooks:
//
//   void onParseBegin(const Parser& parser, const std::string& str) {
//     ...
//   }
//
//   void onParseBegin(const Parser& parser, const Value& result) {
//     ...
//   }
//
// clang-format off
#include "tree.h"

	using namespace pint;
	using Value = std::shared_ptr<Expr>;  // clang-format on

namespace syntax {

/**
 * Tokenizer class.
 */
// clang-format off
/**
 * Generic tokenizer used by the parser in the Syntax tool.
 *
 * https://www.npmjs.com/package/syntax-cli
 */

#ifndef __Syntax_Tokenizer_h
#define __Syntax_Tokenizer_h

class Tokenizer;

// ------------------------------------------------------------------
// TokenType.

enum class TokenType {
  __EMPTY = -1,
  // clang-format off
  SEMICOLON = 10,
  IDENTIFIER = 11,
  UNIT = 12,
  SEPARATOR = 13,
  USES = 14,
  CONST = 15,
  VAR = 16,
  BEGIN = 17,
  IF = 18,
  THEN = 19,
  ELSE = 20,
  CASE = 21,
  IS = 22,
  END = 23,
  CONSTRUCTOR = 24,
  FUNCTION = 25,
  BOOL = 26,
  INT = 27,
  FLOAT = 28,
  STRING = 29,
  TOKEN_TYPE_30 = 30,
  TOKEN_TYPE_31 = 31,
  TOKEN_TYPE_32 = 32,
  __EOF = 33
  // clang-format on
};

// ------------------------------------------------------------------
// Token.

struct Token {
  TokenType type;
  std::string value;

  int startOffset;
  int endOffset;
  int startLine;
  int endLine;
  int startColumn;
  int endColumn;
};

using SharedToken = std::shared_ptr<Token>;

typedef TokenType (*LexRuleHandler)(const Tokenizer&, const std::string&);

// ------------------------------------------------------------------
// Lex rule: [regex, handler]

struct LexRule {
  std::regex regex;
  LexRuleHandler handler;
};

// ------------------------------------------------------------------
// Token.

enum TokenizerState {
  // clang-format off
  INITIAL
  // clang-format on
};

// ------------------------------------------------------------------
// Tokenizer.

class Tokenizer {
 public:
  /**
   * Initializes a parsing string.
   */
  void initString(const std::string& str) {
    str_ = str;

    // Initialize states.
    states_.clear();
    states_.push_back(TokenizerState::INITIAL);

    cursor_ = 0;
    currentLine_ = 1;
    currentColumn_ = 0;
    currentLineBeginOffset_ = 0;

    tokenStartOffset_ = 0;
    tokenEndOffset_ = 0;
    tokenStartLine_ = 0;
    tokenEndLine_ = 0;
    tokenStartColumn_ = 0;
    tokenEndColumn_ = 0;
  }

  /**
   * Whether there are still tokens in the stream.
   */
  inline bool hasMoreTokens() { return cursor_ <= str_.length(); }

  /**
   * Returns current tokenizing state.
   */
  TokenizerState getCurrentState() { return states_.back(); }

  /**
   * Enters a new state pushing it on the states stack.
   */
  void pushState(TokenizerState state) { states_.push_back(state); }

  /**
   * Alias for `push_state`.
   */
  void begin(TokenizerState state) { states_.push_back(state); }

  /**
   * Exits a current state popping it from the states stack.
   */
  TokenizerState popState() {
    auto state = states_.back();
    states_.pop_back();
    return state;
  }

  /**
   * Returns next token.
   */
  SharedToken getNextToken() {
    if (!hasMoreTokens()) {
      yytext = __EOF;
      return toToken(TokenType::__EOF);
    }

    auto strSlice = str_.substr(cursor_);

    auto lexRulesForState = lexRulesByStartConditions_.at(getCurrentState());

    for (const auto& ruleIndex : lexRulesForState) {
      auto rule = lexRules_[ruleIndex];
      std::smatch sm;

      if (std::regex_search(strSlice, sm, rule.regex)) {
        yytext = sm[0];

        captureLocations_(yytext);
        cursor_ += yytext.length();

        // Manual handling of EOF token (the end of string). Return it
        // as `EOF` symbol.
        if (yytext.length() == 0) {
          cursor_++;
        }

        auto tokenType = rule.handler(*this, yytext);

        if (tokenType == TokenType::__EMPTY) {
          return getNextToken();
        }

        return toToken(tokenType);
      }
    }

    if (isEOF()) {
      cursor_++;
      yytext = __EOF;
      return toToken(TokenType::__EOF);
    }

    throwUnexpectedToken(std::string(1, strSlice[0]), currentLine_,
                         currentColumn_);
  }

  /**
   * Whether the cursor is at the EOF.
   */
  inline bool isEOF() { return cursor_ == str_.length(); }

  SharedToken toToken(TokenType tokenType) {
    return std::shared_ptr<Token>(new Token{
        .type = tokenType,
        .value = yytext,
        .startOffset = tokenStartOffset_,
        .endOffset = tokenEndOffset_,
        .startLine = tokenStartLine_,
        .endLine = tokenEndLine_,
        .startColumn = tokenStartColumn_,
        .endColumn = tokenEndColumn_,
    });
  }

  /**
   * Throws default "Unexpected token" exception, showing the actual
   * line from the source, pointing with the ^ marker to the bad token.
   * In addition, shows `line:column` location.
   */
  [[noreturn]] void throwUnexpectedToken(const std::string& symbol, int line,
                                         int column) {
    std::stringstream ss{str_};
    std::string lineStr;
    int currentLine = 1;

    while (currentLine++ <= line) {
      std::getline(ss, lineStr, '\n');
    }

    auto pad = std::string(column, ' ');

    std::stringstream errMsg;

    errMsg << "Syntax Error:\n\n"
           << lineStr << "\n"
           << pad << "^\nUnexpected token \"" << symbol << "\" at " << line
           << ":" << column << "\n\n";

    std::cerr << errMsg.str();
    throw new std::runtime_error(errMsg.str().c_str());
  }

  /**
   * Matched text.
   */
  std::string yytext;

 private:
  /**
   * Captures token locations.
   */
  void captureLocations_(const std::string& matched) {
    auto len = matched.length();

    // Absolute offsets.
    tokenStartOffset_ = cursor_;

    // Line-based locations, start.
    tokenStartLine_ = currentLine_;
    tokenStartColumn_ = tokenStartOffset_ - currentLineBeginOffset_;

    // Extract `\n` in the matched token.
    std::stringstream ss{matched};
    std::string lineStr;
    std::getline(ss, lineStr, '\n');
    while (ss.tellg() > 0 && ss.tellg() <= len) {
      currentLine_++;
      currentLineBeginOffset_ = tokenStartOffset_ + ss.tellg();
      std::getline(ss, lineStr, '\n');
    }

    tokenEndOffset_ = cursor_ + len;

    // Line-based locations, end.
    tokenEndLine_ = currentLine_;
    tokenEndColumn_ = tokenEndOffset_ - currentLineBeginOffset_;
    currentColumn_ = tokenEndColumn_;
  }

  /**
   * Lexical rules.
   */
  // clang-format off
  static constexpr size_t LEX_RULES_COUNT = 26;
  static std::array<LexRule, LEX_RULES_COUNT> lexRules_;
  static std::map<TokenizerState, std::vector<size_t>> lexRulesByStartConditions_;
  // clang-format on

  /**
   * Special EOF token.
   */
  static std::string __EOF;

  /**
   * Tokenizing string.
   */
  std::string str_;

  /**
   * Cursor for current symbol.
   */
  int cursor_;

  /**
   * States.
   */
  std::vector<TokenizerState> states_;

  /**
   * Line-based location tracking.
   */
  int currentLine_;
  int currentColumn_;
  int currentLineBeginOffset_;

  /**
   * Location data of a matched token.
   */
  int tokenStartOffset_;
  int tokenEndOffset_;
  int tokenStartLine_;
  int tokenEndLine_;
  int tokenStartColumn_;
  int tokenEndColumn_;
};

// ------------------------------------------------------------------
// Lexical rule handlers.

std::string Tokenizer::__EOF("$");

// clang-format off
inline TokenType _lexRule1(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::TOKEN_TYPE_30;
}

inline TokenType _lexRule2(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::TOKEN_TYPE_31;
}

inline TokenType _lexRule3(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::TOKEN_TYPE_32;
}

inline TokenType _lexRule4(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::__EMPTY;
}

inline TokenType _lexRule5(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::__EMPTY;
}

inline TokenType _lexRule6(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::__EMPTY;
}

inline TokenType _lexRule7(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::UNIT;
}

inline TokenType _lexRule8(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::USES;
}

inline TokenType _lexRule9(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::CONST;
}

inline TokenType _lexRule10(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::VAR;
}

inline TokenType _lexRule11(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::BEGIN;
}

inline TokenType _lexRule12(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::IF;
}

inline TokenType _lexRule13(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::THEN;
}

inline TokenType _lexRule14(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::ELSE;
}

inline TokenType _lexRule15(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::CASE;
}

inline TokenType _lexRule16(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::IS;
}

inline TokenType _lexRule17(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::END;
}

inline TokenType _lexRule18(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::CONSTRUCTOR;
}

inline TokenType _lexRule19(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::FUNCTION;
}

inline TokenType _lexRule20(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::BOOL;
}

inline TokenType _lexRule21(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::INT;
}

inline TokenType _lexRule22(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::FLOAT;
}

inline TokenType _lexRule23(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::STRING;
}

inline TokenType _lexRule24(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::IDENTIFIER;
}

inline TokenType _lexRule25(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::SEMICOLON;
}

inline TokenType _lexRule26(const Tokenizer& tokenizer, const std::string& yytext) {
return TokenType::SEPARATOR;
}
// clang-format on

// ------------------------------------------------------------------
// Lexical rules.

// clang-format off
std::array<LexRule, Tokenizer::LEX_RULES_COUNT> Tokenizer::lexRules_ = {{
  {std::regex(R"(^\.)"), &_lexRule1},
  {std::regex(R"(^as)"), &_lexRule2},
  {std::regex(R"(^,)"), &_lexRule3},
  {std::regex(R"(^\s+)"), &_lexRule4},
  {std::regex(R"(^\{[^\}]*\})"), &_lexRule5},
  {std::regex(R"(^\/\/.*\n)"), &_lexRule6},
  {std::regex(R"(^unit)"), &_lexRule7},
  {std::regex(R"(^uses)"), &_lexRule8},
  {std::regex(R"(^const)"), &_lexRule9},
  {std::regex(R"(^var)"), &_lexRule10},
  {std::regex(R"(^begin)"), &_lexRule11},
  {std::regex(R"(^if)"), &_lexRule12},
  {std::regex(R"(^then)"), &_lexRule13},
  {std::regex(R"(^else)"), &_lexRule14},
  {std::regex(R"(^case)"), &_lexRule15},
  {std::regex(R"(^is)"), &_lexRule16},
  {std::regex(R"(^end)"), &_lexRule17},
  {std::regex(R"(^constructor)"), &_lexRule18},
  {std::regex(R"(^function)"), &_lexRule19},
  {std::regex(R"(^true|false)"), &_lexRule20},
  {std::regex(R"(^[0-9])"), &_lexRule21},
  {std::regex(R"(^[0-9]+\.[0-9]+)"), &_lexRule22},
  {std::regex(R"(^['"](.*?[^\\])?(\\\\)*['"])"), &_lexRule23},
  {std::regex(R"(^[a-zA-Z_][a-zA-Z0-9_]*)"), &_lexRule24},
  {std::regex(R"(^;)"), &_lexRule25},
  {std::regex(R"(^\\)"), &_lexRule26}
}};
std::map<TokenizerState, std::vector<size_t>> Tokenizer::lexRulesByStartConditions_ =  {{TokenizerState::INITIAL, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}}};
// clang-format on

#endif
// clang-format on

#define POP_V()              \
  parser.valuesStack.back(); \
  parser.valuesStack.pop_back()

#define POP_T()              \
  parser.tokensStack.back(); \
  parser.tokensStack.pop_back()

#define PUSH_VR() parser.valuesStack.push_back(__)
#define PUSH_TR() parser.tokensStack.push_back(__)

/**
 * Parsing table type.
 */
enum class TE {
  Accept,
  Shift,
  Reduce,
  Transit,
};

/**
 * Parsing table entry.
 */
struct TableEntry {
  TE type;
  int value;
};

// clang-format off
class parser;
// clang-format on

using yyparse = parser;

typedef void (*ProductionHandler)(yyparse&);

/**
 * Encoded production.
 *
 * opcode - encoded index
 * rhsLength - length of the RHS to pop.
 */
struct Production {
  int opcode;
  int rhsLength;
  ProductionHandler handler;
};

// Key: Encoded symbol (terminal or non-terminal) index
// Value: TableEntry
using Row = std::map<int, TableEntry>;

/**
 * Parser class.
 */
// clang-format off
class parser {
  // clang-format on
 public:
  /**
   * Parsing values stack.
   */
  std::vector<Value> valuesStack;

  /**
   * Token values stack.
   */
  std::vector<std::string> tokensStack;

  /**
   * Parsing states stack.
   */
  std::vector<int> statesStack;

  /**
   * Tokenizer.
   */
  Tokenizer tokenizer;

  /**
   * Previous state to calculate the next one.
   */
  int previousState;

  /**
   * Parses a string.
   */
  Value parse(const std::string& str) {
    // clang-format off
    
    // clang-format on

    // Initialize the tokenizer and the string.
    tokenizer.initString(str);

    // Initialize the stacks.
    valuesStack.clear();
    tokensStack.clear();
    statesStack.clear();

    // Initial 0 state.
    statesStack.push_back(0);

    auto token = tokenizer.getNextToken();
    auto shiftedToken = token;

    // Main parsing loop.
    for (;;) {
      auto state = statesStack.back();
      auto column = (int)token->type;

      if (table_[state].count(column) == 0) {
        throwUnexpectedToken(token);
      }

      auto entry = table_[state].at(column);

      // Shift a token, go to state.
      if (entry.type == TE::Shift) {
        // Push token.
        tokensStack.push_back(token->value);

        // Push next state number: "s5" -> 5
        statesStack.push_back(entry.value);

        shiftedToken = token;
        token = tokenizer.getNextToken();
      }

      // Reduce by production.
      else if (entry.type == TE::Reduce) {
        auto productionNumber = entry.value;
        auto production = productions_[productionNumber];

        tokenizer.yytext = shiftedToken->value;

        auto rhsLength = production.rhsLength;
        while (rhsLength > 0) {
          statesStack.pop_back();
          rhsLength--;
        }

        // Call the handler.
        production.handler(*this);

        auto previousState = statesStack.back();

        auto symbolToReduceWith = production.opcode;
        auto nextStateEntry = table_[previousState].at(symbolToReduceWith);
        assert(nextStateEntry.type == TE::Transit);

        statesStack.push_back(nextStateEntry.value);
      }

      // Accept the string.
      else if (entry.type == TE::Accept) {
        // Pop state number.
        statesStack.pop_back();

        // Pop the parsed value.
        // clang-format off
        auto result = valuesStack.back(); valuesStack.pop_back();
        // clang-format on

        if (statesStack.size() != 1 || statesStack.back() != 0 ||
            tokenizer.hasMoreTokens()) {
          throwUnexpectedToken(token);
        }

        statesStack.pop_back();

        // clang-format off
        
        // clang-format on

        return result;
      }
    }
  }

 private:
  /**
   * Throws parser error on unexpected token.
   */
  [[noreturn]] void throwUnexpectedToken(SharedToken token) {
    if (token->type == TokenType::__EOF && !tokenizer.hasMoreTokens()) {
      std::string errMsg = "Unexpected end of input.\n";
      std::cerr << errMsg;
      throw std::runtime_error(errMsg.c_str());
    }
    tokenizer.throwUnexpectedToken(token->value, token->startLine,
                                   token->startColumn);
  }

  // clang-format off
  static constexpr size_t PRODUCTIONS_COUNT = 18;
  static std::array<Production, PRODUCTIONS_COUNT> productions_;

  static constexpr size_t ROWS_COUNT = 24;
  static std::array<Row, ROWS_COUNT> table_;
  // clang-format on
};

// ------------------------------------------------------------------
// Productions.

// clang-format off
void _handler1(yyparse& parser) {
// Semantic action prologue.
auto _1 = POP_V();

auto __ = _1;

 // Semantic action epilogue.
PUSH_VR();

}

void _handler2(yyparse& parser) {
// Semantic action prologue.
parser.tokensStack.pop_back();
auto _1 = POP_V();

auto __ = std::make_shared<ListExpr>(_1);

 // Semantic action epilogue.
PUSH_VR();

}

void _handler3(yyparse& parser) {
// Semantic action prologue.
parser.tokensStack.pop_back();
auto _2 = POP_V();
auto _1 = POP_V();

as(ListExpr, _1)->list.push_back(_2);
		auto __ = _1;

 // Semantic action epilogue.
PUSH_VR();

}

void _handler4(yyparse& parser) {
// Semantic action prologue.
auto _1 = POP_V();

auto __ = _1;

 // Semantic action epilogue.
PUSH_VR();

}

void _handler5(yyparse& parser) {
// Semantic action prologue.
auto _1 = POP_V();

auto __ = _1;

 // Semantic action epilogue.
PUSH_VR();

}

void _handler6(yyparse& parser) {
// Semantic action prologue.
auto _1 = POP_T();

auto __ = std::make_shared<IdentifierExpr>(_1);

 // Semantic action epilogue.
PUSH_VR();

}

void _handler7(yyparse& parser) {
// Semantic action prologue.
auto _1 = POP_V();

auto __ = std::make_shared<ListExpr>(_1); // TODO: customize
	;

 // Semantic action epilogue.
PUSH_VR();

}

void _handler8(yyparse& parser) {
// Semantic action prologue.
auto _3 = POP_V();
parser.tokensStack.pop_back();
auto _1 = POP_V();

as(ListExpr, _1)->list.push_back(_3);
		auto __ = _1;

 // Semantic action epilogue.
PUSH_VR();

}

void _handler9(yyparse& parser) {
// Semantic action prologue.
auto _1 = POP_V();

auto __ = std::make_shared<IdentifierPathAsExpr>(as(ListExpr, _1));

 // Semantic action epilogue.
PUSH_VR();

}

void _handler10(yyparse& parser) {
// Semantic action prologue.
auto _3 = POP_V();
parser.tokensStack.pop_back();
auto _1 = POP_V();

auto __ = std::make_shared<IdentifierPathAsExpr>(as(ListExpr, _1), as(IdentifierExpr, _3));

 // Semantic action epilogue.
PUSH_VR();

}

void _handler11(yyparse& parser) {
// Semantic action prologue.
auto _3 = POP_V();
parser.tokensStack.pop_back();
auto _1 = POP_V();

as(IdentifierPathAsExpr, _1)->setAlias(as(IdentifierExpr, _3));
		auto __ = _1;

 // Semantic action epilogue.
PUSH_VR();

}

void _handler12(yyparse& parser) {
// Semantic action prologue.
auto _2 = POP_V();
parser.tokensStack.pop_back();

auto __ = std::make_shared<UnitExpr>(as(ListExpr, _2));

 // Semantic action epilogue.
PUSH_VR();

}

void _handler13(yyparse& parser) {
// Semantic action prologue.
auto _1 = POP_V();

auto __ = std::make_shared<ListExpr>(_1);

 // Semantic action epilogue.
PUSH_VR();

}

void _handler14(yyparse& parser) {
// Semantic action prologue.
auto _3 = POP_V();
parser.tokensStack.pop_back();
auto _1 = POP_V();

as(ListExpr, _1)->list.push_back(_3);
		auto __ = _1;

 // Semantic action epilogue.
PUSH_VR();

}

void _handler15(yyparse& parser) {
// Semantic action prologue.
auto _2 = POP_V();
parser.tokensStack.pop_back();

auto __ = std::make_shared<UsesExpr>(as(ListExpr, _2));

 // Semantic action epilogue.
PUSH_VR();

}

void _handler16(yyparse& parser) {
// Semantic action prologue.
auto _1 = POP_V();

auto __ = std::make_shared<ListExpr>(_1);

 // Semantic action epilogue.
PUSH_VR();

}

void _handler17(yyparse& parser) {
// Semantic action prologue.
auto _3 = POP_V();
parser.tokensStack.pop_back();
auto _1 = POP_V();

as(ListExpr, _1)->list.push_back(_3);
		auto __ = _1;

 // Semantic action epilogue.
PUSH_VR();

}

void _handler18(yyparse& parser) {
// Semantic action prologue.
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();
parser.tokensStack.pop_back();

auto __ = std::make_shared<Expr>();

 // Semantic action epilogue.
PUSH_VR();

}
// clang-format on

// clang-format off
std::array<Production, yyparse::PRODUCTIONS_COUNT> yyparse::productions_ = {{{-1, 1, &_handler1},
{0, 2, &_handler2},
{0, 3, &_handler3},
{1, 1, &_handler4},
{1, 1, &_handler5},
{2, 1, &_handler6},
{3, 1, &_handler7},
{3, 3, &_handler8},
{4, 1, &_handler9},
{4, 3, &_handler10},
{4, 3, &_handler11},
{5, 2, &_handler12},
{6, 1, &_handler13},
{6, 3, &_handler14},
{7, 2, &_handler15},
{8, 1, &_handler16},
{8, 3, &_handler17},
{9, 19, &_handler18}}};
// clang-format on

// ------------------------------------------------------------------
// Parsing table.

// clang-format off
std::array<Row, yyparse::ROWS_COUNT> yyparse::table_ = {
    Row {{0, {TE::Transit, 1}}, {1, {TE::Transit, 2}}, {5, {TE::Transit, 3}}, {7, {TE::Transit, 4}}, {12, {TE::Shift, 5}}, {14, {TE::Shift, 6}}},
    Row {{1, {TE::Transit, 7}}, {5, {TE::Transit, 3}}, {7, {TE::Transit, 4}}, {12, {TE::Shift, 5}}, {14, {TE::Shift, 6}}, {33, {TE::Accept, 0}}},
    Row {{10, {TE::Shift, 23}}},
    Row {{10, {TE::Reduce, 3}}},
    Row {{10, {TE::Reduce, 4}}},
    Row {{2, {TE::Transit, 10}}, {6, {TE::Transit, 9}}, {11, {TE::Shift, 11}}},
    Row {{2, {TE::Transit, 10}}, {4, {TE::Transit, 15}}, {6, {TE::Transit, 16}}, {8, {TE::Transit, 14}}, {11, {TE::Shift, 11}}},
    Row {{10, {TE::Shift, 8}}},
    Row {{12, {TE::Reduce, 2}}, {14, {TE::Reduce, 2}}, {33, {TE::Reduce, 2}}},
    Row {{10, {TE::Reduce, 11}}, {13, {TE::Shift, 12}}},
    Row {{10, {TE::Reduce, 12}}, {13, {TE::Reduce, 12}}, {30, {TE::Reduce, 12}}, {31, {TE::Reduce, 12}}, {32, {TE::Reduce, 12}}},
    Row {{10, {TE::Reduce, 5}}, {13, {TE::Reduce, 5}}, {30, {TE::Reduce, 5}}, {31, {TE::Reduce, 5}}, {32, {TE::Reduce, 5}}},
    Row {{2, {TE::Transit, 13}}, {11, {TE::Shift, 11}}},
    Row {{10, {TE::Reduce, 13}}, {13, {TE::Reduce, 13}}, {30, {TE::Reduce, 13}}, {31, {TE::Reduce, 13}}, {32, {TE::Reduce, 13}}},
    Row {{10, {TE::Reduce, 14}}, {32, {TE::Shift, 17}}},
    Row {{10, {TE::Reduce, 15}}, {31, {TE::Shift, 19}}, {32, {TE::Reduce, 15}}},
    Row {{10, {TE::Reduce, 8}}, {13, {TE::Shift, 12}}, {30, {TE::Shift, 21}}, {31, {TE::Reduce, 8}}, {32, {TE::Reduce, 8}}},
    Row {{2, {TE::Transit, 10}}, {4, {TE::Transit, 18}}, {6, {TE::Transit, 16}}, {11, {TE::Shift, 11}}},
    Row {{10, {TE::Reduce, 16}}, {31, {TE::Shift, 19}}, {32, {TE::Reduce, 16}}},
    Row {{2, {TE::Transit, 20}}, {11, {TE::Shift, 11}}},
    Row {{10, {TE::Reduce, 10}}, {31, {TE::Reduce, 10}}, {32, {TE::Reduce, 10}}},
    Row {{2, {TE::Transit, 22}}, {11, {TE::Shift, 11}}},
    Row {{10, {TE::Reduce, 9}}, {31, {TE::Reduce, 9}}, {32, {TE::Reduce, 9}}},
    Row {{12, {TE::Reduce, 1}}, {14, {TE::Reduce, 1}}, {33, {TE::Reduce, 1}}}
};
// clang-format on

}  // namespace syntax

#endif