
#include <future>
#include <iostream>
#include <atomic>
#include <mutex>
#include <queue>
#include "Engine.h"
using namespace std;

void main()
{
	Crawler::Engine e(3,5);
	e.start();
	/*
	Crawler::ItemPipeline* a = new Crawler::ItemPipeline();
	a->process_item(shared_ptr<Crawler::Item>());
	*/
	system("pause");
}