#pragma once

#include "../ISystem.h"
#include "ThreadPool.h"

class GeneralSystem : public ISystem {
private:
	std::unique_ptr<ThreadPool> threadPool;

public:
	GeneralSystem();
	~GeneralSystem();

	void init() override;

	void queueThreadJob(const std::function<void()>& job);
};