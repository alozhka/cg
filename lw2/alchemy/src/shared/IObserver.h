#pragma once

class IObserver
{
public:
	virtual void OnUpdate() = 0;
	virtual ~IObserver() = default;
};