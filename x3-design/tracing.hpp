#ifndef X3_DESIGN_TRACING_HPP_2016_08_28_LJE
#define X3_DESIGN_TRACING_HPP_2016_08_28_LJE
#include <iostream>
#define X3_DESIGN_USE_IOSTREAMS_UTILITY 1
#if X3_DESIGN_USE_IOSTREAMS_UTILITY
  #include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
  //The code for above #include *should* be available here:
  /*
  https://github.com/cppljevans/variadic_templates/tree/master/boost/iostreams/utility
   */
#else
  //Otherwise, the following do-nothing code can be used instead; however,
  //no indentation of output will happen ;(
  namespace boost{namespace iostreams{
  template<typename Ch=char, typename Tr=std::char_traits<Ch> >
  class indent_scoped_ostreambuf 
  {
  public:
      typedef std::basic_ostream<Ch,Tr> std_ostrm_type;
      indent_scoped_ostreambuf
      ( std_ostrm_type& a_ostrm
      , int width=2
      )
      {}
  };
  }}//exit boost::iostreams namespace
  template<class Ch, class Tr>
  inline std::basic_ostream<Ch, Tr>&
  indent_buf_in(std::basic_ostream<Ch, Tr>& os)
  {
      return os;
  }
  template<class Ch, class Tr>
  inline std::basic_ostream<Ch, Tr>&
  indent_buf_out(std::basic_ostream<Ch, Tr>& os)
  {
      return os;
  }
#endif //X3_DESIGN_USE_IOSTREAMS_UTILITY
#include <string>
namespace boost
{
  template
  < typename Enter=std::string
  , typename Exit=Enter
  >
  class
indent_scope
  //Purpose:
  //  Indicate on a std::ostream when a scope entered and exited.
  {
  public:
    indent_scope
      ( Enter a_enter=""
      , Exit a_exit=""
      , std::ostream& a_strm=std::cout
      )
      : my_exit(a_exit)
      , my_strm(a_strm)
      {
      ; my_strm<<a_enter
      ; my_strm<<indent_buf_in
      ; my_strm.flush()
      ;}
    ~indent_scope(void)
      {
      ; my_strm<<indent_buf_out
      ; my_strm<<my_exit
      ; my_strm.flush()
      ;}
  private:
      Exit
    my_exit
      ;
      std::ostream&
    my_strm
      ;
  };//end indent_scope class
  class
trace_scope
  //Purpose:
  //  Indicate on a std::ostream when a scope entered and exited.
  : indent_scope<>
  {
  public:
      template
      < typename ScopeName
      >
    trace_scope
      ( ScopeName a_scopeName
      , std::ostream& a_strm=std::cout
      )
      : indent_scope<>
        ( std::string("===>Enter:{{")+a_scopeName+"\n"
        , std::string("===>Exit_:}}")+a_scopeName+"\n"
        , a_strm
        )
      {
      }
  };//end trace_scope class
}//exit boost namespace

#define X3_DESIGN_USE_TRACE 1
#if X3_DESIGN_USE_TRACE
#include <boost/core/demangle.hpp>
#include <typeinfo>
    template
    < typename Type
    >
    std::string
  type_name_str()
    {
      auto type_name=typeid(Type).name();
    #if 0
      return boost::core::demangle(type_name);
    #else
      return type_name;
    #endif
    }  
    template
    < typename Type
    >
    std::string
  type_name_str(Type)
    {
      return type_name_str<Type>();
    }  
    template
    < typename Type
    >
    std::size_t
  type_name_size()
    {
      return type_name_str<Type>().size();
    }
    template
    < typename Iterator
    >
    void
  print_range
    ( Iterator& first
    , Iterator const& last
    )
    {
      std::cout<<"first...last=\n";
      for(Iterator f=first; f!=last; ++f) std::cout<<*f;
      std::cout<<"\n";
    }
    
  #define TRACE_PARSE_INP(tag) trace_scope ts_parse(tag); \
    std::cout<<":context_name_size="<<type_name_size<Context>()<<"\n";\
    print_range(first,last);
  #define TRACE_PARSE_OUT std::cout<<":ok_parse="<<ok_parse<<"\n";\
    print_range(first,last);
#else
  #define TRACE_PARSE_INP(tag)
  #define TRACE_PARSE_OUT
#endif //X3_DESIGN_USE_TRACE  
#endif //X3_DESIGN_TRACING_HPP_2016_08_28_LJE
