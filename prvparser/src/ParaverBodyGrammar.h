#ifndef DOMAIN_BODYGRAMMAR_H
#define DOMAIN_BODYGRAMMAR_H

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/spirit/include/classic_increment_actor.hpp>
#include <boost/bind.hpp>

#include <boost/spirit/include/phoenix1_primitives.hpp>
#include <boost/spirit/include/phoenix1_operators.hpp>
#include <boost/spirit/include/classic_insert_at_actor.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/include/classic_push_back_actor.hpp>
#include <boost/spirit/include/classic_functor_parser.hpp>
using BOOST_SPIRIT_CLASSIC_NS::grammar;
using BOOST_SPIRIT_CLASSIC_NS::rule;
using BOOST_SPIRIT_CLASSIC_NS::space_p;
using BOOST_SPIRIT_CLASSIC_NS::end_p;
using BOOST_SPIRIT_CLASSIC_NS::anychar_p;
using BOOST_SPIRIT_CLASSIC_NS::eol_p;
using BOOST_SPIRIT_CLASSIC_NS::lexeme_d;
using BOOST_SPIRIT_CLASSIC_NS::alnum_p;
using BOOST_SPIRIT_CLASSIC_NS::alpha_p;
using BOOST_SPIRIT_CLASSIC_NS::int_p;
using BOOST_SPIRIT_CLASSIC_NS::uint_p;
using BOOST_SPIRIT_CLASSIC_NS::real_p;
using BOOST_SPIRIT_CLASSIC_NS::str_p;
using BOOST_SPIRIT_CLASSIC_NS::ch_p;
using BOOST_SPIRIT_CLASSIC_NS::uint_parser;
using BOOST_SPIRIT_CLASSIC_NS::functor_parser;
using BOOST_SPIRIT_CLASSIC_NS::assign_a;
using BOOST_SPIRIT_CLASSIC_NS::increment_a;
using BOOST_SPIRIT_CLASSIC_NS::push_back_a;
using BOOST_SPIRIT_CLASSIC_NS::parse_info;
using BOOST_SPIRIT_CLASSIC_NS::file_iterator;
using BOOST_SPIRIT_CLASSIC_NS::scanner;
using boost::ref;
using boost::bind; 
using phoenix::var;


using namespace boost;
using namespace boost::spirit;
using namespace phoenix;

namespace libparaver {

#include "ParaverRecord.h"

template<typename Actions>
class ParaverBodyGrammar : public ::grammar<ParaverBodyGrammar<Actions> > {
	protected:
	Actions & actions;

	mutable struct object_id_t OID;
	mutable struct state_t State;
	mutable struct multievent_t MultiEvent;
	mutable struct comm_t Comm;
	mutable struct event_t Event;
	mutable string Comment;
	mutable string Communicator;

	public:
	ParaverBodyGrammar(Actions & actions);

	template<typename ScannerT>
	class definition {
		protected:
		rule<ScannerT> prvbody_rule;
		rule<ScannerT> skip_rule;
		rule<ScannerT> state_rule;
		rule<ScannerT> event_rule;
		rule<ScannerT> communication_rule;
		rule<ScannerT> communicator_rule;
		
		uint_parser<unsigned long long, 10, 1, -1> big_uint;

		public:
		definition(const ParaverBodyGrammar<Actions> & self);
		rule<ScannerT> const & start() const;
	};
    
};
template<typename Actions>
ParaverBodyGrammar<Actions>::ParaverBodyGrammar(Actions & actions):actions(actions) {
}

template<typename Actions>
template<typename ScannerT>
ParaverBodyGrammar<Actions>::definition<ScannerT>::definition(const ParaverBodyGrammar<Actions> & self) {
	Actions & actions = self.actions;

	prvbody_rule = (skip_rule | state_rule | event_rule | communication_rule | communicator_rule ) >> end_p;

	skip_rule = ("#" >> (*anychar_p)[assign_a(self.Comment)])
			[boost::bind(&Actions::processComment, boost::ref(actions), boost::ref(self.Comment))];
	
	communicator_rule = ("c" >> (*anychar_p)[assign_a(self.Communicator)])
			[boost::bind(&Actions::processCommunicator, boost::ref(actions), boost::ref(self.Communicator))];

	state_rule = 
		("1:" >>
		uint_p[assign_a(self.State.ObjectID.cpu)] >> ':' >>
		uint_p[assign_a(self.State.ObjectID.ptask)] >> ':' >>
		uint_p[assign_a(self.State.ObjectID.task)] >> ':' >>
		uint_p[assign_a(self.State.ObjectID.thread)] >> ':' >>
		big_uint[assign_a(self.State.Begin_Timestamp)] >> ':' >>
		big_uint[assign_a(self.State.End_Timestamp)] >> ':' >>
		uint_p[assign_a(self.State.State)])
		[boost::bind(&Actions::processState, boost::ref(actions), boost::ref(self.State))];

	event_rule = 
		(str_p("2:")[boost::bind(&Actions::clearEvent, boost::ref(actions), boost::ref(self.MultiEvent))] >>
		uint_p[assign_a(self.MultiEvent.ObjectID.cpu)] >> ':' >>
		uint_p[assign_a(self.MultiEvent.ObjectID.ptask)] >> ':' >>
		uint_p[assign_a(self.MultiEvent.ObjectID.task)] >> ':' >>
		uint_p[assign_a(self.MultiEvent.ObjectID.thread)] >> ':' >>
		big_uint[assign_a(self.MultiEvent.Timestamp)] >>
		+(':' >>big_uint[assign_a(self.Event.Type)] >> ':' >> big_uint[assign_a(self.Event.Value)])
			[boost::bind(&Actions::newEvent, boost::ref(actions), boost::ref(self.MultiEvent), boost::ref(self.Event))])
		[boost::bind(&Actions::processEvents, boost::ref(actions), boost::ref(self.MultiEvent))];

	communication_rule = 
		("3:" >>
		uint_p[assign_a(self.Comm.Send_ObjectID.cpu)] >> ':' >>
		uint_p[assign_a(self.Comm.Send_ObjectID.ptask)] >> ':' >>
		uint_p[assign_a(self.Comm.Send_ObjectID.task)] >> ':' >>
		uint_p[assign_a(self.Comm.Send_ObjectID.thread)] >> ':' >>
		big_uint[assign_a(self.Comm.Logical_Send)] >> ':' >>
		big_uint[assign_a(self.Comm.Physical_Send)] >> ':' >>
		uint_p[assign_a(self.Comm.Recv_ObjectID.cpu)] >> ':' >>
		uint_p[assign_a(self.Comm.Recv_ObjectID.ptask)] >> ':' >>
		uint_p[assign_a(self.Comm.Recv_ObjectID.task)] >> ':' >>
		uint_p[assign_a(self.Comm.Recv_ObjectID.thread)] >> ':' >>
		big_uint[assign_a(self.Comm.Logical_Recv)] >> ':' >>
		big_uint[assign_a(self.Comm.Physical_Recv)] >> ':' >>
		uint_p[assign_a(self.Comm.Size)] >> ':' >>
		uint_p[assign_a(self.Comm.Tag)])
		[boost::bind(&Actions::processCommunication, boost::ref(actions), boost::ref(self.Comm))];

	BOOST_SPIRIT_DEBUG_NODE(prvbody_rule);
	BOOST_SPIRIT_DEBUG_NODE(skip_rule);
	BOOST_SPIRIT_DEBUG_NODE(state_rule);
	BOOST_SPIRIT_DEBUG_NODE(event_rule);
	BOOST_SPIRIT_DEBUG_NODE(communication_rule);
	BOOST_SPIRIT_DEBUG_NODE(communicator_rule);
}

template<typename Actions>
template<typename ScannerT>
rule<ScannerT> const & ParaverBodyGrammar<Actions>::definition<ScannerT>::start() const {
	return prvbody_rule;
}


} // namespace libparaver
#endif
