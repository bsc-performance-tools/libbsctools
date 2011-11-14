#ifndef _PARAVER_TRACE_CONFIG_H
#define _PARAVER_TRACE_CONFIG_H

#include <map>
#include <string>
#include <iostream>
#include <iomanip>

#include <boost/shared_ptr.hpp>
#include <boost/throw_exception.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

#include "debug.h"

#include "UIParaverTraceConfig.h"
#include "ParaverTraceConfigGrammar.h"

namespace libparaver {

class ParaverTraceConfig {
public:


    class GradientColor : public UIParaverTraceConfig::GradientColor {
    public:
        GradientColor(int red_, int green_, int blue_);

        virtual std::string toString() const;
    };


    class StateColor : public UIParaverTraceConfig::StateColor {
    public:
        StateColor(int red_, int green_, int blue_);

        virtual std::string toString() const;
    };


    class EventValues {
    public:
        typedef std::map<int, std::string> eventValues_type;

        void addValue(int key, std::string descr);

        inline const std::string getDescription(int key) const;

        inline const int getKey(std::string descr) const;

        int getValue(const std::string eventValue) const;

        std::string getValue(const int eventKey) const;

        std::vector<unsigned int> getValues() const;

        virtual std::string toString() const;

    protected:
        eventValues_type eventValues;
    };

    class EventType : public UIParaverTraceConfig::EventType {
    public:
        typedef boost::shared_ptr<EventValues> EventValuesPtr;
        
        EventType(int color_, int key_, std::string descr_);

        const EventValuesPtr getEventValues() const;

        void setEventValues(EventValues * eventValues_);

        virtual std::string toString() const;
    protected:
        EventValuesPtr eventValues;

    };

    typedef std::string::const_iterator string_iterator_type;

    typedef std::map<int,std::string> states_type;

    typedef std::map<int, StateColor*> states_color_type;

    typedef std::map<int,std::string> gradient_names_type;

    typedef std::map<int, GradientColor*> gradient_color_type;

    typedef std::map<int, EventType*> event_types_type;

private:
    bool debug;

    std::string level;

    std::string units;

    int look_back;

    int speed;

    std::string flag_icons;

    int num_of_state_colors;

    int ymax_scale;

    std::string thread_func;

    states_type states;

    states_color_type states_color;

    gradient_names_type gradient_names;

    gradient_color_type gradient_colors;

    event_types_type event_types;

public:
    ParaverTraceConfig();

    virtual ~ParaverTraceConfig();

    void setDebug(bool value);

    inline const bool getDebug() const;

    void setLevel(std::string value);

    inline const std::string getLevel() const;

    void setUnits(std::string value);

    inline const std::string getUnits() const;

    void setLook_back(int value);

    inline const int getLook_back() const;

    void setSpeed(int value);

    inline const int getSpeed() const;

    void setFlag_icons(std::string value);

    inline const std::string getFlag_icons() const;

    void setNum_of_state_colors(int value);

    inline const int getNum_of_state_colors() const;

    void setYmax_scale(int value);

    inline const int getYmax_scale() const;

    void setThread_func(std::string value);

    inline const std::string getThread_func() const;

    void addState(int key, std::string value);

    std::vector<int> getStates() const;

    std::string getState(int key) const;

    void addStateColor(int key, int r, int g, int b);

    std::vector<int> getStateColors() const;

    UIParaverTraceConfig::StateColor getStateColor(int key) const;

    void addGradientName(int key, std::string value);

    std::vector<int> getGradientNames() const;

    std::string getGradientName(int key) const;

    void addGradientColor(int key, int r, int g, int b);

    std::vector<int> getGradientColors() const;

    UIParaverTraceConfig::GradientColor getGradientColor(int key) const;

    void addEventTypes(std::vector<EventType *> & eventTypes_);

    bool parse(string_iterator_type begin, string_iterator_type end);

    bool parse(std::istream & input, const std::string & filename);

    std::string toString() const;

    int getEventType(const std::string eventTypeValue) const;

    std::string getEventType(const int eventTypeKey) const;

    std::vector<unsigned int> getValues() const;

    int getEventValue(const int eventTypeKey, const std::string eventValue) const;

    std::string getEventValue(const int eventTypeKey, const int eventValueKey) const;

    std::vector<unsigned int > getEventTypes() const;

    std::vector<unsigned int > getEventValues(unsigned int eventTypeKey) const;
};

#include "ParaverTraceConfig.ipp"

}

#endif

