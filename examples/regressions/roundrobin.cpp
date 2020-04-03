/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2.
 *
 * OTest2 is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * OTest2 is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OTest2.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "roundrobin.h"

#include <otest2/testmarkbuilder.h>

namespace OTest2 {

namespace Examples {

Command::Command() = default;
Command::~Command() = default;

RoundRobin::RoundRobin() {

}

RoundRobin::~RoundRobin() {

}

void RoundRobin::scheduleCommand(
    int priority_,
    std::shared_ptr<Command> command_) {
  /* -- get the queue */
  auto queue_(scheduler.find(priority_));
  if(queue_ == scheduler.end()) {
    auto res_(scheduler.emplace(
        std::piecewise_construct,
        std::make_tuple(priority_),
        std::make_tuple()));
    queue_ = res_.first;
  }

  /* -- schedule the command */
  (*queue_).second.push_back(command_);
}

void RoundRobin::executeNextCommand() {
  auto queue_(scheduler.begin());
  if(queue_ != scheduler.end()) {
    /* -- get the record */
    std::shared_ptr<Command> command_((*queue_).second.front());

    /* -- execute the command */
    bool finished_(command_->execute());

    /* -- remove the command from the front of the queue */
    (*queue_).second.pop_front();

    /* -- if the command is not finished, schedule it at the end of the queue */
    if(!finished_)
      (*queue_).second.push_back(command_);

    /* -- destroy the priority queue if it's empty */
    if((*queue_).second.empty())
      scheduler.erase(queue_);
  }
}

void RoundRobin::test_testMark(
    TestMarkBuilder& builder_) const noexcept {
  builder_.openList("RoundRobin");

  for(const auto& iter_ : scheduler) {
    builder_.openMap();

    builder_.setKey("priority");
    builder_.appendInt(iter_.first);

    builder_.setKey("queue");
    builder_.openList();
    for(const auto& command_ : iter_.second)
      builder_.appendString(command_->getName());
    builder_.closeContainer();

    builder_.closeContainer();
  }

  builder_.closeContainer();
}

} /* -- namespace Examples */

} /* -- namespace OTest2 */
