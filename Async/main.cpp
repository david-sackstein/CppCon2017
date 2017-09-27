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

#include <iostream>
#include <future>
#include <boost/asio/system_timer.hpp>
#include <boost/asio/use_future.hpp>

using namespace boost::asio;
using namespace std::chrono;
using std::puts;

std::future<void> sleepy(io_service &io) {
	
	io_service::work keep_io_service_alive { io };
	system_timer timer(io);

	timer.expires_from_now(1000ms);

	co_await timer.async_wait(boost::asio::use_future);

	std::cout << "After co_await 1\n";
	timer.expires_from_now(1000ms);
	
	co_await timer.async_wait(boost::asio::use_future);

	std::cout << "After co_await 2\n";
	timer.expires_from_now(1000ms);

	co_await timer.async_wait(boost::asio::use_future);

	std::cout << "After co_await 3\n";
}

int main() {
	io_service io;
	sleepy(io);
	io.run();
};

