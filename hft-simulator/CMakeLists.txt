# CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(MatchingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- 1. Fetch and configure nlohmann_json ---
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# --- 2. Fetch and configure librdkafka ---
FetchContent_Declare(
  rdkafka
  GIT_REPOSITORY https://github.com/confluentinc/librdkafka.git
  GIT_TAG v2.4.0
)
FetchContent_MakeAvailable(rdkafka)


# Define our executable with all its source files
add_executable(engine
  src/main.cpp
  src/OrderBook.cpp
  src/KafkaProducer.cpp
  src/KafkaConsumer.cpp 
)

# Explicitly tell our 'engine' target where to find the librdkafka headers
target_include_directories(engine PRIVATE ${rdkafka_SOURCE_DIR}/src)

# Link our executable against the libraries we fetched
target_link_libraries(engine PRIVATE
  nlohmann_json::nlohmann_json
  rdkafka++
)

# Tell the compiler where to find our own project's header files
target_include_directories(engine PUBLIC include)