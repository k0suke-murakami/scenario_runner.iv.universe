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

#include "scenario_sequence/sequence.h"

namespace scenario_sequence
{

Sequence::Sequence(
  const scenario_expression::Context& context,
  const YAML::Node& sequence_definition)
  : context_ { context }
  , name_ { sequence_definition["Name"].as<std::string>() }
  , ignited_ { false }
  , currently { state_is::sleeping }
{
  event_manager_ =
    std::make_shared<scenario_sequence::EventManager>(
      context, sequence_definition["Events"]);

  if (const auto start_condition { sequence_definition["StartCondition"] })
  {
    start_condition_ = scenario_expression::read(context_, start_condition);
  }
  else // NOTE: If StartCondition unspecified, the sequence starts unconditionally.
  {
    ignited_ = true;
  }
}

void Sequence::touch()
{
  context_.json << (indent++) << "{\n";
  context_.json << indent << "Name: " << std::quoted(name_) << ",\n";
  context_.json << (indent++) << "StartConditions: [\n";
  context_.json << start_condition_;
  context_.json << (--indent) << "],\n";

  (*event_manager_).touch();

  context_.json << indent << "State: " << currently << "\n";
  context_.json << (--indent) << "}";
}

state_is Sequence::update(
  const std::shared_ptr<scenario_intersection::IntersectionManager>&)
{
  context_.json << (indent++) << "{\n";
  context_.json << indent << "Name: " << std::quoted(name_) << ",\n";
  context_.json << (indent++) << "StartConditions: [\n";
  context_.json << start_condition_;
  context_.json << (--indent) << "],\n";

  ignited_ = start_condition_.evaluate(context_);

  if (ignited_)
  {
    currently = (*event_manager_).update(context_.intersections_pointer());
  }
  else
  {
    (*event_manager_).touch();
    currently = state_is::running;
  }

  context_.json << indent << "State: " << currently << "\n";
  context_.json << (--indent) << "}";

  return currently;
}

} // namespace scenario_sequence
