#/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/CostModel.hh"
#include <cassert>
#include <iostream>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

CostModel::CostModel() noexcept {
  // initialize unit costs
  link_unit_costs[1] = 2.0;
  link_unit_costs[2] = 4.0;
  link_unit_costs[3] = 4.0;
  link_unit_costs[4] = 7.8;

  switch_unit_costs[1] = -1;
  switch_unit_costs[2] = 13.0;
  switch_unit_costs[3] = 13.0;
  switch_unit_costs[4] = 18.0;

  nic_unit_costs[1] = -1;
  nic_unit_costs[2] = -1;
  nic_unit_costs[3] = -1;
  nic_unit_costs[4] = 31.6;

  // unused values
  link_unit_costs[0] = -1;
  switch_unit_costs[0] = -1;
  nic_unit_costs[0] = -1;

  // initialize costs map
  initialize_costs(&link_costs, link_unit_costs);
  initialize_costs(&switch_costs, switch_unit_costs);
  initialize_costs(&nic_costs, nic_unit_costs);
}

void CostModel::initialize_costs(
    ResourceCosts* resource_costs,
    const UnitCosts& unit_costs) noexcept {
  // depending on the total_dim, the element changes
  (*resource_costs)[1] = {-1, unit_costs[2]};
  (*resource_costs)[2] = {-1, unit_costs[2], unit_costs[4]};
  (*resource_costs)[3] = {-1, unit_costs[2], unit_costs[3], unit_costs[4]};
  (*resource_costs)[4] = {
      -1, unit_costs[1], unit_costs[2], unit_costs[3], unit_costs[4]};

  // unused values
  (*resource_costs)[0] = {};
}

CostModel::DollarCost CostModel::get_link_cost(
    const int current_dim,
    const int total_dim) const noexcept {
  return get_resource_cost(link_costs, current_dim, total_dim);
}

CostModel::DollarCost CostModel::get_nic_cost(
    const int current_dim,
    const int total_dim) const noexcept {
  return get_resource_cost(nic_costs, current_dim, total_dim);
}

CostModel::DollarCost CostModel::get_switch_cost(
    const int current_dim,
    const int total_dim) const noexcept {
  return get_resource_cost(switch_costs, current_dim, total_dim);
}

CostModel::DollarCost CostModel::get_resource_cost(
    const ResourceCosts& resource_costs,
    const int current_dim,
    const int total_dim) const noexcept {
  // check the validity
  assert(1 <= current_dim && current_dim <= MAX_DIM);
  assert(1 <= total_dim && total_dim <= MAX_DIM);

  // query the validity of the total_dim
  const auto& cost_dim = resource_costs[total_dim];
  assert(cost_dim.size() - 1 == total_dim); // index 0 is not used

  // get the current_dim link cost
  const auto cost = cost_dim[current_dim];
  return cost;
}
