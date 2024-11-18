/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/Switch.hh"
#include <cassert>
#include <iostream>

using namespace NetworkAnalyticalCongestionAware;

Switch::Switch(
    const int npus_count,
    const Bandwidth bandwidth,
    const Latency latency) noexcept
    : BasicTopology(npus_count, npus_count + 1, bandwidth, latency) {
  // e.g., if npus_count=8, then
  // there are total 9 devices, where ordinary npus are 0-7, and switch is 8
  assert(npus_count > 0);
  assert(bandwidth > 0);
  assert(latency >= 0);

  // set topology type
  set_basic_topology_type(TopologyBuildingBlock::Switch);

  // set switch
  this->switch_device = devices[npus_count];

  // construct connectivity
  construct_connections();
}

Switch::Switch(
    const Devices& npus,
    std::shared_ptr<Device> switch_device,
    const Bandwidth bandwidth,
    const Latency latency) noexcept
    : switch_device(nullptr),
      BasicTopology(npus, Devices({switch_device}), bandwidth, latency) {
  assert(!npus.empty());
  assert(bandwidth > 0);
  assert(latency >= 0);

  // set topology type
  set_basic_topology_type(TopologyBuildingBlock::Switch);

  // set switch
  this->switch_device = std::move(switch_device);

  // construct connectivity
  construct_connections();
}

Route Switch::route(DeviceId src, DeviceId dest) const noexcept {
  // assert npus are in valid range
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);

  // construct route
  // start at source, and go to switch, then go to destination
  auto route = Route();
  route.push_back(devices[src]);
  route.push_back(switch_device);
  route.push_back(devices[dest]);

  return route;
}

void Switch::construct_connections() noexcept {
  // connect npus and switches, the link should be bidirectional
  for (auto i = 0; i < npus_count; i++) {
    auto npu = devices[i];
    connect(devices[i], switch_device, bandwidth, latency, true);
  }

  // add switch cost
  const auto switch_cost = cost_model.get_switch_cost(1, 1);
  assert(switch_cost > 0);
  topology_cost += switch_cost * npus_count * bandwidth; // radix = #npus

  // add nic cost, if nic is used
  const auto nic_cost = cost_model.get_nic_cost(1, 1);
  if (nic_cost > 0) { // nic is used
    topology_cost += (nic_cost * npus_count * bandwidth); // #nics == #npus
  }
}

Switch::DollarCost Switch::get_topology_cost_block(
    const int current_dim,
    const int total_dim) const noexcept {
  DollarCost cost = 0.0;

  // get the cost of the link
  auto link_cost = cost_model.get_link_cost(current_dim, total_dim);
  assert(link_cost > 0);
  link_cost *= bandwidth;

  const auto links_count = 2 * npus_count; // npus to switch, switch to npus
  cost += (link_cost * links_count);

  // add switch cost
  const auto switch_cost = cost_model.get_switch_cost(current_dim, total_dim);
  assert(switch_cost > 0);
  cost += switch_cost * npus_count * bandwidth; // radix = #npus

  // add nic cost, if nic is used
  const auto nic_cost = cost_model.get_nic_cost(current_dim, total_dim);
  if (nic_cost > 0) { // nic is used
    cost += (nic_cost * npus_count * bandwidth); // #nics == #npus
  }

  return cost;
}
