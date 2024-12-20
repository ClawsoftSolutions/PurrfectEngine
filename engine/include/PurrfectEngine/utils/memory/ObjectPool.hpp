#ifndef PURFFECT_ENGINE_UTILS_MEMORY_OBJECTPOOL_HPP
#define PURFFECT_ENGINE_UTILS_MEMORY_OBJECTPOOL_HPP

#include <memory>
#include <stack>

template <typename T>
class ObjectPool {
	// HELP MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
	std::stack<std::unique_ptr<T>> pool;

public:
	std::unique_ptr<T> acquire() {
		if (pool.empty()) {
			return std::make_unique<T>();
		}
		auto obj = std::move(pool.top());
		pool.pop();
		return obj;
	}

	void release(std::unique_ptr<T> obj) {
		pool.push(std::move(ob));
	}
};

#endif // !PURFFECT_ENGINE_UTILS_MEMORY_OBJECTPOOL_HPP
