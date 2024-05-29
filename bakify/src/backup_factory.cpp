/*
 *  main.cpp - CLI setup and pass along to thread pool
 *  Copyright (C) 2024  Sebastian Pineda (spineda.wpi.alum@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "include/backup_factory.hpp"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace {

std::queue<std::string> paths{};
std::mutex job_lock{};
std::mutex print_lock{};
std::condition_variable job_condition{};
std::atomic<bool> stop_threads{false};

void BackupFile(const std::filesystem::path&& file) {
  {
    std::unique_lock<std::mutex> lock{print_lock};
    std::cout << "Backing Up: " << file << std::endl;
  }
}

void WaitingRoom(void) {
  std::string current_file_name{};

  while (true) {
    {
      std::unique_lock<std::mutex> lock{job_lock};
      job_condition.wait(lock,
                         [] { return !paths.empty() || stop_threads.load(); });

      if (stop_threads.load()) [[unlikely]] {
        return;
      }

      paths.front().swap(current_file_name);
      paths.pop();
    }

    BackupFile(std::filesystem::canonical(
        std::filesystem::path{std::move(current_file_name)}));
  }
}

}  // namespace

void StartBackingUp(std::vector<std::string>&& file_names) {
  const std::size_t cores{std::thread::hardware_concurrency()};
  const std::size_t file_count{file_names.size()};
  const std::size_t thread_count = file_count > cores ? cores : file_count;

  std::vector<std::jthread> jobs{};
  jobs.reserve(thread_count);

  for (std::size_t thread{0}; thread < thread_count; thread++) {
    jobs.emplace_back(WaitingRoom);
  }

  std::ranges::for_each(file_names, [](const std::string& file) {
    {
      std::unique_lock<std::mutex> lock{job_lock};
      paths.emplace(std::move(file));
    }
    job_condition.notify_one();
  });

  while (true) {
    if (std::unique_lock<std::mutex> lock{job_lock}; paths.size() <= 0)
        [[unlikely]] {
      break;
    }
  }

  stop_threads.store(true);
  job_condition.notify_all();
  jobs.clear();
}
