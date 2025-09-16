#include "core/node.hpp"

namespace marathon {

Node::Node(const std::string& type)
    : Object(type) {}
Node::~Node() {}

/// NOTE: parent/child management should not use eachother as calls=
/// otherwise this will cause a circular loop or require the user to only use one of the functions
/// for valid functionality

// parenting heirarchy
std::weak_ptr<Node> Node::GetParent() {
    return _mParent;
}
void Node::SetParent(std::weak_ptr<Node> parent, bool keepWorldTransform) {
    MT_CORE_DEBUG("Node::SetParent: parent = {0}; keepWorldTransform = {1}", parent.expired(), keepWorldTransform);
    // if node already has a parent, remove it from the parent's children
    if (auto lockParent = _mParent.lock()) {
        MT_CORE_DEBUG("Node::SetParent: removing node from parent's children");
        lockParent->_mChildren.erase(std::remove(lockParent->_mChildren.begin(), lockParent->_mChildren.end(), shared_from_this()), lockParent->_mChildren.end());
    } else {
        MT_CORE_DEBUG("Node::SetParent: existing parent is nullptr");
    }
    MT_CORE_DEBUG("Node::SetParent: setting new parent");
    _mParent = parent;
    if (auto lockParent = _mParent.lock()) {
        MT_CORE_DEBUG("Node::SetParent: adding node to new parent's children");
        lockParent->_mChildren.push_back(shared_from_this());
    } else {
        MT_CORE_DEBUG("Node::SetParent: new parent is nullptr");
    }
}

// children management
void Node::AddChild(std::shared_ptr<Node> child) {
    if (child == nullptr) {
        MT_CORE_ERROR("Node::AddChild: child is nullptr");
        return;
    }
    auto it = std::find(_mChildren.begin(), _mChildren.end(), child);
    if (it != _mChildren.end()) {
        MT_CORE_ERROR("Node::AddChild: child already exists in children");
        return;
    }
    _mChildren.push_back(child);
    child->_mParent = shared_from_this();
}
void Node::RemoveChild(std::shared_ptr<Node> child) {
    if (child == nullptr) {
        MT_CORE_ERROR("Node::RemoveChild: child is nullptr");
        return;
    }
    auto it = std::find(_mChildren.begin(), _mChildren.end(), child);
    if (it == _mChildren.end()) {
        MT_CORE_ERROR("Node::RemoveChild: child does not exist in children");
        return;
    }
    _mChildren.erase(it);
    child->_mParent.reset();
}
int Node::GetChildCount() const {
    return _mChildren.size();
}
std::shared_ptr<Node> Node::GetChild(int index) {
    if (index < 0 || index >= _mChildren.size()) {
        MT_CORE_ERROR("Node::GetChild: index out of range");
        return nullptr;
    }
    return _mChildren[index];
}
std::shared_ptr<Node> Node::FindChild(const std::string& name) {
    auto it = std::find_if(_mChildren.begin(), _mChildren.end(), [&name](const std::shared_ptr<Node>& node) {
        return node->GetName() == name;
    });
    if (it == _mChildren.end()) {
        MT_CORE_WARN("Node::FindChild: child not found");
        return nullptr;
    }
    return *it;
}
std::vector<std::shared_ptr<Node>>& Node::GetChildren() {
    return _mChildren;
}

}