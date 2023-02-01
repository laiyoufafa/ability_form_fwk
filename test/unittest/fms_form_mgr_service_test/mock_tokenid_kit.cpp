/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>

#include "tokenid_kit.h"
#include "hilog_wrapper.h"

namespace {
    bool isSystemAppByFullTokenID = true;
}

void MockIsSystemAppByFullTokenID(bool mockRet)
{
    isSystemAppByFullTokenID = mockRet;
}

namespace OHOS {
namespace Security {
namespace AccessToken {
bool TokenIdKit::IsSystemAppByFullTokenID(uint64_t tokenId)
{
    GTEST_LOG_(INFO) << "IsSystemAppByFullTokenID called " << isSystemAppByFullTokenID;
    return isSystemAppByFullTokenID;
}
} // namespace AccessToken
} // namespace Security
} // namespace OHOS