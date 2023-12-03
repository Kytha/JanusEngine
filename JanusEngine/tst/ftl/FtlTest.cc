#include <gtest/gtest.h>
#include "ftl/task_counter.h"
#include "ftl/task_scheduler.h"

struct NumberSubset {
	uint64_t start;
	uint64_t end;

	uint64_t total;
};

void AddNumberSubset(ftl::TaskScheduler *taskScheduler, void *arg) {
	(void)taskScheduler;
	NumberSubset *subset = reinterpret_cast<NumberSubset *>(arg);

	subset->total = 0;

	while (subset->start != subset->end) {
		subset->total += subset->start;
		++subset->start;
	}

	subset->total += subset->end;
}

TEST(FiberTaskingLibTest, TestFiberScheduler)
{
	// Create the task scheduler and bind the main thread to it
	ftl::TaskScheduler taskScheduler;
	taskScheduler.Init();

	// Define the constants to test
	constexpr uint64_t triangleNum = 47593243ULL;
	constexpr uint64_t numAdditionsPerTask = 10000ULL;
	constexpr uint64_t numTasks = (triangleNum + numAdditionsPerTask - 1ULL) / numAdditionsPerTask;

	// Create the tasks
	// FTL allows you to create Tasks on the stack.
	// However, in this case, that would cause a stack overflow
	ftl::Task *tasks = new ftl::Task[numTasks];
	NumberSubset *subsets = new NumberSubset[numTasks];
	uint64_t nextNumber = 1ULL;

	for (uint64_t i = 0ULL; i < numTasks; ++i) {
		NumberSubset *subset = &subsets[i];

		subset->start = nextNumber;
		subset->end = nextNumber + numAdditionsPerTask - 1ULL;
		if (subset->end > triangleNum) {
			subset->end = triangleNum;
		}

		tasks[i] = { AddNumberSubset, subset };

		nextNumber = subset->end + 1;
	}

	// Schedule the tasks
	ftl::TaskCounter counter(&taskScheduler);
	taskScheduler.AddTasks(numTasks, tasks, ftl::TaskPriority::Normal, &counter);

	// FTL creates its own copies of the tasks, so we can safely delete the memory
	delete[] tasks;

	// Wait for the tasks to complete
	taskScheduler.WaitForCounter(&counter);

	// Add the results
	uint64_t result = 0ULL;
	for (uint64_t i = 0; i < numTasks; ++i) {
		result += subsets[i].total;
	}

	// Test
	EXPECT_EQ(triangleNum * (triangleNum + 1ULL) / 2ULL, result);
	(void)result;

	// Cleanup
	delete[] subsets;

	// The destructor of TaskScheduler will shut down all the worker threads
	// and unbind the main thread
}
