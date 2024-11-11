#pragma once
#include <set>
#include <string>

namespace meta
{
    struct Taggable
    {
    protected:
        std::set<std::string> _tags;

    public:
        std::set<std::string> GetTags() const;
        void AddTag(const std::string& tag);
        void RemoveTag(const std::string& tag);
        bool HasTag(const std::string& tag) const;
    
    };

}
