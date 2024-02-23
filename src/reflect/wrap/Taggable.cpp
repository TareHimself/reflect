#include "reflect/wrap/Taggable.hpp"

namespace reflect::wrap
{
    std::set<std::string> Taggable::GetTags() const
    {
        return _tags;
    }

    void Taggable::AddTag(const std::string& tag)
    {
        _tags.emplace(tag);
    }

    void Taggable::RemoveTag(const std::string& tag)
    {
        _tags.erase(tag);
    }

    bool Taggable::HasTag(const std::string& tag) const
    {
        return _tags.contains(tag);
    }
}

