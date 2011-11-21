#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
/* boost::bind/val/ref stuff */
#include <boost/spirit/include/lex_lexertl.hpp>

/* phoenix::construct */
#include <boost/spirit/include/phoenix_object.hpp>


/* file iterators */
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>



namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;
namespace classic = boost::spirit::classic;
namespace ascii = boost::spirit::ascii;


#define SKIPPER qi::space | qi::eol
typedef BOOST_TYPEOF(SKIPPER) skipper_type;


template <typename Iterator>
class mini_grammar
        : public qi::grammar<Iterator, skipper_type> {
private:
    qi::rule<Iterator, skipper_type> start;
    qi::rule<Iterator, skipper_type> event_type;
    qi::rule<Iterator, skipper_type> event_value;
    qi::rule<Iterator, skipper_type> phrase;
    bool debug;

public:
    mini_grammar(bool debug);
};

template <typename Iterator>
mini_grammar<Iterator>::mini_grammar(bool debug):mini_grammar::base_type(start, "start") {

    phrase.name("phrase");
    /* a % b is equal to "a >> *(b >> a)" */
    phrase %= qi::lexeme[+(ascii::char_ - qi::eol)];

    start.name("start");
    start = +event_type;

    event_type.name("event_type");
    event_type =
        qi::lit("EVENT_TYPE") >
        qi::int_ > qi::int_ > phrase >
        *(event_value)
        ;

    event_value.name("event_value");
    event_value =
        qi::lit("VALUES") >
        qi::int_ > phrase
        ;

    if(debug) {
        std::cout<<"DEBUG:True\n";
        qi::debug(start);
        qi::debug(phrase);
        qi::debug(event_type);
        qi::debug(event_value);
    } else std::cout<<"DEBUG:False\n";

    qi::on_error<qi::fail>(start,
                           std::cout
                           << phoenix::val("Error! Expecting ")
                           << qi::_4
                           << phoenix::val(" here \"")
                           << phoenix::construct<std::string>(qi::_3,qi::_2)
                           << phoenix::val("\"")
                           << std::endl
                          );

}


int main(int argc, char **argv)
{
    bool debug=false;
    bool result=false;
    std::string inputFile="";
    for(int i=1; i<argc; ++i) {
        std::string param=argv[i];
        if(param[0]=='-') {
            if(param[1]=='v' && param.length()==2) debug=true;
        }
        else inputFile=param;
    }

    if(inputFile.length()>0) {
        std::cout<<"Opening file "<<inputFile<<std::endl;
        std::ifstream ifinput(inputFile.c_str());
        if(!ifinput.is_open()) {
            std::cout<<"Error opening file: "<<inputFile<<std::endl;
            exit(-1);
        }
        std::istream & input = ifinput;

        // iterate over stream input
        typedef std::istreambuf_iterator<char> base_iterator_type;
        base_iterator_type in_begin(input);

        // convert input iterator to forward iterator, usable by spirit parser
        typedef boost::spirit::multi_pass<base_iterator_type> forward_iterator_type;
        forward_iterator_type fwd_begin = boost::spirit::make_default_multi_pass(in_begin);
        forward_iterator_type fwd_end;

        // wrap forward iterator with position iterator, to record the position
        typedef classic::position_iterator2<forward_iterator_type> pos_iterator_type;
        pos_iterator_type position_begin(fwd_begin, fwd_end, inputFile);
        pos_iterator_type position_end;

        mini_grammar<pos_iterator_type> m_grammar(debug);

        //m_grammar.setDebug(debug);
        result=qi::phrase_parse(position_begin,position_end,m_grammar,SKIPPER);
    } else {

        typedef classic::position_iterator2<std::string::const_iterator> pos_iterator_type;

        std::string input = "EVENT_TYPE\n9   50000001    MPI Point-to-point\nVALUES\n";
        input += "4    MPI_Irecv\n3   MPI_Isend\n5   MPI_Wait\n6   MPI_Waitall\n0   End";
        pos_iterator_type position_begin(input.begin(), input.end(), "-");
        pos_iterator_type position_end;

        mini_grammar<pos_iterator_type> m_grammar(debug);

        //m_grammar.setDebug(debug);
        result=qi::phrase_parse(position_begin,position_end,m_grammar,SKIPPER);
    }

    if (result)
    {
        std::cout << "success.";
    }
    else
    {
        std::cout << "parse error";
    }
    std::cout << std::endl;
}

