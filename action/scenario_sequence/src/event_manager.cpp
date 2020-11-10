// Copyright 2020 Tier IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "scenario_sequence/event_manager.h"

namespace scenario_sequence
{

EventManager::EventManager(
  const scenario_expression::Context& context,
  const YAML::Node& events_definition)
  : context_ { context }
  , currently { state_is::sleeping }
{
  for (const auto& each : events_definition)
  {
    events_.emplace_back(context, each);
  }

  cursor = std::begin(events_);
}

void EventManager::touch() const
{
  context_.json << (indent++) << "Events: [\n";

  for (auto iter { std::begin(events_) }; iter != cursor; ++iter)
  {
    (*iter).touch();
    context_.json << (std::next(iter) != std::end(events_) ? ",\n" : "\n");
  }

  if (cursor != std::end(events_))
  {
    (*cursor).touch();
    context_.json << (std::next(cursor) != std::end(events_) ? ",\n" : "\n");

    for (auto iter { std::next(cursor) }; iter != std::end(events_); ++iter)
    {
      (*iter).touch();
      context_.json << (std::next(iter) != std::end(events_) ? ",\n" : "\n");
    }
  }

  context_.json << (--indent) << "],\n";
}

state_is EventManager::update(
  const std::shared_ptr<scenario_intersection::IntersectionManager>&)
{
  context_.json << (indent++) << "Events: [\n";

  for (auto iter { std::begin(events_) }; iter != cursor; ++iter)
  {
    (*iter).touch();
    context_.json << (std::next(iter) != std::end(events_) ? ",\n" : "\n");
  }

  if (cursor != std::end(events_))
  {
    currently = (*cursor).update(context_.intersections_pointer());

    context_.json << (std::next(cursor) != std::end(events_) ? ",\n" : "\n");

    switch (currently)
    {
    case state_is::finished:
      for (auto iter { ++cursor }; iter != std::end(events_); ++iter)
      {
        (*iter).touch();
        context_.json << (std::next(iter) != std::end(events_) ? ",\n" : "\n");
      }
      break;

    default:
      for (auto iter { std::next(cursor) }; iter != std::end(events_); ++iter)
      {
        (*iter).touch();
        context_.json << (std::next(iter) != std::end(events_) ? ",\n" : "\n");
      }
      break;
    }
  }
  else
  {
    return currently = state_is::finished;
  }

  context_.json << (--indent) << "],\n";

  return currently;
}

} // namespace scenario_sequence
