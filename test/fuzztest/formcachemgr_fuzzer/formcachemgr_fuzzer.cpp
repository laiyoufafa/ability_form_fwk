/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "formcachemgr_fuzzer.h"

#include <cstddef>
#include <cstdint>

#include "form_batch_delete_connection.h"
#define private public
#define protected public
#include "form_cache_mgr.h"
#include "form_event_handler.h"
#undef private
#undef protected
#include "form_cast_temp_connection.h"
#include "form_delete_connection.h"
#include "securec.h"

using namespace OHOS::AppExecFwk;

namespace OHOS {
constexpr size_t FOO_MAX_LEN = 1024;
uint32_t GetU32Data(const char* ptr)
{
    // convert fuzz input data to an integer
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}
bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    FormCacheMgr formCacheMgr;
    int64_t formId = static_cast<int64_t>(GetU32Data(data));
    std::string datas(data, size);
    formCacheMgr.GetData(formId, datas);
    formCacheMgr.AddData(formId, datas);
    formCacheMgr.DeleteData(formId);
    formCacheMgr.UpdateData(formId, datas);
    std::set<int64_t> formIds;
    formIds.insert(formId);
    std::string bundleName(data, size);
    std::string abilityName(data, size);
    FormBatchDeleteConnection formBatchDeleteConnection(formIds, bundleName, abilityName);
    AppExecFwk::ElementName element;
    sptr<IRemoteObject> remoteObject = nullptr;
    int resultCode = static_cast<int>(GetU32Data(data));
    formBatchDeleteConnection.OnAbilityConnectDone(element, remoteObject, resultCode);
    FormCastTempConnection formCastTempConnection(formId, bundleName, abilityName);
    formCastTempConnection.OnAbilityConnectDone(element, remoteObject, resultCode);
    FormDeleteConnection formDeleteConnection(formId, bundleName, abilityName);
    formDeleteConnection.OnAbilityConnectDone(element, remoteObject, resultCode);
    std::shared_ptr<AppExecFwk::EventRunner> runner = nullptr;
    FormEventHandler formEventHandler(runner);
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get();
    formEventHandler.ProcessEvent(event);
    formEventHandler.GetEventId();
    std::shared_ptr<FormEventTimeoutObserver> observer = nullptr;
    formEventHandler.RegisterEventTimeoutObserver(observer);
    formEventHandler.UnregisterEventTimeoutObserver(observer);
    return formCacheMgr.IsExist(formId);
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    if (data == nullptr) {
        return 0;
    }

    /* Validate the length of size */
    if (size == 0 || size > OHOS::FOO_MAX_LEN) {
        return 0;
    }

    char* ch = (char *)malloc(size + 1);
    if (ch == nullptr) {
        return 0;
    }

    (void)memset_s(ch, size + 1, 0x00, size + 1);
    if (memcpy_s(ch, size, data, size) != EOK) {
        free(ch);
        ch = nullptr;
        return 0;
    }

    OHOS::DoSomethingInterestingWithMyAPI(ch, size);
    free(ch);
    ch = nullptr;
    return 0;
}

