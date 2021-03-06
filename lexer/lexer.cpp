#include "lexer.h"

#include <algorithm>
//#include <charconv>
#include <unordered_map>

using namespace std;

namespace Parse {

bool operator == (const Token& lhs, const Token& rhs) {
  using namespace TokenType;

  if (lhs.index() != rhs.index()) {
    return false;
  }
  if (lhs.Is<Char>()) {
    return lhs.As<Char>().value == rhs.As<Char>().value;
  } else if (lhs.Is<Number>()) {
    return lhs.As<Number>().value == rhs.As<Number>().value;
  } else if (lhs.Is<String>()) {
    return lhs.As<String>().value == rhs.As<String>().value;
  } else if (lhs.Is<Id>()) {
    return lhs.As<Id>().value == rhs.As<Id>().value;
  } else {
    return true;
  }
}

std::ostream& operator << (std::ostream& os, const Token& rhs) {
  using namespace TokenType;

#define VALUED_OUTPUT(type) \
  if (auto p = rhs.TryAs<type>()) return os << #type << '{' << p->value << '}';

  VALUED_OUTPUT(Number);
  VALUED_OUTPUT(Id);
  VALUED_OUTPUT(String);
  VALUED_OUTPUT(Char);

#undef VALUED_OUTPUT

#define UNVALUED_OUTPUT(type) \
    if (rhs.Is<type>()) return os << #type;

  UNVALUED_OUTPUT(Class);
  UNVALUED_OUTPUT(Return);
  UNVALUED_OUTPUT(If);
  UNVALUED_OUTPUT(Else);
  UNVALUED_OUTPUT(Def);
  UNVALUED_OUTPUT(Newline);
  UNVALUED_OUTPUT(Print);
  UNVALUED_OUTPUT(Indent);
  UNVALUED_OUTPUT(Dedent);
  UNVALUED_OUTPUT(And);
  UNVALUED_OUTPUT(Or);
  UNVALUED_OUTPUT(Not);
  UNVALUED_OUTPUT(Eq);
  UNVALUED_OUTPUT(NotEq);
  UNVALUED_OUTPUT(LessOrEq);
  UNVALUED_OUTPUT(GreaterOrEq);
  UNVALUED_OUTPUT(None);
  UNVALUED_OUTPUT(True);
  UNVALUED_OUTPUT(False);
  UNVALUED_OUTPUT(Eof);

#undef UNVALUED_OUTPUT

  return os << "Unknown token :(";
}


Lexer::Lexer(std::istream& input)
  : input_(input)
  , last_token_(NextToken())
{}

const Token& Lexer::CurrentToken() const {
  return last_token_;
}

Token Lexer::NextToken() {
  if (input_.eof()) {
    return Token(TokenType::Eof{});
  }
  return ReadToken();
}

Token Lexer::ReadToken() {
  if (isdigit(input_.peek()) || input_.peek() == '-') {
    return ReadNumber(input_.peek() == '-');
  }
}

Token Lexer::ReadNumber(bool negative) {
  if (negative) {
    input_.get();
  }
  int result = 0;

  while (isdigit(input_.peek())) {
    result *= 10;
    result += input_.get() - '0';
  }

  return negative ? Token(TokenType::Number{result * -1})
    : Token(TokenType::Number{result});
}

Token Lexer::ReadString() {
  
}

}