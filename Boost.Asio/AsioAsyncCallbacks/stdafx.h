#pragma once

#include "targetver.h"

#include <vector>
#include <memory>
#include <iostream>
#include <atomic>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using tcp = boost::asio::ip::tcp;
using error_code = boost::system::error_code;
