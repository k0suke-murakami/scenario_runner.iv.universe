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

#ifndef ENTITY_PLUGINS__VEHICLE_ENTITY_HPP_
#define ENTITY_PLUGINS__VEHICLE_ENTITY_HPP_

#include "scenario_entities/entity_base.hpp"

namespace entity_plugins
{

struct VehicleEntity
  : public scenario_entities::EntityBase
{
  VehicleEntity();
};

}  // namespace entity_plugins

#endif  // ENTITY_PLUGINS__VEHICLE_ENTITY_HPP_