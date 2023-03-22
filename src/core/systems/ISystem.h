#pragma once

class ISystem {
public:
	virtual void init() = 0;

	virtual ~ISystem() {}
};