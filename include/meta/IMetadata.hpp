#pragma once
#include <memory>
#include "Metadata.hpp"

namespace meta
{
    struct IMetadata
    {
        virtual ~IMetadata() = default;
        virtual std::shared_ptr<Metadata> GetMeta() const = 0;
    };
}
