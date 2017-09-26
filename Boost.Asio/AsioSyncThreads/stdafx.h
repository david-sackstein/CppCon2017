#pragma once

#include "targetver.h"

#include <set>
#include <vector>
#include <memory>
#include <thread>
#include <iostream>
#include <atomic>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using error_code = boost::system::error_code; 
using tcp = boost::asio::ip::tcp;