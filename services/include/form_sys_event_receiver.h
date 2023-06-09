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

#ifndef FOUNDATION_APPEXECFWK_SERVICES_FORMMGR_INCLUDE_FORM_PROVIDER_RECEIVER_H
#define FOUNDATION_APPEXECFWK_SERVICES_FORMMGR_INCLUDE_FORM_PROVIDER_RECEIVER_H

#include "bundle_pack_info.h"
#include "common_event_subscriber.h"
#include "common_event_subscribe_info.h"
#include "event_handler.h"
#include "form_id_key.h"
#include "form_info.h"
#include "form_record.h"
#include "form_timer.h"

namespace OHOS {
namespace AppExecFwk {
/**
 * @class FormSysEventReceiver
 * Receive system common event.
 */
class FormSysEventReceiver : public EventFwk::CommonEventSubscriber {
public:
    FormSysEventReceiver() = default;
    FormSysEventReceiver(const EventFwk::CommonEventSubscribeInfo &subscriberInfo);
    virtual ~FormSysEventReceiver() = default;
    /**
     * @brief System common event receiver.
     * @param eventData Common event data.
     */
    virtual void OnReceiveEvent(const EventFwk::CommonEventData &eventData) override;

    /**
     * @brief SetEventHandler.
     * @param handler event handler
     */
    inline void SetEventHandler(const std::shared_ptr<AppExecFwk::EventHandler> &handler)
    {
        eventHandler_ = handler;
    }
private:
    void HandleProviderUpdated(const std::string &bundleName, const int userId);
    bool ProviderFormUpdated(const int64_t formId, const FormRecord &formRecord,
        const std::vector<FormInfo> &targetForms);
    bool ProviderFormUpdated(int64_t formId, const FormRecord &formRecord,
        const BundlePackInfo &bundlePackInfo);
    void HandleBundleFormInfoChanged(const std::string &bundleName, int32_t userId);
    void HandleBundleFormInfoRemoved(const std::string &bundleName, int32_t userId);
    void HandleProviderRemoved(const std::string &bundleName, const int32_t userId);
    void HandleBundleDataCleared(const std::string &bundleName, int32_t userId);
    void HandleFormHostDataCleared(const int uid);
    void ClearFormDBRecordData(const int uid, std::map<int64_t, bool> &removedFormsMap);
    void ClearTempFormRecordData(const int uid, std::map<int64_t, bool> &removedFormsMap);
    void BatchDeleteNoHostDBForms(const int uid, std::map<FormIdKey, std::set<int64_t>> &noHostFormDbMap,
    std::map<int64_t, bool> &removedFormsMap);
    void HandleUserIdRemoved(const int32_t userId); // multiuser
    /**
     * @brief Delete no host temp forms.
     * @param uid The caller uid.
     * @param noHostTempFormsMap no host temp forms.
     * @param foundFormsMap Form Id list.
     */
    void BatchDeleteNoHostTempForms(const int uid, std::map<FormIdKey, std::set<int64_t>> &noHostTempFormsMap,
    std::map<int64_t, bool> &foundFormsMap);
    void ReCreateForm(const int64_t formId);
    bool IsSameForm(const FormRecord &record, const FormInfo &formInfo);
    void GetTimerCfg(const bool updateEnabled, const int updateDuration, const std::string &configUpdateAt,
    FormTimerCfg &cfg);
    void HandleTimerUpdate(const int64_t formId, const FormRecord &record, const FormTimerCfg &timerCfg);
    void HandleBundleScanFinished(int32_t userId);
private:
    std::shared_ptr<AppExecFwk::EventHandler> eventHandler_ = nullptr;
};
}  // namespace AppExecFwk
}  // namespace OHOS

#endif // FOUNDATION_APPEXECFWK_SERVICES_FORMMGR_INCLUDE_FORM_PROVIDER_RECEIVER_H