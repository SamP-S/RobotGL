#pragma once

#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>

#include "core/object.hpp"
#include "core/logger.hpp"


namespace marathon {

/// CONSIDER: should whether a node was created at runtime be stored in the node?

// Node class as a base for all instances in a scene
class Node : public Object, public std::enable_shared_from_this<Node> {
private:
    /// NOTE: parent must be weak to avoid circular references
    std::weak_ptr<Node> _mParent = {};
    std::vector<std::shared_ptr<Node>> _mChildren = {};

public:
    Node(const std::string& type);
    ~Node();

    std::weak_ptr<Node> GetParent();
    virtual void SetParent(std::weak_ptr<Node> parent, bool keepWorldTransform=false);
    void AddChild(std::shared_ptr<Node> child);
    void RemoveChild(std::shared_ptr<Node> child);
    int GetChildCount() const;
    std::shared_ptr<Node> GetChild(int index);
    std::shared_ptr<Node> FindChild(const std::string& name);
    std::vector<std::shared_ptr<Node>>& GetChildren();
};

} // marathon