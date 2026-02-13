#pragma once
#include "IObserver.h"

#include <unordered_set>

class Observable
{
public:
	void AddObserver(IObserver* observer)
	{
		m_observers.insert(observer);
	}

	void RemoveObserver(IObserver* observer)
	{
		m_observers.erase(observer);
	}

	void Notify()
	{
		for (IObserver* observer : m_observers)
		{
			observer->OnUpdate();
		}
	}

	virtual ~Observable() = default;

private:
	std::unordered_set<IObserver*> m_observers;
};