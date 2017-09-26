//Copyright 2017 Google Inc.
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//https ://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#include <cstdio>
#include <future>
#include <boost/asio/system_timer.hpp>
#include <boost/asio/use_future.hpp>

using namespace boost::asio;
using namespace std::chrono;
using std::puts;

std::future<void> sleepy(io_service &io) {
	// make sure io_service does not exit until this function is done
	io_service::work w{ io };
	system_timer timer(io);
	timer.expires_from_now(1000ms);
	co_await timer.async_wait(boost::asio::use_future);
	puts("tick1");
	timer.expires_from_now(1000ms);
	co_await timer.async_wait(boost::asio::use_future);
	puts("tick2");
	timer.expires_from_now(1000ms);
	co_await timer.async_wait(boost::asio::use_future);
	puts("tick3");
}

int main() {
	io_service io;
	sleepy(io);
	io.run();
};