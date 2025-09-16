#pragma once

// std libs
#include <queue>
#include <vector>
#include <unordered_map>
#include <memory>
#include <variant>

// marathon
#include "core/module.hpp"

namespace marathon {

namespace events {

typedef std::variant<int, float, double, bool, std::string> EventProperty;

// A signal consists of const data so it cannot be modified.
// Signals should be handled as is and discarded after use.
class Signal {
public:
    Signal(const std::string& name, const std::unordered_map<std::string, EventProperty>& data);
    ~Signal() = default;

    const std::string name;
    const std::unordered_map<std::string, EventProperty> data;
};

class Events : public marathon::Module {
protected:
    Events(const std::string& name);

    // internal event queue
    std::queue<std::shared_ptr<Signal>> _queue;

public:
    // virtual destructor to prevent event from being instantiated
    virtual ~Events() = default;
    
    // singleton instance accessor
    static Events& Instance();

    // module interface
    virtual bool Boot() = 0;
    virtual bool Shutdown() = 0;

    // common
    virtual void Fetch() = 0;   // collect all events from backend into our system
    virtual void Clear();   // dump all events from queue, backend should use its own clear method and this
    bool Poll(std::shared_ptr<Signal>& event);   // pass as reference to accept signal assignment
    void Push(std::shared_ptr<Signal> event);   // pass by value to copy event to event queue
    
    // add wait system for reduced CPU usage
};

} // events
    
} //  marathon
