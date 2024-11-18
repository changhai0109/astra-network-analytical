/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <array>
#include <vector>
#include "common/Type.hh"
#include "congestion_aware/Type.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

class CostModel {
 public:
  using DollarCost = double;
  constexpr static int MAX_DIM = 4;
  constexpr static int ARRAY_SIZE = MAX_DIM + 1; // index 0 is not used

  CostModel() noexcept;

  [[nodiscard]] DollarCost get_link_cost(int current_dim, int total_dim)
      const noexcept;

  [[nodiscard]] DollarCost get_nic_cost(int current_dim, int total_dim)
      const noexcept;

  [[nodiscard]] DollarCost get_switch_cost(int current_dim, int total_dim)
      const noexcept;

 private:
  using UnitCosts = std::array<DollarCost, ARRAY_SIZE>;
  using ResourceCosts = std::array<std::vector<DollarCost>, ARRAY_SIZE>;

  UnitCosts link_unit_costs;
  UnitCosts switch_unit_costs;
  UnitCosts nic_unit_costs;

  ResourceCosts link_costs;
  ResourceCosts switch_costs;
  ResourceCosts nic_costs;

  void initialize_costs(
      ResourceCosts* resource_costs,
      const UnitCosts& unit_costs) noexcept;

  [[nodiscard]] DollarCost get_resource_cost(
      const ResourceCosts& resource_costs,
      int current_dim,
      int total_dim) const noexcept;
};

} // namespace NetworkAnalyticalCongestionAware
