#pragma once
#include <memory>
#include <span>
#include "name.h"

namespace evo::content
{
	class ContentLoadingManager;
	class StorageBase;

	class ContentLoaderBase {
	public:
		virtual void load(Name name, std::span<const std::byte>) = 0;
		virtual const void* pointerTo(const Name&) const = 0;
		virtual void resolvePointers(ContentLoadingManager&) = 0;
		virtual std::unique_ptr<StorageBase> store() = 0;
	};
}