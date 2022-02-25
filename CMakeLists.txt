cmake_minimum_required(VERSION 3.16)
project(tiny_web_cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
# add_executable(tiny_web_cmake main.cpp Config.cpp Config.h webserver.cpp webserver.h lock/locker.h log/block_queue.h log/log.cpp log/log.h timer/lst_timer.cpp timer/lst_timer.h)
add_executable(tiny_web_cmake main.cpp Config.cpp Config.h webserver.cpp webserver.h lock/locker.h log/block_queue.h log/log.cpp log/log.h timer/lst_timer.cpp timer/lst_timer.h http/http_conn.cpp http/http_conn.h CGImysql/sql_connection_pool.cpp CGImysql/sql_connection_pool.h threadpool/threadpool.h redis/redis_con.h redis/redis_con.cpp)
# add_executable(tiny_web_cmake main.cpp timer/lst_timer.cpp http/http_conn.cpp log/log.cpp CGImysql/sql_connection_pool.cpp webserver.cpp Config.cpp)
target_link_libraries(tiny_web_cmake mysqlclient hiredis)