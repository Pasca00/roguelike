#include "GeneralSystem.h"

GeneralSystem::GeneralSystem() : ISystem() {
	threadPool = std::make_unique<ThreadPool>();
}

GeneralSystem::~GeneralSystem() {
	this->threadPool->stop();
}

void GeneralSystem::init() {
	this->threadPool->start();
}

void GeneralSystem::queueThreadJob(const std::function<void()>& job) {
	this->threadPool->queueJob(job);
}