//OriginalSource:
//  [emp]
//    Copy&pasted from:
//      http://coliru.stacked-crooked.com/a/41425cb30e49bc8a
//    At 2016-08-26.1835.  That url was mentioned in post:
//      http://boost.2283326.n4.nabble.com/Spirit-X3-employee-cpp-compile-error-tp4687704p4687731.html
//  [spirit]
//    https://github.com/boostorg/spirit/tree/develop
//  [calc1]
//    [spirit]/example/x3/calc/calc1.cpp  
//Purpose:
//  Help understand *how* spirit uses the context to define the RHS of a rule
//  and understand limitations of this method for recursive rules.
//Result:
//  Will not work with mutually recursive rules because the rule_def's
//  need to be defined before used; however, with mutual recursion,
//  that's impossible.  See calc1 grammar with:
//    #define USE_MUTUAL_RECURSION 1
//    #define USE_SPIRIT_DEFINE 0
//  Instead, only using BOOST_SPIRIT_DEFINE with mutually recursive
//  rules will work.
//========================================================
#include "tracing.hpp"
#include <boost/spirit/home/x3.hpp>
#include <iostream>
#define USE_EMPLOYEE 0
#if USE_EMPLOYEE
//The following modified slightly from [emp]:
#include <boost/fusion/adapted/std_pair.hpp>
#include <map>
#include <string>

// Define CSV-formatted output for vectors.
template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    for (auto x : v) {
        os << x << ",";
    }

    return os;
}


namespace employee {

    namespace x3 = boost::spirit::x3;

    using x3::int_;
    using x3::ascii::char_;
    using x3::rule;

    auto const process = rule<class process, std::string>()
                       = '<' >> +(char_ - '>') >> '>';

    auto const node = rule<class node, std::pair<int, std::vector<std::string>> >()
                    = int_ >> ',' >> (+process % ',');


    typedef std::map<int, std::vector<std::string>> result_type;

    auto const node_map = rule<class node_map, result_type>()
                        = *node;
}
#endif //USE_EMPLOYEE
#define USE_CALC1 1
#if USE_CALC1
namespace calc1 {
    namespace x3 = boost::spirit::x3;
    using x3::int_;
    using x3::rule;
//The following is pretty much copied from [calc1]:
    class term_rul{};
    rule<term_rul> const term;
    class expr_rul{};
    rule<expr_rul> const expr;

  #define USE_SPIRIT_DEFINE 0
  #if USE_SPIRIT_DEFINE
    auto const term_def =
            int_
        |   '(' >> expr >> ')'
        ; 
    auto const expr_def =
        term
        >> *( '+' >> term
            )
        ;
    BOOST_SPIRIT_DEFINE(
          term
        , expr
        );
    auto const start=expr;
  #else
    #define USE_MUTUAL_RECURSION 1
    auto const term_def = ( term =
            int_
      #if USE_MUTUAL_RECURSION
        |   '(' >> expr_def >> ')'
      #endif
        )
        ; 
    auto const expr_def = ( expr =
        term_def
        >> *( '+' >> term_def
            )
        )
        ;
    auto const start=expr_def;
  #endif
}
#endif //USE_CALC1

int
main()
{
#if USE_EMPLOYEE
  {
    std::cout
      <<"employee parse\n"
      <<"==============\n"
      ;
    using boost::spirit::x3::ascii::space;
    std::string input(
        "0, <P00>, <P01>, <P02>"
        "1, <P10>"
        "2, <P21>,<P22>"
        "3, <P31>"
        "4, <P41>,<P42>,<P43>,<P44>"
    );
    auto iter = input.begin();
    employee::result_type res;
    bool ret = phrase_parse(iter, input.end(), employee::node_map, space, res);
    if (ret && iter == input.end())	{
      std::cout << "-------------------------\n";
      std::cout << "Parsing succeeded\n";
      for (auto const& x : res) {
        std::cout << "(" << x.first << ": " << x.second << ")" << std::endl;
      }
      std::cout << "\n-------------------------\n";
    } else {
      std::cout << "-------------------------\n";
      std::cout << "Parsing failed\n";
      std::cout << "-------------------------\n";
    }
  }        
#endif //USE_EMPLOYEE
#if USE_CALC1
  {
    std::cout
      <<"calc1 parse\n"
      <<"===========\n"
      ;
    using boost::spirit::x3::ascii::space;
    std::vector<std::string> inputs=
    { "1+2"
    , "1+(2+3)+4"
    };
    unsigned n=inputs.size();
    for(unsigned i=0; i<n; ++i) {
      auto input= inputs[i];
      auto iter=input.begin();
      std::cout<<"inputs["<<i<<"]:\n";
      bool ret = phrase_parse(iter, input.end(), calc1::start, space);
      if (ret && iter == input.end()) {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "\n-------------------------\n";
      } else {
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "-------------------------\n";
      }
    }
  }        
#endif
  std::cout<<"main:return 0\n";
  return 0;        
}
