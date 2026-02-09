#pragma once
#include <set>

class IObserver
{
public:
	virtual void update() = 0;
	virtual ~IObserver() = default;
};

class IObservable
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

	void NotifyObservers()
	{
		for (auto* observer : m_observers)
		{
			observer->update();
		}
	}
	virtual ~IObservable() = default;

private:
	std::set<IObserver*> m_observers;
};
