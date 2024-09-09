//
//
// Tencent is pleased to support the open source community by making tRPC available.
//
// Copyright (C) 2023 THL A29 Limited, a Tencent company.
// All rights reserved.
//
// If you have downloaded a copy of the tRPC source code from Tencent,
// please note that tRPC source code is licensed under the  Apache 2.0 License,
// A copy of the Apache 2.0 License is included in this file.
//
//

#include "trpc/common/config/load_balance_naming_conf.h"
#include "gtest/gtest.h"
#include "trpc/common/config/load_balance_naming_conf_parser.h"
#include "yaml-cpp/yaml.h"
TEST(SWRoundrobinLoadBalanceConfig, load_test) {
  // 创建一个包含多个服务的 SWRoundrobinLoadBalanceConfig 实例
  trpc::naming::SWRoundrobinLoadBalanceConfig config;

  // 配置 serviceA
  config.services["serviceA"] = {
      {"127.0.0.1:10000", 5},
      {"127.0.0.1:10003", 2},
  };

  // 配置 serviceB
  config.services["serviceB"] = {
      {"127.0.0.2:20000", 3},
      {"127.0.0.2:20003", 4},
  };

  // 打印原始配置（假设有 Display 方法）
  config.Display();

  // 将配置编码为 YAML
  YAML::convert<trpc::naming::SWRoundrobinLoadBalanceConfig> c;
  YAML::Node config_node = c.encode(config);

  // 从 YAML 重新解码回 SWRoundrobinLoadBalanceConfig 对象
  trpc::naming::SWRoundrobinLoadBalanceConfig tmp;
  ASSERT_TRUE(c.decode(config_node, tmp));

  // 打印解码后的配置
  tmp.Display();

  // 检查原始配置和解码后配置的相等性
  ASSERT_EQ(config.services.size(), tmp.services.size());

  for (const auto& [service_name, address_weight] : config.services) {
    // 检查每个服务的地址权重映射
    ASSERT_EQ(tmp.services[service_name].size(), address_weight.size());

    for (const auto& [address, weight] : address_weight) {
      ASSERT_EQ(tmp.services[service_name][address], weight);
    }
  }
}
