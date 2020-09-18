/*
 * Copyright (c) 2020 Deomid "rojer" Ryabkov
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "shelly_main.hpp"

namespace shelly {

void CreatePeripherals(std::vector<std::unique_ptr<Input>> *inputs,
                       std::vector<std::unique_ptr<Output>> *outputs,
                       std::vector<std::unique_ptr<PowerMeter>> *pms) {
  outputs->emplace_back(new OutputPin(1, 4, 1));
  outputs->emplace_back(new OutputPin(2, 5, 1));
  auto *in1 = new InputPin(1, 12, 1, MGOS_GPIO_PULL_NONE, true);
  in1->AddHandler(std::bind(&HandleInputResetSequence, in1, 4, _1, _2));
  inputs->emplace_back(in1);
  auto *in2 = new InputPin(1, 14, 1, MGOS_GPIO_PULL_NONE, true);
  in2->AddHandler(std::bind(&HandleInputResetSequence, in2, 5, _1, _2));
  inputs->emplace_back(in2);
  (void) pms;
}

void CreateComponents(std::vector<Component *> *comps,
                      std::vector<std::unique_ptr<hap::Accessory>> *accs,
                      HAPAccessoryServerRef *svr) {
  // Use legacy layout if upgraded from older version.
  // However, presence of detached inputs overrides it.
  bool to_pri_acc = (mgos_sys_config_get_shelly_legacy_hap_layout() &&
                     mgos_sys_config_get_sw1_in_mode() != 3 &&
                     mgos_sys_config_get_sw2_in_mode() != 3);
  CreateHAPSwitch(1, mgos_sys_config_get_sw1(), mgos_sys_config_get_ssw1(),
                  comps, accs, svr, to_pri_acc);
  CreateHAPSwitch(2, mgos_sys_config_get_sw2(), mgos_sys_config_get_ssw2(),
                  comps, accs, svr, to_pri_acc);
}

}  // namespace shelly