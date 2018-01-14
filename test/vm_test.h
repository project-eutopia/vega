#pragma once

#include "test_helper.h"

#include "vega/vm.h"

using namespace vega;

TEST(VMTest, vm_test) {
  {
    const VM vm(std::string("1"));
    EXPECT_EQ(vm.limits().first, 1);
    EXPECT_EQ(vm.limits().second, 1);
    EXPECT_EQ(vm.multiple(), false);
  }
  {
    const VM vm(std::string("2-5"));
    EXPECT_EQ(vm.limits().first, 2);
    EXPECT_EQ(vm.limits().second, 5);
    EXPECT_EQ(vm.multiple(), false);
  }
  {
    const VM vm(std::string("3-n"));
    EXPECT_EQ(vm.limits().first, 3);
    EXPECT_EQ(vm.limits().second, 1);
    EXPECT_EQ(vm.multiple(), true);
  }
  {
    const VM vm(std::string("3-4n"));
    EXPECT_EQ(vm.limits().first, 3);
    EXPECT_EQ(vm.limits().second, 4);
    EXPECT_EQ(vm.multiple(), true);
  }
}
