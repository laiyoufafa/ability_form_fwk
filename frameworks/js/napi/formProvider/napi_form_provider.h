/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef NAPI_FORM_PROVIDER_H_
#define NAPI_FORM_PROVIDER_H_

#include "ability.h"
#include "form_provider_info.h"
#include "napi_form_util.h"
#include "napi/native_api.h"
#include "napi/native_common.h"
#include "napi/native_node_api.h"
#include "nlohmann/json.hpp"
#include "want.h"

namespace OHOS {
namespace AbilityRuntime {
struct AsyncNextRefreshTimeFormCallbackInfo {
    napi_env env;
    OHOS::AppExecFwk::Ability *ability;
    napi_async_work asyncWork;
    napi_deferred deferred;
    napi_ref callback;
    int64_t formId;
    int32_t time;
    int32_t result;
};

struct AsyncUpdateFormCallbackInfo {
    napi_env env;
    OHOS::AppExecFwk::Ability *ability;
    napi_async_work asyncWork;
    napi_deferred deferred;
    napi_ref callback;
    int64_t formId;
    std::shared_ptr<OHOS::AppExecFwk::FormProviderData> formProviderData;
    int32_t result;
};

struct AsyncRequestPublishFormCallbackInfo {
    napi_env env = nullptr;
    OHOS::AppExecFwk::Ability *ability = nullptr;
    napi_async_work asyncWork = nullptr;
    napi_deferred deferred = nullptr;
    napi_ref callback = nullptr;
    Want want {};
    bool withFormBindingData = false;
    std::unique_ptr<OHOS::AppExecFwk::FormProviderData> formProviderData = nullptr;
    int32_t result = OHOS::ERR_OK;
    int64_t formId = 0;
};

struct AsyncAddFormInfoCallbackInfo {
    napi_env env = nullptr;
    OHOS::AppExecFwk::Ability *ability = nullptr;
    napi_async_work asyncWork = nullptr;
    napi_deferred deferred = nullptr;
    napi_ref callback = nullptr;
    OHOS::AppExecFwk::FormInfo formInfo {};
    int32_t result = OHOS::ERR_OK;
};

struct AsyncRemoveFormInfoCallbackInfo {
    napi_env env = nullptr;
    OHOS::AppExecFwk::Ability *ability = nullptr;
    napi_async_work asyncWork = nullptr;
    napi_deferred deferred = nullptr;
    napi_ref callback = nullptr;
    std::string moduleName;
    std::string formName;
    int32_t result = OHOS::ERR_OK;
};

struct AsyncIsRequestPublishFormSupportedCallbackInfo : AsyncCallbackInfoBase {
    bool result = false;

    explicit AsyncIsRequestPublishFormSupportedCallbackInfo(napi_env env) : AsyncCallbackInfoBase(env) {};
    ~AsyncIsRequestPublishFormSupportedCallbackInfo() override = default;
};

napi_value NAPI_SetFormNextRefreshTime(napi_env env, napi_callback_info info);
napi_value NAPI_UpdateForm(napi_env env, napi_callback_info info);
napi_value NAPI_RequestPublishForm(napi_env env, napi_callback_info info);
napi_value NAPI_AddFormInfo(napi_env env, napi_callback_info info);
napi_value NAPI_RemoveFormInfo(napi_env env, napi_callback_info info);

/**
 * @brief Check if the request to publish a form to the form host is supported.
 *
 * @param[in] env The environment that the Node-API call is invoked under
 * @param[in] info This is an opaque pointer that is used to represent a JavaScript value
 *
 * @return This is an opaque pointer that is used to represent a JavaScript value
 *         which is true if the request of publishing form is supported and false otherwise
 */
napi_value NAPI_IsRequestPublishFormSupported(napi_env env, napi_callback_info info);

class JsFormProvider {
public:
    JsFormProvider() = default;
    ~JsFormProvider() = default;

    static void Finalizer(NativeEngine* engine, void* data, void* hint);
    static NativeValue* GetFormsInfo(NativeEngine* engine, NativeCallbackInfo* info);
private:
    NativeValue* OnGetFormsInfo(NativeEngine &engine, NativeCallbackInfo &info);
};
}  // namespace AbilityRuntime
}  // namespace OHOS
#endif /* NAPI_FORM_PROVIDER_H_ */
