#pragma once

#include <string>
#include <functional>

class IInteractable {
private:
	std::function<void(void)> callback;
	std::string trigger;
	
	int repeat;

public:
	IInteractable(std::string trigger, std::function<void(void)> callback, int repeat = 1);
	
	void call();
	bool isDone();

	std::string& getTrigger();
};