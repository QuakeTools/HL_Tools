#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <vector>

#include <glm/vec3.hpp>

#include "graphics/GraphicsDefinitions.hpp"
#include "graphics/IRenderable.hpp"

namespace graphics
{
class RenderQueue
{
private:
	static const std::size_t DefaultCapacity = 250;

	struct Entry
	{
		Entry(const RenderOrderKey key, const std::size_t index)
			: Key(key)
			, Index(index)
		{
		}

		Entry(const Entry&) = default;
		Entry& operator=(const Entry&) = default;

		RenderOrderKey Key;
		std::size_t Index;
	};

public:

	class iterator
	{
	public:
		// iterator traits
		using difference_type = std::ptrdiff_t;
		using value_type = IRenderable*;
		using pointer = IRenderable**;
		using reference = IRenderable*&;
		using iterator_category = std::forward_iterator_tag;

		friend class RenderQueue;

		iterator()
			: _index(std::numeric_limits<std::size_t>::max())
		{
		}

	protected:
		iterator(RenderQueue* const queue)
			: _queue(queue)
			, _index(0)
		{
		}

	public:
		iterator& operator++()
		{
			if (_index + 1 < _queue->GetCount())
			{
				++_index;
			}
			else
			{
				_queue = nullptr;
				_index = std::numeric_limits<std::size_t>::max();
			}

			return *this;
		}

		iterator operator++(int)
		{
			auto retval = *this;
			++(*this);
			return retval;
		}

		bool operator==(iterator other) const
		{
			return _queue == other._queue && _index == other._index;
		}

		bool operator!=(iterator other) const
		{
			return !(*this == other);
		}

		reference operator*()
		{
			return _queue->_renderables[_queue->_indices[_index].Index];
		}

	private:
		RenderQueue* _queue = nullptr;
		std::size_t _index = 0;
	};

	friend class iterator;

public:
	RenderQueue()
	{
		_indices.reserve(DefaultCapacity);
		_renderables.reserve(DefaultCapacity);
	}

	std::size_t GetCount() const { return _renderables.size(); }

	auto begin()
	{
		if (_renderables.empty())
		{
			return iterator();
		}

		return iterator(this);
	}

	auto end() { return iterator(); }

	void Clear()
	{
		_indices.clear();
		_renderables.clear();
	}

	void AddRange(const std::vector<IRenderable*>& renderables, const glm::vec3& viewPosition)
	{
		for (const auto& renderable : renderables)
		{
			Add(renderable, viewPosition);
		}
	}

	void Add(IRenderable* renderable, const glm::vec3& viewPosition)
	{
		_indices.emplace_back(renderable->GetRenderOrderKey(viewPosition), _renderables.size());
		_renderables.emplace_back(renderable);
	}

	void Sort()
	{
		std::sort(_indices.begin(), _indices.end(), [](const Entry& lhs, const Entry& rhs)
			{
				return lhs.Key.Value < rhs.Key.Value;
			}
		);
	}

private:
	std::vector<Entry> _indices;
	std::vector<IRenderable*> _renderables;
};
}
