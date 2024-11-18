/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <memory>
#include <vector>
#include "common/EventQueue.hh"
#include "congestion_aware/Chunk.hh"
#include "congestion_aware/CostModel.hh"
#include "congestion_aware/Device.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

/**
 * Topology abstracts a network topology.
 */
class Topology {
 public:
  using DollarCost = CostModel::DollarCost;

  /**
   * Set the event queue to be used by the topology.
   *
   * @param event_queue pointer to the event queue
   */
  static void set_event_queue(std::shared_ptr<EventQueue> event_queue) noexcept;

  /**
   * Constructor.
   */
  Topology() noexcept;

  /**
   * Construct the route from src to dest.
   * Route is a list of devices (pointers) that the chunk should traverse,
   * including the src and dest devices themselves.
   *
   * e.g., route(0, 3) = [0, 5, 7, 2, 3]
   *
   * @param src src NPU id
   * @param dest dest NPU id
   *
   * @return route from src NPU to dest NPU
   */
  [[nodiscard]] virtual Route route(DeviceId src, DeviceId dest)
      const noexcept = 0;

  /**
   * @brief Get the dollar cost of the topology
   *
   * @return DollarCost
   */
  [[nodiscard]] DollarCost get_topology_cost() const noexcept;

  /**
   * Initiate a transmission of a chunk.
   *
   * @param chunk chunk to be transmitted
   */
  void send(std::unique_ptr<Chunk> chunk) const noexcept;

  /**
   * Get the number of NPUs in the topology.
   * NPU excludes non-NPU devices such as switches.
   *
   * @return number of NPUs in the topology
   */
  [[nodiscard]] int get_npus_count() const noexcept;

  /**
   * Get the number of devices in the topology.
   * Device includes non-NPU devices such as switches.
   *
   * @return number of devices in the topology
   */
  [[nodiscard]] int get_devices_count() const noexcept;

  /**
   * Get the number of network dimensions.
   *
   * @return number of network dimensions
   */
  [[nodiscard]] int get_dims_count() const noexcept;

  /**
   * Get the topology shape of the given dimension.
   * @param dim dimension to query
   *
   * @return topology shape of the dimension
   */
  [[nodiscard]] TopologyBuildingBlock get_topology_of_dim(
      int dim) const noexcept;

  /**
   * Get the number of NPUs of the given dimension.
   * @param dim dimension to query
   *
   * @return number of NPUs of the dimension
   */
  [[nodiscard]] int get_npus_count_of_dim(int dim) const noexcept;

  /**
   * Get the link bandwidth of the given dimension.
   * @param dim dimension to query
   *
   * @return bandwidth of the dimension
   */
  [[nodiscard]] Bandwidth get_bandwidth_of_dim(int dim) const noexcept;

  /**
   * Get the link latency of the given dimension.
   * @param dim dimension to query
   *
   * @return bandwidth of the dimension
   */
  [[nodiscard]] Latency get_latency_of_dim(int dim) const noexcept;

  /**
   * Get the number of NPUs per each dimension.
   *
   * @return number of NPUs per each dimension
   */
  [[nodiscard]] std::vector<int> get_npus_count_per_dim() const noexcept;

  /**
   * Get the bandwidth per each network dimension.
   *
   * @return bandwidth per each dimension
   */
  [[nodiscard]] std::vector<Bandwidth> get_bandwidth_per_dim() const noexcept;

 protected:
  /**
   * Connect src -> dest with the given bandwidth and latency.
   * (i.e., a `Link` gets constructed between the two npus)
   *
   * if bidirectional=true, dest -> src connection is also established.
   *
   * @param src src device pointer
   * @param dest dest device pointer
   * @param bandwidth bandwidth of link
   * @param latency latency of link
   * @param bidirectional true if connection is bidirectional, false otherwise
   */
  void connect(
      const std::shared_ptr<Device>& src,
      const std::shared_ptr<Device>& dest,
      Bandwidth bandwidth,
      Latency latency,
      bool bidirectional = true) noexcept;

  /// cost model
  static CostModel cost_model;

  /// cost of the topology
  DollarCost topology_cost;

  /// number of total devices in the topology
  /// device includes non-NPU devices such as switches
  int devices_count;

  /// number of NPUs in the topology
  /// NPU excludes non-NPU devices such as switches
  int npus_count;

  /// number of network dimensions
  int dims_count;

  /// number of NPUs per each dimension
  std::vector<int> npus_count_per_dim;

  /// holds the entire device instances in the topology
  Devices devices;

  /// topology shape per each network dimension
  std::vector<TopologyBuildingBlock> topology_per_dim;

  /// bandwidth per each network dimension
  std::vector<Bandwidth> bandwidth_per_dim;

  /// network latency (ns) per each network dimension
  std::vector<Latency> latency_per_dim;

  /**
   * Create a new Device object with the given ID.
   * @param id id of the new device
   *
   * @return pointer to the created device
   */
  [[nodiscard]] std::shared_ptr<Device> create_device(DeviceId id) noexcept;
};

} // namespace NetworkAnalyticalCongestionAware
